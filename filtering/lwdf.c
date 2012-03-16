/**
 * @file
 * @brief Lattice Wave Digital Filter library
 *
 * @author James A. C. Patterson
 * @version 0.1
 * @date 12/12/2011
 *
 * @section LICENSE
 *
 * Free Beer License
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "lwdf.h"

/**
 *	Lattice Wave Digital Filter definitions
 */

static LWDF_TYPE lwdf_multiply(LWDF_ALPHA alpha, LWDF_TYPE value) {
	LWDF_TYPE t_h;
	LWDF_TYPE t_l;
	t_h = alpha*(value>>16);
	t_l = alpha*(value&0xFFFF);
	t_h += (t_l>>16);
	if (t_l&(1<<15)) return t_h+1;
	else return t_h;
}

void lwdf_gamma2alpha(float gamma, LWDF_ALPHA *alpha, uint8_t *type) {
	float alpha_f = 0;
	// Determine the adaptor type and alpha value
	if ((gamma > 0.5) && (gamma < 1.0)) {
		*type = 0;
		alpha_f = 1 - gamma;
	}
	else if ((gamma > 0.0) && (gamma <= 0.5)) {
		*type = 1;
		alpha_f = gamma;
	}
	else if ((gamma >= -0.5) && (gamma <= 0.0)) {
		*type = 2;
		alpha_f = -gamma;
	}
	else if ((gamma > -1) && (gamma < -0.5)) {
		*type = 3;
		alpha_f = 1 + gamma;
	}
	// Shift up the alpha value for fixed point arithmetic
	alpha_f *= (1<<16);
	// Store the alpha
	*alpha = (LWDF_ALPHA)(alpha_f+0.5);
}

static void lwdf_processAdaptor(LWDF_ALPHA alpha, uint8_t type, LWDF_PORT* port) {
	LWDF_TYPE diff;
	// Type specific processing
	switch (type) {
	case 0:
		diff = port->A.in - port->B.in;
		port->B.out = lwdf_multiply(alpha,diff) + port->B.in;
		port->A.out = port->B.out - diff;
		break;
	case 1:
		diff = lwdf_multiply(alpha,(port->B.in - port->A.in));
		port->B.out = diff + port->A.in;
		port->A.out = diff + port->B.in;
		break;
	case 2:
		diff = lwdf_multiply(alpha,(port->A.in - port->B.in));
		port->B.out = diff - port->A.in;
		port->A.out = diff - port->B.in;
		break;
	case 3:
		diff = port->B.in - port->A.in;
		port->B.out = lwdf_multiply(alpha,diff) - port->B.in;
		port->A.out = port->B.out - diff;
		break;
	default:
		// Set output to zero for any issues
		port->A.out = 0;
		port->B.out = 0;
		break;
	}
	return;
}

void lwdf_initFilter(LWDF_FILTER* filter, uint8_t order, LWDF_ALPHA *alphas, uint8_t *types) {
	// Don't bother with even order filters
	if ((order%2)==0) {
		// TODO: need some error handling
		return;
	}
	// Create a new filter structure
	filter->order = order;
	uint8_t i;
	for (i=0;i<order;i++) {
		// Initialise all adaptors
		filter->alphas[i] = alphas[i];
		filter->types[i] = types[i];
		// Initialise all registers to zero
		filter->registers[i] = 0;
	}
	return;
}
LWDF_FILTER* lwdf_newFilter(void) {
	return (LWDF_FILTER*)malloc(sizeof(LWDF_FILTER));
}
void lwdf_delFilter(LWDF_FILTER* filter) {
	free(filter);
}
void lwdf_write(LWDF_FILTER* filter, int16_t input) {
	LWDF_TYPE temp_value;
	LWDF_PORT temp_port;
	uint8_t i,j,k;
	// Process the real-pole
	temp_port.A.in = ((LWDF_TYPE)input<<16);
	temp_port.B.in = filter->registers[0];
	lwdf_processAdaptor(filter->alphas[0],filter->types[0],&temp_port);
	temp_value = temp_port.A.out;
	filter->registers[0] = temp_port.B.out;
	// Process the top branch
	for (i=0; i<(filter->order>>2); i++) {
		// Get register addresses
		j = (i<<2)+3;
		k = j + 1;
		// Process the end adaptor
		temp_port.A.in = filter->registers[j];
		temp_port.B.in = filter->registers[k];
		lwdf_processAdaptor(filter->alphas[k],filter->types[k],&temp_port);
		filter->registers[k] = temp_port.B.out;
		// Process in input adaptor
		temp_port.A.in = temp_value;
		temp_port.B.in = temp_port.A.out;
		lwdf_processAdaptor(filter->alphas[j],filter->types[j],&temp_port);
		temp_value = temp_port.A.out;
		filter->registers[j] = temp_port.B.out;
	}
	filter->output[0] = temp_value;
	// Process the bottom branch
	temp_value = ((LWDF_TYPE)input<<16);
	for (i=0; i<((filter->order+1)>>2); i++) {
		// Get register addresses
		j = (i<<2)+1;
		k = j + 1;
		// Process the end adaptor
		temp_port.A.in = filter->registers[j];
		temp_port.B.in = filter->registers[k];
		lwdf_processAdaptor(filter->alphas[k],filter->types[k],&temp_port);
		filter->registers[k] = temp_port.B.out;
		// Process in input adaptor
		temp_port.A.in = temp_value;
		temp_port.B.in = temp_port.A.out;
		lwdf_processAdaptor(filter->alphas[j],filter->types[j],&temp_port);
		temp_value = temp_port.A.out;
		filter->registers[j] = temp_port.B.out;
	}
	filter->output[1] = temp_value;
}
LWDF_TYPE* lwdf_read(LWDF_FILTER* filter) {
	return filter->output;
}
LWDF_TYPE lwdf_read_lpf(LWDF_FILTER* filter) {
	return (filter->output[0] + filter->output[1] + 1)>>1;
}
LWDF_TYPE lwdf_read_hpf(LWDF_FILTER* filter) {
	return (filter->output[0] - filter->output[1] + 1)>>1;
}


/**
 * @file
 * @brief Library for Quadrature Delineation
 *
 * @author James A. C. Patterson
 * @version 0.1
 * @date 30/12/2011
 *
 * @section LICENSE
 *
 * Free Beer License
 *
 */

#include <stdlib.h>
#include "quad_delineator.h"

DELINEATOR* quad_new(void) {
	// Create a new delineator structure
	DELINEATOR* delineator;
	delineator = malloc(sizeof(DELINEATOR));
	if (delineator==NULL) return NULL;
	return delineator;
}
void quad_del(DELINEATOR* delineator) {
	free(delineator);
}
void quad_init(DELINEATOR* delineator, DELIN_VALUE delta_raw, DELIN_VALUE delta_diff) {
	// Store parametric variables
	delineator->delta[0] = delta_raw;
	delineator->delta[1] = delta_diff;
	// Reset all other registers
	delineator->previous_value = 0;
	delineator->output.log = 0;
	delineator->output.time = 0;
	unsigned int i;
	for (i=0;i<2;i++) {
		delineator->track[i].log = 0;
		delineator->track[i].time = 0;
		delineator->quad.peak[i] = 0;
		delineator->quad.phase[i] = 0;
		delineator->quad.time[i] = 0;
		delineator->quad.log[i] = 0;
	}
	for (i=2;i<4;i++) {
		delineator->quad.time[i] = 0;
		delineator->quad.log[i] = 0;

	}
}

uint8_t quad_write(DELINEATOR* delineator, DELIN_VALUE value) {
	// Local variables
	uint8_t flags = 0;

	// Haar; log derivative calculation
	DELIN_VALUE dif = value - delineator->previous_value;
	DELIN_VALUE logd = (value + delineator->previous_value);
	// Log derivative conversion
	logd = (dif<<16)/((logd+(1<<7))>>8);
	// Store history for future Haar calculations
	delineator->previous_value = value;

	// Integrate time
	unsigned int i;
	for (i=0;i<4;i++) {
		delineator->quad.time[i]++;
		delineator->quad.log[i] += logd;
	}
	// Normal phase processing
	if (delineator->quad.phase[0]==0) {
		if (value > delineator->quad.peak[0]) {
			delineator->quad.peak[0] = value;
			// Save current state on detection of a new peak
			delineator->track[0].value = logd;
			delineator->track[0].time = delineator->quad.time[3];
			delineator->track[0].log = delineator->quad.log[3];
			// Hold other integrators in reset
			delineator->quad.time[1]=0;
			delineator->quad.log[1]=0;
		}
		else if (((delineator->quad.peak[0]-value) > delineator->delta[0])
				&& (delineator->quad.phase[1]==1)) {
			delineator->quad.peak[0] = value;
			// State transition
			delineator->quad.phase[0] = 1;
			// Copy tracked values to output
			delineator->output = delineator->track[0];
			flags |= (1<<0);
		}
	}
	else if (delineator->quad.phase[0]==1) {
		if (value < delineator->quad.peak[0]) {
			delineator->quad.peak[0] = value;
			// Save current state on detection of a new peak
			delineator->track[0].value = logd;
			delineator->track[0].time = delineator->quad.time[2];
			delineator->track[0].log = delineator->quad.log[2];
			// Hold other integrators in reset
			delineator->quad.time[0]=0;
			delineator->quad.log[0]=0;
		}
		else if (((value-delineator->quad.peak[0]) > delineator->delta[0])
			&& (delineator->quad.phase[1]==0)) {
			delineator->quad.peak[0] = value;
			// State transition
			delineator->quad.phase[0] = 0;
			// Copy tracked values to output
			delineator->output = delineator->track[0];
			flags |= (1<<1);
		}
	}
	// Differentiated phase processing
	if (delineator->quad.phase[1]==0) {
		if (dif > delineator->quad.peak[1]) {
			delineator->quad.peak[1] = dif;
			// Save current state on detection of a new peak
			delineator->track[1].value = logd;
			delineator->track[1].time = delineator->quad.time[0];
			delineator->track[1].log = delineator->quad.log[0];
			// Hold other integrators in reset
			delineator->quad.time[3]=0;
			delineator->quad.log[3]=0;
		}
		else if (((delineator->quad.peak[1]-dif) > delineator->delta[1])
			&& (delineator->quad.phase[0]==0)) {
			delineator->quad.peak[1] = dif;
			// State transition
			delineator->quad.phase[1] = 1;
			// Copy tracked values to output
			delineator->output = delineator->track[1];
			flags |= (1<<2);
		}
	}
	else if (delineator->quad.phase[1]==1) {
		if (dif < delineator->quad.peak[1]) {
			delineator->quad.peak[1] = dif;
			// Save current state on detection of a new peak
			delineator->track[1].value = logd;
			delineator->track[1].time = delineator->quad.time[1];
			delineator->track[1].log = delineator->quad.log[1];
			// Hold other integrators in reset
			delineator->quad.time[2]=0;
			delineator->quad.log[2]=0;
		}
		else if (((dif-delineator->quad.peak[1]) > delineator->delta[1])
			&& (delineator->quad.phase[0]==1)) {
			delineator->quad.peak[1] = dif;
			// State transition
			delineator->quad.phase[1] = 0;
			// Copy tracked values to output
			delineator->output = delineator->track[1];
			flags |= (1<<3);
		}
	}
	return flags;
}

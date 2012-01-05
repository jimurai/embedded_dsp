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
void quad_init(DELINEATOR* delineator, DELIN_VALUE delta_x, DELIN_VALUE delta_dx) {
	// Store parametric variables
	delineator->delta[0] = delta_x;
	delineator->delta[1] = delta_dx;
	// Reset all other registers
	delineator->state[0] = 0;
	delineator->state[1] = 0;
	delineator->peak[0] = 0;
	delineator->peak[1] = 0;
}
uint8_t quad_write(DELINEATOR* delineator, DELIN_VALUE x, DELIN_VALUE dx) {
	// By default clear the tracking and valid flags
	delineator->state[0] &= ~(QD_STATE_TRACKING|QD_STATE_VALID);
	delineator->state[1] &= ~(QD_STATE_TRACKING|QD_STATE_VALID);
	// Normal phase processing
	if ((delineator->state[0]&QD_STATE_PHASE)==0) {
		if (x > delineator->peak[0]) {
			delineator->state[0] |= QD_STATE_TRACKING;
			delineator->peak[0] = x;
		}
		else if (((delineator->peak[0]-x) > delineator->delta[0])
				&& ((delineator->state[1]&QD_STATE_PHASE)!=0)) {
			delineator->peak[0] = x;
			// State transition
			delineator->state[0] |= QD_STATE_PHASE;
			// Copy tracked values to output
			delineator->state[0] |= QD_STATE_VALID;
		}
	}
	else if ((delineator->state[0]&QD_STATE_PHASE)!=0) {
		if (x < delineator->peak[0]) {
			delineator->state[0] |= QD_STATE_TRACKING;
			delineator->peak[0] = x;
		}
		else if (((x-delineator->peak[0]) > delineator->delta[0])
			&& ((delineator->state[1]&QD_STATE_PHASE)==0)) {
			delineator->peak[0] = x;
			// State transition
			delineator->state[0] &= ~QD_STATE_PHASE;
			// Copy tracked values to output
			delineator->state[0] |= QD_STATE_VALID;
		}
	}
	// Differentiated phase processing
	if ((delineator->state[1]&QD_STATE_PHASE)==0) {
		if (dx > delineator->peak[1]) {
			delineator->state[1] |= QD_STATE_TRACKING;
			delineator->peak[1] = dx;
		}
		else if (((delineator->peak[1]-dx) > delineator->delta[1])
			&& ((delineator->state[0]&QD_STATE_PHASE)==0)) {
			delineator->peak[1] = dx;
			// State transition
			delineator->state[1] |= QD_STATE_PHASE;
			// Copy tracked values to output
			delineator->state[1] |= QD_STATE_VALID;
		}
	}
	else if ((delineator->state[1]&QD_STATE_PHASE)!=0) {
		if (dx < delineator->peak[1]) {
			delineator->state[1] |= QD_STATE_TRACKING;
			delineator->peak[1] = dx;
		}
		else if (((dx-delineator->peak[1]) > delineator->delta[1])
			&& ((delineator->state[0]&QD_STATE_PHASE)!=0)) {
			delineator->peak[1] = dx;
			// State transition
			delineator->state[1] &= ~QD_STATE_PHASE;
			// Copy tracked values to output
			delineator->state[1] |= QD_STATE_VALID;
		}
	}
	return delineator->state[0] + (delineator->state[1]<<4);
}

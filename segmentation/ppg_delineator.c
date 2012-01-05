/**
 * @file
 * @brief Library for Photoplethysmography Quadrature Delineation
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
#include "ppg_delineator.h"

PPG_DELINEATOR* ppgd_new(void) {
	// Create a new delineator structure
	PPG_DELINEATOR* delineator;
	delineator = malloc(sizeof(PPG_DELINEATOR));
	if (delineator==NULL) return NULL;
	return delineator;
}
void ppgd_del(PPG_DELINEATOR* delineator) {
	free(delineator);
}
void ppgd_init(PPG_DELINEATOR* delineator, DELIN_VALUE delta_x, DELIN_VALUE delta_dx) {
	unsigned int i;
	delineator->x[0] = 0;
	delineator->x[1] = 0;
	for (i=0;i<4;i++) {
		delineator->time[i] = 0;
		delineator->value[i] = 0;
	}
	quad_init(&delineator->delineator,delta_x,delta_dx);
}
uint8_t ppgd_write(PPG_DELINEATOR* delineator, DELIN_VALUE value) {
	// Return value
	uint8_t result = 0;

	// Log derivative calculation
	DELIN_VALUE dif = value - delineator->x[0];
	DELIN_VALUE logd = (value + delineator->x[0]);
	// Log derivative conversion
	logd = (dif<<16)/((logd+(1<<7))>>8);
	// Store history for future Haar calculations
	delineator->x[1] = delineator->x[0];
	delineator->x[0] = value;

	// Integrate time &
	unsigned int i;
	for (i=0;i<4;i++) {
		delineator->time[i]++;
		delineator->value[i] += logd;
	}

	// Perform quadrature delineation on the input signal
	uint8_t res = quad_write(&delineator->delineator,value,dif);

	// Process integrators with delineator state machine outputs

	// - Nominal peak detector
	if ((res&QD_STATE_TRACKING)==0) {
		// Track log conversion and integrator outputs
		delineator->track[0].x = logd;
		if ((res&QD_STATE_PHASE)==0) {
			delineator->track[0].t = delineator->time[3];
			delineator->track[0].dx = delineator->value[3];
			// Hold other integrators in reset
			delineator->time[1]=0;
			delineator->value[1]=0;
		}
		else {
			delineator->track[0].t = delineator->time[2];
			delineator->track[0].dx = delineator->value[2];
			// Hold other integrators in reset
			delineator->time[0]=0;
			delineator->value[0]=0;
		}

	}
	if ((res&QD_STATE_VALID)==0) {
		// Copy tracked values to output register
		delineator->output = delineator->track[0];
		// Remember phase has been changed since last peak was validated
		if ((res&QD_STATE_PHASE)==0) result += 2;
		else result += 1;
	}

	// - Derivative peak detector
	if ((res&(QD_STATE_TRACKING<<4))==0) {
		// Track log conversion and integrator outputs
		delineator->track[1].x = logd;
		if ((res&(QD_STATE_PHASE<<4))==0) {
			delineator->track[1].t = delineator->time[0];
			delineator->track[1].dx = delineator->value[0];
			// Hold other integrators in reset
			delineator->time[3]=0;
			delineator->value[3]=0;
		}
		else {
			delineator->track[1].t = delineator->time[1];
			delineator->track[1].dx = delineator->value[1];
			// Hold other integrators in reset
			delineator->time[2]=0;
			delineator->value[2]=0;

		}

	}
	if ((res&(QD_STATE_VALID<<4))==0) {
		// Copy tracked values to output register
		delineator->output = delineator->track[1];
		// Remember phase has been changed since last peak was validated
		if ((res&(QD_STATE_PHASE<<4))==0) result += 4;
		else result += 3;
	}

	return result;
}

/**
 * @file
 * @brief Peak detector library with embedded integrator
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

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "i_peak_detector.h"

I_PEAK_DETECTOR* ipd_new(void) {
	I_PEAK_DETECTOR* detector;
	// Create a new detector structure
	detector = malloc(sizeof(I_PEAK_DETECTOR));
	if (detector==NULL) return NULL;
	return detector;
}
void ipd_init(I_PEAK_DETECTOR* detector, PEAK_VALUE delta) {
	// Initialise detector variables
	detector->integrator[0] = 0;
	detector->integrator[1] = 0;
	detector->hold[0] = 0;
	detector->hold[1] = 0;
	detector->output[0] = 0;
	detector->output[1] = 0;
	detector->last_state = PD_STATE_T_RISE;
	pd_init(&detector->detector,delta);
}
void ipd_del(I_PEAK_DETECTOR* detector) {
	free(detector);
}
uint8_t ipd_write(I_PEAK_DETECTOR* detector, PEAK_VALUE value, PEAK_VALUE integrand) {
	// Process the peak detector
	PEAK_STATE state = pd_write(&detector->detector,value);
	uint8_t output = 0;
	// Maxima validated
	if ((state==PD_STATE_T_FALL) && (detector->last_state==PD_STATE_V_RISE))
		output = 1;
	// Minima validated
	if ((state==PD_STATE_T_RISE) && (detector->last_state==PD_STATE_V_FALL))
		output = 2;
	// Latch values
	if (output!=0) {
		detector->output[0] = detector->hold[0];
		detector->output[1] = detector->hold[1];
	}
	// State dependent register transfers
	switch (state) {
	case PD_STATE_T_RISE:	// Tracking rising value
		detector->hold[0] = value;
		detector->hold[1] = detector->integrator[0];
		detector->integrator[1] = 0;
		break;
	case PD_STATE_V_RISE:	// Validate maxima
		break;
	case PD_STATE_T_FALL:	// Tracking falling value
		detector->hold[0] = value;
		detector->hold[1] = detector->integrator[1];
		detector->integrator[0] = 0;
		break;
	case PD_STATE_V_FALL:	// Validate minima
		break;
	default:
		break;
	}
	// Integrate the integrand (generally number of samples since last write = 1)
	detector->integrator[0] += integrand;
	detector->integrator[1] += integrand;
	// Record the last state
	detector->last_state=state;
	return output;
}

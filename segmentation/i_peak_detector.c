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
	detector->down = 0;
	detector->up = 0;
	detector->hold = 0;
	detector->output = 0;
	pd_init(&detector->detector,delta);
}
void ipd_del(I_PEAK_DETECTOR* detector) {
	free(detector);
}
void ipd_write(I_PEAK_DETECTOR* detector, PEAK_VALUE value, PEAK_VALUE integrand) {
	// Integrate the integrand (generally number of samples since last write = 1)
	detector->up += integrand;
	detector->down += integrand;
	// Process the peak detector
	pd_write(&detector->detector,value);
	// State dependent register transfers
	switch (detector->detector.state) {
	case PD_STATE_TRACKING:
		// Track input value & reset alt integrator
		if (detector->detector.mode==PD_MODE_UP) {
			detector->hold = detector->up;
			detector->down = 0;
		}
		else if (detector->detector.mode==PD_MODE_DOWN) {
			detector->hold = detector->down;
			detector->up = 0;
		}
		break;
	case PD_STATE_VALID:
		// Latch peak to output register
		detector->output = detector->hold;
		break;
	case PD_STATE_HOLDING:
	default:
		break;
	}
}

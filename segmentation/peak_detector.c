/**
 * @file
 * @brief Peak Detector library
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

#include "peak_detector.h"

/**
 *	Peak detector definitions
 */
PEAK_DETECTOR* pd_new(void) {
	// Create a new detector structure
	PEAK_DETECTOR* detector;
	detector = malloc(sizeof(PEAK_DETECTOR));
	if (detector==NULL) return NULL;
	return detector;
}
void pd_init(PEAK_DETECTOR* detector,PEAK_VALUE delta) {
	// Initialise detector variables
	detector->delta = delta;
	detector->output = 0;
	detector->peak = 0;
	detector->state = PD_MODE_UP;
	detector->mode = PD_STATE_HOLDING;
}
void pd_del(PEAK_DETECTOR* detector) {
	free(detector);
}
void pd_write(PEAK_DETECTOR* detector, PEAK_VALUE value) {
	// State transitions
	// Check if looking for a maximum
	if (detector->mode==PD_MODE_UP) {
		// Check for turning point
		if (value > detector->peak) {
			detector->state = PD_STATE_TRACKING;
		}
		// Check if previous peak validated
		else if ((detector->peak-value) > detector->delta) {
			detector->state = PD_STATE_VALID;
		}
		// Hold on to last peak value
		else {
			detector->state = PD_STATE_HOLDING;
		}
	}
	else if (detector->mode==PD_MODE_DOWN) {
		// Check for turning point
		if (value < detector->peak) {
			detector->state = PD_STATE_TRACKING;
		}
		// Check if previous peak validated
		else if ((value-detector->peak) > detector->delta) {
			detector->state = PD_STATE_VALID;
		}
		// Hold on to last peak value
		else {
			detector->state = PD_STATE_HOLDING;
		}
	}
	// State dependent register transfers
	switch (detector->state) {
	case PD_STATE_TRACKING:
		// Track input value
		detector->peak = value;
		break;
	case PD_STATE_VALID:
		// Latch peak to output register
		detector->output = detector->peak;
		// Toggle mode
		if (detector->mode==PD_MODE_UP) detector->mode = PD_MODE_DOWN;
		else detector->mode = PD_MODE_UP;
		break;
	case PD_STATE_HOLDING:
	default:
		break;
	}
}
void pd_reset(PEAK_DETECTOR* detector, PEAK_VALUE value, PEAK_MODE mode) {
	detector->peak = value;
	detector->mode = mode;
}

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
	detector->peak = 0;
	detector->state = PD_STATE_T_RISE;
}
void pd_del(PEAK_DETECTOR* detector) {
	free(detector);
}
PEAK_STATE pd_write(PEAK_DETECTOR* detector, PEAK_VALUE value) {
	// State transitions
	switch (detector->state) {
	case PD_STATE_T_RISE:	// Track rising value
		if (value<detector->peak)
			detector->state = PD_STATE_V_RISE;
		break;
	case PD_STATE_V_RISE:	// Validate maxima
		if (value>detector->peak)
			detector->state = PD_STATE_T_RISE;
		else if ((detector->peak-value)>detector->delta)
			detector->state = PD_STATE_T_FALL;
		break;
	case PD_STATE_T_FALL:	// Track falling value
		if (value>detector->peak)
			detector->state = PD_STATE_V_FALL;
		break;
	case PD_STATE_V_FALL:	// Validate minima
		if (value<detector->peak)
			detector->state = PD_STATE_T_FALL;
		else if ((value-detector->peak)>detector->delta)
			detector->state = PD_STATE_T_RISE;
		break;
	default:
		detector->state = PD_STATE_T_RISE;
		break;
	}
	// State based register transfers
	switch (detector->state) {
	case PD_STATE_T_RISE:	// Tracking rising value
		detector->peak = value;
		break;
	case PD_STATE_T_FALL:	// Tracking falling value
		detector->peak = value;
		break;
	default:
		break;
	}
	return detector->state;
}
void pd_reset(PEAK_DETECTOR* detector, PEAK_VALUE value, PEAK_STATE state) {
	detector->peak = value;
	detector->state = state;
}

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

#ifndef PEAK_DETECTOR_H
#define PEAK_DETECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
/**
 *	Peak detector declarations
 */
typedef int32_t PEAK_VALUE;
typedef int8_t PEAK_MODE;
typedef uint8_t PEAK_STATE;

#define PD_STATE_TRACKING	0
#define PD_STATE_HOLDING	1
#define PD_STATE_VALID		2

#define PD_MODE_OFF		0	// Unused
#define PD_MODE_UP		1
#define PD_MODE_DOWN	-1

typedef struct peak_detector {
	PEAK_VALUE peak;	// R
	PEAK_VALUE output;	// R
	PEAK_VALUE delta;	// R/W
	PEAK_MODE mode;		// R/W
	PEAK_STATE state;	// R
} PEAK_DETECTOR;

PEAK_DETECTOR* pd_new(void);
void pd_init(PEAK_DETECTOR* detector,PEAK_VALUE delta);
void pd_del(PEAK_DETECTOR* detector);
void pd_write(PEAK_DETECTOR* detector, PEAK_VALUE value);
void pd_reset(PEAK_DETECTOR* detector, PEAK_VALUE value, PEAK_MODE mode);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* PEAK_DETECTOR_H */

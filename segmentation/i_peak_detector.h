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

#ifndef I_PEAK_DETECTOR_H
#define I_PEAK_DETECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "peak_detector.h"

typedef struct i_detector {
	uint32_t down;
	uint32_t up;
	uint32_t hold;
	uint32_t output;
	PEAK_DETECTOR detector;
} I_PEAK_DETECTOR;

I_PEAK_DETECTOR* ipd_new(void);
void ipd_init(I_PEAK_DETECTOR* detector,PEAK_VALUE delta);
void ipd_del(I_PEAK_DETECTOR* detector);
void ipd_write(I_PEAK_DETECTOR* detector, PEAK_VALUE value, PEAK_VALUE integrand);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* PEAK_DETECTOR_H */

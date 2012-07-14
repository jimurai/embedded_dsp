/**
 * @file
 * @brief PPG delineation library
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

#ifndef PPG_DELINEATOR_H
#define PPG_DELINEATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "quad_delineator.h"

typedef struct delineation_data {
	DELIN_VALUE x;
	DELIN_VALUE dx;
	DELIN_TIME t;
} PPG_DATA;

typedef struct ppg_delineator_machine {
	DELIN_VALUE x[2];
	PPG_DATA track[2];
	PPG_DATA output;
	DELIN_TIME time[4];
	DELIN_VALUE value[4];
	DELINEATOR delineator;
} PPG_DELINEATOR;

void ppgd_init(PPG_DELINEATOR* delineator, DELIN_VALUE delta_x, DELIN_VALUE delta_dx);
uint8_t ppgd_write(PPG_DELINEATOR* delineator, DELIN_VALUE value);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* PPG_DELINEATOR_H */

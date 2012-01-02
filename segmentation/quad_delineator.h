/**
 * @file
 * @brief QUAD delineation library
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

#ifndef QUAD_DELINEATOR_H
#define QUAD_DELINEATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint32_t DELIN_TIME;
typedef int32_t DELIN_VALUE;

typedef struct delineation_data {
	DELIN_VALUE value;
	DELIN_VALUE log;
	DELIN_TIME time;
} DELIN_DATA;

typedef struct delineation_quadrature {
	uint8_t phase[2];
	DELIN_VALUE peak[2];
	DELIN_TIME time[4];
	DELIN_VALUE log[4];
} QUADRATURE;

typedef struct delineator_machine {
	DELIN_VALUE previous_value;
	DELIN_VALUE delta[2];
	DELIN_DATA track[2];
	DELIN_DATA output;
	QUADRATURE quad;
} DELINEATOR;

DELINEATOR* quad_new(void);
void quad_del(DELINEATOR* delineator);
void quad_init(DELINEATOR* delineator, DELIN_VALUE delta_raw, DELIN_VALUE delta_diff);
uint8_t quad_write(DELINEATOR* delineator, DELIN_VALUE value);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* QUAD_DELINEATOR_H */

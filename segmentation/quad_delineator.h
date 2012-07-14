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

#define QD_STATE_PHASE		(1<<0)
#define QD_STATE_TRACKING	(1<<1)
#define QD_STATE_VALID		(1<<2)

typedef struct delineator_machine {
	uint8_t state[2];
	DELIN_VALUE delta[2];
	DELIN_VALUE peak[2];
} DELINEATOR;

void quad_init(DELINEATOR* delineator, DELIN_VALUE delta_x, DELIN_VALUE delta_dx);
uint8_t quad_write(DELINEATOR* delineator, DELIN_VALUE x, DELIN_VALUE dx);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* QUAD_DELINEATOR_H */

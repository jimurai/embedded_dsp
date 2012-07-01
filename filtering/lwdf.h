/**
 * @file
 * @brief Lattice Wave Digital Filter library
 *
 * @author James A. C. Patterson
 * @version 0.1
 * @date 12/12/2011
 *
 * @section LICENSE
 *
 * Free Beer License
 *
 */

#ifndef LWDF_H
#define LWDF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 *	Lattice Wave Digital Filter declarations
 */
#define LWDF_MAX_ORDER 9
typedef int32_t LWDF_TYPE;
typedef uint16_t LWDF_ALPHA;

typedef struct wave {
	LWDF_TYPE in;
	LWDF_TYPE out;
} LWDF_WAVE;
typedef struct port {
	LWDF_WAVE A;
	LWDF_WAVE B;
} LWDF_PORT;

typedef struct filter{
	uint8_t order;
	LWDF_ALPHA alphas[LWDF_MAX_ORDER];
	uint8_t types[LWDF_MAX_ORDER];
	LWDF_TYPE registers[LWDF_MAX_ORDER];
	LWDF_TYPE output[2];
} LWDF_FILTER;

void lwdf_initFilter(LWDF_FILTER* filter, uint8_t order, LWDF_ALPHA *alphas, uint8_t *types);
void lwdf_write(LWDF_FILTER* filter, int16_t input);
LWDF_TYPE* lwdf_read(LWDF_FILTER* filter);
LWDF_TYPE lwdf_read_lpf(LWDF_FILTER* filter);
LWDF_TYPE lwdf_read_hpf(LWDF_FILTER* filter);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* LWDF_H */

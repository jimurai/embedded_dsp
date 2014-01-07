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
 * Copyright (c) 2011 Jim Patterson
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

void lwdf_gamma2alpha(float gamma, LWDF_ALPHA *alpha, uint8_t *type);
void lwdf_initFilter(LWDF_FILTER* filter, uint8_t order, LWDF_ALPHA *alphas, uint8_t *types);
LWDF_FILTER* lwdf_newFilter(void);
void lwdf_delFilter(LWDF_FILTER* filter);
void lwdf_write(LWDF_FILTER* filter, int16_t input);
LWDF_TYPE* lwdf_read(LWDF_FILTER* filter);
LWDF_TYPE lwdf_read_lpf(LWDF_FILTER* filter);
LWDF_TYPE lwdf_read_hpf(LWDF_FILTER* filter);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* LWDF_H */

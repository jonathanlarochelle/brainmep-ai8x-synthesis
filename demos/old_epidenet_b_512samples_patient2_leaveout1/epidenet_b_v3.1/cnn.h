/**************************************************************************************************
* Copyright (C) 2019-2021 Maxim Integrated Products, Inc. All Rights Reserved.
*
* Maxim Integrated Products, Inc. Default Copyright Notice:
* https://www.maximintegrated.com/en/aboutus/legal/copyrights.html
**************************************************************************************************/

/*
 * This header file was automatically @generated for the epidenet_b_v3.1 network from a template.
 * Please do not edit; instead, edit the template and regenerate.
 */

#ifndef __CNN_H__
#define __CNN_H__

#include <stdint.h>
typedef int32_t q31_t;
typedef int16_t q15_t;

/* Return codes */
#define CNN_FAIL 0
#define CNN_OK 1

/*
  SUMMARY OF OPS
  Hardware: 189,284 ops (182,064 macc; 7,044 comp; 176 add; 0 mul; 0 bitwise)
    Layer 0 (block1_conv1d_bn_relu_1): 10,260 ops (8,208 macc; 2,052 comp; 0 add; 0 mul; 0 bitwise)
    Layer 1 (block1_maxpool1d_1): 2,048 ops (0 macc; 2,048 comp; 0 add; 0 mul; 0 bitwise)
    Layer 2 (block2_conv1d_bn_relu_1): 35,520 ops (34,560 macc; 960 comp; 0 add; 0 mul; 0 bitwise)
    Layer 3 (block2_conv1d_bn_relu_2): 117,648 ops (116,736 macc; 912 comp; 0 add; 0 mul; 0 bitwise)
    Layer 4 (block2_maxpool1d_1): 896 ops (0 macc; 896 comp; 0 add; 0 mul; 0 bitwise)
    Layer 5 (block3_conv1d_bn_relu_1): 22,704 ops (22,528 macc; 176 comp; 0 add; 0 mul; 0 bitwise)
    Layer 6 (block5_avgpool1d): 176 ops (0 macc; 0 comp; 176 add; 0 mul; 0 bitwise)
    Layer 7 (block6_dense): 32 ops (32 macc; 0 comp; 0 add; 0 mul; 0 bitwise)

  RESOURCE USAGE
  Weight memory: 4,720 bytes out of 442,368 bytes total (1.1%)
  Bias memory:   52 bytes out of 2,048 bytes total (2.5%)
*/

/* Number of outputs for this network */
#define CNN_NUM_OUTPUTS 2

/* Port pin actions used to signal that processing is active */

#define CNN_START LED_On(1)
#define CNN_COMPLETE LED_Off(1)
#define SYS_START LED_On(0)
#define SYS_COMPLETE LED_Off(0)

/* Run software SoftMax on unloaded data */
void softmax_q17p14_q15(const q31_t * vec_in, const uint16_t dim_vec, q15_t * p_out);
/* Shift the input, then calculate SoftMax */
void softmax_shift_q17p14_q15(q31_t * vec_in, const uint16_t dim_vec, uint8_t in_shift, q15_t * p_out);

/* Stopwatch - holds the runtime when accelerator finishes */
extern volatile uint32_t cnn_time;

/* Custom memcopy routines used for weights and data */
void memcpy32(uint32_t *dst, const uint32_t *src, int n);
void memcpy32_const(uint32_t *dst, int n);

/* Enable clocks and power to accelerator, enable interrupt */
int cnn_enable(uint32_t clock_source, uint32_t clock_divider);

/* Disable clocks and power to accelerator */
int cnn_disable(void);

/* Perform minimum accelerator initialization so it can be configured */
int cnn_init(void);

/* Configure accelerator for the given network */
int cnn_configure(void);

/* Load accelerator weights */
int cnn_load_weights(void);

/* Verify accelerator weights (debug only) */
int cnn_verify_weights(void);

/* Load accelerator bias values (if needed) */
int cnn_load_bias(void);

/* Start accelerator processing */
int cnn_start(void);

/* Force stop accelerator */
int cnn_stop(void);

/* Continue accelerator after stop */
int cnn_continue(void);

/* Unload results from accelerator */
int cnn_unload(uint32_t *out_buf);

/* Turn on the boost circuit */
int cnn_boost_enable(mxc_gpio_regs_t *port, uint32_t pin);

/* Turn off the boost circuit */
int cnn_boost_disable(mxc_gpio_regs_t *port, uint32_t pin);

#endif // __CNN_H__
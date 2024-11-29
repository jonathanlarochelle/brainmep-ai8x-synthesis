/*******************************************************************************
* Copyright (C) 2019-2023 Maxim Integrated Products, Inc., All rights Reserved.
*
* This software is protected by copyright laws of the United States and
* of foreign countries. This material may also be protected by patent laws
* and technology transfer regulations of the United States and of foreign
* countries. This software is furnished under a license agreement and/or a
* nondisclosure agreement and may only be used or reproduced in accordance
* with the terms of those agreements. Dissemination of this information to
* any party or parties not specified in the license agreement and/or
* nondisclosure agreement is expressly prohibited.
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************/

// ep_v1.6_768_och4_ker2_pad0_maxp12_maxp2
// This file was @generated by ai8xize.py --verbose --test-dir demos\energy_profiling\kernel_size --prefix ep_v1.6_768_och4_ker2_pad0_maxp12_maxp2 --checkpoint-file .\trained_custom\checkpoints\energy_profiling\kernel_size\ep_v1.6_768_och4_ker2_pad0_maxp12_maxp2_qat_best-q.pth.tar --config-file .\networks\energy_profiling.yaml --device MAX78000 --compact-data --softmax --sample-input .\trained_custom\sample_data\energy_profiling\kernel_size\sample_chbmit_singlech_768samples_patient_5_leave_out_seizure_1_ep_v1.6_768_och4_ker2_pad0_maxp12_maxp2.npy --overwrite --energy

// DO NOT EDIT - regenerate this file instead!

// Configuring 4 layers
// Input data: CHW
// Layer 0: 1x768, no pooling, conv1d with kernel size 2, stride 1, pad 0, ReLU, 4x767 output
// Layer 1: 4x767x1, max pool 12x1 with stride 12/12, no convolution, 4x63x1 output
// Layer 2: 4x63x1, max pool 2x1 with stride 2/2, no convolution, 4x31x1 output
// Layer 3: 4x31x1 flattened to 124x1x1, no pooling, linear, no activation, 2x1x1 output

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "mxc.h"
#include "gcfr_regs.h"
#include "cnn.h"
#include "weights.h"

void CNN_ISR(void)
{
  // Acknowledge interrupt to all quadrants
  *((volatile uint32_t *) 0x50100000) &= ~((1<<12) | 1);

#ifdef CNN_INFERENCE_TIMER
  cnn_time = MXC_TMR_SW_Stop(CNN_INFERENCE_TIMER);
#else
  cnn_time = 1;
#endif
}

int cnn_continue(void)
{
  cnn_time = 0;

  *((volatile uint32_t *) 0x50100000) |= 1; // Re-enable quadrant 0

  return CNN_OK;
}

int cnn_stop(void)
{
  *((volatile uint32_t *) 0x50100000) &= ~1; // Disable quadrant 0

  return CNN_OK;
}

void memcpy32(uint32_t *dst, const uint32_t *src, int n)
{
  while (n-- > 0) {
    *dst++ = *src++;
  }
}

static const uint32_t kernels[] = KERNELS;

int cnn_load_weights(void)
{
  uint32_t len;
  volatile uint32_t *addr;
  const uint32_t *ptr = kernels;

  while ((addr = (volatile uint32_t *) *ptr++) != 0) {
    *((volatile uint8_t *) ((uint32_t) addr | 1)) = 0x01; // Set address
    len = *ptr++;
    while (len-- > 0)
      *addr++ = *ptr++;
  }

  return CNN_OK;
}

static const uint8_t bias_0[] = BIAS_0;

static void memcpy_8to32(uint32_t *dst, const uint8_t *src, int n)
{
  while (n-- > 0) {
    *dst++ = *src++;
  }
}

int cnn_load_bias(void)
{
  memcpy_8to32((uint32_t *) 0x50108000, bias_0, sizeof(uint8_t) * 4);

  return CNN_OK;
}

int cnn_init(void)
{
  *((volatile uint32_t *) 0x50001000) = 0x00000000; // AON control
  *((volatile uint32_t *) 0x50100000) = 0x00100008; // Stop SM
  *((volatile uint32_t *) 0x50100004) = 0x0000040e; // SRAM control
  *((volatile uint32_t *) 0x50100008) = 0x00000003; // Layer count

  return CNN_OK;
}

int cnn_configure(void)
{
  // Layer 0 quadrant 0
  *((volatile uint32_t *) 0x50100010) = 0x000002ff; // Rows
  *((volatile uint32_t *) 0x50100310) = 0x00001000; // SRAM write ptr
  *((volatile uint32_t *) 0x50100410) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x50100590) = 0x00000b60; // Layer control
  *((volatile uint32_t *) 0x50100a10) = 0x00001800; // Layer control 2
  *((volatile uint32_t *) 0x50100610) = 0x00000018; // Mask offset and count
  *((volatile uint32_t *) 0x50100110) = 0x00001200; // 1D
  *((volatile uint32_t *) 0x50100790) = 0x00009000; // Post processing register
  *((volatile uint32_t *) 0x50100710) = 0x00010001; // Mask and processor enables

  // Layer 1 quadrant 0
  *((volatile uint32_t *) 0x50100014) = 0x000002fe; // Rows
  *((volatile uint32_t *) 0x50100194) = 0x0000000b; // Pooling rows
  *((volatile uint32_t *) 0x50100294) = 0x0000000b; // Stride
  *((volatile uint32_t *) 0x50100394) = 0x00000800; // Write ptr time slot offs
  *((volatile uint32_t *) 0x50100514) = 0x00001000; // SRAM read ptr
  *((volatile uint32_t *) 0x50100594) = 0x000009a0; // Layer control
  *((volatile uint32_t *) 0x50100114) = 0x00000100; // 1D
  *((volatile uint32_t *) 0x50100794) = 0x03000000; // Post processing register
  *((volatile uint32_t *) 0x50100714) = 0x0000000f; // Mask and processor enables

  // Layer 2 quadrant 0
  *((volatile uint32_t *) 0x50100018) = 0x0000003e; // Rows
  *((volatile uint32_t *) 0x50100198) = 0x00000001; // Pooling rows
  *((volatile uint32_t *) 0x50100298) = 0x00000001; // Stride
  *((volatile uint32_t *) 0x50100318) = 0x00001000; // SRAM write ptr
  *((volatile uint32_t *) 0x50100398) = 0x00000800; // Write ptr time slot offs
  *((volatile uint32_t *) 0x50100598) = 0x000009a0; // Layer control
  *((volatile uint32_t *) 0x50100118) = 0x00000100; // 1D
  *((volatile uint32_t *) 0x50100798) = 0x03000000; // Post processing register
  *((volatile uint32_t *) 0x50100718) = 0x0000000f; // Mask and processor enables

  // Layer 3 quadrant 0
  *((volatile uint32_t *) 0x5010039c) = 0x00000001; // Write ptr time slot offs
  *((volatile uint32_t *) 0x5010041c) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x5010051c) = 0x00001000; // SRAM read ptr
  *((volatile uint32_t *) 0x5010059c) = 0x00010920; // Layer control
  *((volatile uint32_t *) 0x50100a1c) = 0x0000080e; // Layer control 2
  *((volatile uint32_t *) 0x5010061c) = 0x01200308; // Mask offset and count
  *((volatile uint32_t *) 0x5010011c) = 0x00000100; // 1D
  *((volatile uint32_t *) 0x5010069c) = 0x001e003d; // TRAM ptr max
  *((volatile uint32_t *) 0x5010079c) = 0x08064000; // Post processing register
  *((volatile uint32_t *) 0x5010071c) = 0x000f000f; // Mask and processor enables


  return CNN_OK;
}

int cnn_start(void)
{
  cnn_time = 0;

  *((volatile uint32_t *) 0x50100000) = 0x00100808; // Enable quadrant 0

#ifdef CNN_INFERENCE_TIMER
  MXC_TMR_SW_Start(CNN_INFERENCE_TIMER);
#endif

  *((volatile uint32_t *) 0x50100000) = 0x00100009; // Master enable quadrant 0

  return CNN_OK;
}

int cnn_unload(uint32_t *out_buf)
{
  volatile uint32_t *addr;

  // Custom unload for this network, layer 3 (block6_dense): 32-bit data, shape: (2, 1, 1)
  addr = (volatile uint32_t *) 0x50400000;
  *out_buf++ = *addr++;
  *out_buf++ = *addr++;

  return CNN_OK;
}

int cnn_enable(uint32_t clock_source, uint32_t clock_divider)
{
  // Reset all domains, restore power to CNN
  MXC_GCFR->reg3 = 0xf; // Reset
  MXC_GCFR->reg1 = 0x1; // Mask memory
  MXC_GCFR->reg0 = 0x1; // Power
  MXC_GCFR->reg2 = 0xe; // Iso
  MXC_GCFR->reg3 = 0x0; // Reset

  MXC_GCR->pclkdiv = (MXC_GCR->pclkdiv & ~(MXC_F_GCR_PCLKDIV_CNNCLKDIV | MXC_F_GCR_PCLKDIV_CNNCLKSEL))
                     | clock_divider | clock_source;
  MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_CNN); // Enable CNN clock

  MXC_NVIC_SetVector(CNN_IRQn, CNN_ISR); // Set CNN complete vector

  return CNN_OK;
}

int cnn_boost_enable(mxc_gpio_regs_t *port, uint32_t pin)
{
  mxc_gpio_cfg_t gpio_out;
  gpio_out.port = port;
  gpio_out.mask = pin;
  gpio_out.pad = MXC_GPIO_PAD_NONE;
  gpio_out.func = MXC_GPIO_FUNC_OUT;
  MXC_GPIO_Config(&gpio_out);
  MXC_GPIO_OutSet(gpio_out.port, gpio_out.mask);

  return CNN_OK;
}

int cnn_boost_disable(mxc_gpio_regs_t *port, uint32_t pin)
{
  mxc_gpio_cfg_t gpio_out;
  gpio_out.port = port;
  gpio_out.mask = pin;
  gpio_out.pad = MXC_GPIO_PAD_NONE;
  gpio_out.func = MXC_GPIO_FUNC_OUT;
  MXC_GPIO_Config(&gpio_out);
  MXC_GPIO_OutClr(gpio_out.port, gpio_out.mask);

  return CNN_OK;
}

int cnn_disable(void)
{
  // Disable CNN clock
  MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_CNN);

  // Disable power to CNN
  MXC_GCFR->reg3 = 0xf; // Reset
  MXC_GCFR->reg2 |= 0xf; // Iso
  MXC_GCFR->reg0 = 0x0; // Power
  MXC_GCFR->reg1 = 0x0; // Mask memory
  MXC_GCFR->reg3 = 0x0; // Reset

  return CNN_OK;
}


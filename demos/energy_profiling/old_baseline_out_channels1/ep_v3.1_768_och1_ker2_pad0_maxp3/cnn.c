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

// ep_v3.1_768_och1_ker2_pad0_maxp3
// This file was @generated by ai8xize.py --verbose --test-dir demos --prefix ep_v3.1_768_och1_ker2_pad0_maxp3 --checkpoint-file .\trained_custom\checkpoints\energy_profiling\ep_v3.1_768_och1_ker2_pad0_maxp3_qat_best-q.pth.tar --config-file .\networks\energy_profiling.yaml --device MAX78000 --compact-data --softmax --sample-input .\trained_custom\sample_data\energy_profiling\sample_chbmit_singlech_768samples_patient_5_leave_out_seizure_1_ep_v3.1_768_och1_ker2_pad0_maxp3.npy --overwrite --energy

// DO NOT EDIT - regenerate this file instead!

// Configuring 5 layers
// Input data: CHW
// Layer 0: 1x768, no pooling, conv1d with kernel size 2, stride 1, pad 0, ReLU, 1x767 output
// Layer 1: 1x767, no pooling, conv1d with kernel size 2, stride 1, pad 0, ReLU, 1x766 output
// Layer 2: 1x766, no pooling, conv1d with kernel size 2, stride 1, pad 0, ReLU, 1x765 output
// Layer 3: 1x765x1, max pool 3x1 with stride 3/3, no convolution, 1x255x1 output
// Layer 4: 1x255x1 flattened to 255x1x1, no pooling, linear, no activation, 2x1x1 output

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
  *((volatile uint32_t *) 0x50500000) &= ~((1<<12) | 1);
  *((volatile uint32_t *) 0x50900000) &= ~((1<<12) | 1);

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
static const uint8_t bias_1[] = BIAS_1;
static const uint8_t bias_2[] = BIAS_2;

static void memcpy_8to32(uint32_t *dst, const uint8_t *src, int n)
{
  while (n-- > 0) {
    *dst++ = *src++;
  }
}

int cnn_load_bias(void)
{
  memcpy_8to32((uint32_t *) 0x50108000, bias_0, sizeof(uint8_t) * 1);
  memcpy_8to32((uint32_t *) 0x50508000, bias_1, sizeof(uint8_t) * 1);
  memcpy_8to32((uint32_t *) 0x50908000, bias_2, sizeof(uint8_t) * 1);

  return CNN_OK;
}

int cnn_init(void)
{
  *((volatile uint32_t *) 0x50001000) = 0x00000000; // AON control
  *((volatile uint32_t *) 0x50100000) = 0x00100008; // Stop SM
  *((volatile uint32_t *) 0x50100004) = 0x0000040e; // SRAM control
  *((volatile uint32_t *) 0x50100008) = 0x00000004; // Layer count
  *((volatile uint32_t *) 0x50500000) = 0x00100008; // Stop SM
  *((volatile uint32_t *) 0x50500004) = 0x0000040e; // SRAM control
  *((volatile uint32_t *) 0x50500008) = 0x00000004; // Layer count
  *((volatile uint32_t *) 0x50900000) = 0x00100008; // Stop SM
  *((volatile uint32_t *) 0x50900004) = 0x0000040e; // SRAM control
  *((volatile uint32_t *) 0x50900008) = 0x00000004; // Layer count

  return CNN_OK;
}

int cnn_configure(void)
{
  // Layer 0 quadrant 0
  *((volatile uint32_t *) 0x50100010) = 0x000002ff; // Rows
  *((volatile uint32_t *) 0x50100310) = 0x0000a000; // SRAM write ptr
  *((volatile uint32_t *) 0x50100410) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x50100590) = 0x00002b60; // Layer control
  *((volatile uint32_t *) 0x50100110) = 0x00001200; // 1D
  *((volatile uint32_t *) 0x50100790) = 0x00007000; // Post processing register

  // Layer 0 quadrant 1
  *((volatile uint32_t *) 0x50500010) = 0x000002ff; // Rows
  *((volatile uint32_t *) 0x50500310) = 0x0000a000; // SRAM write ptr
  *((volatile uint32_t *) 0x50500410) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x50500590) = 0x00000b60; // Layer control
  *((volatile uint32_t *) 0x50500110) = 0x00001200; // 1D
  *((volatile uint32_t *) 0x50500790) = 0x00006000; // Post processing register
  *((volatile uint32_t *) 0x50500710) = 0x00010001; // Mask and processor enables

  // Layer 0 quadrant 2
  *((volatile uint32_t *) 0x50900010) = 0x000002ff; // Rows
  *((volatile uint32_t *) 0x50900310) = 0x0000a000; // SRAM write ptr
  *((volatile uint32_t *) 0x50900410) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x50900590) = 0x00000b60; // Layer control
  *((volatile uint32_t *) 0x50900110) = 0x00001200; // 1D
  *((volatile uint32_t *) 0x50900790) = 0x00006000; // Post processing register

  // Layer 1 quadrant 0
  *((volatile uint32_t *) 0x50100014) = 0x000002fe; // Rows
  *((volatile uint32_t *) 0x50100314) = 0x0000c000; // SRAM write ptr
  *((volatile uint32_t *) 0x50100414) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x50100594) = 0x00002b20; // Layer control
  *((volatile uint32_t *) 0x50100114) = 0x00001200; // 1D
  *((volatile uint32_t *) 0x50100794) = 0x00002000; // Post processing register

  // Layer 1 quadrant 1
  *((volatile uint32_t *) 0x50500014) = 0x000002fe; // Rows
  *((volatile uint32_t *) 0x50500314) = 0x0000c000; // SRAM write ptr
  *((volatile uint32_t *) 0x50500414) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x50500594) = 0x00000b20; // Layer control
  *((volatile uint32_t *) 0x50500114) = 0x00001200; // 1D
  *((volatile uint32_t *) 0x50500794) = 0x00003000; // Post processing register
  *((volatile uint32_t *) 0x50500714) = 0x00100010; // Mask and processor enables

  // Layer 1 quadrant 2
  *((volatile uint32_t *) 0x50900014) = 0x000002fe; // Rows
  *((volatile uint32_t *) 0x50900314) = 0x0000c000; // SRAM write ptr
  *((volatile uint32_t *) 0x50900414) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x50900594) = 0x00000b20; // Layer control
  *((volatile uint32_t *) 0x50900114) = 0x00001200; // 1D
  *((volatile uint32_t *) 0x50900794) = 0x00002000; // Post processing register

  // Layer 2 quadrant 0
  *((volatile uint32_t *) 0x50100018) = 0x000002fd; // Rows
  *((volatile uint32_t *) 0x50100318) = 0x00010000; // SRAM write ptr
  *((volatile uint32_t *) 0x50100418) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x50100598) = 0x00006b20; // Layer control
  *((volatile uint32_t *) 0x50100118) = 0x00001200; // 1D
  *((volatile uint32_t *) 0x50100798) = 0x00006000; // Post processing register

  // Layer 2 quadrant 1
  *((volatile uint32_t *) 0x50500018) = 0x000002fd; // Rows
  *((volatile uint32_t *) 0x50500318) = 0x00010000; // SRAM write ptr
  *((volatile uint32_t *) 0x50500418) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x50500598) = 0x00000b20; // Layer control
  *((volatile uint32_t *) 0x50500118) = 0x00001200; // 1D
  *((volatile uint32_t *) 0x50500798) = 0x00006000; // Post processing register
  *((volatile uint32_t *) 0x50500718) = 0x01000100; // Mask and processor enables

  // Layer 2 quadrant 2
  *((volatile uint32_t *) 0x50900018) = 0x000002fd; // Rows
  *((volatile uint32_t *) 0x50900318) = 0x00010000; // SRAM write ptr
  *((volatile uint32_t *) 0x50900418) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x50900598) = 0x00000b20; // Layer control
  *((volatile uint32_t *) 0x50900118) = 0x00001200; // 1D
  *((volatile uint32_t *) 0x50900798) = 0x00007000; // Post processing register

  // Layer 3 quadrant 0
  *((volatile uint32_t *) 0x5010001c) = 0x000002fc; // Rows
  *((volatile uint32_t *) 0x5010019c) = 0x00000002; // Pooling rows
  *((volatile uint32_t *) 0x5010029c) = 0x00000002; // Stride
  *((volatile uint32_t *) 0x5010039c) = 0x00000800; // Write ptr time slot offs
  *((volatile uint32_t *) 0x5010059c) = 0x000009a0; // Layer control
  *((volatile uint32_t *) 0x5010011c) = 0x00000100; // 1D
  *((volatile uint32_t *) 0x5010079c) = 0x03000000; // Post processing register

  // Layer 3 quadrant 1
  *((volatile uint32_t *) 0x5050001c) = 0x000002fc; // Rows
  *((volatile uint32_t *) 0x5050019c) = 0x00000002; // Pooling rows
  *((volatile uint32_t *) 0x5050029c) = 0x00000002; // Stride
  *((volatile uint32_t *) 0x5050039c) = 0x00000800; // Write ptr time slot offs
  *((volatile uint32_t *) 0x5050059c) = 0x000009a0; // Layer control
  *((volatile uint32_t *) 0x5050011c) = 0x00000100; // 1D
  *((volatile uint32_t *) 0x5050079c) = 0x03000000; // Post processing register

  // Layer 3 quadrant 2
  *((volatile uint32_t *) 0x5090001c) = 0x000002fc; // Rows
  *((volatile uint32_t *) 0x5090019c) = 0x00000002; // Pooling rows
  *((volatile uint32_t *) 0x5090029c) = 0x00000002; // Stride
  *((volatile uint32_t *) 0x5090031c) = 0x00012000; // SRAM write ptr
  *((volatile uint32_t *) 0x5090039c) = 0x00000800; // Write ptr time slot offs
  *((volatile uint32_t *) 0x5090059c) = 0x000009a0; // Layer control
  *((volatile uint32_t *) 0x5090011c) = 0x00000100; // 1D
  *((volatile uint32_t *) 0x5090079c) = 0x03000000; // Post processing register
  *((volatile uint32_t *) 0x5090071c) = 0x00000001; // Mask and processor enables

  // Layer 4 quadrant 0
  *((volatile uint32_t *) 0x501003a0) = 0x00000001; // Write ptr time slot offs
  *((volatile uint32_t *) 0x50100420) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x501005a0) = 0x00014920; // Layer control
  *((volatile uint32_t *) 0x50100a20) = 0x0000080e; // Layer control 2
  *((volatile uint32_t *) 0x50100620) = 0x00000fe8; // Mask offset and count
  *((volatile uint32_t *) 0x50100120) = 0x00000100; // 1D
  *((volatile uint32_t *) 0x501006a0) = 0x00fe01fd; // TRAM ptr max
  *((volatile uint32_t *) 0x501007a0) = 0x083e6000; // Post processing register

  // Layer 4 quadrant 1
  *((volatile uint32_t *) 0x505003a0) = 0x00000001; // Write ptr time slot offs
  *((volatile uint32_t *) 0x50500420) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x505005a0) = 0x00010920; // Layer control
  *((volatile uint32_t *) 0x50500a20) = 0x0000080e; // Layer control 2
  *((volatile uint32_t *) 0x50500620) = 0x00000fe8; // Mask offset and count
  *((volatile uint32_t *) 0x50500120) = 0x00000100; // 1D
  *((volatile uint32_t *) 0x505006a0) = 0x00fe01fd; // TRAM ptr max
  *((volatile uint32_t *) 0x505007a0) = 0x083e6000; // Post processing register

  // Layer 4 quadrant 2
  *((volatile uint32_t *) 0x509003a0) = 0x00000001; // Write ptr time slot offs
  *((volatile uint32_t *) 0x50900420) = 0x00002000; // Write ptr mask offs
  *((volatile uint32_t *) 0x509005a0) = 0x00010920; // Layer control
  *((volatile uint32_t *) 0x50900a20) = 0x0000080e; // Layer control 2
  *((volatile uint32_t *) 0x50900620) = 0x00000fe8; // Mask offset and count
  *((volatile uint32_t *) 0x50900120) = 0x00000100; // 1D
  *((volatile uint32_t *) 0x509006a0) = 0x00fe01fd; // TRAM ptr max
  *((volatile uint32_t *) 0x509007a0) = 0x083e6000; // Post processing register
  *((volatile uint32_t *) 0x50900720) = 0x00100010; // Mask and processor enables


  return CNN_OK;
}

int cnn_start(void)
{
  cnn_time = 0;

  *((volatile uint32_t *) 0x50100000) = 0x00100808; // Enable quadrant 0
  *((volatile uint32_t *) 0x50500000) = 0x00100809; // Enable quadrant 1
  *((volatile uint32_t *) 0x50900000) = 0x00100809; // Enable quadrant 2

#ifdef CNN_INFERENCE_TIMER
  MXC_TMR_SW_Start(CNN_INFERENCE_TIMER);
#endif

  *((volatile uint32_t *) 0x50100000) = 0x00100009; // Master enable quadrant 0

  return CNN_OK;
}

int cnn_unload(uint32_t *out_buf)
{
  volatile uint32_t *addr;

  // Custom unload for this network, layer 4 (block6_dense): 32-bit data, shape: (2, 1, 1)
  addr = (volatile uint32_t *) 0x50400000;
  *out_buf++ = *addr++;
  *out_buf++ = *addr++;

  return CNN_OK;
}

int cnn_enable(uint32_t clock_source, uint32_t clock_divider)
{
  // Reset all domains, restore power to CNN
  MXC_GCFR->reg3 = 0xf; // Reset
  MXC_GCFR->reg1 = 0x7; // Mask memory
  MXC_GCFR->reg0 = 0x7; // Power
  MXC_GCFR->reg2 = 0x8; // Iso
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


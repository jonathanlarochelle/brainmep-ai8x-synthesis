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

// ep_v1.4_768_och4_ker8_pad0_maxp12
// This file was @generated by ai8xize.py --verbose --test-dir demos\energy_profiling\kernel_size --prefix ep_v1.4_768_och4_ker8_pad0_maxp12 --checkpoint-file .\trained_custom\checkpoints\energy_profiling\kernel_size\ep_v1.4_768_och4_ker8_pad0_maxp12_qat_best-q.pth.tar --config-file .\networks\energy_profiling.yaml --device MAX78000 --compact-data --softmax --sample-input .\trained_custom\sample_data\energy_profiling\kernel_size\sample_chbmit_singlech_768samples_patient_5_leave_out_seizure_1_ep_v1.4_768_och4_ker8_pad0_maxp12.npy --overwrite --energy

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "mxc.h"
#include "cnn.h"
#include "sampledata.h"
#include "sampleoutput.h"

volatile uint32_t cnn_time; // Stopwatch

void fail(void)
{
  printf("\n*** FAIL ***\n\n");
  while (1);
}

// 1-channel 768x1 data input (768 bytes / 192 32-bit words):
// CHW 768x1, channel 0
static const uint32_t input_0[] = SAMPLE_INPUT_0;

void load_input(void)
{
  // This function loads the sample data input -- replace with actual data

  memcpy32((uint32_t *) 0x50400000, input_0, 192);
}

// Expected output of layer 2 (block6_dense) for ep_v1.4_768_och4_ker8_pad0_maxp12 given the sample input (known-answer test)
// Delete this function for production code
static const uint32_t sample_output[] = SAMPLE_OUTPUT;
int check_output(void)
{
  int i;
  uint32_t mask, len;
  volatile uint32_t *addr;
  const uint32_t *ptr = sample_output;

  while ((addr = (volatile uint32_t *) *ptr++) != 0) {
    mask = *ptr++;
    len = *ptr++;
    for (i = 0; i < len; i++)
      if ((*addr++ & mask) != *ptr++) {
        printf("Data mismatch (%d/%d) at address 0x%08x: Expected 0x%08x, read 0x%08x.\n",
               i + 1, len, addr - 1, *(ptr - 1), *(addr - 1) & mask);
        return CNN_FAIL;
      }
  }

  return CNN_OK;
}

// Classification layer:
static int32_t ml_data[CNN_NUM_OUTPUTS];
static q15_t ml_softmax[CNN_NUM_OUTPUTS];

void softmax_layer(void)
{
  cnn_unload((uint32_t *) ml_data);
  softmax_q17p14_q15((const q31_t *) ml_data, CNN_NUM_OUTPUTS, ml_softmax);
}

int main(void)
{
  int i;
  int digs, tens;

  MXC_ICC_Enable(MXC_ICC0); // Enable cache

  // Switch to 100 MHz clock
  MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
  SystemCoreClockUpdate();

  printf("Waiting...\n");

  // DO NOT DELETE THIS LINE:
  MXC_Delay(SEC(2)); // Let debugger interrupt if needed

  cnn_disable(); // Disable clock and power to CNN
  // Enable primary clock
  MXC_SYS_ClockSourceEnable(MXC_SYS_CLOCK_IPO);

  printf("Measuring system base (idle) power...\n");
  SYS_START;
  MXC_Delay(SEC(1));
  SYS_COMPLETE;

  // Enable peripheral, enable CNN interrupt, turn on CNN clock
  // CNN clock: APB (50 MHz) div 1
  cnn_enable(MXC_S_GCR_PCLKDIV_CNNCLKSEL_PCLK, MXC_S_GCR_PCLKDIV_CNNCLKDIV_DIV1);

  printf("\n*** CNN Inference Test ep_v1.4_768_och4_ker8_pad0_maxp12 ***\n");

  cnn_init(); // Bring state machine into consistent state

  printf("Measuring weight loading...\n");
  CNN_START;
  for (i = 0; i < 100; i++)
    cnn_load_weights(); // Load kernels
  CNN_COMPLETE;

  MXC_TMR_Delay(MXC_TMR0, 500000);
  printf("Measuring input loading...\n");
  CNN_START;
  for (i = 0; i < 100; i++)
    load_input(); // Load data input
  CNN_COMPLETE;

  cnn_load_bias();
  cnn_configure(); // Configure state machine

  MXC_TMR_Delay(MXC_TMR0, 500000);
  printf("Measuring input load + inference...\n");
  CNN_START; // Allow capture of processing time
  for (i = 0; i < 100; i++) {
    load_input(); // Load data input
    cnn_start(); // Run inference
    while (cnn_time == 0)
      MXC_LP_EnterSleepMode(); // Wait for CNN
  }
  CNN_COMPLETE;

  if (check_output() != CNN_OK) fail();
  softmax_layer();

  printf("\n*** PASS ***\n\n");

#ifdef CNN_INFERENCE_TIMER
  printf("Approximate inference time: %u us\n\n", cnn_time);
#endif

  printf("See monitor display for inference energy.\n\n");

  cnn_disable(); // Shut down CNN clock, disable peripheral

  printf("Classification results:\n");
  for (i = 0; i < CNN_NUM_OUTPUTS; i++) {
    digs = (1000 * ml_softmax[i] + 0x4000) >> 15;
    tens = digs % 10;
    digs = digs / 10;
    printf("[%7d] -> Class %d: %d.%d%%\n", ml_data[i], i, digs, tens);
  }

  return 0;
}

/*
  SUMMARY OF OPS
  Hardware: 30,924 ops (24,856 macc; 6,068 comp; 0 add; 0 mul; 0 bitwise)
    Layer 0 (block1_conv1d_bn_relu_1): 27,396 ops (24,352 macc; 3,044 comp; 0 add; 0 mul; 0 bitwise)
    Layer 1 (block1_maxpool1d_1): 3,024 ops (0 macc; 3,024 comp; 0 add; 0 mul; 0 bitwise)
    Layer 2 (block6_dense): 504 ops (504 macc; 0 comp; 0 add; 0 mul; 0 bitwise)

  RESOURCE USAGE
  Weight memory: 536 bytes out of 442,368 bytes total (0.1%)
  Bias memory:   4 bytes out of 2,048 bytes total (0.2%)
*/


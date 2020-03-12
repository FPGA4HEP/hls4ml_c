/**********
Copyright (c) 2018, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/

/*******************************************************************************
Description:
    HLS pragmas can be used to optimize the design : improve throughput, reduce latency and 
    device resource utilization of the resulting RTL code
    This is a wrapper to be used with an hls4ml project to enable proper handling by SDAccel
*******************************************************************************/

#define PROJ_HDR <MYPROJ.h>

#include PROJ_HDR
#include "kernel_params.h"

/*
    HLS4ML Kernel Implementation 
    Arguments:
        in    (input)     --> Input Vector
        out   (output)    --> Output Vector
   */
extern "C" {
void aws_hls4ml(
        const data_t *in, // Read-Only Vector
        data_t *out       // Output Result
        )
{
// SDAccel kernel must have one and only one s_axilite interface which will be used by host application to configure the kernel.
// Here bundle control is defined which is s_axilite interface and associated with all the arguments (in and out),
// control interface must also be associated with "return".
// All the global memory access arguments must be associated to one m_axi(AXI Master Interface). Here all two arguments(in, out) are 
// associated to bundle gmem which means that a AXI master interface named "gmem" will be created in Kernel and all these variables will be 
// accessing global memory through this interface.
// Multiple interfaces can also be created based on the requirements. For example when multiple memory accessing arguments need access to
// global memory simultaneously, user can create multiple master interfaces and can connect to different arguments.
#pragma HLS INTERFACE m_axi port=in  offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=out offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=in   bundle=control
#pragma HLS INTERFACE s_axilite port=out  bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
  //#pragma HLS dataflow
    //unsigned short insize, outsize;
    //necessary for hls4ml kernel, not used
    
    input_t in_buf[STREAMSIZE][DATA_SIZE_IN];
    layer11_t out_buf[STREAMSIZE][DATA_SIZE_OUT];
    //these will get partitioned properly in the hls4ml code

    #pragma HLS ARRAY_PARTITION variable=in_buf complete dim=2
    #pragma HLS ARRAY_PARTITION variable=out_buf complete dim=2 
    //#pragma HLS ARRAY_RESHAPE   variable=in_buf block factor=11
    //#pragma HLS ARRAY_RESHAPE   variable=out_buf block factor=11
    
    //getting data from axi stream and formatting properly
    for (int i = 0; i < STREAMSIZE; i++) {
//#pragma HLS PIPELINE II=11 rewind
      for (int j = 0; j < DATA_SIZE_IN; j++) {
//#pragma HLS LOOP UNROLL
	in_buf[i][j] = (input_t)in[i*DATA_SIZE_IN+j];
      }
    }

    //run inference
    for (int i = 0; i < STREAMSIZE; i++) {
//#pragma HLS PIPELINE II=1 rewind
#pragma HLS DATAFLOW
      hls4ml: MYPROJ(in_buf[i],out_buf[i]);//,insize,outsize);
    }

    //place output into axi stream output
    for (int i = 0; i < STREAMSIZE; i++) {
//#pragma HLS PIPELINE II=1 rewind
      for (int j = 0; j < DATA_SIZE_OUT; j++) {
//#pragma HLS LOOP UNROLL
	out[i*DATA_SIZE_OUT+j] = (data_t)out_buf[i][j];
      }
    }
}
}

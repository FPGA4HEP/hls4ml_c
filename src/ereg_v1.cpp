//
//    rfnoc-hls-neuralnet: Vivado HLS code for neural-net building blocks
//
//    Copyright (C) 2017 EJ Kreinar
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include <iostream>

#include "ereg_v1.h"

//hls-fpga-machine-learning insert weights
#include "weights/w2.h"
#include "weights/b2.h"
#include "weights/w4.h"
#include "weights/b4.h"
#include "weights/w6.h"
#include "weights/b6.h"

void ereg_v1(
    input_t input[N_INPUT_1_1],
    layer6_t layer6_out[N_LAYER_6],
    unsigned short &const_size_in_1,
    unsigned short &const_size_out_1
) {

    //hls-fpga-machine-learning insert IO
    #pragma HLS ARRAY_RESHAPE variable=input complete dim=0 
    #pragma HLS ARRAY_RESHAPE variable=layer6_out complete dim=0 
    #pragma HLS INTERFACE ap_vld port=input,layer6_out 
    #pragma HLS PIPELINE 

    const_size_in_1 = N_INPUT_1_1;
    const_size_out_1 = N_LAYER_6;

#ifndef __SYNTHESIS__
    static bool loaded_weights = false;
    if (!loaded_weights) {
        //hls-fpga-machine-learning insert load weights
        nnet::load_weights_from_txt<model_default_t, 165>(w2, "w2.txt");
        nnet::load_weights_from_txt<model_default_t, 15>(b2, "b2.txt");
        nnet::load_weights_from_txt<model_default_t, 75>(w4, "w4.txt");
        nnet::load_weights_from_txt<model_default_t, 5>(b4, "b4.txt");
        nnet::load_weights_from_txt<model_default_t, 5>(w6, "w6.txt");
        nnet::load_weights_from_txt<model_default_t, 1>(b6, "b6.txt");
        loaded_weights = true;
    }
#endif

    // ****************************************
    // NETWORK INSTANTIATION
    // ****************************************

    //hls-fpga-machine-learning insert layers

    layer2_t layer2_out[N_LAYER_2];
    #pragma HLS ARRAY_PARTITION variable=layer2_out complete dim=0
    nnet::dense_latency<input_t, layer2_t, config2>(input, layer2_out, w2, b2);

    layer4_t layer4_out[N_LAYER_4];
    #pragma HLS ARRAY_PARTITION variable=layer4_out complete dim=0
    nnet::dense_latency<layer2_t, layer4_t, config4>(layer2_out, layer4_out, w4, b4);

    nnet::dense_latency<layer4_t, layer6_t, config6>(layer4_out, layer6_out, w6, b6);

}

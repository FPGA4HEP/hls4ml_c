#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <complex>
#include "ap_int.h"
#include "ap_fixed.h"
#include "nnet_utils/nnet_dense.h"
#include "nnet_utils/nnet_dense_large.h"
#include "nnet_utils/nnet_dense_compressed.h"
#include "nnet_utils/nnet_conv.h"
#include "nnet_utils/nnet_conv2d.h"
#include "nnet_utils/nnet_activation.h"
#include "nnet_utils/nnet_common.h"
#include "nnet_utils/nnet_batchnorm.h"
#include "nnet_utils/nnet_pooling.h"
#include "nnet_utils/nnet_merge.h"
#include "nnet_utils/nnet_helpers.h"

//hls-fpga-machine-learning insert numbers
#define N_INPUT_1_1 11
#define N_LAYER_2 15
#define N_LAYER_4 5
#define N_LAYER_6 1

//hls-fpga-machine-learning insert layer-precision
typedef ap_fixed<32,14> model_default_t;
typedef ap_fixed<32,14> input_t;
typedef ap_fixed<32,14> layer2_t;
typedef ap_fixed<32,14> layer4_t;
typedef ap_fixed<32,14> layer6_t;

//hls-fpga-machine-learning insert layer-config
struct config2 : nnet::dense_config {
    static const unsigned n_in = N_INPUT_1_1;
    static const unsigned n_out = N_LAYER_2;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 1;
    static const unsigned n_zeros = 0;
    static const unsigned n_nonzeros = 165;
    static const bool store_weights_in_bram = false;
    typedef ap_fixed<32,14> accum_t;
    typedef model_default_t bias_t;
    typedef model_default_t weight_t;
    typedef ap_uint<1> index_t;
};

struct config4 : nnet::dense_config {
    static const unsigned n_in = N_LAYER_2;
    static const unsigned n_out = N_LAYER_4;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 1;
    static const unsigned n_zeros = 0;
    static const unsigned n_nonzeros = 75;
    static const bool store_weights_in_bram = false;
    typedef ap_fixed<32,14> accum_t;
    typedef model_default_t bias_t;
    typedef model_default_t weight_t;
    typedef ap_uint<1> index_t;
};

struct config6 : nnet::dense_config {
    static const unsigned n_in = N_LAYER_4;
    static const unsigned n_out = N_LAYER_6;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 1;
    static const unsigned n_zeros = 0;
    static const unsigned n_nonzeros = 5;
    static const bool store_weights_in_bram = false;
    typedef ap_fixed<32,14> accum_t;
    typedef model_default_t bias_t;
    typedef model_default_t weight_t;
    typedef ap_uint<1> index_t;
};


#endif

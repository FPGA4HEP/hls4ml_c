#include "ap_fixed.h"
#include <parameters.h>

#define STREAMSIZE 16384
//how many consecutive sets of inputs to run over per kernel execution

#define DATA_SIZE_IN N_INPUT_1_1
#define DATA_SIZE_OUT N_LAYER_11

typedef ap_fixed<16,6> data_t;

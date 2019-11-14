#include "ap_fixed.h"
#include <parameters.h>

#define STREAMSIZE 32//16384
//how many consecutive sets of inputs to run over per kernel execution

#ifdef IS_DENSE
#define DATA_SIZE_IN N_INPUT_1_1
#endif
#ifdef IS_CONV1D
#define DATA_SIZE_IN (Y_INPUTS*N_CHAN)
#endif
#define DATA_SIZE_OUT N_LAYER_6

typedef ap_fixed<32,14> data_t;

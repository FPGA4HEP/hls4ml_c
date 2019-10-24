COMMON_REPO :=  ${HOME}/SDAccel_Examples/

#--v--v--
#these need to be set by the user for their specific installation
HLS4ML_BASE := ${HOME}/hls4ml
HLS4ML_PROJECT := my-hls-test-$${FAVOURITEMODEL}
HLS4ML_NAME := myproject
HLS4ML_PROJ_TYPE := DENSE
#possible options are: DENSE, CONV1D
#--^--^--
ifeq ($(filter $(HLS4ML_PROJ_TYPE),DENSE CONV1D),)
$(error invalid HLS4ML_PROJ_TYPE, must be DENSE or CONV1D)
endif

include $(COMMON_REPO)/utility/boards.mk
include $(COMMON_REPO)/libs/xcl2/xcl2.mk
include $(COMMON_REPO)/libs/opencl/opencl.mk

# Host Application
host_SRCS=./src/host.cpp $(xcl2_SRCS)
host_HDRS=$(xcl2_HDRS)
host_CXXFLAGS=-I./src/ -I$(HLS4ML_BASE)/example-models/$(HLS4ML_PROJECT)/firmware/nnet_utils/ -I$(HLS4ML_BASE)/example-models/$(HLS4ML_PROJECT)/firmware/ $(xcl2_CXXFLAGS) $(opencl_CXXFLAGS) -DIS_$(HLS4ML_PROJ_TYPE) -DHLS4ML_DATA_DIR=$(HLS4ML_BASE)/example-models/$(HLS4ML_PROJECT)/tb_data/ -DWEIGHTS_DIR=$(HLS4ML_BASE)/example-models/$(HLS4ML_PROJECT)/firmware/weights -std=c++11 
host_LDFLAGS=$(opencl_LDFLAGS) -I$(XILINX_VIVADO)/include/ -I$(XILINX_SDACCEL)/include/ -Wno-unknown-pragmas

# alveo_hls4ml Kernels
alveo_hls4ml_SRCS=./src/alveo_hls4ml.cpp $(HLS4ML_BASE)/example-models/$(HLS4ML_PROJECT)/firmware/$(HLS4ML_NAME).cpp
alveo_hls4ml_CLFLAGS=-k alveo_hls4ml -DMYPROJ=$(HLS4ML_NAME) -DIS_$(HLS4ML_PROJ_TYPE) -DWEIGHTS_DIR=$(HLS4ML_BASE)/example-models/$(HLS4ML_PROJECT)/firmware/weights -I./src/ -I$(HLS4ML_BASE)/example-models/$(HLS4ML_PROJECT)/firmware/ -I$(HLS4ML_BASE)/example-models/$(HLS4ML_PROJECT)/firmware/weights -I$(HLS4ML_BASE)/example-models/$(HLS4ML_PROJECT)/firmware/nnet_utils/ --xp "prop:solution.hls_pre_tcl=./config.tcl" --xp "prop:kernel.alveo_hls4ml.kernel_flags=-std=c++11"

EXES=host
XCLBINS=alveo_hls4ml

XOS=alveo_hls4ml

alveo_hls4ml_XOS=alveo_hls4ml

# check
check_EXE=host
check_XCLBINS=alveo_hls4ml

CHECKS=check

include $(COMMON_REPO)/utility/rules.mk


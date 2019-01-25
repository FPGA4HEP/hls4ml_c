#COMMON_REPO := ../../../
COMMON_REPO := /home/centos/src/project_data/aws-fpga/SDAccel/examples/xilinx/

#--v--v--
#these need to be set by the user for their specific installation
HLS4ML_BASE := /home/centos/hls4ml
HLS4ML_PROJECT := my-hls-test-1layer
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
host_CXXFLAGS=-I./src/ -I$(HLS4ML_BASE)/nnet_utils/ -I$(HLS4ML_BASE)/keras-to-hls/$(HLS4ML_PROJECT)/firmware/ $(xcl2_CXXFLAGS) $(opencl_CXXFLAGS) -DIS_$(HLS4ML_PROJ_TYPE) -DHLS4ML_DATA_DIR=$(HLS4ML_BASE)/keras-to-hls/$(HLS4ML_PROJECT)/tb_data/ -std=c++11
host_LDFLAGS=$(opencl_LDFLAGS) -I$(XILINX_VIVADO)/include/ -I$(XILINX_SDACCEL)/include/ -Wno-unknown-pragmas

# aws_hls4ml Kernels
aws_hls4ml_SRCS=./src/aws_hls4ml.cpp $(HLS4ML_BASE)/keras-to-hls/$(HLS4ML_PROJECT)/firmware/$(HLS4ML_NAME).cpp
aws_hls4ml_CLFLAGS=-k aws_hls4ml -DMYPROJ=$(HLS4ML_NAME) -DIS_$(HLS4ML_PROJ_TYPE) -I./src/ -I$(HLS4ML_BASE)/keras-to-hls/$(HLS4ML_PROJECT)/firmware/ -I$(HLS4ML_BASE)/keras-to-hls/$(HLS4ML_PROJECT)/firmware/weights -I$(HLS4ML_BASE)/nnet_utils/

EXES=host
XCLBINS=aws_hls4ml

XOS=aws_hls4ml

aws_hls4ml_XOS=aws_hls4ml

# check
check_EXE=host
check_XCLBINS=aws_hls4ml

CHECKS=check

include $(COMMON_REPO)/utility/rules.mk

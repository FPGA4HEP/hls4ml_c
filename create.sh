#!/bin/sh

binname=`ls xclbin/ | grep "\.xclbin" | grep "\.hw\." | sed 's/\.xclbin//'`
$SDACCEL_DIR/tools/create_sdaccel_afi.sh -xclbin=xclbin/$binname.xclbin -o=$binname -s3_bucket=fpga4hep-afi -s3_dcp_key=inputfiles -s3_logs_key=logs

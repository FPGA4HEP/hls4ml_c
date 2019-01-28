#!/bin/sh

afiid=`cat *_afi_id.txt | grep -m 1 "FpgaImageId" | tail -n 1 | sed 's/^.*"\(afi-.*\)",/\1/'`
aws ec2 describe-fpga-images --region eu-west-1 --fpga-image-ids $afiid

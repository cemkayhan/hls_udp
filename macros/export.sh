#!/bin/sh

CFLAGS="-DD_ENABLE_PIXEL_CNTR_ -DD_ENABLE_LINE_CNTR_ -DD_ENABLE_FRAME_CNTR_ -DD_PIX_DEPTH_=10 -DD_PPC_=2" vitis_hls -f export_impl_ip.tcl

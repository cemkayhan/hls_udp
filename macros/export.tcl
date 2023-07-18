set target export

source open_project.tcl
source common_cflags.tcl
source add_files.tcl

source config_rtl.tcl
source config_dataflow.tcl
csynth_design
file copy -force -- vitis_hls.log $prj_dir/solution1/vitis_hls.log
export_design -flow $flow -format $format

puts {---------------------------------------------}
puts " PRJ_DIR: $prj_dir"
puts {---------------------------------------------}
exit

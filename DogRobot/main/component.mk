#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)



#对的，你没看错，只有几行注释，但是这个文件却是必须有的，没有这个文件，
#Makefile在编译工程时就会找不到app_main这个启动函数。在这个文件中也可以写入main文件夹下的头文件和源文件路径
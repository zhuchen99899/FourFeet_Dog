COMPONENT_ADD_INCLUDEDIRS :=./include ./bsp/wifi ./bsp/bsp_init ./bsp/iic ./bsp/PCA9685 ./bsp/servo
COMPONENT_SRCDIRS :=  ./bsp/wifi ./bsp/bsp_init ./bsp/iic  ./bsp/PCA9685 ./bsp/servo 
# 表示组件源文件所需的头文件都到component.mk所在目录的include文件夹下找，
# 别的组件如果引用到本组件的所包含的头文件也会到这个include文件夹下找。
# 如果组件框架比较复杂，我们可能还需要添加其他路径，
# 假如my_comp组件下还含有lib/inc（.h文件）和lib/src（.c文件）文件夹，
# 那我们就需要将其路径添加在component.mk中，以便编译器能找到它们。 
#如:COMPONENT_ADD_INCLUDEDIRS := ./lib/inc ./include
#COMPONENT_SRCDIRS :=  ./ lib/src


#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := my_project

EXTRA_COMPONENT_DIRS = $(PROJECT_PATH)/freertos_app , $(PROJECT_PATH)/protocol , $(PROJECT_PATH)/Robot_FSM #ESP32添加自定义文件夹目录到编译工程

include $(IDF_PATH)/make/project.mk



#每个ESP32工程都会有这样一个Makefile文件，用来配置整个项目。没有这个文件，就不能使用make命令来编译你的工程。
#第一行PROJECT_NAME := my_project用来设置工程编译完成后的bin文件的名称；
#第二行include $(IDF_PATH)/make/project.mk用来设置编译所需的组件库的路径，将你的工程文件和esp-idf这个组件库链接起来。
#现在，我们在终端输入make来编译整个工程

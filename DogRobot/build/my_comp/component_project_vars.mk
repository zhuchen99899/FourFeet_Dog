# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/my_comp/include $(PROJECT_PATH)/components/my_comp/bsp/wifi $(PROJECT_PATH)/components/my_comp/bsp/bsp_init $(PROJECT_PATH)/components/my_comp/bsp/iic $(PROJECT_PATH)/components/my_comp/bsp/PCA9685 $(PROJECT_PATH)/components/my_comp/bsp/servo
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/my_comp -lmy_comp
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += my_comp
COMPONENT_LDFRAGMENTS += 
component-my_comp-build: 

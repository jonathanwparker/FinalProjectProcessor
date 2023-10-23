################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := clean

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include RTE/USB/subdir.mk
-include RTE/File_System/subdir.mk
-include RTE/Device/STM32F407VGTx/subdir.mk
-include RTE/Compiler/subdir.mk
-include RTE/CMSIS_Driver/STM32F407VGTx/subdir.mk
-include RTE/CMSIS/subdir.mk
-include RTE/Board_Support/STM32F407VGTx/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := FinalProjectProcessor
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: FinalProjectProcessor.axf

# Tool invocations
FinalProjectProcessor.axf: $(OBJS) D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/cmsis_rte_STM32F407VGTx.sct $(USER_OBJS) $(LIBS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: Arm Linker 6'
	armlink.exe --entry=Reset_Handler --scatter="D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/cmsis_rte_STM32F407VGTx.sct" --info=sizes -o "FinalProjectProcessor.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES) $(OBJS) $(C_DEPS) FinalProjectProcessor.axf 
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets

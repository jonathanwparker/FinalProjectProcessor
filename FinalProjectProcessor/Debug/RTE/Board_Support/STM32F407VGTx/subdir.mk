################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Boards/ST/STM32F4-Discovery/Common/Buttons_F4Discovery.c \
C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Boards/ST/STM32F4-Discovery/Common/LED_F4Discovery.c 

OBJS += \
./RTE/Board_Support/STM32F407VGTx/Buttons_F4Discovery.o \
./RTE/Board_Support/STM32F407VGTx/LED_F4Discovery.o 

C_DEPS += \
./RTE/Board_Support/STM32F407VGTx/Buttons_F4Discovery.d \
./RTE/Board_Support/STM32F407VGTx/LED_F4Discovery.d 


# Each subdirectory must supply rules for building sources it contributes
RTE/Board_Support/STM32F407VGTx/Buttons_F4Discovery.o: C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Boards/ST/STM32F4-Discovery/Common/Buttons_F4Discovery.c RTE/Board_Support/STM32F407VGTx/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Arm C Compiler 6'
	armclang.exe --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -mlittle-endian -DHSE_VALUE="8000000" -DSTM32F407xx -DUSE_STM32F4_DISCO -DARM_MATH_CM4 -D_RTE_ -DSTM32F407xx -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/Core/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/DSP/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/DSP/PrivateInclude" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/Driver/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/RTOS/RTX/INC" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/RTOS/RTX/SRC/ARM" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/RTOS/RTX/UserCodeTemplates" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.16.0/Board" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.16.0/FileSystem/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.16.0/USB/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Device/Source/ARM" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Templates/Inc" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Templates_LL/Inc" -I"D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/RTE" -I"D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/RTE/Device/STM32F407VGTx" -I"D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/RTE/File_System" -I"D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/RTE/USB" -include"D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/RTE/Pre_Include_Global.h" -xc -std=c99 -O0 -g -fshort-enums -fshort-wchar -MD -MP -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Board_Support/STM32F407VGTx/LED_F4Discovery.o: C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Boards/ST/STM32F4-Discovery/Common/LED_F4Discovery.c RTE/Board_Support/STM32F407VGTx/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Arm C Compiler 6'
	armclang.exe --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -mlittle-endian -DHSE_VALUE="8000000" -DSTM32F407xx -DUSE_STM32F4_DISCO -DARM_MATH_CM4 -D_RTE_ -DSTM32F407xx -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/Core/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/DSP/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/DSP/PrivateInclude" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/Driver/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/RTOS/RTX/INC" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/RTOS/RTX/SRC/ARM" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/RTOS/RTX/UserCodeTemplates" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.16.0/Board" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.16.0/FileSystem/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.16.0/USB/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Device/Source/ARM" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Templates/Inc" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Templates_LL/Inc" -I"D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/RTE" -I"D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/RTE/Device/STM32F407VGTx" -I"D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/RTE/File_System" -I"D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/RTE/USB" -include"D:\Software\Cadence_Work\git\FinalProjectProces\FinalProjectProcessor/RTE/Pre_Include_Global.h" -xc -std=c99 -O0 -g -fshort-enums -fshort-wchar -MD -MP -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



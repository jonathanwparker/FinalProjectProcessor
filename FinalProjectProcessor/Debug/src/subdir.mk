################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FS.c \
../src/UART_driver.c \
../src/cs43l22.c \
../src/main.c \
../src/stm32f4_discovery.c \
../src/stm32f4_discovery_audio.c \
../src/stm32f4xx_it.c 

OBJS += \
./src/FS.o \
./src/UART_driver.o \
./src/cs43l22.o \
./src/main.o \
./src/stm32f4_discovery.o \
./src/stm32f4_discovery_audio.o \
./src/stm32f4xx_it.o 

C_DEPS += \
./src/FS.d \
./src/UART_driver.d \
./src/cs43l22.d \
./src/main.d \
./src/stm32f4_discovery.d \
./src/stm32f4_discovery_audio.d \
./src/stm32f4xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Arm C Compiler 6'
	armclang.exe --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -mlittle-endian -DHSE_VALUE="8000000" -DSTM32F407xx -DUSE_STM32F4_DISCO -DARM_MATH_CM4 -D_RTE_ -DSTM32F407xx -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/Core/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/DSP/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/DSP/PrivateInclude" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/Driver/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/RTOS/RTX/INC" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/RTOS/RTX/SRC/ARM" -I"C:/Users/jonat/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/RTOS/RTX/UserCodeTemplates" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.16.0/Board" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.16.0/FileSystem/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.16.0/USB/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Device/Source/ARM" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Templates/Inc" -I"C:/Users/jonat/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.17.1/MDK/Templates_LL/Inc" -I"C:\Users\jonat\Development Studio Workspace\Lab_11/RTE" -I"C:\Users\jonat\Development Studio Workspace\Lab_11/RTE/Device/STM32F407VGTx" -I"C:\Users\jonat\Development Studio Workspace\Lab_11/RTE/File_System" -I"C:\Users\jonat\Development Studio Workspace\Lab_11/RTE/USB" -include"C:\Users\jonat\Development Studio Workspace\Lab_11/RTE/Pre_Include_Global.h" -xc -std=c99 -O0 -g -fshort-enums -fshort-wchar -MD -MP -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DAC.c \
../Core/Src/DAC_Unit_Test.c \
../Core/Src/Heater_Controller.c \
../Core/Src/PID.c \
../Core/Src/TMP117.c \
../Core/Src/TuningControlBoard.c \
../Core/Src/UI.c \
../Core/Src/controller.c \
../Core/Src/funcs.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/stringfifo.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

C_DEPS += \
./Core/Src/DAC.d \
./Core/Src/DAC_Unit_Test.d \
./Core/Src/Heater_Controller.d \
./Core/Src/PID.d \
./Core/Src/TMP117.d \
./Core/Src/TuningControlBoard.d \
./Core/Src/UI.d \
./Core/Src/controller.d \
./Core/Src/funcs.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/stringfifo.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 

OBJS += \
./Core/Src/DAC.o \
./Core/Src/DAC_Unit_Test.o \
./Core/Src/Heater_Controller.o \
./Core/Src/PID.o \
./Core/Src/TMP117.o \
./Core/Src/TuningControlBoard.o \
./Core/Src/UI.o \
./Core/Src/controller.o \
./Core/Src/funcs.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/stringfifo.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DAC.d ./Core/Src/DAC.o ./Core/Src/DAC.su ./Core/Src/DAC_Unit_Test.d ./Core/Src/DAC_Unit_Test.o ./Core/Src/DAC_Unit_Test.su ./Core/Src/Heater_Controller.d ./Core/Src/Heater_Controller.o ./Core/Src/Heater_Controller.su ./Core/Src/PID.d ./Core/Src/PID.o ./Core/Src/PID.su ./Core/Src/TMP117.d ./Core/Src/TMP117.o ./Core/Src/TMP117.su ./Core/Src/TuningControlBoard.d ./Core/Src/TuningControlBoard.o ./Core/Src/TuningControlBoard.su ./Core/Src/UI.d ./Core/Src/UI.o ./Core/Src/UI.su ./Core/Src/controller.d ./Core/Src/controller.o ./Core/Src/controller.su ./Core/Src/funcs.d ./Core/Src/funcs.o ./Core/Src/funcs.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/stringfifo.d ./Core/Src/stringfifo.o ./Core/Src/stringfifo.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src


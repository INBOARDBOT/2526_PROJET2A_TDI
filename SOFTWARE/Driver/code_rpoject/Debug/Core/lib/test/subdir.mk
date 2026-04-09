################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/lib/test/debug_struct.c 

OBJS += \
./Core/lib/test/debug_struct.o 

C_DEPS += \
./Core/lib/test/debug_struct.d 


# Each subdirectory must supply rules for building sources it contributes
Core/lib/test/%.o Core/lib/test/%.su Core/lib/test/%.cyclo: ../Core/lib/test/%.c Core/lib/test/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Core/Lib -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-lib-2f-test

clean-Core-2f-lib-2f-test:
	-$(RM) ./Core/lib/test/debug_struct.cyclo ./Core/lib/test/debug_struct.d ./Core/lib/test/debug_struct.o ./Core/lib/test/debug_struct.su

.PHONY: clean-Core-2f-lib-2f-test


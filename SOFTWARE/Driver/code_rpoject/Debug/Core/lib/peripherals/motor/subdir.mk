################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/lib/peripherals/motor/motor_utils.c 

OBJS += \
./Core/lib/peripherals/motor/motor_utils.o 

C_DEPS += \
./Core/lib/peripherals/motor/motor_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Core/lib/peripherals/motor/%.o Core/lib/peripherals/motor/%.su Core/lib/peripherals/motor/%.cyclo: ../Core/lib/peripherals/motor/%.c Core/lib/peripherals/motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Core/Lib -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-lib-2f-peripherals-2f-motor

clean-Core-2f-lib-2f-peripherals-2f-motor:
	-$(RM) ./Core/lib/peripherals/motor/motor_utils.cyclo ./Core/lib/peripherals/motor/motor_utils.d ./Core/lib/peripherals/motor/motor_utils.o ./Core/lib/peripherals/motor/motor_utils.su

.PHONY: clean-Core-2f-lib-2f-peripherals-2f-motor


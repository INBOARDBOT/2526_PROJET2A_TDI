################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/lib/FOC/foc_config.c \
../Core/lib/FOC/foc_math.c \
../Core/lib/FOC/foc_utils.c 

OBJS += \
./Core/lib/FOC/foc_config.o \
./Core/lib/FOC/foc_math.o \
./Core/lib/FOC/foc_utils.o 

C_DEPS += \
./Core/lib/FOC/foc_config.d \
./Core/lib/FOC/foc_math.d \
./Core/lib/FOC/foc_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Core/lib/FOC/%.o Core/lib/FOC/%.su Core/lib/FOC/%.cyclo: ../Core/lib/FOC/%.c Core/lib/FOC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Core/Lib -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-lib-2f-FOC

clean-Core-2f-lib-2f-FOC:
	-$(RM) ./Core/lib/FOC/foc_config.cyclo ./Core/lib/FOC/foc_config.d ./Core/lib/FOC/foc_config.o ./Core/lib/FOC/foc_config.su ./Core/lib/FOC/foc_math.cyclo ./Core/lib/FOC/foc_math.d ./Core/lib/FOC/foc_math.o ./Core/lib/FOC/foc_math.su ./Core/lib/FOC/foc_utils.cyclo ./Core/lib/FOC/foc_utils.d ./Core/lib/FOC/foc_utils.o ./Core/lib/FOC/foc_utils.su

.PHONY: clean-Core-2f-lib-2f-FOC


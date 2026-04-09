################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/lib/application/app_utils.c \
../Core/lib/application/comm_handler.c \
../Core/lib/application/displayPyGraph.c \
../Core/lib/application/fsm_control.c 

OBJS += \
./Core/lib/application/app_utils.o \
./Core/lib/application/comm_handler.o \
./Core/lib/application/displayPyGraph.o \
./Core/lib/application/fsm_control.o 

C_DEPS += \
./Core/lib/application/app_utils.d \
./Core/lib/application/comm_handler.d \
./Core/lib/application/displayPyGraph.d \
./Core/lib/application/fsm_control.d 


# Each subdirectory must supply rules for building sources it contributes
Core/lib/application/%.o Core/lib/application/%.su Core/lib/application/%.cyclo: ../Core/lib/application/%.c Core/lib/application/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Core/Lib -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-lib-2f-application

clean-Core-2f-lib-2f-application:
	-$(RM) ./Core/lib/application/app_utils.cyclo ./Core/lib/application/app_utils.d ./Core/lib/application/app_utils.o ./Core/lib/application/app_utils.su ./Core/lib/application/comm_handler.cyclo ./Core/lib/application/comm_handler.d ./Core/lib/application/comm_handler.o ./Core/lib/application/comm_handler.su ./Core/lib/application/displayPyGraph.cyclo ./Core/lib/application/displayPyGraph.d ./Core/lib/application/displayPyGraph.o ./Core/lib/application/displayPyGraph.su ./Core/lib/application/fsm_control.cyclo ./Core/lib/application/fsm_control.d ./Core/lib/application/fsm_control.o ./Core/lib/application/fsm_control.su

.PHONY: clean-Core-2f-lib-2f-application


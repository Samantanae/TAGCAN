################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/COM_utils/com_utils.c 

OBJS += \
./Core/Src/COM_utils/com_utils.o 

C_DEPS += \
./Core/Src/COM_utils/com_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/COM_utils/%.o Core/Src/COM_utils/%.su Core/Src/COM_utils/%.cyclo: ../Core/Src/COM_utils/%.c Core/Src/COM_utils/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-COM_utils

clean-Core-2f-Src-2f-COM_utils:
	-$(RM) ./Core/Src/COM_utils/com_utils.cyclo ./Core/Src/COM_utils/com_utils.d ./Core/Src/COM_utils/com_utils.o ./Core/Src/COM_utils/com_utils.su

.PHONY: clean-Core-2f-Src-2f-COM_utils


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Flight_Management_Control/control_allocator.c \
../Core/Src/Flight_Management_Control/flight_management.c \
../Core/Src/Flight_Management_Control/modes_management.c \
../Core/Src/Flight_Management_Control/servo_mixers.c 

OBJS += \
./Core/Src/Flight_Management_Control/control_allocator.o \
./Core/Src/Flight_Management_Control/flight_management.o \
./Core/Src/Flight_Management_Control/modes_management.o \
./Core/Src/Flight_Management_Control/servo_mixers.o 

C_DEPS += \
./Core/Src/Flight_Management_Control/control_allocator.d \
./Core/Src/Flight_Management_Control/flight_management.d \
./Core/Src/Flight_Management_Control/modes_management.d \
./Core/Src/Flight_Management_Control/servo_mixers.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Flight_Management_Control/%.o Core/Src/Flight_Management_Control/%.su Core/Src/Flight_Management_Control/%.cyclo: ../Core/Src/Flight_Management_Control/%.c Core/Src/Flight_Management_Control/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H7B0xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Flight_Management_Control

clean-Core-2f-Src-2f-Flight_Management_Control:
	-$(RM) ./Core/Src/Flight_Management_Control/control_allocator.cyclo ./Core/Src/Flight_Management_Control/control_allocator.d ./Core/Src/Flight_Management_Control/control_allocator.o ./Core/Src/Flight_Management_Control/control_allocator.su ./Core/Src/Flight_Management_Control/flight_management.cyclo ./Core/Src/Flight_Management_Control/flight_management.d ./Core/Src/Flight_Management_Control/flight_management.o ./Core/Src/Flight_Management_Control/flight_management.su ./Core/Src/Flight_Management_Control/modes_management.cyclo ./Core/Src/Flight_Management_Control/modes_management.d ./Core/Src/Flight_Management_Control/modes_management.o ./Core/Src/Flight_Management_Control/modes_management.su ./Core/Src/Flight_Management_Control/servo_mixers.cyclo ./Core/Src/Flight_Management_Control/servo_mixers.d ./Core/Src/Flight_Management_Control/servo_mixers.o ./Core/Src/Flight_Management_Control/servo_mixers.su

.PHONY: clean-Core-2f-Src-2f-Flight_Management_Control


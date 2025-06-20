################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Libraries/BMP280.c \
../Core/Src/Libraries/BNO050.c \
../Core/Src/Libraries/LED.c \
../Core/Src/Libraries/Lora_LR03.c \
../Core/Src/Libraries/PWM.c \
../Core/Src/Libraries/SBUS.c \
../Core/Src/Libraries/SD.c 

OBJS += \
./Core/Src/Libraries/BMP280.o \
./Core/Src/Libraries/BNO050.o \
./Core/Src/Libraries/LED.o \
./Core/Src/Libraries/Lora_LR03.o \
./Core/Src/Libraries/PWM.o \
./Core/Src/Libraries/SBUS.o \
./Core/Src/Libraries/SD.o 

C_DEPS += \
./Core/Src/Libraries/BMP280.d \
./Core/Src/Libraries/BNO050.d \
./Core/Src/Libraries/LED.d \
./Core/Src/Libraries/Lora_LR03.d \
./Core/Src/Libraries/PWM.d \
./Core/Src/Libraries/SBUS.d \
./Core/Src/Libraries/SD.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Libraries/%.o Core/Src/Libraries/%.su Core/Src/Libraries/%.cyclo: ../Core/Src/Libraries/%.c Core/Src/Libraries/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H7B0xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Libraries

clean-Core-2f-Src-2f-Libraries:
	-$(RM) ./Core/Src/Libraries/BMP280.cyclo ./Core/Src/Libraries/BMP280.d ./Core/Src/Libraries/BMP280.o ./Core/Src/Libraries/BMP280.su ./Core/Src/Libraries/BNO050.cyclo ./Core/Src/Libraries/BNO050.d ./Core/Src/Libraries/BNO050.o ./Core/Src/Libraries/BNO050.su ./Core/Src/Libraries/LED.cyclo ./Core/Src/Libraries/LED.d ./Core/Src/Libraries/LED.o ./Core/Src/Libraries/LED.su ./Core/Src/Libraries/Lora_LR03.cyclo ./Core/Src/Libraries/Lora_LR03.d ./Core/Src/Libraries/Lora_LR03.o ./Core/Src/Libraries/Lora_LR03.su ./Core/Src/Libraries/PWM.cyclo ./Core/Src/Libraries/PWM.d ./Core/Src/Libraries/PWM.o ./Core/Src/Libraries/PWM.su ./Core/Src/Libraries/SBUS.cyclo ./Core/Src/Libraries/SBUS.d ./Core/Src/Libraries/SBUS.o ./Core/Src/Libraries/SBUS.su ./Core/Src/Libraries/SD.cyclo ./Core/Src/Libraries/SD.d ./Core/Src/Libraries/SD.o ./Core/Src/Libraries/SD.su

.PHONY: clean-Core-2f-Src-2f-Libraries


################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Delay.cpp \
../src/I2C.cpp \
../src/LCD_LCP1343.cpp \
../src/LiquidCrystal_I2C.cpp \
../src/adc_pot.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc13xx.cpp \
../src/ds18b20.cpp 

C_SRCS += \
../src/crp.c 

OBJS += \
./src/Delay.o \
./src/I2C.o \
./src/LCD_LCP1343.o \
./src/LiquidCrystal_I2C.o \
./src/adc_pot.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc13xx.o \
./src/crp.o \
./src/ds18b20.o 

CPP_DEPS += \
./src/Delay.d \
./src/I2C.d \
./src/LCD_LCP1343.d \
./src/LiquidCrystal_I2C.d \
./src/adc_pot.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc13xx.d \
./src/ds18b20.d 

C_DEPS += \
./src/crp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC13xx -DCPP_USE_HEAP -D__LPC13XX__ -I"C:\Users\bourc\Documents\uC Projects\LPC1343_incubator\CMSIS_CORE_LPC13xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC13xx -DCPP_USE_HEAP -D__LPC13XX__ -I"C:\Users\bourc\Documents\uC Projects\LPC1343_incubator\CMSIS_CORE_LPC13xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ADC_pot.cpp \
../src/Delay.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc13xx.cpp 

C_SRCS += \
../src/crp.c 

OBJS += \
./src/ADC_pot.o \
./src/Delay.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc13xx.o \
./src/crp.o 

CPP_DEPS += \
./src/ADC_pot.d \
./src/Delay.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc13xx.d 

C_DEPS += \
./src/crp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DDONT_ENABLE_SWVTRACECLK -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC13xx -DCPP_USE_HEAP -D__LPC13XX__ -I"C:\Users\bourc\Documents\uC Projects\LPC1343_incubator\CMSIS_CORE_LPC13xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__NEWLIB__ -DDONT_ENABLE_SWVTRACECLK -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC13xx -DCPP_USE_HEAP -D__LPC13XX__ -I"C:\Users\bourc\Documents\uC Projects\LPC1343_incubator\CMSIS_CORE_LPC13xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



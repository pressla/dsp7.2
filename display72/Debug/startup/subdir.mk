################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -I"C:/git/dsp/display72/inc" -I"C:/git/dsp/display72/CMSIS/core" -I"C:/git/dsp/display72/CMSIS/device" -I"C:/git/dsp/display72/StdPeriph_Driver/inc" -x assembler-with-cpp  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"


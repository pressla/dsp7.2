################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/fonts/font.c \
../src/fonts/lucida_console12.c \
../src/fonts/lucida_console14.c \
../src/fonts/lucida_console18.c \
../src/fonts/lucida_console22.c \
../src/fonts/lucida_console_bold_18.c \
../src/fonts/verdana10.c \
../src/fonts/verdana12.c \
../src/fonts/verdana18.c \
../src/fonts/verdana22.c \
../src/fonts/verdana36.c 

OBJS += \
./src/fonts/font.o \
./src/fonts/lucida_console12.o \
./src/fonts/lucida_console14.o \
./src/fonts/lucida_console18.o \
./src/fonts/lucida_console22.o \
./src/fonts/lucida_console_bold_18.o \
./src/fonts/verdana10.o \
./src/fonts/verdana12.o \
./src/fonts/verdana18.o \
./src/fonts/verdana22.o \
./src/fonts/verdana36.o 

C_DEPS += \
./src/fonts/font.d \
./src/fonts/lucida_console12.d \
./src/fonts/lucida_console14.d \
./src/fonts/lucida_console18.d \
./src/fonts/lucida_console22.d \
./src/fonts/lucida_console_bold_18.d \
./src/fonts/verdana10.d \
./src/fonts/verdana12.d \
./src/fonts/verdana18.d \
./src/fonts/verdana22.d \
./src/fonts/verdana36.d 


# Each subdirectory must supply rules for building sources it contributes
src/fonts/%.o: ../src/fonts/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/inc" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/src/fatfs" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/CMSIS/core" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/CMSIS/device" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/StdPeriph_Driver/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



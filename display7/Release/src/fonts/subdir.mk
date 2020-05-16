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
src/fonts/font.o: ../src/fonts/font.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fonts/font.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fonts/lucida_console12.o: ../src/fonts/lucida_console12.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fonts/lucida_console12.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fonts/lucida_console14.o: ../src/fonts/lucida_console14.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fonts/lucida_console14.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fonts/lucida_console18.o: ../src/fonts/lucida_console18.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fonts/lucida_console18.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fonts/lucida_console22.o: ../src/fonts/lucida_console22.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fonts/lucida_console22.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fonts/lucida_console_bold_18.o: ../src/fonts/lucida_console_bold_18.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fonts/lucida_console_bold_18.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fonts/verdana10.o: ../src/fonts/verdana10.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fonts/verdana10.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fonts/verdana12.o: ../src/fonts/verdana12.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fonts/verdana12.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fonts/verdana18.o: ../src/fonts/verdana18.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fonts/verdana18.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fonts/verdana22.o: ../src/fonts/verdana22.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fonts/verdana22.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fonts/verdana36.o: ../src/fonts/verdana36.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fonts/verdana36.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"


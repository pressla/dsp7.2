################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/fatfs/diskio.c \
../src/fatfs/fatfs_sd.c \
../src/fatfs/ff.c \
../src/fatfs/syscall.c \
../src/fatfs/tm_stm32f4_fatfs.c \
../src/fatfs/tm_stm32f4_gpio.c \
../src/fatfs/tm_stm32f4_spi.c \
../src/fatfs/unicode.c 

OBJS += \
./src/fatfs/diskio.o \
./src/fatfs/fatfs_sd.o \
./src/fatfs/ff.o \
./src/fatfs/syscall.o \
./src/fatfs/tm_stm32f4_fatfs.o \
./src/fatfs/tm_stm32f4_gpio.o \
./src/fatfs/tm_stm32f4_spi.o \
./src/fatfs/unicode.o 

C_DEPS += \
./src/fatfs/diskio.d \
./src/fatfs/fatfs_sd.d \
./src/fatfs/ff.d \
./src/fatfs/syscall.d \
./src/fatfs/tm_stm32f4_fatfs.d \
./src/fatfs/tm_stm32f4_gpio.d \
./src/fatfs/tm_stm32f4_spi.d \
./src/fatfs/unicode.d 


# Each subdirectory must supply rules for building sources it contributes
src/fatfs/diskio.o: ../src/fatfs/diskio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fatfs/diskio.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fatfs/fatfs_sd.o: ../src/fatfs/fatfs_sd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fatfs/fatfs_sd.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fatfs/ff.o: ../src/fatfs/ff.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fatfs/ff.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fatfs/syscall.o: ../src/fatfs/syscall.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fatfs/syscall.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fatfs/tm_stm32f4_fatfs.o: ../src/fatfs/tm_stm32f4_fatfs.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fatfs/tm_stm32f4_fatfs.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fatfs/tm_stm32f4_gpio.o: ../src/fatfs/tm_stm32f4_gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fatfs/tm_stm32f4_gpio.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fatfs/tm_stm32f4_spi.o: ../src/fatfs/tm_stm32f4_spi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fatfs/tm_stm32f4_spi.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fatfs/unicode.o: ../src/fatfs/unicode.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fatfs/unicode.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"


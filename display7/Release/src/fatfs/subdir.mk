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
src/fatfs/%.o: ../src/fatfs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/inc" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/src/fatfs" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/CMSIS/core" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/CMSIS/device" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/StdPeriph_Driver/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



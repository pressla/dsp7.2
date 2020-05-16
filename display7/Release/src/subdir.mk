################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc3_dma.c \
../src/adc_dma.c \
../src/adc_getvals.c \
../src/antsel.c \
../src/bandsel.c \
../src/bargraph.c \
../src/bargraph_defines.c \
../src/calib.c \
../src/controller.c \
../src/fan.c \
../src/flashdata.c \
../src/freq_meas.c \
../src/gpio.c \
../src/history.c \
../src/instrument.c \
../src/language.c \
../src/led.c \
../src/main.c \
../src/mainloop.c \
../src/menu.c \
../src/messagebox.c \
../src/ntc.c \
../src/powerlog.c \
../src/pwrswr.c \
../src/setup.c \
../src/smallflash.c \
../src/syscalls.c \
../src/system.c \
../src/system2.c \
../src/system_stm32f4xx.c \
../src/tft_highlevel.c \
../src/tft_lowlevel.c \
../src/tick1ms.c \
../src/touch_xsp2046.c \
../src/touchbutton.c \
../src/uart.c \
../src/uart_civ.c \
../src/wifi_if.c 

OBJS += \
./src/adc3_dma.o \
./src/adc_dma.o \
./src/adc_getvals.o \
./src/antsel.o \
./src/bandsel.o \
./src/bargraph.o \
./src/bargraph_defines.o \
./src/calib.o \
./src/controller.o \
./src/fan.o \
./src/flashdata.o \
./src/freq_meas.o \
./src/gpio.o \
./src/history.o \
./src/instrument.o \
./src/language.o \
./src/led.o \
./src/main.o \
./src/mainloop.o \
./src/menu.o \
./src/messagebox.o \
./src/ntc.o \
./src/powerlog.o \
./src/pwrswr.o \
./src/setup.o \
./src/smallflash.o \
./src/syscalls.o \
./src/system.o \
./src/system2.o \
./src/system_stm32f4xx.o \
./src/tft_highlevel.o \
./src/tft_lowlevel.o \
./src/tick1ms.o \
./src/touch_xsp2046.o \
./src/touchbutton.o \
./src/uart.o \
./src/uart_civ.o \
./src/wifi_if.o 

C_DEPS += \
./src/adc3_dma.d \
./src/adc_dma.d \
./src/adc_getvals.d \
./src/antsel.d \
./src/bandsel.d \
./src/bargraph.d \
./src/bargraph_defines.d \
./src/calib.d \
./src/controller.d \
./src/fan.d \
./src/flashdata.d \
./src/freq_meas.d \
./src/gpio.d \
./src/history.d \
./src/instrument.d \
./src/language.d \
./src/led.d \
./src/main.d \
./src/mainloop.d \
./src/menu.d \
./src/messagebox.d \
./src/ntc.d \
./src/powerlog.d \
./src/pwrswr.d \
./src/setup.d \
./src/smallflash.d \
./src/syscalls.d \
./src/system.d \
./src/system2.d \
./src/system_stm32f4xx.d \
./src/tft_highlevel.d \
./src/tft_lowlevel.d \
./src/tick1ms.d \
./src/touch_xsp2046.d \
./src/touchbutton.d \
./src/uart.d \
./src/uart_civ.d \
./src/wifi_if.d 


# Each subdirectory must supply rules for building sources it contributes
src/adc3_dma.o: ../src/adc3_dma.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/adc3_dma.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/adc_dma.o: ../src/adc_dma.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/adc_dma.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/adc_getvals.o: ../src/adc_getvals.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/adc_getvals.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/antsel.o: ../src/antsel.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/antsel.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/bandsel.o: ../src/bandsel.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/bandsel.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/bargraph.o: ../src/bargraph.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/bargraph.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/bargraph_defines.o: ../src/bargraph_defines.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/bargraph_defines.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/calib.o: ../src/calib.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/calib.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/controller.o: ../src/controller.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/controller.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/fan.o: ../src/fan.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/fan.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/flashdata.o: ../src/flashdata.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/flashdata.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/freq_meas.o: ../src/freq_meas.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/freq_meas.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/gpio.o: ../src/gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/gpio.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/history.o: ../src/history.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/history.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/instrument.o: ../src/instrument.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/instrument.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/language.o: ../src/language.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/language.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/led.o: ../src/led.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/led.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/main.o: ../src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/main.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/mainloop.o: ../src/mainloop.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/mainloop.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/menu.o: ../src/menu.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/menu.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/messagebox.o: ../src/messagebox.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/messagebox.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/ntc.o: ../src/ntc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/ntc.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/powerlog.o: ../src/powerlog.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/powerlog.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/pwrswr.o: ../src/pwrswr.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/pwrswr.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/setup.o: ../src/setup.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/setup.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/smallflash.o: ../src/smallflash.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/smallflash.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/syscalls.o: ../src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/syscalls.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/system.o: ../src/system.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/system.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/system2.o: ../src/system2.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/system2.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/system_stm32f4xx.o: ../src/system_stm32f4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/system_stm32f4xx.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/tft_highlevel.o: ../src/tft_highlevel.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/tft_highlevel.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/tft_lowlevel.o: ../src/tft_lowlevel.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/tft_lowlevel.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/tick1ms.o: ../src/tick1ms.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/tick1ms.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/touch_xsp2046.o: ../src/touch_xsp2046.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/touch_xsp2046.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/touchbutton.o: ../src/touchbutton.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/touchbutton.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/uart.o: ../src/uart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/uart.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/uart_civ.o: ../src/uart_civ.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/uart_civ.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/wifi_if.o: ../src/wifi_if.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -c -I"C:/Users/Ca$her/git/dsp/display7/inc" -I"C:/Users/Ca$her/git/dsp/display7/src/fatfs" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/core" -I"C:/Users/Ca$her/git/dsp/display7/CMSIS/device" -I"C:/Users/Ca$her/git/dsp/display7/StdPeriph_Driver/inc" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"src/wifi_if.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"


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
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4 -DSTM32F427VGTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F427_437xx -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/inc" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/src/fatfs" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/CMSIS/core" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/CMSIS/device" -I"/Daten1/Daten/funk/unidisplay/dsp7a/display7/StdPeriph_Driver/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



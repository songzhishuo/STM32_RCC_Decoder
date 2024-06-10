################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/DS1302/DS1302.c 

OBJS += \
./User/DS1302/DS1302.o 

C_DEPS += \
./User/DS1302/DS1302.d 


# Each subdirectory must supply rules for building sources it contributes
User/DS1302/%.o: ../User/DS1302/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"E:\work\2022_BiShe\STM32dianbozhong\CH32V003F4P6_VFD_电波_v0.2\Debug" -I"E:\work\2022_BiShe\STM32dianbozhong\CH32V003F4P6_VFD_电波_v0.2\Core" -I"E:\work\2022_BiShe\STM32dianbozhong\CH32V003F4P6_VFD_电波_v0.2\User" -I"E:\work\2022_BiShe\STM32dianbozhong\CH32V003F4P6_VFD_电波_v0.2\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@


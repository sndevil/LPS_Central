################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../ADF7020.cpp \
../FIFO.cpp \
../functions.cpp \
../GlobalVariables.cpp \
../main.cpp \
../Message.cpp \
../Packet.cpp \
../Serial.cpp \
../StationInfo.cpp \
../Transceiver.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
ADF7020.o \
FIFO.o \
functions.o \
GlobalVariables.o \
main.o \
Message.o \
Packet.o \
Serial.o \
StationInfo.o \
Transceiver.o

OBJS_AS_ARGS +=  \
ADF7020.o \
FIFO.o \
functions.o \
GlobalVariables.o \
main.o \
Message.o \
Packet.o \
Serial.o \
StationInfo.o \
Transceiver.o

C_DEPS +=  \
ADF7020.d \
FIFO.d \
functions.d \
GlobalVariables.d \
main.d \
Message.d \
Packet.d \
Serial.d \
StationInfo.d \
Transceiver.d

C_DEPS_AS_ARGS +=  \
ADF7020.d \
FIFO.d \
functions.d \
GlobalVariables.d \
main.d \
Message.d \
Packet.d \
Serial.d \
StationInfo.d \
Transceiver.d

OUTPUT_FILE_PATH +=BaseStationCenter.elf

OUTPUT_FILE_PATH_AS_ARGS +=BaseStationCenter.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler





















./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -O3 -ffunction-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega32a -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Linker : 3.4.2
	$(QUOTE)C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="BaseStationCenter.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega32a 
	@echo Finished building target: $@
	"C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "BaseStationCenter.elf" "BaseStationCenter.hex"
	"C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "BaseStationCenter.elf" "BaseStationCenter.eep" || exit 0
	"C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "BaseStationCenter.elf" > "BaseStationCenter.lss"
	"C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "BaseStationCenter.elf" "BaseStationCenter.srec"
	"C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-size.exe" "BaseStationCenter.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "BaseStationCenter.elf" "BaseStationCenter.a" "BaseStationCenter.hex" "BaseStationCenter.lss" "BaseStationCenter.eep" "BaseStationCenter.map" "BaseStationCenter.srec"
	
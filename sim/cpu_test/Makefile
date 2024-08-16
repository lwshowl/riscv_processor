
CXX = riscv64-linux-gnu-g++
OBJDUMP = riscv64-linux-gnu-objdump
OBJCOPY = riscv64-linux-gnu-objcopy
LD = riscv64-linux-gnu-ld
SRC = ./src
BUILD = ./build

CXXFLAGS += -fno-builtin -ffreestanding -fno-rtti -fno-exceptions
CXXFLAGS += -nostdlib 
CXXFLAGS += -march=rv64g -mcmodel=medany -mstrict-align -fno-pic
CXXFLAGS += -I./include

all: $(TARGET).o start.o
	@ echo linking objects
	@ $(LD) $(BUILD)/$(TARGET).o $(BUILD)/start.o -T linker.ld -o $(BUILD)/$(TARGET).elf
	@ echo copy disassembly and binary
	@ $(OBJDUMP) $(BUILD)/$(TARGET).elf -D >  $(BUILD)/$(TARGET).disasm
	@ $(OBJCOPY) -O binary $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin 
 
$(TARGET).o: $(SRC)/$(TARGET).cpp
	@ echo +CXX $<
	@ $(CXX) -c $< $(CXXFLAGS) -o $(BUILD)/$@

start.o: start.s
	@ echo +CXX $<
	@ $(CXX) -c $< $(CXXFLAGS) -o $(BUILD)/$@

	
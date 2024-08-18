
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

all: clean $(TARGET).o start.o std_libs
	@ echo linking objects
	@ $(LD) $(BUILD)/*.o -T linker.ld -o $(BUILD)/$(TARGET).elf
	@ echo copy disassembly and binary
	@ $(OBJDUMP) $(BUILD)/$(TARGET).elf -D >  $(BUILD)/$(TARGET).disasm
	@ $(OBJCOPY) -O binary $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin

std_libs:
	$(CXX) -c $(SRC)/stdlib/stdio.c $(CXXFLAGS) -o $(BUILD)/stdio.o
	$(CXX) -c $(SRC)/stdlib/stdlib.c $(CXXFLAGS) -o $(BUILD)/stdlib.o
	$(CXX) -c $(SRC)/stdlib/string.c $(CXXFLAGS) -o $(BUILD)/string.o

$(TARGET).o: $(SRC)/$(TARGET).cpp
	@ echo +CXX $<
	@ $(CXX) -c $< $(CXXFLAGS) -o $(BUILD)/$@

start.o: start.s
	@ echo +CXX $<
	@ $(CXX) -c $< $(CXXFLAGS) -o $(BUILD)/$@

clean:
	@ -rm build/*


	
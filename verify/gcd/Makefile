CC = riscv32-unknown-elf-gcc
LDFLAGS = -nostdlib -mcmodel=medany
MARCH = -march=rv32i
ABI = -mabi=ilp32
LINKER = link.ld


tinyrv32: start.o main.o
	$(CC) $(MARCH) $(ABI) $(LDFLAGS) -T $(LINKER)  $^ -o $@.elf

start.o: ../start.s
	$(CC) -c $(MARCH) $(ABI) $(LDFLAGS) $^ -o $@

main.o : main.cpp
	$(CC) -c $(MARCH) $(ABI) $(LDFLAGS) $^ -o $@

hex: tinyrv32
	elf2hex 1 512 $^.elf > ../tinyrv32.hex

disass: tinyrv32
	riscv32-unknown-elf-objdump -D tinyrv32.elf > ../tinyrv32.disass

clean:
	rm *.o *.elf *.hex

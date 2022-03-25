CC = riscv32-unknown-elf-gcc
LDFLAGS = -nostdlib -mcmodel=medany
MARCH = -march=rv32i
ABI = -mabi=ilp32
OPTI = -O0
LINKER = link.ld


tinyrv32: start.o main.o delay.o
	$(CC) $(MARCH) $(ABI) $(LDFLAGS) $(OPTI) -T ../$(LINKER)  $^ -o $@.elf

start.o: ../start.s
	$(CC) -c $(MARCH) $(ABI) $(OPTI) $(LDFLAGS) $^ -o $@

main.o: main.cpp
	$(CC) -c $(MARCH) $(ABI) $(OPTI) $(LDFLAGS) $^ -o $@

delay.o : ../delay.s
	$(CC) -c $(MARCH) $(ABI) $(OPTI) $(LDFLAGS) $^ -o $@

hex: tinyrv32
	elf2hex 1 2048 $^.elf > ../tinyrv32.hex

coe: tinyrv32
	elf2hex 4 2048 $^.elf > ../tinyrv32.hex4
	../../util/a.out

disass: tinyrv32
	riscv32-unknown-elf-objdump -D tinyrv32.elf > ../tinyrv32.disass

clean:
	rm *.o *.elf ../*.hex ../*.disass

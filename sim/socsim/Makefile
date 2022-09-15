TARGET = core
V_DIR = ../verilator
BUILD = ./build
CXX = g++
SRC = ./src
VERILATOR_HOME = /usr/share/verilator/include

IMAGE = ../cpu_test/build/basic_test.bin
DIFF_TEST_SO = ../difftest/spike-diff/build/riscv64-spike-so


INCLUDE_DIR = -I../verilator \
			  -I../verilator/include \
			  -I../verilator/obj_dir \
			  -I./ \
			  -I../ \
			  -I$(VERILATOR_HOME) \
			  -I$(VERILATOR_HOME)/vltstd

SOC_OBJS += $(BUILD)/socsim.o
SOC_OBJS += $(BUILD)/axi_slave_mem.o
SOC_OBJS += $(BUILD)/difftest.o
SOC_OBJS += $(BUILD)/monitor.o

CFLAGS += -DIMAGE=\"$(IMAGE)\"
CFLAGS += -DDIFF_TEST_SO=\"$(DIFF_TEST_SO)\"
CFLAGS += -fPIC

VC_LDS = socsim.o monitor.o difftest.o axi_slave_mem.o

# to build binary ,both difftest and verilator model object is required
# last command link difftest and verilated model together
binary: verilate spike-diff $(SOC_OBJS)
	@cp $(BUILD)/*.o ../verilator/obj_dir
	@make -C $(V_DIR) recipes EXT_LD+=./socsim.o EXT_LD+=./monitor.o EXT_LD+=./difftest.o EXT_LD+=./axi_slave_mem.o

run: binary
	@$(V_DIR)/obj_dir/Vcore

gdb: binary
	@gdb $(V_DIR)/obj_dir/Vcore

# verilate verilator model
verilate:
	@make -C $(V_DIR) verilate

spike-diff:
	@make -C ../difftest/spike-diff GUEST_ISA=riscv64 --silent

# build socsim
$(BUILD)/%.o: ./src/%.cpp
	@echo + CXX $<
	@mkdir -p $(dir $(BUILD))
	@$(CXX) -c -g  $(INCLUDE_DIR) $(CFLAGS) -ldl -o $@ $<

clean:
	@make -C $(V_DIR) clean
	-rm ./build/*

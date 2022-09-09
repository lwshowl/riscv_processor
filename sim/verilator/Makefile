TARGET = core
OBJ_DIR = ./obj_dir
V_DIR = ../../rtl/core
TESTBENCH_DIR = ./src
VERILATOR_INC = /usr/share/verilator/include
NEMUHOME := $(shell echo $$NEMU_HOME)
CXX = g++

C_SRC_DIR = ~/npc/rv64/sim/verilator/src

V_FLAGS =   -Wall -cc --trace --exe \
			-I../../rtl/core \
		 	-I../../rtl/bus \
		 	-I../../rtl/templates

VC_FLAGS =  -CFLAGS -I./inc \
			-CFLAGS -I./src \
			-CFLAGS -I./obj_dir \
			-CFLAGS -I./ \
			-CFLAGS -I../ \
			-CFLAGS -I../../ \
			-CFLAGS -g

VC_LDFLAGS = -LDFLAGS ../build_tb/core_dump.o \
			 -LDFLAGS ../build_tb/util.o \
			 -LDFLAGS -lpthread

OBJS += ./build_tb/core_dump.o 
OBJS += ./build_tb/util.o

# to verilated model in nemu
nemu : build_nemu run_nemu

# build nemu (link verilated model with nemu)
build_nemu : verilate recipes copy_obj
	$(info building nemu)	
	@ make -C $(NEMU_HOME)

# run NEMU
run_nemu:
	$(info launching nemu)
	@ make -C $(NEMU_HOME) run

# build recipes for verilated exectuable (link cpp and verilated model together)
recipes: verilate $(OBJS)
	@ cd $(OBJ_DIR)
	@ make -C $(OBJ_DIR) -f V$(TARGET).mk --silent

# build cpp
./build_tb/%.o: ./src/%.cpp
	@echo + CXX $<
	@mkdir -p $(dir $@)
	@$(CXX) -I./inc -I./src -I./obj_dir -I./ -I$(VERILATOR_INC) -I$(VERILATOR_INC)/vltstd -c -o $@ $<

# veriate verilog file
verilate:
	@echo verilating
	@verilator $(V_FLAGS) $(VC_FLAGS) $(VC_LDFLAGS) $(EXT_LD) $(V_DIR)/$(TARGET).v $(TESTBENCH_DIR)/tb_$(TARGET).cpp

# copy objs and archives to nemu
copy_obj : recipes
	$(info copying objects to nemu/isa/npc/verilated)
	@ mv $(OBJ_DIR)/*.o $(NEMU_HOME)/src/isa/npc/verilated
	@ mv $(OBJ_DIR)/*.a $(NEMU_HOME)/src/isa/npc/verilated

# run verilated model
run :recipes
	$(OBJ_DIR)/V$(TARGET)

clean:
	-rm ./build_tb/*
	-rm $(OBJ_DIR)/*


TARGET = npc
OBJ_DIR = ./obj_dir
TESTBENCH_DIR = ./test_benches
HOST_DIR = ./hosts
NEMUHOME := $(shell echo $$NEMU_HOME) 

V_SRC = /home/xy/npc/rv64

nemu : build_nemu run_nemu

build_nemu : verilate compile_host recipes copy_obj
	$(info building nemu)	
	@ make -C $(NEMU_HOME)

run_nemu:
	$(info launching nemu)
	@ make -C $(NEMU_HOME) run

compile_host:
	$(info compiling hosts)
	@ g++ -c -Wall -std=c++11 -O2 -o $(HOST_DIR)/host.o $(HOST_DIR)/host.cpp

recipes: verilate
	@ $(info verilating target modules)
	@ cd $(OBJ_DIR)
	@- make -C $(OBJ_DIR) -f V$(TARGET).mk --silent

verilate:
	@ verilator -Wall --cc --trace -CFLAGS -lstdc++  $(V_SRC)/$(TARGET).v --exe $(TESTBENCH_DIR)/tb_$(TARGET).cpp

copy_obj : recipes
	$(info copying objects to nemu/isa/npc/verilated)
	@ mv $(OBJ_DIR)/*.o $(NEMU_HOME)/src/isa/npc/verilated
	@ mv $(OBJ_DIR)/*.a $(NEMU_HOME)/src/isa/npc/verilated


run :recipes
	$(OBJ_DIR)/V$(TARGET)

clean:
	rm $(OBJ_DIR)/*


TARGET = top
OBJ_DIR = ./obj_dir
TESTBENCH_DIR = ./test_benches
HOST_DIR = ./hosts
NEMUHOME := $(shell echo $$NEMU_HOME) 

V_SRC =  /home/xy/npc/rv64/rtl/core \
		 /home/xy/npc/rv64/rtl/bus \
		 /home/xy/npc/rv64/rtl/templates

nemu : build_nemu run_nemu

build_nemu : verilate recipes copy_obj
	$(info building nemu)	
	@ make -C $(NEMU_HOME)

run_nemu:
	$(info launching nemu)
	@ make -C $(NEMU_HOME) run

recipes: verilate
	@ $(info verilating target modules)
	@ cd $(OBJ_DIR)
	@- make -C $(OBJ_DIR) -f V$(TARGET).mk --silent

verilate:
	@ verilator -Wall --cc --trace -CFLAGS -lstdc++  -IV_SRC $(V_SRC)/$(TARGET).v --exe $(TESTBENCH_DIR)/tb_$(TARGET).cpp

copy_obj : recipes
	$(info copying objects to nemu/isa/npc/verilated)
	@ mv $(OBJ_DIR)/*.o $(NEMU_HOME)/src/isa/npc/verilated
	@ mv $(OBJ_DIR)/*.a $(NEMU_HOME)/src/isa/npc/verilated

run :recipes
	$(OBJ_DIR)/V$(TARGET)

clean:
	rm $(OBJ_DIR)/*


TARGET = core
V_DIR = ../verilator
BUILD = ./build
CXX = g++
SRC = ./src
VERILATOR_HOME = /usr/share/verilator/include

INCLUDE_DIR = -I../verilator \
			  -I../verilator/inc \
			  -I../verilator/obj_dir \
			  -I./ \
			  -I../ \
			  -I$(VERILATOR_HOME) \
			  -I$(VERILATOR_HOME)/vltstd

DIFF_OBJS += $(BUILD)/socsim.o
DIFF_OBJS += $(BUILD)/axi_slave_mem.o

# to build binary ,both difftest and verilator model object is required
# last command link difftest and verilated model together
binary: verilate $(DIFF_OBJS)
	@cp $(BUILD)/*.o ../verilator/obj_dir
	@make -C $(V_DIR) recipes EXT_LD+=./socsim.o EXT_LD+=./axi_slave_mem.o

run: binary
	$(V_DIR)/obj_dir/Vcore

# verilate verilator model
verilate:
	@make -C $(V_DIR) verilate

# build socsim
$(BUILD)/%.o: ./src/%.cpp
	@echo + CXX $<
	@mkdir -p $(dir $(BUILD))
	@$(CXX) -c -g -lpthread $(INCLUDE_DIR) -o $@ $<

clean:
	@make -C $(V_DIR) clean
	-rm ./build/*

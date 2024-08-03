VERILATOR_INC = /usr/share/verilator/include
V_FLAGS =   -Wall --lint-only \
			-I./core \
		 	-I./bus \
		 	-I./templates \

lint:
	@echo lint
	verilator $(V_FLAGS) core/core.sv

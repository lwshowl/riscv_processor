VERILATOR_INC = /usr/share/verilator/include
V_FLAGS =   -Wall --lint-only \
			-I./core \
		 	-I./bus \
		 	-I./templates \
			-I./core/intermediate \
			-I./core/pipeline_control

lint:
	@echo lint
	verilator $(V_FLAGS) core/core.sv

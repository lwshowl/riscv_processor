#pragma once

#include <stdint.h>
#include <queue>
#include <stdlib.h>
#include <iostream>
#include <Vcore.h>
#include <verilated.h>
#include <Vcore__Dpi.h>
#include <verilated_vcd_c.h>
#include <thread>
#include "instructions.h"

#define MAX_SIM_TIME -1
vluint64_t sim_time = 0;
vluint64_t posedge_count = 0;
Vcore *dut = new Vcore;
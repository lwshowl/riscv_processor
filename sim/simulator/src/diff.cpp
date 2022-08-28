#include <iostream>
#include "verilator/obj_dir/Vcore.h"
#include "verilator/inc/sim_conf.h"
#include "inc/axi_slave_mem.h"

#define bin_path "0"

int main(int *argc, char **argv)
{
    // init axi4 slave mem provided by cyy
    mem.load_binary(bin_path, 0x80000000);

    // connect to core's axi interface

    // write address
    mem_ptr.awaddr = &(dut->core__DOT__ac0__DOT__axi_aw_addr);
    mem_ptr.awburst = &(dut->core__DOT__ac0__DOT__axi_aw_burst);
    mem_ptr.awid = &(dut->core__DOT__ac0__DOT__axi_aw_id);
    mem_ptr.awlen = &(dut->core__DOT__ac0__DOT__axi_aw_len);
    mem_ptr.awready = &(dut->core__DOT__ac0__DOT__axi_aw_ready);
    mem_ptr.awsize = &(dut->core__DOT__ac0__DOT__axi_aw_size);
    mem_ptr.awvalid = &(dut->core__DOT__ac0__DOT__axi_aw_valid);

    // write data
    mem_ptr.wdata = &(dut->core__DOT__ac0__DOT__axi_w_data);
    mem_ptr.wlast = &(dut->core__DOT__ac0__DOT__axi_w_last);
    mem_ptr.wready = &(dut->core__DOT__ac0__DOT__axi_w_ready);
    mem_ptr.wstrb = &(dut->core__DOT__ac0__DOT__axi_w_strb);
    mem_ptr.wvalid = &(dut->core__DOT__ac0__DOT__axi_w_valid);

    // write response
    mem_ptr.bid = &(dut->core__DOT__ac0__DOT__axi_b_id);
    mem_ptr.bready = &(dut->core__DOT__ac0__DOT__axi_b_ready);
    mem_ptr.bresp = &(dut->core__DOT__ac0__DOT__axi_b_resp);
    mem_ptr.bvalid = &(dut->core__DOT__ac0__DOT__axi_b_valid);

    // read address
    mem_ptr.araddr = &(dut->core__DOT__ac0__DOT__axi_ar_addr);
    mem_ptr.arburst = &(dut->core__DOT__ac0__DOT__axi_ar_burst);
    mem_ptr.arid = &(dut->core__DOT__ac0__DOT__axi_ar_id);
    mem_ptr.arlen = &(dut->core__DOT__ac0__DOT__axi_ar_len);
    mem_ptr.arready = &(dut->core__DOT__ac0__DOT__axi_r_ready);
    mem_ptr.arsize = &(dut->core__DOT__ac0__DOT__axi_ar_size);
    mem_ptr.arvalid = &(dut->core__DOT__ac0__DOT__axi_ar_valid);

    // read data
    mem_ptr.rdata = &(dut->core__DOT__ac0__DOT__axi_r_data);
    mem_ptr.rid = &(dut->core__DOT__ac0__DOT__axi_r_id);
    mem_ptr.rlast = &(dut->core__DOT__ac0__DOT__axi_r_last);
    mem_ptr.rready = &(dut->core__DOT__ac0__DOT__axi_r_ready);
    mem_ptr.rresp = &(dut->core__DOT__ac0__DOT__axi_r_resp);
    mem_ptr.rvalid = &(dut->core__DOT__ac0__DOT__axi_r_valid);

    assert(mem_ptr.check());
    printf("\033[1;32m axi4 slave memory connected \033[0m\n");




}
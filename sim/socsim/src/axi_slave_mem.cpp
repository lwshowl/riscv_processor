#include "inc/axi_slave_mem.h"

extern Vcore *dut;

void axi_mem_connect(axi4_ptr<64, 64, 4> &ptr)
{
    // write address
    ptr.awaddr = &(dut->axi_aw_addr);
    ptr.awburst = &(dut->axi_aw_burst);
    ptr.awid = &(dut->axi_aw_id);
    ptr.awlen = &(dut->axi_aw_len);
    ptr.awready = &(dut->axi_aw_ready);
    ptr.awsize = &(dut->axi_aw_size);
    ptr.awvalid = &(dut->axi_aw_valid);

    // write data
    ptr.wdata = &(dut->axi_w_data);
    ptr.wlast = &(dut->axi_w_last);
    ptr.wready = &(dut->axi_w_ready);
    ptr.wstrb = &(dut->axi_w_strb);
    ptr.wvalid = &(dut->axi_w_valid);

    // write response
    ptr.bid = &(dut->axi_b_id);
    ptr.bready = &(dut->axi_b_ready);
    ptr.bresp = &(dut->axi_b_resp);
    ptr.bvalid = &(dut->axi_b_valid);

    // read address
    ptr.araddr = &(dut->axi_ar_addr);
    ptr.arburst = &(dut->axi_ar_burst);
    ptr.arid = &(dut->axi_ar_id);
    ptr.arlen = &(dut->axi_ar_len);
    ptr.arready = &(dut->axi_ar_ready);
    ptr.arsize = &(dut->axi_ar_size);
    ptr.arvalid = &(dut->axi_ar_valid);

    // read data
    ptr.rdata = &(dut->axi_r_data);
    ptr.rid = &(dut->axi_r_id);
    ptr.rlast = &(dut->axi_r_last);
    ptr.rready = &(dut->axi_r_ready);
    ptr.rresp = &(dut->axi_r_resp);
    ptr.rvalid = &(dut->axi_r_valid);

    assert(ptr.check());
    printf("\033[1;32m axi4 slave memory connected \033[0m\n");
}
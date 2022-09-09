#include "inc/core_dump.h"
#include "inc/nemu.h"
#include "instructions.h"
#include <iostream>
#include <iomanip>
#include <bitset>
using namespace std;

void dump_axi_ctl()
{
    cout << " axi control state: " << (uint64_t)dut->core__DOT__ac0__DOT__state
         << " axi fifo cnt: " << (uint64_t)dut->core__DOT__ac0__DOT__cnt
         << " axi fifo data: " << (uint64_t)dut->core__DOT__ac0__DOT__axi_rdata
         << " axi fifo wen: " << (uint64_t)dut->core__DOT__ac0__DOT__fifo_wen
         << endl;

    // check if an axi req has finished
    int ac0_state_fifo = 3;
    if (dut->core__DOT__ac0__DOT__state == ac0_state_fifo)
    {
        cout << " ac0: an axi request has finished , fifo dump: " << endl;
        for (uint64_t i : dut->core__DOT__ac0__DOT__fifo)
        {
            cout << std::hex << std::setfill('0') << std::setw(8) << (uint64_t)i << " ";
        }
        cout << endl;
    }
}

void dump_axi()
{
    cout << " axi rw valid: " << (uint64_t)dut->core__DOT__ac0__DOT__axi_rw_valid

         << " axi rd state: " << (uint64_t)dut->core__DOT__ac0__DOT__a0__DOT__rd_state
         << " axi ar valid: " << (uint64_t)dut->axi_ar_valid
         << " axi ar ready: " << (uint64_t)dut->axi_ar_ready
         << " axi r valid: " << (uint64_t)dut->axi_r_valid
         << " axi r last: " << (uint64_t)dut->axi_r_last
         << " axi ar addr: " << (uint64_t)dut->axi_ar_addr
         << " axi r data: " << (uint64_t)dut->axi_r_data
         << endl;
}

void dump_icache()
{
    cout << " cache addr: " << std::hex << (uint64_t)dut->core__DOT__pc_out
         << " presence bits: " << std::bitset<8>((uint64_t)dut->core__DOT__ic0__DOT__presence_w)
         << " cache data: " << hex << setfill('0') << setw(8) << (uint64_t)dut->core__DOT__ic0__DOT__way_ram_out[dut->core__DOT__ic0__DOT__hit_way]
         << " cache replace: " << (uint64_t)dut->core__DOT__ic0__DOT__way_replace
         << " cache state: " << std::hex << (uint64_t)dut->core__DOT__ic0__DOT__state
         << " cache waddr: " << std::hex << (uint64_t)dut->core__DOT__ic0__DOT__w_offset
         << " cache fifo: " << std::hex << (uint64_t)dut->core__DOT__if_axi_data
         << " cache fifo cnt: " << std::hex << (uint64_t)dut->core__DOT__ic0__DOT__cnt
         << " cache hit_way: " << std::hex << (uint64_t)(dut->core__DOT__ic0__DOT__m0__DOT__i0__DOT__lut_out)
         << endl;
    int cache_refill = 1;

    for (auto var : dut->core__DOT__ic0__DOT__genblk1__BRA__5__KET____DOT__ram__DOT__mem[0])
        cout << std::hex << std::setfill('0') << std::setw(2) << (uint64_t)var << " ";
    cout << endl;

    // // check if a line is replaced
    // if (dut->core__DOT__ic0__DOT__state == cache_refill && dut->core__DOT__ic0__DOT__w_offset == 65)
    // {
    //     cout << " a line has been replaced "
    //          << " replace way number " << (uint64_t)dut->core__DOT__ic0__DOT__way_replace
    //          << " mem dump" << endl;
    //     for (auto line : dut->core__DOT__ic0__DOT__genblk1__BRA__5__KET____DOT__ram__DOT__mem)
    //     {
    //         for (int offset = 0; offset < 64; offset++)
    //             cout << std::hex << std::setfill('0') << std::setw(2) << (uint64_t)line[offset] << " ";
    //         cout << endl;
    //     }
    // }
}

void update_dnpc()
{
    if (dut->core__DOT__dmem_branch_out == 1 && dut->core__DOT__dmem_instrId_out != i_jalr && dut->core__DOT__dmem_result_out)
    {
        // cout << "dnpc pushed at " << dut->core__DOT__dmem_pc_out << " target " << dut->core__DOT__dmem_pc_out + dut->core__DOT__dmem_imm64_out << endl;
        dnpc_queue.push(dut->core__DOT__dmem_pc_out + dut->core__DOT__dmem_imm64_out);
        // cout << endl
        //      << "update dnpc pc= << dut->core__DOT__dmem_pc_out << " imm= " << dut->core__DOT__dmem_imm64_out
        //      << endl
        //      << endl;
    }

    else if (dut->core__DOT__dmem_instrId_out == i_jalr)
    {
        dnpc_queue.push(dut->core__DOT__dmem_rs1val_out + dut->core__DOT__dmem_imm64_out);
        // cout << "update dnpc pc= " << dut->core__DOT__dmem_pc_out << " sum= " << dut->core__DOT__dmem_rs1val_out + dut->core__DOT__dmem_imm64_out;
    }

    else if (dut->core__DOT__dmem_excep_out > 0)
    {
        dnpc_queue.push(dut->core__DOT__csr__DOT__mtvec);
    }

    else if (dut->core__DOT__wb_instrId_out == i_ecall)
    {
        dnpc_queue.push(dut->core__DOT__csr__DOT__mtvec);
    }

    else if (dut->core__DOT__wb_instrId_out == i_mret)
    {
        dnpc_queue.push(dut->core__DOT__csr__DOT__mepc);
    }
}

void core_reset()
{
    dut->rst = 1;
    dut->clk ^= 1;
    dut->eval();
    dut->clk ^= 1;
    dut->eval();
    dut->clk ^= 1;
    dut->eval();
    dut->rst = 0;
}

void core_dump_registers(uint64_t *regs)
{
    for (int i = 0; i < 32; i++)
        regs[i] = dut->core__DOT__registerFile__DOT__registers[i];
}

// void core_mem_read(long long raddr, long long *rdata)
// {
//     // std::cout << "pmem_read: " << hex << raddr;
//     // cout << hex << " val: " << *rdata << endl;
//     // cout << "fetch pc :" << dut->core__DOT__pc_out << " visit addr :" << raddr << endl;
//     // cout << "dmem pc :" << dut->core__DOT__dmem_pc_out << " visit addr :" << raddr << endl;

//     *rdata = vaddr_read(raddr, 8);
// }

// // DPI import at core.v:231
// void core_mem_write(long long addr, long long wdata, char wmask)
// {
//     // std::cout << "instr: " << dec << (uint64_t)(dut->core__DOT__dmem_instrId);
//     // std::cout << " pmem_write: " << hex << addr;
//     // cout << hex << " val: " << wdata << endl;
//     vaddr_write(addr, wmask, wdata);
// }

int sanity_check()
{
    if (dut->core__DOT__wb_instrId_out == 63)
        if (sim_time > 10)
        {
            uint64_t pc = dut->core__DOT__wb_pc_out;
            // invalid_inst(pc);
            return 1;
        }
    return 0;
}

void dump_csr()
{
    cout << "mtvec " << hex << (uint64_t)dut->core__DOT__csr__DOT__mtvec
         << " mcause " << hex << (uint64_t)dut->core__DOT__csr__DOT__mcause
         << " mepc " << hex << (uint64_t)dut->core__DOT__csr__DOT__mepc
         << " mstatus " << hex << (uint64_t)dut->core__DOT__csr__DOT__mstatus
         << endl;
}

void dump_pc()
{
    cout << "pc out: " << hex << (uint64_t)dut->core__DOT__pc_out
         << "pc abs_b: " << hex << (uint64_t)dut->core__DOT__pc_abs_branch
         << "pc rel_b: " << hex << (uint64_t)dut->core__DOT__pc_rel_branch
         << endl;
}

void dump_regfile()
{
    cout << "reg instru: " << dec << (uint64_t)dut->core__DOT__regfile_instrId_out
         << " reg pc: " << hex << (uint64_t)dut->core__DOT__regfile_pc_out
         << " reg rs1: " << hex << (uint64_t)dut->core__DOT__regfile_rs1_out
         << " reg rs2: " << hex << (uint64_t)dut->core__DOT__regfile_rs2_out
         << " reg rd: " << hex << (uint64_t)dut->core__DOT__regfile_rd_out
         << " reg branch: " << hex << (uint64_t)dut->core__DOT__regfile_branch_out
         << " reg regw: " << hex << (uint64_t)dut->core__DOT__regfile_regw_out
         << " reg alubypass1: " << dec << (uint64_t)dut->core__DOT__regfile_alubypass_rs1
         << " reg alubypass2: " << dec << (uint64_t)dut->core__DOT__regfile_alubypass_rs2
         << " reg membypass1: " << dec << (uint64_t)dut->core__DOT__regfile_dmembypass_rs1
         << " reg membypass2: " << dec << (uint64_t)dut->core__DOT__regfile_dmembypass_rs2
         << " reg memorybubble: " << dec << (uint64_t)dut->core__DOT__memory_bubble
         << endl;
}

void dump_wb()
{
    cout << "wb instru: " << dec << (uint64_t)dut->core__DOT__wb_instrId_out
         << " wb pc: " << hex << (uint64_t)dut->core__DOT__wb_pc_out
         << " wb rd: " << reg_name[(uint64_t)dut->core__DOT__wb_rd_out]
         << " wb memdata: " << hex << (uint64_t)dut->core__DOT__wb_memdata_out
         << " wb regw: " << hex << (uint64_t)dut->core__DOT__wb_regw_out
         << " wb branch: " << dec << (uint64_t)dut->core__DOT__wb_branch_out
         << " wb memw: " << dec << (uint64_t)dut->core__DOT__wb_memw_out
         << " wb opcode: " << dec << (uint64_t)dut->core__DOT__wb_opcode_out
         << " wb rs1val: " << hex << (uint64_t)dut->core__DOT__wb_rs1val_out
         << " wb rs2val: " << hex << (uint64_t)dut->core__DOT__wb_rs2val_out
         << " wb result: " << hex << (uint64_t)dut->core__DOT__wb_result_out
         << " wb exception: " << hex << (uint64_t)dut->core__DOT__wb_exception
         << " wb mcause:" << hex << (uint64_t)dut->core__DOT__csr__DOT__mcause
         << " wb mepc:" << hex << (uint64_t)dut->core__DOT__csr__DOT__mepc
         << " wb mtvec:" << hex << (uint64_t)dut->core__DOT__csr__DOT__mtvec
         << endl;
}

void dump_dmem()
{
    cout << "dmem instru: " << dec << (uint64_t)dut->core__DOT__dmem_instrId_out
         << " dmem pc: " << hex << (uint64_t)dut->core__DOT__dmem_pc_out
         << " dmem memr: " << dec << (uint64_t)dut->core__DOT__dmem_memr_out
         << " dmem memw: " << dec << (uint64_t)dut->core__DOT__dmem_memw_out
         << " dmem rs2val: " << hex << (uint64_t)dut->core__DOT__dmem_rs2val_out
         << " dmem result: " << hex << (uint64_t)dut->core__DOT__dmem_result_out
         << " dmem branch: " << dec << (uint64_t)dut->core__DOT__dmem_branch_out
         << endl;
}

void dump_alu()
{
    cout << "alu instru: " << dec << (uint64_t)dut->core__DOT__alu_instrId_out
         << " alu pc: " << hex << (uint64_t)dut->core__DOT__alu_pc_out
         << " alu rs1: " << hex << (uint64_t)dut->core__DOT__alu_rs1val_out
         << " alu rs2: " << hex << (uint64_t)dut->core__DOT__alu_rs2val_out
         << " alu imm: " << hex << (uint64_t)dut->core__DOT__alu_imm64_out
         << " alu result: " << hex << (uint64_t)dut->core__DOT__alu_result
         << " dmem imm64: " << hex << (uint64_t)dut->core__DOT__alu_imm64_out
         << " dmem benable: " << dec << (uint64_t)dut->core__DOT__pc_rel_branch
         << endl;
}

void dump_decode()
{

    cout << "dec instru: " << hex << (uint64_t)dut->core__DOT__decode_instr_out
         << " dec pc: " << hex << (uint64_t)dut->core__DOT__decode_pc_out
         << " dec id: " << dec << (uint64_t)dut->core__DOT__instr_id
         << endl;
}

void dump_fetch()
{
    if (dut->clk == 1)
    {
        cout << " fetch pc: " << hex << (uint64_t)dut->core__DOT__pc_out
             //  << " fetch instr: " << hex << (uint32_t)dut->core__DOT__ic_data
             << endl;
    }
}

uint64_t is_committing()
{
    if (dut->core__DOT__wb_instrId_out != i_invalid && dut->core__DOT__wb_instrId_out != i_bubble)
        return (uint64_t)dut->core__DOT__wb_pc_out;
    else
        return 0;
}
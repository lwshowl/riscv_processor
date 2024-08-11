#include "include/core_dump.h"
#include "include/instructions.h"
#include "include/utils.h"
#include <iostream>
#include <iomanip>
#include <bitset>
using namespace std;

void dump_axi_ctl()
{
    cout << " axi control state: " << (uint64_t)dut->core__DOT__ac0__DOT__state
         << ",axi rw: " << (uint64_t)dut->core__DOT__ac0__DOT__axi_rw_req
         << ",axi req1: " << (uint64_t)dut->core__DOT__dmem_axi_req
         << ",axi req2: " << (uint64_t)dut->core__DOT__if_axi_req
         << ",axi ctl addr: " << std::hex << (uint64_t)dut->core__DOT__ac0__DOT__axi_req_addr
         << ",axi port sel: " << (uint64_t)dut->core__DOT__ac0__DOT__port_sel
         << ",axi fifo cnt: " << (uint64_t)dut->core__DOT__ac0__DOT__cnt
         << ",axi fifo data: " << std::hex << setfill('0') << setw(16) << (uint64_t)dut->core__DOT__ac0__DOT__axi_rdata
         << ",axi fifo wen: " << (uint64_t)dut->core__DOT__ac0__DOT__axi_fifo_wen
         << endl;

    // check if an axi req has finished
    const int ac0_state_fifo = 2;

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
         << " axi wr state: " << (uint64_t)dut->core__DOT__ac0__DOT__a0__DOT__wr_state
         << " axi ar valid: " << (uint64_t)dut->axi_ar_valid
         << " axi ar ready: " << (uint64_t)dut->axi_ar_ready
         << " axi r valid: " << (uint64_t)dut->axi_r_valid
         << " axi r last: " << (uint64_t)dut->axi_r_last
         << " axi ar addr: " << hex << (uint64_t)dut->axi_ar_addr
         << " axi r data: " << (uint64_t)dut->axi_r_data
         << endl;
}

void dump_icache()
{
    cout << " icache core addr: " << std::hex << (uint64_t)dut->core__DOT__pc
         << " icache axi addr: " << std::hex << (uint64_t)dut->core__DOT__if_axi_addr
         << " icache state: " << std::hex << (uint64_t)dut->core__DOT__ic0__DOT__state
         << " presence bits: " << std::bitset<8>((uint64_t)dut->core__DOT__ic0__DOT__presence_w)
         << " cache data: " << hex << setfill('0') << setw(8) << (uint64_t)dut->core__DOT__ic0__DOT__way_ram_out[dut->core__DOT__ic0__DOT__hit_way]
         << " cache w_offset: " << std::hex << (uint64_t)dut->core__DOT__ic0__DOT__ram_w_addr
         //  << " cache axi data: " << std::hex << (uint64_t)dut->core__DOT__if_axi_data
         << " cache fifo cnt: " << std::hex << (uint64_t)dut->core__DOT__ic0__DOT__cnt
         << " cache axi done: " << std::hex << (uint64_t)dut->core__DOT__if_axi_done
         << " replacing way: " << (uint64_t)dut->core__DOT__ic0__DOT__cur_replace_way
         << " cache way wen: " << std::hex << (uint64_t)dut->core__DOT__ic0__DOT__way_wen[dut->core__DOT__ic0__DOT__cur_replace_way]
         << " cache way ramin: " << hex << setfill('0') << setw(16) << (uint64_t)dut->core__DOT__ic0__DOT__way_ram_in[dut->core__DOT__dc0__DOT__cur_replace_way]
         << " cache hit_way: " << std::hex << (uint64_t)(dut->core__DOT__ic0__DOT__m0__DOT__i0__DOT__lut_out)
         << endl;

    int cache_refill = 1;

    int index = dut->core__DOT__pc >> 6 & 0b111111;

    if (dut->core__DOT__ic0__DOT__cur_replace_way == 0)
        for (auto var : dut->core__DOT__ic0__DOT__genblk1__BRA__0__KET____DOT__ram__DOT__mem[index])
            cout << std::hex << std::setfill('0') << std::setw(2) << (uint64_t)var << " ";

    else if (dut->core__DOT__ic0__DOT__cur_replace_way == 1)
        for (auto var : dut->core__DOT__ic0__DOT__genblk1__BRA__1__KET____DOT__ram__DOT__mem[index])
            cout << std::hex << std::setfill('0') << std::setw(2) << (uint64_t)var << " ";

    else if (dut->core__DOT__ic0__DOT__cur_replace_way == 2)
        for (auto var : dut->core__DOT__ic0__DOT__genblk1__BRA__2__KET____DOT__ram__DOT__mem[index])
            cout << std::hex << std::setfill('0') << std::setw(2) << (uint64_t)var << " ";
    else if (dut->core__DOT__ic0__DOT__cur_replace_way == 3)
        for (auto var : dut->core__DOT__ic0__DOT__genblk1__BRA__3__KET____DOT__ram__DOT__mem[index])
            cout << std::hex << std::setfill('0') << std::setw(2) << (uint64_t)var << " ";
    else if (dut->core__DOT__ic0__DOT__cur_replace_way == 4)
        for (auto var : dut->core__DOT__ic0__DOT__genblk1__BRA__4__KET____DOT__ram__DOT__mem[index])
            cout << std::hex << std::setfill('0') << std::setw(2) << (uint64_t)var << " ";
    else if (dut->core__DOT__ic0__DOT__cur_replace_way == 5)
        for (auto var : dut->core__DOT__ic0__DOT__genblk1__BRA__5__KET____DOT__ram__DOT__mem[index])
            cout << std::hex << std::setfill('0') << std::setw(2) << (uint64_t)var << " ";
    else if (dut->core__DOT__ic0__DOT__cur_replace_way == 6)
        for (auto var : dut->core__DOT__ic0__DOT__genblk1__BRA__6__KET____DOT__ram__DOT__mem[index])
            cout << std::hex << std::setfill('0') << std::setw(2) << (uint64_t)var << " ";
    else if (dut->core__DOT__ic0__DOT__cur_replace_way == 7)
        for (auto var : dut->core__DOT__ic0__DOT__genblk1__BRA__7__KET____DOT__ram__DOT__mem[index])
            cout << std::hex << std::setfill('0') << std::setw(2) << (uint64_t)var << " ";

    cout << endl;
}

void dump_dcache()
{
    int index = (dut->core__DOT__dmem_pc >> 6) & 0b111111;

    cout << " dcache pc: " << std::hex << (uint64_t)dut->core__DOT__dmem_pc
         << " cache data: " << std::hex << (uint64_t)dut->core__DOT__dc0__DOT__way_ram_out[dut->core__DOT__dc0__DOT__hit_way]
         << " dcache req: " << (uint64_t)dut->core__DOT__dmem_cache_req
         << " dcache core addr: " << std::hex << (uint64_t)dut->core__DOT__dmem_result
         << " dcache core data: " << std::hex << (uint64_t)dut->core__DOT__dmem_rs2val
         << " dcache state: " << (uint64_t)dut->core__DOT__dc0__DOT__state
         << " dcache rw: " << std::hex << (uint64_t)dut->core__DOT__dmem_memw
         << " dcache cnt: " << std::hex << (uint64_t)dut->core__DOT__dc0__DOT__cnt
         << " dcache dirty: " << std::hex << (uint64_t)dut->core__DOT__dc0__DOT__line_dirty[dut->core__DOT__dc0__DOT__hit_way][index]
         << " dcache req addr: " << std::hex << (uint64_t)dut->core__DOT__dmem_axi_addr
         << " cache way wen: " << std::hex << (uint64_t)dut->core__DOT__dc0__DOT__way_wen[dut->core__DOT__dc0__DOT__cur_replace_way]
         << " cache ram wmask: " << (uint64_t)(dut->core__DOT__dc0__DOT__ram_write_mask)
         << " cache way ramin: " << hex << setfill('0') << setw(16) << (uint64_t)dut->core__DOT__dc0__DOT__way_ram_in[dut->core__DOT__dc0__DOT__cur_replace_way]
         << " cache w_addr: " << (uint64_t)(dut->core__DOT__dc0__DOT__ram_w_addr)
         << " presence bits: " << std::bitset<8>((uint64_t)dut->core__DOT__dc0__DOT__presence_w)
         << " cache hit_way: " << std::hex << (uint64_t)(dut->core__DOT__dc0__DOT__hit_way)
         << " current replacing: " << (uint64_t)(dut->core__DOT__dc0__DOT__cur_replace_way)
         << " cache ram data: " << std::hex << (uint64_t)dut->core__DOT__dc0__DOT__way_ram_in[index]
         << " cache fifo cnt: " << std::hex << (uint64_t)dut->core__DOT__dc0__DOT__cnt
         << " cache fifo idx: " << std::hex << (uint64_t)dut->core__DOT__axi_fifo_idx

         << endl;
}

void update_dnpc()
{
    if (dut->core__DOT__dmem_branch == 1 && dut->core__DOT__dmem_instrId != i_jalr && dut->core__DOT__dmem_result)
    {
        // cout << "dnpc pushed at " << dut->core__DOT__dmem_pc << " target " << dut->core__DOT__dmem_pc + dut->core__DOT__dmem_imm64 << endl;
        dnpc_queue.push(dut->core__DOT__dmem_pc + dut->core__DOT__dmem_imm64);
        // cout << endl
        //      << "update dnpc pc= << dut->core__DOT__dmem_pc << " imm= " << dut->core__DOT__dmem_imm64
        //      << endl
        //      << endl;
    }

    else if (dut->core__DOT__dmem_instrId == i_jalr)
    {
        dnpc_queue.push(dut->core__DOT__dmem_rs1val + dut->core__DOT__dmem_imm64);
        // cout << "update dnpc pc= " << dut->core__DOT__dmem_pc << " sum= " << dut->core__DOT__dmem_rs1val + dut->core__DOT__dmem_imm64;
    }

    else if (dut->core__DOT__wb_instrId == i_ecall)
    {
        dnpc_queue.push(dut->core__DOT__csr__DOT__mtvec);
    }

    else if (dut->core__DOT__wb_instrId == i_mret)
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

int sanity_check()
{
    if (dut->core__DOT__wb_instrId == 63)
        if (sim_time > 10)
        {
            uint64_t pc = dut->core__DOT__wb_pc;
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
    cout << " pc out: " << hex << (uint64_t)dut->core__DOT__pc
         << " pc abs_b: " << hex << (uint64_t)dut->core__DOT__pc_abs_branch
         << " pc rel_b: " << hex << (uint64_t)dut->core__DOT__pc_rel_branch
         << " pc buuble :" << (((uint64_t)dut->core__DOT__ic0__DOT__presence_w == 0) || dut->core__DOT__memory_bubble)
         << endl;
}

void dump_regfile()
{
    cout << "reg instru: " << dec << enum_to_ins((uint64_t)dut->core__DOT__instr_id)
         << " reg rs1: " << hex << (uint64_t)dut->core__DOT__rf_rs1
         << " reg rs2: " << hex << (uint64_t)dut->core__DOT__rf_rs2
         << " reg rd: " << hex << (uint64_t)dut->core__DOT__rf_rd
         << " reg regw: " << hex << (uint64_t)dut->core__DOT__reg_w
         << " reg alubypass1: " << dec << (uint64_t)dut->core__DOT__rs1_alu_bypass
         << " reg alubypass2: " << dec << (uint64_t)dut->core__DOT__rs2_alu_bypss
         << " reg membypass1: " << dec << (uint64_t)dut->core__DOT__rs1_dmem_bypass
         << " reg membypass2: " << dec << (uint64_t)dut->core__DOT__rs2_dmem_bypass
         << " reg memorybubble: " << dec << (uint64_t)dut->core__DOT__memory_bubble
         << endl;
}

void dump_wb()
{
    cout << "wb instru: " << dec << enum_to_ins((uint64_t)dut->core__DOT__wb_instrId)
         << " wb pc: " << hex << (uint64_t)dut->core__DOT__wb_pc
         << " wb rd: " << reg_name[(uint64_t)dut->core__DOT__wb_rd]
         << " wb memdata: " << hex << (uint64_t)dut->core__DOT__wb_memdata
         << " wb regw: " << hex << (uint64_t)dut->core__DOT__wb_regw
         << " wb branch: " << dec << (uint64_t)dut->core__DOT__wb_branch
         << " wb memw: " << dec << (uint64_t)dut->core__DOT__wb_memw
         << " wb opcode: " << dec << (uint64_t)dut->core__DOT__wb_opcode
         << " wb rs1val: " << hex << (uint64_t)dut->core__DOT__wb_rs1val
         << " wb rs2val: " << hex << (uint64_t)dut->core__DOT__wb_rs2val
         << " wb result: " << hex << (uint64_t)dut->core__DOT__wb_result
         << " wb mcause:" << hex << (uint64_t)dut->core__DOT__csr__DOT__mcause
         << " wb mepc:" << hex << (uint64_t)dut->core__DOT__csr__DOT__mepc
         << " wb mtvec:" << hex << (uint64_t)dut->core__DOT__csr__DOT__mtvec
         << endl;
}

void dump_dmem()
{
    cout << "dmem instru: " << dec << (uint64_t)dut->core__DOT__dmem_instrId
         << " dmem pc: " << hex << (uint64_t)dut->core__DOT__dmem_pc
         << " dmem memr: " << dec << (uint64_t)dut->core__DOT__dmem_memr
         << " dmem memw: " << dec << (uint64_t)dut->core__DOT__dmem_memw
         << " dmem regw: " << dec << (uint64_t)dut->core__DOT__dmem_regw
         << " dmem rd: " << hex << (uint64_t)dut->core__DOT__dmem_rd
         << " dmem rs2val: " << hex << (uint64_t)dut->core__DOT__dmem_rs2val
         << " dmem result: " << hex << (uint64_t)dut->core__DOT__dmem_result
         << " dmem branch: " << dec << (uint64_t)dut->core__DOT__dmem_branch
         << " dcache hold: " << dec << (uint64_t)dut->core__DOT__dcache_hold
         << endl;
}

void dump_alu()
{
    cout << "alu instru: " << dec << enum_to_ins((uint64_t)dut->core__DOT__alu_instrId)
         << " alu pc: " << hex << (uint64_t)dut->core__DOT__alu_pc
         << " alu alu_rs1_bypass: " << hex << (uint64_t)dut->core__DOT__rs1_alu_bypass
         << " alu alu_rs2_bypass: " << hex << (uint64_t)dut->core__DOT__rs2_alu_bypss
         << " alu dmem_rs1_bypass: " << hex << (uint64_t)dut->core__DOT__rs1_dmem_bypass
         << " alu dmem_rs2_bypass: " << hex << (uint64_t)dut->core__DOT__rs2_dmem_bypass
         << " alu rs1: " << hex << reg_name[(uint64_t)dut->core__DOT__alu_rs1]
         << " alu rs1val: " << hex << (uint64_t)dut->core__DOT__rs1val
         << " alu rs2: " << hex << reg_name[(uint64_t)dut->core__DOT__alu_rs2]
         << " alu rs2val: " << hex << (uint64_t)dut->core__DOT__rs2val
         << " alu imm: " << hex << (uint64_t)dut->core__DOT__alu_imm64
         << " alu result: " << hex << (uint64_t)dut->core__DOT__alu_result
         << " alu imm64: " << hex << (uint64_t)dut->core__DOT__alu_imm64
         << " alu benable: " << dec << (uint64_t)dut->core__DOT__pc_rel_branch
         << " alu regw: " << dec << (uint64_t)dut->core__DOT__alu_regw
         << " alu memw: " << dec << (uint64_t)dut->core__DOT__alu_memw
         << " dcache hold: " << dec << (uint64_t)dut->core__DOT__dcache_hold
         << " memory bubble: " << dec << (uint64_t)dut->core__DOT__memory_bubble
         << endl;
}

void dump_decode()
{

    cout << "dec instru: " << hex << (uint64_t)dut->core__DOT__decode_instr
         << " dec pc: " << hex << (uint64_t)dut->core__DOT__decode_pc
         << " dec id: " << enum_to_ins((uint64_t)dut->core__DOT__instr_id)
         << " dec bubble: " << dec << (uint64_t)dut->core__DOT__memory_bubble
         << endl;
}

void dump_fetch()
{
    if (dut->clk == 1)
    {
        cout << " fetch pc: " << hex << (uint64_t)dut->core__DOT__pc
             //  << " fetch instr: " << hex << (uint32_t)dut->core__DOT__ic_data
             << endl;
    }
}

uint64_t is_committing()
{
    if (dut->core__DOT__wb_instrId != i_invalid && dut->core__DOT__wb_instrId != i_bubble)
        return (uint64_t)dut->core__DOT__wb_pc;
    else
        return 0;
}
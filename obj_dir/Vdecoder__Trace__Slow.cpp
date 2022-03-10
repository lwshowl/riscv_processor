// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vdecoder__Syms.h"


//======================

void Vdecoder::trace(VerilatedVcdC* tfp, int, int) {
    tfp->spTrace()->addCallback(&Vdecoder::traceInit, &Vdecoder::traceFull, &Vdecoder::traceChg, this);
}
void Vdecoder::traceInit(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->open()
    Vdecoder* t = (Vdecoder*)userthis;
    Vdecoder__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (!Verilated::calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
                        "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vcdp->scopeEscape(' ');
    t->traceInitThis(vlSymsp, vcdp, code);
    vcdp->scopeEscape('.');
}
void Vdecoder::traceFull(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    Vdecoder* t = (Vdecoder*)userthis;
    Vdecoder__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    t->traceFullThis(vlSymsp, vcdp, code);
}

//======================


void Vdecoder::traceInitThis(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    vcdp->module(vlSymsp->name());  // Setup signal names
    // Body
    {
        vlTOPp->traceInitThis__1(vlSymsp, vcdp, code);
    }
}

void Vdecoder::traceFullThis(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vlTOPp->traceFullThis__1(vlSymsp, vcdp, code);
    }
    // Final
    vlTOPp->__Vm_traceActivity = 0U;
}

void Vdecoder::traceInitThis__1(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->declBit(c+121,"clk", false,-1);
        vcdp->declBus(c+129,"instr", false,-1, 31,0);
        vcdp->declBus(c+137,"rs1", false,-1, 4,0);
        vcdp->declBus(c+145,"rs2", false,-1, 4,0);
        vcdp->declBus(c+153,"rd", false,-1, 4,0);
        vcdp->declBus(c+161,"func3", false,-1, 2,0);
        vcdp->declBus(c+169,"func7", false,-1, 6,0);
        vcdp->declBus(c+177,"imm", false,-1, 24,0);
        vcdp->declBus(c+185,"instr_id", false,-1, 5,0);
        vcdp->declBit(c+121,"decoder clk", false,-1);
        vcdp->declBus(c+129,"decoder instr", false,-1, 31,0);
        vcdp->declBus(c+137,"decoder rs1", false,-1, 4,0);
        vcdp->declBus(c+145,"decoder rs2", false,-1, 4,0);
        vcdp->declBus(c+153,"decoder rd", false,-1, 4,0);
        vcdp->declBus(c+161,"decoder func3", false,-1, 2,0);
        vcdp->declBus(c+169,"decoder func7", false,-1, 6,0);
        vcdp->declBus(c+177,"decoder imm", false,-1, 24,0);
        vcdp->declBus(c+185,"decoder instr_id", false,-1, 5,0);
        vcdp->declBus(c+193,"decoder i_lui", false,-1, 31,0);
        vcdp->declBus(c+201,"decoder i_auipc", false,-1, 31,0);
        vcdp->declBus(c+209,"decoder i_jal", false,-1, 31,0);
        vcdp->declBus(c+217,"decoder i_jalr", false,-1, 31,0);
        vcdp->declBus(c+225,"decoder i_beq", false,-1, 31,0);
        vcdp->declBus(c+233,"decoder i_bne", false,-1, 31,0);
        vcdp->declBus(c+241,"decoder i_blt", false,-1, 31,0);
        vcdp->declBus(c+249,"decoder i_bge", false,-1, 31,0);
        vcdp->declBus(c+257,"decoder i_bltu", false,-1, 31,0);
        vcdp->declBus(c+265,"decoder i_bgeu", false,-1, 31,0);
        vcdp->declBus(c+273,"decoder i_lb", false,-1, 31,0);
        vcdp->declBus(c+281,"decoder i_lh", false,-1, 31,0);
        vcdp->declBus(c+289,"decoder i_lw", false,-1, 31,0);
        vcdp->declBus(c+297,"decoder i_lbu", false,-1, 31,0);
        vcdp->declBus(c+305,"decoder i_lhu", false,-1, 31,0);
        vcdp->declBus(c+313,"decoder i_sb", false,-1, 31,0);
        vcdp->declBus(c+321,"decoder i_sh", false,-1, 31,0);
        vcdp->declBus(c+329,"decoder i_sw", false,-1, 31,0);
        vcdp->declBus(c+337,"decoder i_addi", false,-1, 31,0);
        vcdp->declBus(c+345,"decoder i_slti", false,-1, 31,0);
        vcdp->declBus(c+353,"decoder i_sltiu", false,-1, 31,0);
        vcdp->declBus(c+361,"decoder i_xori", false,-1, 31,0);
        vcdp->declBus(c+369,"decoder i_ori", false,-1, 31,0);
        vcdp->declBus(c+377,"decoder i_andi", false,-1, 31,0);
        vcdp->declBus(c+385,"decoder i_slli", false,-1, 31,0);
        vcdp->declBus(c+393,"decoder i_srli", false,-1, 31,0);
        vcdp->declBus(c+401,"decoder i_srai", false,-1, 31,0);
        vcdp->declBus(c+409,"decoder i_add", false,-1, 31,0);
        vcdp->declBus(c+417,"decoder i_sub", false,-1, 31,0);
        vcdp->declBus(c+425,"decoder i_sll", false,-1, 31,0);
        vcdp->declBus(c+433,"decoder i_slt", false,-1, 31,0);
        vcdp->declBus(c+441,"decoder i_sltu", false,-1, 31,0);
        vcdp->declBus(c+449,"decoder i_xor", false,-1, 31,0);
        vcdp->declBus(c+457,"decoder i_srl", false,-1, 31,0);
        vcdp->declBus(c+465,"decoder i_sra", false,-1, 31,0);
        vcdp->declBus(c+473,"decoder i_or", false,-1, 31,0);
        vcdp->declBus(c+481,"decoder i_and", false,-1, 31,0);
        vcdp->declBus(c+489,"decoder i_ecall", false,-1, 31,0);
        vcdp->declBus(c+497,"decoder i_ebreak", false,-1, 31,0);
        vcdp->declBus(c+505,"decoder i_csrrw", false,-1, 31,0);
        vcdp->declBus(c+513,"decoder i_csrrs", false,-1, 31,0);
        vcdp->declBus(c+521,"decoder i_csrrc", false,-1, 31,0);
        vcdp->declBus(c+529,"decoder i_csrrwi", false,-1, 31,0);
        vcdp->declBus(c+537,"decoder i_csrrsi", false,-1, 31,0);
        vcdp->declBus(c+545,"decoder i_csrrci", false,-1, 31,0);
        {int i; for (i=0; i<4; i++) {
                vcdp->declBus(c+1+i*1,"decoder i_opcode", true,(i+0), 6,0);}}
        {int i; for (i=0; i<2; i++) {
                vcdp->declBus(c+33+i*1,"decoder u_opcode", true,(i+0), 6,0);}}
        vcdp->declBus(c+553,"decoder j_opcode", false,-1, 6,0);
        vcdp->declBus(c+561,"decoder b_opcode", false,-1, 6,0);
        vcdp->declBus(c+569,"decoder s_opcode", false,-1, 6,0);
        vcdp->declBus(c+577,"decoder r_opcode", false,-1, 6,0);
        vcdp->declBus(c+585,"decoder ebreak", false,-1, 31,0);
        vcdp->declBus(c+593,"decoder ecall", false,-1, 31,0);
        vcdp->declBus(c+49,"decoder instr_r", false,-1, 31,0);
        vcdp->declBus(c+57,"decoder opcode_r", false,-1, 6,0);
        vcdp->declBus(c+65,"decoder func3_r", false,-1, 2,0);
        vcdp->declBus(c+73,"decoder func7_r", false,-1, 6,0);
        vcdp->declBus(c+81,"decoder imm_r", false,-1, 24,0);
        vcdp->declBus(c+89,"decoder instr_id_r", false,-1, 5,0);
        vcdp->declBus(c+97,"decoder rs1_r", false,-1, 4,0);
        vcdp->declBus(c+105,"decoder rs2_r", false,-1, 4,0);
        vcdp->declBus(c+113,"decoder rd_r", false,-1, 4,0);
    }
}

void Vdecoder::traceFullThis__1(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->fullBus(c+1,(vlTOPp->decoder__DOT__i_opcode[0]),7);
        vcdp->fullBus(c+2,(vlTOPp->decoder__DOT__i_opcode[1]),7);
        vcdp->fullBus(c+3,(vlTOPp->decoder__DOT__i_opcode[2]),7);
        vcdp->fullBus(c+4,(vlTOPp->decoder__DOT__i_opcode[3]),7);
        vcdp->fullBus(c+33,(vlTOPp->decoder__DOT__u_opcode[0]),7);
        vcdp->fullBus(c+34,(vlTOPp->decoder__DOT__u_opcode[1]),7);
        vcdp->fullBus(c+49,(vlTOPp->decoder__DOT__instr_r),32);
        vcdp->fullBus(c+57,(vlTOPp->decoder__DOT__opcode_r),7);
        vcdp->fullBus(c+65,(vlTOPp->decoder__DOT__func3_r),3);
        vcdp->fullBus(c+73,(vlTOPp->decoder__DOT__func7_r),7);
        vcdp->fullBus(c+81,(vlTOPp->decoder__DOT__imm_r),25);
        vcdp->fullBus(c+89,(vlTOPp->decoder__DOT__instr_id_r),6);
        vcdp->fullBus(c+97,(vlTOPp->decoder__DOT__rs1_r),5);
        vcdp->fullBus(c+105,(vlTOPp->decoder__DOT__rs2_r),5);
        vcdp->fullBus(c+113,(vlTOPp->decoder__DOT__rd_r),5);
        vcdp->fullBit(c+121,(vlTOPp->clk));
        vcdp->fullBus(c+129,(vlTOPp->instr),32);
        vcdp->fullBus(c+137,(vlTOPp->rs1),5);
        vcdp->fullBus(c+145,(vlTOPp->rs2),5);
        vcdp->fullBus(c+153,(vlTOPp->rd),5);
        vcdp->fullBus(c+161,(vlTOPp->func3),3);
        vcdp->fullBus(c+169,(vlTOPp->func7),7);
        vcdp->fullBus(c+177,(vlTOPp->imm),25);
        vcdp->fullBus(c+185,(vlTOPp->instr_id),6);
        vcdp->fullBus(c+193,(0U),32);
        vcdp->fullBus(c+201,(1U),32);
        vcdp->fullBus(c+209,(2U),32);
        vcdp->fullBus(c+217,(3U),32);
        vcdp->fullBus(c+225,(4U),32);
        vcdp->fullBus(c+233,(5U),32);
        vcdp->fullBus(c+241,(6U),32);
        vcdp->fullBus(c+249,(7U),32);
        vcdp->fullBus(c+257,(8U),32);
        vcdp->fullBus(c+265,(9U),32);
        vcdp->fullBus(c+273,(0xaU),32);
        vcdp->fullBus(c+281,(0xbU),32);
        vcdp->fullBus(c+289,(0xcU),32);
        vcdp->fullBus(c+297,(0xdU),32);
        vcdp->fullBus(c+305,(0xeU),32);
        vcdp->fullBus(c+313,(0xfU),32);
        vcdp->fullBus(c+321,(0x10U),32);
        vcdp->fullBus(c+329,(0x11U),32);
        vcdp->fullBus(c+337,(0x12U),32);
        vcdp->fullBus(c+345,(0x13U),32);
        vcdp->fullBus(c+353,(0x14U),32);
        vcdp->fullBus(c+361,(0x15U),32);
        vcdp->fullBus(c+369,(0x16U),32);
        vcdp->fullBus(c+377,(0x17U),32);
        vcdp->fullBus(c+385,(0x18U),32);
        vcdp->fullBus(c+393,(0x19U),32);
        vcdp->fullBus(c+401,(0x1aU),32);
        vcdp->fullBus(c+409,(0x1bU),32);
        vcdp->fullBus(c+417,(0x1cU),32);
        vcdp->fullBus(c+425,(0x1dU),32);
        vcdp->fullBus(c+433,(0x1eU),32);
        vcdp->fullBus(c+441,(0x1fU),32);
        vcdp->fullBus(c+449,(0x20U),32);
        vcdp->fullBus(c+457,(0x21U),32);
        vcdp->fullBus(c+465,(0x22U),32);
        vcdp->fullBus(c+473,(0x23U),32);
        vcdp->fullBus(c+481,(0x24U),32);
        vcdp->fullBus(c+489,(0x25U),32);
        vcdp->fullBus(c+497,(0x26U),32);
        vcdp->fullBus(c+505,(0x27U),32);
        vcdp->fullBus(c+513,(0x28U),32);
        vcdp->fullBus(c+521,(0x29U),32);
        vcdp->fullBus(c+529,(0x2aU),32);
        vcdp->fullBus(c+537,(0x2bU),32);
        vcdp->fullBus(c+545,(0x2cU),32);
        vcdp->fullBus(c+553,(0x6fU),7);
        vcdp->fullBus(c+561,(0x63U),7);
        vcdp->fullBus(c+569,(0x23U),7);
        vcdp->fullBus(c+577,(0x33U),7);
        vcdp->fullBus(c+585,(0x73U),32);
        vcdp->fullBus(c+593,(0x1000073U),32);
    }
}

// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vdecoder__Syms.h"


//======================

void Vdecoder::traceChg(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    Vdecoder* t = (Vdecoder*)userthis;
    Vdecoder__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (vlSymsp->getClearActivity()) {
        t->traceChgThis(vlSymsp, vcdp, code);
    }
}

//======================


void Vdecoder::traceChgThis(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        if (VL_UNLIKELY((1U & vlTOPp->__Vm_traceActivity))) {
            vlTOPp->traceChgThis__2(vlSymsp, vcdp, code);
        }
        if (VL_UNLIKELY((2U & vlTOPp->__Vm_traceActivity))) {
            vlTOPp->traceChgThis__3(vlSymsp, vcdp, code);
        }
        vlTOPp->traceChgThis__4(vlSymsp, vcdp, code);
    }
    // Final
    vlTOPp->__Vm_traceActivity = 0U;
}

void Vdecoder::traceChgThis__2(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBus(c+1,(vlTOPp->decoder__DOT__i_opcode[0]),7);
        vcdp->chgBus(c+2,(vlTOPp->decoder__DOT__i_opcode[1]),7);
        vcdp->chgBus(c+3,(vlTOPp->decoder__DOT__i_opcode[2]),7);
        vcdp->chgBus(c+4,(vlTOPp->decoder__DOT__i_opcode[3]),7);
        vcdp->chgBus(c+33,(vlTOPp->decoder__DOT__u_opcode[0]),7);
        vcdp->chgBus(c+34,(vlTOPp->decoder__DOT__u_opcode[1]),7);
    }
}

void Vdecoder::traceChgThis__3(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBus(c+49,(vlTOPp->decoder__DOT__instr_r),32);
        vcdp->chgBus(c+57,(vlTOPp->decoder__DOT__opcode_r),7);
        vcdp->chgBus(c+65,(vlTOPp->decoder__DOT__func3_r),3);
        vcdp->chgBus(c+73,(vlTOPp->decoder__DOT__func7_r),7);
        vcdp->chgBus(c+81,(vlTOPp->decoder__DOT__imm_r),25);
        vcdp->chgBus(c+89,(vlTOPp->decoder__DOT__instr_id_r),6);
        vcdp->chgBus(c+97,(vlTOPp->decoder__DOT__rs1_r),5);
        vcdp->chgBus(c+105,(vlTOPp->decoder__DOT__rs2_r),5);
        vcdp->chgBus(c+113,(vlTOPp->decoder__DOT__rd_r),5);
    }
}

void Vdecoder::traceChgThis__4(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBit(c+121,(vlTOPp->clk));
        vcdp->chgBus(c+129,(vlTOPp->instr),32);
        vcdp->chgBus(c+137,(vlTOPp->rs1),5);
        vcdp->chgBus(c+145,(vlTOPp->rs2),5);
        vcdp->chgBus(c+153,(vlTOPp->rd),5);
        vcdp->chgBus(c+161,(vlTOPp->func3),3);
        vcdp->chgBus(c+169,(vlTOPp->func7),7);
        vcdp->chgBus(c+177,(vlTOPp->imm),25);
        vcdp->chgBus(c+185,(vlTOPp->instr_id),6);
    }
}

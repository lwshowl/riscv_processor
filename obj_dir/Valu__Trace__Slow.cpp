// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Valu__Syms.h"


//======================

void Valu::trace(VerilatedVcdC* tfp, int, int) {
    tfp->spTrace()->addCallback(&Valu::traceInit, &Valu::traceFull, &Valu::traceChg, this);
}
void Valu::traceInit(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->open()
    Valu* t = (Valu*)userthis;
    Valu__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (!Verilated::calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
                        "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vcdp->scopeEscape(' ');
    t->traceInitThis(vlSymsp, vcdp, code);
    vcdp->scopeEscape('.');
}
void Valu::traceFull(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    Valu* t = (Valu*)userthis;
    Valu__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    t->traceFullThis(vlSymsp, vcdp, code);
}

//======================


void Valu::traceInitThis(Valu__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Valu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    vcdp->module(vlSymsp->name());  // Setup signal names
    // Body
    {
        vlTOPp->traceInitThis__1(vlSymsp, vcdp, code);
    }
}

void Valu::traceFullThis(Valu__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Valu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vlTOPp->traceFullThis__1(vlSymsp, vcdp, code);
    }
    // Final
    vlTOPp->__Vm_traceActivity = 0U;
}

void Valu::traceInitThis__1(Valu__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Valu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->declBit(c+49,"clk", false,-1);
        vcdp->declBit(c+57,"rst", false,-1);
        vcdp->declBus(c+65,"op_in", false,-1, 1,0);
        vcdp->declBus(c+73,"a_in", false,-1, 31,0);
        vcdp->declBus(c+81,"b_in", false,-1, 31,0);
        vcdp->declBit(c+89,"in_valid", false,-1);
        vcdp->declBus(c+97,"out", false,-1, 31,0);
        vcdp->declBit(c+105,"out_valid", false,-1);
        vcdp->declBus(c+113,"alu WIDTH", false,-1, 31,0);
        vcdp->declBit(c+49,"alu clk", false,-1);
        vcdp->declBit(c+57,"alu rst", false,-1);
        vcdp->declBus(c+65,"alu op_in", false,-1, 1,0);
        vcdp->declBus(c+73,"alu a_in", false,-1, 31,0);
        vcdp->declBus(c+81,"alu b_in", false,-1, 31,0);
        vcdp->declBit(c+89,"alu in_valid", false,-1);
        vcdp->declBus(c+97,"alu out", false,-1, 31,0);
        vcdp->declBit(c+105,"alu out_valid", false,-1);
        vcdp->declBus(c+1,"alu a_in_r", false,-1, 31,0);
        vcdp->declBus(c+9,"alu b_in_r", false,-1, 31,0);
        vcdp->declBit(c+17,"alu in_valid_r", false,-1);
        vcdp->declBus(c+25,"alu result", false,-1, 31,0);
        vcdp->declBus(c+33,"alu op_in_r", false,-1, 1,0);
        vcdp->declBit(c+41,"alu done", false,-1);
        vcdp->declBus(c+121,"alu add", false,-1, 31,0);
        vcdp->declBus(c+129,"alu sub", false,-1, 31,0);
        vcdp->declBus(c+137,"alu nop", false,-1, 31,0);
    }
}

void Valu::traceFullThis__1(Valu__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Valu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->fullBus(c+1,(vlTOPp->alu__DOT__a_in_r),32);
        vcdp->fullBus(c+9,(vlTOPp->alu__DOT__b_in_r),32);
        vcdp->fullBit(c+17,(vlTOPp->alu__DOT__in_valid_r));
        vcdp->fullBus(c+25,(vlTOPp->alu__DOT__result),32);
        vcdp->fullBus(c+33,(vlTOPp->alu__DOT__op_in_r),2);
        vcdp->fullBit(c+41,(vlTOPp->alu__DOT__done));
        vcdp->fullBit(c+49,(vlTOPp->clk));
        vcdp->fullBit(c+57,(vlTOPp->rst));
        vcdp->fullBus(c+65,(vlTOPp->op_in),2);
        vcdp->fullBus(c+73,(vlTOPp->a_in),32);
        vcdp->fullBus(c+81,(vlTOPp->b_in),32);
        vcdp->fullBit(c+89,(vlTOPp->in_valid));
        vcdp->fullBus(c+97,(vlTOPp->out),32);
        vcdp->fullBit(c+105,(vlTOPp->out_valid));
        vcdp->fullBus(c+113,(0x20U),32);
        vcdp->fullBus(c+121,(1U),32);
        vcdp->fullBus(c+129,(2U),32);
        vcdp->fullBus(c+137,(0U),32);
    }
}

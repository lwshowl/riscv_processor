// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vpc__Syms.h"


//======================

void Vpc::trace(VerilatedVcdC* tfp, int, int) {
    tfp->spTrace()->addCallback(&Vpc::traceInit, &Vpc::traceFull, &Vpc::traceChg, this);
}
void Vpc::traceInit(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->open()
    Vpc* t = (Vpc*)userthis;
    Vpc__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (!Verilated::calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
                        "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vcdp->scopeEscape(' ');
    t->traceInitThis(vlSymsp, vcdp, code);
    vcdp->scopeEscape('.');
}
void Vpc::traceFull(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    Vpc* t = (Vpc*)userthis;
    Vpc__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    t->traceFullThis(vlSymsp, vcdp, code);
}

//======================


void Vpc::traceInitThis(Vpc__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    vcdp->module(vlSymsp->name());  // Setup signal names
    // Body
    {
        vlTOPp->traceInitThis__1(vlSymsp, vcdp, code);
    }
}

void Vpc::traceFullThis(Vpc__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vlTOPp->traceFullThis__1(vlSymsp, vcdp, code);
    }
    // Final
    vlTOPp->__Vm_traceActivity = 0U;
}

void Vpc::traceInitThis__1(Vpc__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->declBit(c+1,"clk", false,-1);
        vcdp->declBit(c+9,"rst", false,-1);
        vcdp->declBit(c+17,"branch", false,-1);
        vcdp->declBus(c+25,"immediate", false,-1, 31,0);
        vcdp->declBus(c+33,"pc_out_reg", false,-1, 31,0);
        vcdp->declBit(c+41,"out_valid", false,-1);
        vcdp->declBus(c+49,"pc WIDTH", false,-1, 31,0);
        vcdp->declBit(c+1,"pc clk", false,-1);
        vcdp->declBit(c+9,"pc rst", false,-1);
        vcdp->declBit(c+17,"pc branch", false,-1);
        vcdp->declBus(c+25,"pc immediate", false,-1, 31,0);
        vcdp->declBus(c+33,"pc pc_out_reg", false,-1, 31,0);
        vcdp->declBit(c+41,"pc out_valid", false,-1);
    }
}

void Vpc::traceFullThis__1(Vpc__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->fullBit(c+1,(vlTOPp->clk));
        vcdp->fullBit(c+9,(vlTOPp->rst));
        vcdp->fullBit(c+17,(vlTOPp->branch));
        vcdp->fullBus(c+25,(vlTOPp->immediate),32);
        vcdp->fullBus(c+33,(vlTOPp->pc_out_reg),32);
        vcdp->fullBit(c+41,(vlTOPp->out_valid));
        vcdp->fullBus(c+49,(0x20U),32);
    }
}

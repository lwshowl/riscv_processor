// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vpc__Syms.h"


//======================

void Vpc::traceChg(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    Vpc* t = (Vpc*)userthis;
    Vpc__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (vlSymsp->getClearActivity()) {
        t->traceChgThis(vlSymsp, vcdp, code);
    }
}

//======================


void Vpc::traceChgThis(Vpc__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vlTOPp->traceChgThis__2(vlSymsp, vcdp, code);
    }
    // Final
    vlTOPp->__Vm_traceActivity = 0U;
}

void Vpc::traceChgThis__2(Vpc__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBit(c+1,(vlTOPp->clk));
        vcdp->chgBit(c+9,(vlTOPp->rst));
        vcdp->chgBit(c+17,(vlTOPp->branch));
        vcdp->chgBus(c+25,(vlTOPp->immediate),32);
        vcdp->chgBus(c+33,(vlTOPp->pc_out_reg),32);
        vcdp->chgBit(c+41,(vlTOPp->out_valid));
    }
}

// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vmemory__Syms.h"


//======================

void Vmemory::traceChg(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    Vmemory* t = (Vmemory*)userthis;
    Vmemory__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (vlSymsp->getClearActivity()) {
        t->traceChgThis(vlSymsp, vcdp, code);
    }
}

//======================


void Vmemory::traceChgThis(Vmemory__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        if (VL_UNLIKELY((2U & vlTOPp->__Vm_traceActivity))) {
            vlTOPp->traceChgThis__2(vlSymsp, vcdp, code);
        }
        vlTOPp->traceChgThis__3(vlSymsp, vcdp, code);
    }
    // Final
    vlTOPp->__Vm_traceActivity = 0U;
}

void Vmemory::traceChgThis__2(Vmemory__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBit(c+1,(vlTOPp->memory__DOT__w_enable_r));
        vcdp->chgBit(c+9,(vlTOPp->memory__DOT__r_enable_r));
        vcdp->chgBus(c+17,(vlTOPp->memory__DOT__addr_select_r),32);
        vcdp->chgBus(c+25,(vlTOPp->memory__DOT__data_in_r),32);
        vcdp->chgBus(c+33,(vlTOPp->memory__DOT__data_out_r),32);
    }
}

void Vmemory::traceChgThis__3(Vmemory__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBit(c+41,(vlTOPp->clk));
        vcdp->chgBit(c+49,(vlTOPp->w_enable));
        vcdp->chgBit(c+57,(vlTOPp->r_enable));
        vcdp->chgBus(c+65,(vlTOPp->addr_select),32);
        vcdp->chgBus(c+73,(vlTOPp->data_in),32);
        vcdp->chgBus(c+81,(vlTOPp->data_out),32);
    }
}

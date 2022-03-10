// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vmemory__Syms.h"


//======================

void Vmemory::trace(VerilatedVcdC* tfp, int, int) {
    tfp->spTrace()->addCallback(&Vmemory::traceInit, &Vmemory::traceFull, &Vmemory::traceChg, this);
}
void Vmemory::traceInit(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->open()
    Vmemory* t = (Vmemory*)userthis;
    Vmemory__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (!Verilated::calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
                        "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vcdp->scopeEscape(' ');
    t->traceInitThis(vlSymsp, vcdp, code);
    vcdp->scopeEscape('.');
}
void Vmemory::traceFull(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    Vmemory* t = (Vmemory*)userthis;
    Vmemory__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    t->traceFullThis(vlSymsp, vcdp, code);
}

//======================


void Vmemory::traceInitThis(Vmemory__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    vcdp->module(vlSymsp->name());  // Setup signal names
    // Body
    {
        vlTOPp->traceInitThis__1(vlSymsp, vcdp, code);
    }
}

void Vmemory::traceFullThis(Vmemory__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vlTOPp->traceFullThis__1(vlSymsp, vcdp, code);
    }
    // Final
    vlTOPp->__Vm_traceActivity = 0U;
}

void Vmemory::traceInitThis__1(Vmemory__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->declBit(c+41,"clk", false,-1);
        vcdp->declBit(c+49,"w_enable", false,-1);
        vcdp->declBit(c+57,"r_enable", false,-1);
        vcdp->declBus(c+65,"addr_select", false,-1, 31,0);
        vcdp->declBus(c+73,"data_in", false,-1, 31,0);
        vcdp->declBus(c+81,"data_out", false,-1, 31,0);
        vcdp->declBus(c+89,"memory SIZE", false,-1, 31,0);
        vcdp->declBus(c+97,"memory WIDTH", false,-1, 31,0);
        vcdp->declBit(c+41,"memory clk", false,-1);
        vcdp->declBit(c+49,"memory w_enable", false,-1);
        vcdp->declBit(c+57,"memory r_enable", false,-1);
        vcdp->declBus(c+65,"memory addr_select", false,-1, 31,0);
        vcdp->declBus(c+73,"memory data_in", false,-1, 31,0);
        vcdp->declBus(c+81,"memory data_out", false,-1, 31,0);
        vcdp->declBit(c+1,"memory w_enable_r", false,-1);
        vcdp->declBit(c+9,"memory r_enable_r", false,-1);
        vcdp->declBus(c+17,"memory addr_select_r", false,-1, 31,0);
        vcdp->declBus(c+25,"memory data_in_r", false,-1, 31,0);
        vcdp->declBus(c+33,"memory data_out_r", false,-1, 31,0);
    }
}

void Vmemory::traceFullThis__1(Vmemory__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->fullBit(c+1,(vlTOPp->memory__DOT__w_enable_r));
        vcdp->fullBit(c+9,(vlTOPp->memory__DOT__r_enable_r));
        vcdp->fullBus(c+17,(vlTOPp->memory__DOT__addr_select_r),32);
        vcdp->fullBus(c+25,(vlTOPp->memory__DOT__data_in_r),32);
        vcdp->fullBus(c+33,(vlTOPp->memory__DOT__data_out_r),32);
        vcdp->fullBit(c+41,(vlTOPp->clk));
        vcdp->fullBit(c+49,(vlTOPp->w_enable));
        vcdp->fullBit(c+57,(vlTOPp->r_enable));
        vcdp->fullBus(c+65,(vlTOPp->addr_select),32);
        vcdp->fullBus(c+73,(vlTOPp->data_in),32);
        vcdp->fullBus(c+81,(vlTOPp->data_out),32);
        vcdp->fullBus(c+89,(0x80U),32);
        vcdp->fullBus(c+97,(0x20U),32);
    }
}

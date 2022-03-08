// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vregfile__Syms.h"


//======================

void Vregfile::trace(VerilatedVcdC* tfp, int, int) {
    tfp->spTrace()->addCallback(&Vregfile::traceInit, &Vregfile::traceFull, &Vregfile::traceChg, this);
}
void Vregfile::traceInit(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->open()
    Vregfile* t = (Vregfile*)userthis;
    Vregfile__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (!Verilated::calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
                        "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vcdp->scopeEscape(' ');
    t->traceInitThis(vlSymsp, vcdp, code);
    vcdp->scopeEscape('.');
}
void Vregfile::traceFull(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    Vregfile* t = (Vregfile*)userthis;
    Vregfile__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    t->traceFullThis(vlSymsp, vcdp, code);
}

//======================


void Vregfile::traceInitThis(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    vcdp->module(vlSymsp->name());  // Setup signal names
    // Body
    {
        vlTOPp->traceInitThis__1(vlSymsp, vcdp, code);
    }
}

void Vregfile::traceFullThis(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vlTOPp->traceFullThis__1(vlSymsp, vcdp, code);
    }
    // Final
    vlTOPp->__Vm_traceActivity = 0U;
}

void Vregfile::traceInitThis__1(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->declBit(c+313,"clk", false,-1);
        vcdp->declBit(c+321,"r_enable", false,-1);
        vcdp->declBit(c+329,"w_enable", false,-1);
        vcdp->declBus(c+337,"r_select", false,-1, 4,0);
        vcdp->declBus(c+345,"w_select", false,-1, 4,0);
        vcdp->declBus(c+353,"w_val", false,-1, 31,0);
        vcdp->declBus(c+361,"r_out", false,-1, 31,0);
        vcdp->declBit(c+369,"valid", false,-1);
        vcdp->declBus(c+377,"regfile WIDTH", false,-1, 31,0);
        vcdp->declBit(c+313,"regfile clk", false,-1);
        vcdp->declBit(c+321,"regfile r_enable", false,-1);
        vcdp->declBit(c+329,"regfile w_enable", false,-1);
        vcdp->declBus(c+337,"regfile r_select", false,-1, 4,0);
        vcdp->declBus(c+345,"regfile w_select", false,-1, 4,0);
        vcdp->declBus(c+353,"regfile w_val", false,-1, 31,0);
        vcdp->declBus(c+361,"regfile r_out", false,-1, 31,0);
        vcdp->declBit(c+369,"regfile valid", false,-1);
        vcdp->declBus(c+1,"regfile r_select_reg", false,-1, 4,0);
        vcdp->declBus(c+9,"regfile w_select_reg", false,-1, 4,0);
        vcdp->declBus(c+17,"regfile w_val_reg", false,-1, 31,0);
        vcdp->declBus(c+25,"regfile r_out_reg", false,-1, 31,0);
        vcdp->declBit(c+33,"regfile valid_reg", false,-1);
        vcdp->declBit(c+41,"regfile r_enable_reg", false,-1);
        vcdp->declBit(c+49,"regfile w_enable_reg", false,-1);
        {int i; for (i=0; i<32; i++) {
                vcdp->declBus(c+57+i*1,"regfile r_file", true,(i+0), 31,0);}}
    }
}

void Vregfile::traceFullThis__1(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->fullBus(c+1,(vlTOPp->regfile__DOT__r_select_reg),5);
        vcdp->fullBus(c+9,(vlTOPp->regfile__DOT__w_select_reg),5);
        vcdp->fullBus(c+17,(vlTOPp->regfile__DOT__w_val_reg),32);
        vcdp->fullBus(c+25,(vlTOPp->regfile__DOT__r_out_reg),32);
        vcdp->fullBit(c+33,(vlTOPp->regfile__DOT__valid_reg));
        vcdp->fullBit(c+41,(vlTOPp->regfile__DOT__r_enable_reg));
        vcdp->fullBit(c+49,(vlTOPp->regfile__DOT__w_enable_reg));
        vcdp->fullBus(c+57,(vlTOPp->regfile__DOT__r_file[0]),32);
        vcdp->fullBus(c+58,(vlTOPp->regfile__DOT__r_file[1]),32);
        vcdp->fullBus(c+59,(vlTOPp->regfile__DOT__r_file[2]),32);
        vcdp->fullBus(c+60,(vlTOPp->regfile__DOT__r_file[3]),32);
        vcdp->fullBus(c+61,(vlTOPp->regfile__DOT__r_file[4]),32);
        vcdp->fullBus(c+62,(vlTOPp->regfile__DOT__r_file[5]),32);
        vcdp->fullBus(c+63,(vlTOPp->regfile__DOT__r_file[6]),32);
        vcdp->fullBus(c+64,(vlTOPp->regfile__DOT__r_file[7]),32);
        vcdp->fullBus(c+65,(vlTOPp->regfile__DOT__r_file[8]),32);
        vcdp->fullBus(c+66,(vlTOPp->regfile__DOT__r_file[9]),32);
        vcdp->fullBus(c+67,(vlTOPp->regfile__DOT__r_file[10]),32);
        vcdp->fullBus(c+68,(vlTOPp->regfile__DOT__r_file[11]),32);
        vcdp->fullBus(c+69,(vlTOPp->regfile__DOT__r_file[12]),32);
        vcdp->fullBus(c+70,(vlTOPp->regfile__DOT__r_file[13]),32);
        vcdp->fullBus(c+71,(vlTOPp->regfile__DOT__r_file[14]),32);
        vcdp->fullBus(c+72,(vlTOPp->regfile__DOT__r_file[15]),32);
        vcdp->fullBus(c+73,(vlTOPp->regfile__DOT__r_file[16]),32);
        vcdp->fullBus(c+74,(vlTOPp->regfile__DOT__r_file[17]),32);
        vcdp->fullBus(c+75,(vlTOPp->regfile__DOT__r_file[18]),32);
        vcdp->fullBus(c+76,(vlTOPp->regfile__DOT__r_file[19]),32);
        vcdp->fullBus(c+77,(vlTOPp->regfile__DOT__r_file[20]),32);
        vcdp->fullBus(c+78,(vlTOPp->regfile__DOT__r_file[21]),32);
        vcdp->fullBus(c+79,(vlTOPp->regfile__DOT__r_file[22]),32);
        vcdp->fullBus(c+80,(vlTOPp->regfile__DOT__r_file[23]),32);
        vcdp->fullBus(c+81,(vlTOPp->regfile__DOT__r_file[24]),32);
        vcdp->fullBus(c+82,(vlTOPp->regfile__DOT__r_file[25]),32);
        vcdp->fullBus(c+83,(vlTOPp->regfile__DOT__r_file[26]),32);
        vcdp->fullBus(c+84,(vlTOPp->regfile__DOT__r_file[27]),32);
        vcdp->fullBus(c+85,(vlTOPp->regfile__DOT__r_file[28]),32);
        vcdp->fullBus(c+86,(vlTOPp->regfile__DOT__r_file[29]),32);
        vcdp->fullBus(c+87,(vlTOPp->regfile__DOT__r_file[30]),32);
        vcdp->fullBus(c+88,(vlTOPp->regfile__DOT__r_file[31]),32);
        vcdp->fullBit(c+313,(vlTOPp->clk));
        vcdp->fullBit(c+321,(vlTOPp->r_enable));
        vcdp->fullBit(c+329,(vlTOPp->w_enable));
        vcdp->fullBus(c+337,(vlTOPp->r_select),5);
        vcdp->fullBus(c+345,(vlTOPp->w_select),5);
        vcdp->fullBus(c+353,(vlTOPp->w_val),32);
        vcdp->fullBus(c+361,(vlTOPp->r_out),32);
        vcdp->fullBit(c+369,(vlTOPp->valid));
        vcdp->fullBus(c+377,(0x20U),32);
    }
}

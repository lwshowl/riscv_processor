// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vregfile__Syms.h"


//======================

void Vregfile::traceChg(VerilatedVcd* vcdp, void* userthis, uint32_t code) {
    // Callback from vcd->dump()
    Vregfile* t = (Vregfile*)userthis;
    Vregfile__Syms* __restrict vlSymsp = t->__VlSymsp;  // Setup global symbol table
    if (vlSymsp->getClearActivity()) {
        t->traceChgThis(vlSymsp, vcdp, code);
    }
}

//======================


void Vregfile::traceChgThis(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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

void Vregfile::traceChgThis__2(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBus(c+1,(vlTOPp->regfile__DOT__r_select_reg),5);
        vcdp->chgBus(c+9,(vlTOPp->regfile__DOT__w_select_reg),5);
        vcdp->chgBus(c+17,(vlTOPp->regfile__DOT__w_val_reg),32);
        vcdp->chgBus(c+25,(vlTOPp->regfile__DOT__r_out_reg),32);
        vcdp->chgBit(c+33,(vlTOPp->regfile__DOT__valid_reg));
        vcdp->chgBit(c+41,(vlTOPp->regfile__DOT__r_enable_reg));
        vcdp->chgBit(c+49,(vlTOPp->regfile__DOT__w_enable_reg));
        vcdp->chgBus(c+57,(vlTOPp->regfile__DOT__r_file[0]),32);
        vcdp->chgBus(c+58,(vlTOPp->regfile__DOT__r_file[1]),32);
        vcdp->chgBus(c+59,(vlTOPp->regfile__DOT__r_file[2]),32);
        vcdp->chgBus(c+60,(vlTOPp->regfile__DOT__r_file[3]),32);
        vcdp->chgBus(c+61,(vlTOPp->regfile__DOT__r_file[4]),32);
        vcdp->chgBus(c+62,(vlTOPp->regfile__DOT__r_file[5]),32);
        vcdp->chgBus(c+63,(vlTOPp->regfile__DOT__r_file[6]),32);
        vcdp->chgBus(c+64,(vlTOPp->regfile__DOT__r_file[7]),32);
        vcdp->chgBus(c+65,(vlTOPp->regfile__DOT__r_file[8]),32);
        vcdp->chgBus(c+66,(vlTOPp->regfile__DOT__r_file[9]),32);
        vcdp->chgBus(c+67,(vlTOPp->regfile__DOT__r_file[10]),32);
        vcdp->chgBus(c+68,(vlTOPp->regfile__DOT__r_file[11]),32);
        vcdp->chgBus(c+69,(vlTOPp->regfile__DOT__r_file[12]),32);
        vcdp->chgBus(c+70,(vlTOPp->regfile__DOT__r_file[13]),32);
        vcdp->chgBus(c+71,(vlTOPp->regfile__DOT__r_file[14]),32);
        vcdp->chgBus(c+72,(vlTOPp->regfile__DOT__r_file[15]),32);
        vcdp->chgBus(c+73,(vlTOPp->regfile__DOT__r_file[16]),32);
        vcdp->chgBus(c+74,(vlTOPp->regfile__DOT__r_file[17]),32);
        vcdp->chgBus(c+75,(vlTOPp->regfile__DOT__r_file[18]),32);
        vcdp->chgBus(c+76,(vlTOPp->regfile__DOT__r_file[19]),32);
        vcdp->chgBus(c+77,(vlTOPp->regfile__DOT__r_file[20]),32);
        vcdp->chgBus(c+78,(vlTOPp->regfile__DOT__r_file[21]),32);
        vcdp->chgBus(c+79,(vlTOPp->regfile__DOT__r_file[22]),32);
        vcdp->chgBus(c+80,(vlTOPp->regfile__DOT__r_file[23]),32);
        vcdp->chgBus(c+81,(vlTOPp->regfile__DOT__r_file[24]),32);
        vcdp->chgBus(c+82,(vlTOPp->regfile__DOT__r_file[25]),32);
        vcdp->chgBus(c+83,(vlTOPp->regfile__DOT__r_file[26]),32);
        vcdp->chgBus(c+84,(vlTOPp->regfile__DOT__r_file[27]),32);
        vcdp->chgBus(c+85,(vlTOPp->regfile__DOT__r_file[28]),32);
        vcdp->chgBus(c+86,(vlTOPp->regfile__DOT__r_file[29]),32);
        vcdp->chgBus(c+87,(vlTOPp->regfile__DOT__r_file[30]),32);
        vcdp->chgBus(c+88,(vlTOPp->regfile__DOT__r_file[31]),32);
    }
}

void Vregfile::traceChgThis__3(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) {
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    int c = code;
    if (0 && vcdp && c) {}  // Prevent unused
    // Body
    {
        vcdp->chgBit(c+313,(vlTOPp->clk));
        vcdp->chgBit(c+321,(vlTOPp->r_enable));
        vcdp->chgBit(c+329,(vlTOPp->w_enable));
        vcdp->chgBus(c+337,(vlTOPp->r_select),5);
        vcdp->chgBus(c+345,(vlTOPp->w_select),5);
        vcdp->chgBus(c+353,(vlTOPp->w_val),32);
        vcdp->chgBus(c+361,(vlTOPp->r_out),32);
        vcdp->chgBit(c+369,(vlTOPp->valid));
    }
}

// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vmemory.h for the primary calling header

#include "Vmemory.h"
#include "Vmemory__Syms.h"

//==========

VL_CTOR_IMP(Vmemory) {
    Vmemory__Syms* __restrict vlSymsp = __VlSymsp = new Vmemory__Syms(this, name());
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vmemory::__Vconfigure(Vmemory__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vmemory::~Vmemory() {
    delete __VlSymsp; __VlSymsp=NULL;
}

void Vmemory::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vmemory::eval\n"); );
    Vmemory__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        vlSymsp->__Vm_activity = true;
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("memory.v", 1, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vmemory::_eval_initial_loop(Vmemory__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    vlSymsp->__Vm_activity = true;
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        _eval_settle(vlSymsp);
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("memory.v", 1, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vmemory::_sequent__TOP__1(Vmemory__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmemory::_sequent__TOP__1\n"); );
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*6:0*/ __Vdlyvdim0__memory__DOT__mem__v0;
    CData/*0:0*/ __Vdlyvset__memory__DOT__mem__v0;
    IData/*31:0*/ __Vdlyvval__memory__DOT__mem__v0;
    // Body
    __Vdlyvset__memory__DOT__mem__v0 = 0U;
    vlTOPp->memory__DOT__r_enable_r = vlTOPp->r_enable;
    if ((1U & (~ (IData)(vlTOPp->memory__DOT__w_enable_r)))) {
        vlTOPp->memory__DOT__data_out_r = vlTOPp->memory__DOT__mem
            [(0x7fU & vlTOPp->memory__DOT__addr_select_r)];
    }
    if (vlTOPp->memory__DOT__w_enable_r) {
        __Vdlyvval__memory__DOT__mem__v0 = vlTOPp->memory__DOT__data_in_r;
        __Vdlyvset__memory__DOT__mem__v0 = 1U;
        __Vdlyvdim0__memory__DOT__mem__v0 = (0x7fU 
                                             & vlTOPp->memory__DOT__addr_select_r);
    }
    if (__Vdlyvset__memory__DOT__mem__v0) {
        vlTOPp->memory__DOT__mem[__Vdlyvdim0__memory__DOT__mem__v0] 
            = __Vdlyvval__memory__DOT__mem__v0;
    }
    vlTOPp->data_out = vlTOPp->memory__DOT__data_out_r;
    vlTOPp->memory__DOT__data_in_r = vlTOPp->data_in;
    vlTOPp->memory__DOT__w_enable_r = vlTOPp->w_enable;
    vlTOPp->memory__DOT__addr_select_r = vlTOPp->addr_select;
}

void Vmemory::_settle__TOP__2(Vmemory__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmemory::_settle__TOP__2\n"); );
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->data_out = vlTOPp->memory__DOT__data_out_r;
}

void Vmemory::_eval(Vmemory__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmemory::_eval\n"); );
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
        vlTOPp->__Vm_traceActivity = (2U | vlTOPp->__Vm_traceActivity);
    }
    // Final
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
}

void Vmemory::_eval_initial(Vmemory__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmemory::_eval_initial\n"); );
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
}

void Vmemory::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmemory::final\n"); );
    // Variables
    Vmemory__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vmemory::_eval_settle(Vmemory__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmemory::_eval_settle\n"); );
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

VL_INLINE_OPT QData Vmemory::_change_request(Vmemory__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmemory::_change_request\n"); );
    Vmemory* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vmemory::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmemory::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((w_enable & 0xfeU))) {
        Verilated::overWidthError("w_enable");}
    if (VL_UNLIKELY((r_enable & 0xfeU))) {
        Verilated::overWidthError("r_enable");}
}
#endif  // VL_DEBUG

void Vmemory::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmemory::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    w_enable = VL_RAND_RESET_I(1);
    r_enable = VL_RAND_RESET_I(1);
    addr_select = VL_RAND_RESET_I(32);
    data_in = VL_RAND_RESET_I(32);
    data_out = VL_RAND_RESET_I(32);
    { int __Vi0=0; for (; __Vi0<128; ++__Vi0) {
            memory__DOT__mem[__Vi0] = VL_RAND_RESET_I(32);
    }}
    memory__DOT__w_enable_r = VL_RAND_RESET_I(1);
    memory__DOT__r_enable_r = VL_RAND_RESET_I(1);
    memory__DOT__addr_select_r = VL_RAND_RESET_I(32);
    memory__DOT__data_in_r = VL_RAND_RESET_I(32);
    memory__DOT__data_out_r = VL_RAND_RESET_I(32);
    __Vm_traceActivity = 0;
}

// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vregfile.h for the primary calling header

#include "Vregfile.h"
#include "Vregfile__Syms.h"

//==========

VL_CTOR_IMP(Vregfile) {
    Vregfile__Syms* __restrict vlSymsp = __VlSymsp = new Vregfile__Syms(this, name());
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vregfile::__Vconfigure(Vregfile__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vregfile::~Vregfile() {
    delete __VlSymsp; __VlSymsp=NULL;
}

void Vregfile::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vregfile::eval\n"); );
    Vregfile__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("regfile.v", 1, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vregfile::_eval_initial_loop(Vregfile__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("regfile.v", 1, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vregfile::_sequent__TOP__1(Vregfile__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vregfile::_sequent__TOP__1\n"); );
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*4:0*/ __Vdlyvdim0__regfile__DOT__r_file__v0;
    CData/*0:0*/ __Vdlyvset__regfile__DOT__r_file__v0;
    IData/*31:0*/ __Vdlyvval__regfile__DOT__r_file__v0;
    // Body
    __Vdlyvset__regfile__DOT__r_file__v0 = 0U;
    if (vlTOPp->regfile__DOT__r_enable_reg) {
        vlTOPp->regfile__DOT__valid_reg = 1U;
    } else {
        if (vlTOPp->regfile__DOT__w_enable_reg) {
            vlTOPp->regfile__DOT__valid_reg = 1U;
        }
    }
    if (vlTOPp->regfile__DOT__r_enable_reg) {
        vlTOPp->regfile__DOT__r_out_reg = vlTOPp->regfile__DOT__r_file
            [vlTOPp->regfile__DOT__r_select_reg];
    }
    if ((1U & (~ (IData)(vlTOPp->regfile__DOT__r_enable_reg)))) {
        if (vlTOPp->regfile__DOT__w_enable_reg) {
            __Vdlyvval__regfile__DOT__r_file__v0 = vlTOPp->regfile__DOT__w_val_reg;
            __Vdlyvset__regfile__DOT__r_file__v0 = 1U;
            __Vdlyvdim0__regfile__DOT__r_file__v0 = vlTOPp->regfile__DOT__w_select_reg;
        }
    }
    if (__Vdlyvset__regfile__DOT__r_file__v0) {
        vlTOPp->regfile__DOT__r_file[__Vdlyvdim0__regfile__DOT__r_file__v0] 
            = __Vdlyvval__regfile__DOT__r_file__v0;
    }
    vlTOPp->valid = vlTOPp->regfile__DOT__valid_reg;
    vlTOPp->r_out = vlTOPp->regfile__DOT__r_out_reg;
    vlTOPp->regfile__DOT__r_select_reg = vlTOPp->r_select;
    vlTOPp->regfile__DOT__w_val_reg = vlTOPp->w_val;
    vlTOPp->regfile__DOT__w_select_reg = vlTOPp->w_select;
    vlTOPp->regfile__DOT__w_enable_reg = vlTOPp->w_enable;
    vlTOPp->regfile__DOT__r_enable_reg = vlTOPp->r_enable;
}

void Vregfile::_settle__TOP__2(Vregfile__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vregfile::_settle__TOP__2\n"); );
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->r_out = vlTOPp->regfile__DOT__r_out_reg;
    vlTOPp->valid = vlTOPp->regfile__DOT__valid_reg;
}

void Vregfile::_eval(Vregfile__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vregfile::_eval\n"); );
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
        vlTOPp->__Vm_traceActivity = (2U | vlTOPp->__Vm_traceActivity);
    }
    // Final
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
}

void Vregfile::_eval_initial(Vregfile__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vregfile::_eval_initial\n"); );
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
}

void Vregfile::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vregfile::final\n"); );
    // Variables
    Vregfile__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vregfile::_eval_settle(Vregfile__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vregfile::_eval_settle\n"); );
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
}

VL_INLINE_OPT QData Vregfile::_change_request(Vregfile__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vregfile::_change_request\n"); );
    Vregfile* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vregfile::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vregfile::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((r_enable & 0xfeU))) {
        Verilated::overWidthError("r_enable");}
    if (VL_UNLIKELY((w_enable & 0xfeU))) {
        Verilated::overWidthError("w_enable");}
    if (VL_UNLIKELY((r_select & 0xe0U))) {
        Verilated::overWidthError("r_select");}
    if (VL_UNLIKELY((w_select & 0xe0U))) {
        Verilated::overWidthError("w_select");}
}
#endif  // VL_DEBUG

void Vregfile::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vregfile::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    r_enable = VL_RAND_RESET_I(1);
    w_enable = VL_RAND_RESET_I(1);
    r_select = VL_RAND_RESET_I(5);
    w_select = VL_RAND_RESET_I(5);
    w_val = VL_RAND_RESET_I(32);
    r_out = VL_RAND_RESET_I(32);
    valid = VL_RAND_RESET_I(1);
    regfile__DOT__r_select_reg = VL_RAND_RESET_I(5);
    regfile__DOT__w_select_reg = VL_RAND_RESET_I(5);
    regfile__DOT__w_val_reg = VL_RAND_RESET_I(32);
    regfile__DOT__r_out_reg = VL_RAND_RESET_I(32);
    regfile__DOT__valid_reg = VL_RAND_RESET_I(1);
    regfile__DOT__r_enable_reg = VL_RAND_RESET_I(1);
    regfile__DOT__w_enable_reg = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            regfile__DOT__r_file[__Vi0] = VL_RAND_RESET_I(32);
    }}
    __Vm_traceActivity = 0;
}

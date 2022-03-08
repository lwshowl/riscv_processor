// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vpc.h for the primary calling header

#include "Vpc.h"
#include "Vpc__Syms.h"

//==========

VL_CTOR_IMP(Vpc) {
    Vpc__Syms* __restrict vlSymsp = __VlSymsp = new Vpc__Syms(this, name());
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vpc::__Vconfigure(Vpc__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vpc::~Vpc() {
    delete __VlSymsp; __VlSymsp=NULL;
}

void Vpc::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vpc::eval\n"); );
    Vpc__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("pc.v", 1, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vpc::_eval_initial_loop(Vpc__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("pc.v", 1, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vpc::_sequent__TOP__1(Vpc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpc::_sequent__TOP__1\n"); );
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->out_valid = 1U;
    vlTOPp->pc_out_reg = ((IData)(vlTOPp->rst) ? 0U
                           : ((IData)(vlTOPp->branch)
                               ? vlTOPp->immediate : 
                              ((IData)(1U) + vlTOPp->pc_out_reg)));
}

void Vpc::_eval(Vpc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpc::_eval\n"); );
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk)))) {
        vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    // Final
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
}

void Vpc::_eval_initial(Vpc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpc::_eval_initial\n"); );
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
}

void Vpc::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpc::final\n"); );
    // Variables
    Vpc__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vpc::_eval_settle(Vpc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpc::_eval_settle\n"); );
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

VL_INLINE_OPT QData Vpc::_change_request(Vpc__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpc::_change_request\n"); );
    Vpc* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vpc::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpc::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((rst & 0xfeU))) {
        Verilated::overWidthError("rst");}
    if (VL_UNLIKELY((branch & 0xfeU))) {
        Verilated::overWidthError("branch");}
}
#endif  // VL_DEBUG

void Vpc::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vpc::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    rst = VL_RAND_RESET_I(1);
    branch = VL_RAND_RESET_I(1);
    immediate = VL_RAND_RESET_I(32);
    pc_out_reg = VL_RAND_RESET_I(32);
    out_valid = VL_RAND_RESET_I(1);
    __Vm_traceActivity = 0;
}

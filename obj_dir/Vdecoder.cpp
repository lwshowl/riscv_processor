// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vdecoder.h for the primary calling header

#include "Vdecoder.h"
#include "Vdecoder__Syms.h"

//==========

VL_CTOR_IMP(Vdecoder) {
    Vdecoder__Syms* __restrict vlSymsp = __VlSymsp = new Vdecoder__Syms(this, name());
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vdecoder::__Vconfigure(Vdecoder__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vdecoder::~Vdecoder() {
    delete __VlSymsp; __VlSymsp=NULL;
}

void Vdecoder::eval() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vdecoder::eval\n"); );
    Vdecoder__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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
            VL_FATAL_MT("decoder.v", 2, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vdecoder::_eval_initial_loop(Vdecoder__Syms* __restrict vlSymsp) {
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
            VL_FATAL_MT("decoder.v", 2, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vdecoder::_initial__TOP__1(Vdecoder__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdecoder::_initial__TOP__1\n"); );
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->decoder__DOT__i_opcode[0U] = 3U;
    vlTOPp->decoder__DOT__i_opcode[1U] = 0x13U;
    vlTOPp->decoder__DOT__i_opcode[2U] = 0x67U;
    vlTOPp->decoder__DOT__i_opcode[3U] = 0x73U;
    vlTOPp->decoder__DOT__u_opcode[0U] = 0x37U;
    vlTOPp->decoder__DOT__u_opcode[1U] = 0x17U;
}

VL_INLINE_OPT void Vdecoder::_sequent__TOP__2(Vdecoder__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdecoder::_sequent__TOP__2\n"); );
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->decoder__DOT__rd_r = (0x1fU & (vlTOPp->instr 
                                           >> 7U));
    vlTOPp->decoder__DOT__rs2_r = (0x1fU & (vlTOPp->instr 
                                            >> 0x14U));
    vlTOPp->decoder__DOT__rs1_r = (0x1fU & (vlTOPp->instr 
                                            >> 0xfU));
    vlTOPp->decoder__DOT__imm_r = 0U;
    if ((0x40U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
        if ((0x20U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
            if ((0x10U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
                if ((1U & (~ ((IData)(vlTOPp->decoder__DOT__opcode_r) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlTOPp->decoder__DOT__opcode_r) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
                            if ((1U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
                                vlTOPp->decoder__DOT__imm_r 
                                    = ((0x1fff000U 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0xfffU & 
                                          (vlTOPp->decoder__DOT__instr_r 
                                           >> 0x14U)));
                            }
                        }
                    }
                }
            } else {
                if ((1U & (~ ((IData)(vlTOPp->decoder__DOT__opcode_r) 
                              >> 3U)))) {
                    if ((4U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
                        if ((2U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
                            if ((1U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
                                vlTOPp->decoder__DOT__imm_r 
                                    = ((0x1fff000U 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0xfffU & 
                                          (vlTOPp->decoder__DOT__instr_r 
                                           >> 0x14U)));
                            }
                        }
                    }
                }
            }
        }
    } else {
        if ((1U & (~ ((IData)(vlTOPp->decoder__DOT__opcode_r) 
                      >> 5U)))) {
            if ((0x10U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
                if ((1U & (~ ((IData)(vlTOPp->decoder__DOT__opcode_r) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlTOPp->decoder__DOT__opcode_r) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
                            if ((1U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
                                vlTOPp->decoder__DOT__imm_r 
                                    = ((0x1fff000U 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0xfffU & 
                                          (vlTOPp->decoder__DOT__instr_r 
                                           >> 0x14U)));
                            }
                        }
                    }
                }
            } else {
                if ((1U & (~ ((IData)(vlTOPp->decoder__DOT__opcode_r) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlTOPp->decoder__DOT__opcode_r) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
                            if ((1U & (IData)(vlTOPp->decoder__DOT__opcode_r))) {
                                vlTOPp->decoder__DOT__imm_r 
                                    = ((0x1fff000U 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0xfffU & 
                                          (vlTOPp->decoder__DOT__instr_r 
                                           >> 0x14U)));
                            }
                        }
                    }
                }
            }
        }
    }
    if ((0x37U == (IData)(vlTOPp->decoder__DOT__opcode_r))) {
        vlTOPp->decoder__DOT__imm_r = ((0x1f00000U 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0xfffffU 
                                          & (vlTOPp->decoder__DOT__instr_r 
                                             >> 0xcU)));
    } else {
        if ((0x17U == (IData)(vlTOPp->decoder__DOT__opcode_r))) {
            vlTOPp->decoder__DOT__imm_r = ((0x1f00000U 
                                            & vlTOPp->decoder__DOT__imm_r) 
                                           | (0xfffffU 
                                              & (vlTOPp->decoder__DOT__instr_r 
                                                 >> 0xcU)));
        }
    }
    if ((0x6fU == (IData)(vlTOPp->decoder__DOT__opcode_r))) {
        vlTOPp->decoder__DOT__imm_r = ((0x1efffffU 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0x100000U 
                                          & (vlTOPp->decoder__DOT__instr_r 
                                             >> 0xbU)));
        vlTOPp->decoder__DOT__imm_r = ((0x1fff801U 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0x7feU & 
                                          (vlTOPp->decoder__DOT__instr_r 
                                           >> 0x14U)));
        vlTOPp->decoder__DOT__imm_r = ((0x1fff7ffU 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0x800U & 
                                          (vlTOPp->decoder__DOT__instr_r 
                                           >> 9U)));
        vlTOPp->decoder__DOT__imm_r = ((0x1f00fffU 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0xff000U 
                                          & vlTOPp->decoder__DOT__instr_r));
    }
    if ((0x63U == (IData)(vlTOPp->decoder__DOT__opcode_r))) {
        vlTOPp->decoder__DOT__imm_r = ((0x1ffefffU 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0x1000U & 
                                          (vlTOPp->decoder__DOT__instr_r 
                                           >> 0x13U)));
        vlTOPp->decoder__DOT__imm_r = ((0x1fff81fU 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0x7e0U & 
                                          (vlTOPp->decoder__DOT__instr_r 
                                           >> 0x14U)));
        vlTOPp->decoder__DOT__imm_r = ((0x1ffffe1U 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0x1eU & (vlTOPp->decoder__DOT__instr_r 
                                                   >> 7U)));
        vlTOPp->decoder__DOT__imm_r = ((0x1ffff7fU 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0x80U & vlTOPp->decoder__DOT__instr_r));
    }
    if ((0x23U == (IData)(vlTOPp->decoder__DOT__opcode_r))) {
        vlTOPp->decoder__DOT__imm_r = ((0x1fff01fU 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0xfe0U & 
                                          (vlTOPp->decoder__DOT__instr_r 
                                           >> 0x14U)));
        vlTOPp->decoder__DOT__imm_r = ((0x1ffffe0U 
                                        & vlTOPp->decoder__DOT__imm_r) 
                                       | (0x1fU & (vlTOPp->decoder__DOT__instr_r 
                                                   >> 7U)));
    }
    vlTOPp->decoder__DOT__instr_id_r = ((0x73U == vlTOPp->decoder__DOT__instr_r)
                                         ? 0x26U : 
                                        ((0x1000073U 
                                          == vlTOPp->decoder__DOT__instr_r)
                                          ? 0x25U : 0x3fU));
    vlTOPp->decoder__DOT__instr_id_r = ((0x37U == (IData)(vlTOPp->decoder__DOT__opcode_r))
                                         ? 0U : ((0x17U 
                                                  == (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                  ? 1U
                                                  : 0x3fU));
    vlTOPp->decoder__DOT__instr_id_r = ((0x6fU == (IData)(vlTOPp->decoder__DOT__opcode_r))
                                         ? 2U : 0x3fU);
    vlTOPp->decoder__DOT__instr_id_r = ((0x63U == (IData)(vlTOPp->decoder__DOT__opcode_r))
                                         ? ((4U & (IData)(vlTOPp->decoder__DOT__func3_r))
                                             ? ((2U 
                                                 & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                 ? 
                                                ((1U 
                                                  & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                  ? 9U
                                                  : 8U)
                                                 : 
                                                ((1U 
                                                  & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                  ? 7U
                                                  : 6U))
                                             : ((2U 
                                                 & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                 ? 0x3fU
                                                 : 
                                                ((1U 
                                                  & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                  ? 5U
                                                  : 4U)))
                                         : 0x3fU);
    vlTOPp->decoder__DOT__instr_id_r = ((0x23U == (IData)(vlTOPp->decoder__DOT__opcode_r))
                                         ? ((0U == (IData)(vlTOPp->decoder__DOT__func3_r))
                                             ? 0xfU
                                             : ((1U 
                                                 == (IData)(vlTOPp->decoder__DOT__func3_r))
                                                 ? 0x10U
                                                 : 
                                                ((2U 
                                                  == (IData)(vlTOPp->decoder__DOT__func3_r))
                                                  ? 0x11U
                                                  : 0x3fU)))
                                         : 0x3fU);
    vlTOPp->decoder__DOT__instr_id_r = ((0x40U & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                         ? ((0x20U 
                                             & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                             ? ((0x10U 
                                                 & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                 ? 
                                                ((8U 
                                                  & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                  ? 0x3fU
                                                  : 
                                                 ((4U 
                                                   & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                   ? 0x3fU
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                     ? 
                                                    ((4U 
                                                      & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                      ? 
                                                     ((2U 
                                                       & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                        ? 0x2cU
                                                        : 0x2bU)
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                        ? 0x2aU
                                                        : 0x3fU))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                        ? 0x29U
                                                        : 0x28U)
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                        ? 0x27U
                                                        : 0x3fU)))
                                                     : 0x3fU)
                                                    : 0x3fU)))
                                                 : 
                                                ((8U 
                                                  & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                  ? 0x3fU
                                                  : 
                                                 ((4U 
                                                   & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                     ? 3U
                                                     : 0x3fU)
                                                    : 0x3fU)
                                                   : 0x3fU)))
                                             : 0x3fU)
                                         : ((0x20U 
                                             & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                             ? 0x3fU
                                             : ((0x10U 
                                                 & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                 ? 
                                                ((8U 
                                                  & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                  ? 0x3fU
                                                  : 
                                                 ((4U 
                                                   & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                   ? 0x3fU
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                     ? 
                                                    ((4U 
                                                      & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                      ? 
                                                     ((2U 
                                                       & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                        ? 0x17U
                                                        : 0x16U)
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                        ? 
                                                       ((0x20U 
                                                         == (IData)(vlTOPp->decoder__DOT__func7_r))
                                                         ? 0x1aU
                                                         : 0x19U)
                                                        : 0x15U))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                        ? 0x14U
                                                        : 0x13U)
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                        ? 0x18U
                                                        : 0x12U)))
                                                     : 0x3fU)
                                                    : 0x3fU)))
                                                 : 
                                                ((8U 
                                                  & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                  ? 0x3fU
                                                  : 
                                                 ((4U 
                                                   & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                   ? 0x3fU
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlTOPp->decoder__DOT__opcode_r))
                                                     ? 
                                                    ((4U 
                                                      & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                      ? 
                                                     ((2U 
                                                       & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                       ? 0x3fU
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                        ? 0xeU
                                                        : 0xdU))
                                                      : 
                                                     ((2U 
                                                       & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                       ? 
                                                      ((1U 
                                                        & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                        ? 0x3fU
                                                        : 0xcU)
                                                       : 
                                                      ((1U 
                                                        & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                        ? 0xbU
                                                        : 0xaU)))
                                                     : 0x3fU)
                                                    : 0x3fU))))));
    vlTOPp->decoder__DOT__instr_id_r = ((0x23U == (IData)(vlTOPp->decoder__DOT__opcode_r))
                                         ? ((4U & (IData)(vlTOPp->decoder__DOT__func3_r))
                                             ? ((2U 
                                                 & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                 ? 
                                                ((1U 
                                                  & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                  ? 0x24U
                                                  : 0x23U)
                                                 : 
                                                ((1U 
                                                  & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                  ? 
                                                 ((0U 
                                                   == (IData)(vlTOPp->decoder__DOT__func7_r))
                                                   ? 0x21U
                                                   : 0x22U)
                                                  : 0x20U))
                                             : ((2U 
                                                 & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                 ? 
                                                ((1U 
                                                  & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                  ? 0x1fU
                                                  : 0x1eU)
                                                 : 
                                                ((1U 
                                                  & (IData)(vlTOPp->decoder__DOT__func3_r))
                                                  ? 0x1dU
                                                  : 
                                                 ((0U 
                                                   == (IData)(vlTOPp->decoder__DOT__func7_r))
                                                   ? 0x1bU
                                                   : 0x1cU))))
                                         : 0x3fU);
    vlTOPp->rd = vlTOPp->decoder__DOT__rd_r;
    vlTOPp->rs2 = vlTOPp->decoder__DOT__rs2_r;
    vlTOPp->rs1 = vlTOPp->decoder__DOT__rs1_r;
    vlTOPp->imm = vlTOPp->decoder__DOT__imm_r;
    vlTOPp->decoder__DOT__instr_r = vlTOPp->instr;
    vlTOPp->instr_id = vlTOPp->decoder__DOT__instr_id_r;
    vlTOPp->decoder__DOT__opcode_r = (0x7fU & vlTOPp->instr);
    vlTOPp->decoder__DOT__func7_r = (0x7fU & (vlTOPp->instr 
                                              >> 0x19U));
    vlTOPp->decoder__DOT__func3_r = (7U & (vlTOPp->instr 
                                           >> 0xcU));
    vlTOPp->func7 = vlTOPp->decoder__DOT__func7_r;
    vlTOPp->func3 = vlTOPp->decoder__DOT__func3_r;
}

void Vdecoder::_settle__TOP__3(Vdecoder__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdecoder::_settle__TOP__3\n"); );
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->rs1 = vlTOPp->decoder__DOT__rs1_r;
    vlTOPp->rs2 = vlTOPp->decoder__DOT__rs2_r;
    vlTOPp->rd = vlTOPp->decoder__DOT__rd_r;
    vlTOPp->instr_id = vlTOPp->decoder__DOT__instr_id_r;
    vlTOPp->imm = vlTOPp->decoder__DOT__imm_r;
    vlTOPp->func3 = vlTOPp->decoder__DOT__func3_r;
    vlTOPp->func7 = vlTOPp->decoder__DOT__func7_r;
}

void Vdecoder::_eval(Vdecoder__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdecoder::_eval\n"); );
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk)))) {
        vlTOPp->_sequent__TOP__2(vlSymsp);
        vlTOPp->__Vm_traceActivity = (2U | vlTOPp->__Vm_traceActivity);
    }
    // Final
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
}

void Vdecoder::_eval_initial(Vdecoder__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdecoder::_eval_initial\n"); );
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_initial__TOP__1(vlSymsp);
    vlTOPp->__Vm_traceActivity = (1U | vlTOPp->__Vm_traceActivity);
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
}

void Vdecoder::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdecoder::final\n"); );
    // Variables
    Vdecoder__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vdecoder::_eval_settle(Vdecoder__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdecoder::_eval_settle\n"); );
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__3(vlSymsp);
}

VL_INLINE_OPT QData Vdecoder::_change_request(Vdecoder__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdecoder::_change_request\n"); );
    Vdecoder* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Vdecoder::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdecoder::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
}
#endif  // VL_DEBUG

void Vdecoder::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vdecoder::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    instr = VL_RAND_RESET_I(32);
    rs1 = VL_RAND_RESET_I(5);
    rs2 = VL_RAND_RESET_I(5);
    rd = VL_RAND_RESET_I(5);
    func3 = VL_RAND_RESET_I(3);
    func7 = VL_RAND_RESET_I(7);
    imm = VL_RAND_RESET_I(25);
    instr_id = VL_RAND_RESET_I(6);
    decoder__DOT__instr_r = VL_RAND_RESET_I(32);
    decoder__DOT__opcode_r = VL_RAND_RESET_I(7);
    decoder__DOT__func3_r = VL_RAND_RESET_I(3);
    decoder__DOT__func7_r = VL_RAND_RESET_I(7);
    decoder__DOT__imm_r = VL_RAND_RESET_I(25);
    decoder__DOT__instr_id_r = VL_RAND_RESET_I(6);
    decoder__DOT__rs1_r = VL_RAND_RESET_I(5);
    decoder__DOT__rs2_r = VL_RAND_RESET_I(5);
    decoder__DOT__rd_r = VL_RAND_RESET_I(5);
    __Vm_traceActivity = 0;
}

// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VDECODER_H_
#define _VDECODER_H_  // guard

#include "verilated.h"

//==========

class Vdecoder__Syms;
class Vdecoder_VerilatedVcd;


//----------

VL_MODULE(Vdecoder) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk,0,0);
    VL_OUT8(rs1,4,0);
    VL_OUT8(rs2,4,0);
    VL_OUT8(rd,4,0);
    VL_OUT8(func3,2,0);
    VL_OUT8(func7,6,0);
    VL_OUT8(instr_id,5,0);
    VL_IN(instr,31,0);
    VL_OUT(imm,24,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*6:0*/ decoder__DOT__opcode_r;
    CData/*2:0*/ decoder__DOT__func3_r;
    CData/*6:0*/ decoder__DOT__func7_r;
    CData/*5:0*/ decoder__DOT__instr_id_r;
    CData/*4:0*/ decoder__DOT__rs1_r;
    CData/*4:0*/ decoder__DOT__rs2_r;
    CData/*4:0*/ decoder__DOT__rd_r;
    IData/*31:0*/ decoder__DOT__instr_r;
    IData/*24:0*/ decoder__DOT__imm_r;
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vclklast__TOP__clk;
    IData/*31:0*/ __Vm_traceActivity;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vdecoder__Syms* __VlSymsp;  // Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    CData/*6:0*/ decoder__DOT__i_opcode[4];
    CData/*6:0*/ decoder__DOT__u_opcode[2];
    
    
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vdecoder);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vdecoder(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vdecoder();
    /// Trace signals in the model; called by application code
    void trace(VerilatedVcdC* tfp, int levels, int options = 0);
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vdecoder__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vdecoder__Syms* symsp, bool first);
  private:
    static QData _change_request(Vdecoder__Syms* __restrict vlSymsp);
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vdecoder__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vdecoder__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vdecoder__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _initial__TOP__1(Vdecoder__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__2(Vdecoder__Syms* __restrict vlSymsp);
    static void _settle__TOP__3(Vdecoder__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void traceChgThis(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceChgThis__2(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceChgThis__3(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceChgThis__4(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceFullThis(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) VL_ATTR_COLD;
    static void traceFullThis__1(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) VL_ATTR_COLD;
    static void traceInitThis(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) VL_ATTR_COLD;
    static void traceInitThis__1(Vdecoder__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) VL_ATTR_COLD;
    static void traceInit(VerilatedVcd* vcdp, void* userthis, uint32_t code);
    static void traceFull(VerilatedVcd* vcdp, void* userthis, uint32_t code);
    static void traceChg(VerilatedVcd* vcdp, void* userthis, uint32_t code);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard

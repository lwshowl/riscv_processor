// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VREGFILE_H_
#define _VREGFILE_H_  // guard

#include "verilated.h"

//==========

class Vregfile__Syms;
class Vregfile_VerilatedVcd;


//----------

VL_MODULE(Vregfile) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk,0,0);
    VL_IN8(r_enable,0,0);
    VL_IN8(w_enable,0,0);
    VL_IN8(r_select,4,0);
    VL_IN8(w_select,4,0);
    VL_OUT8(valid,0,0);
    VL_IN(w_val,31,0);
    VL_OUT(r_out,31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*4:0*/ regfile__DOT__r_select_reg;
    CData/*4:0*/ regfile__DOT__w_select_reg;
    CData/*0:0*/ regfile__DOT__valid_reg;
    CData/*0:0*/ regfile__DOT__r_enable_reg;
    CData/*0:0*/ regfile__DOT__w_enable_reg;
    IData/*31:0*/ regfile__DOT__w_val_reg;
    IData/*31:0*/ regfile__DOT__r_out_reg;
    IData/*31:0*/ regfile__DOT__r_file[32];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vclklast__TOP__clk;
    IData/*31:0*/ __Vm_traceActivity;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vregfile__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vregfile);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vregfile(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vregfile();
    /// Trace signals in the model; called by application code
    void trace(VerilatedVcdC* tfp, int levels, int options = 0);
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vregfile__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vregfile__Syms* symsp, bool first);
  private:
    static QData _change_request(Vregfile__Syms* __restrict vlSymsp);
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vregfile__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vregfile__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vregfile__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__1(Vregfile__Syms* __restrict vlSymsp);
    static void _settle__TOP__2(Vregfile__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void traceChgThis(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceChgThis__2(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceChgThis__3(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code);
    static void traceFullThis(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) VL_ATTR_COLD;
    static void traceFullThis__1(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) VL_ATTR_COLD;
    static void traceInitThis(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) VL_ATTR_COLD;
    static void traceInitThis__1(Vregfile__Syms* __restrict vlSymsp, VerilatedVcd* vcdp, uint32_t code) VL_ATTR_COLD;
    static void traceInit(VerilatedVcd* vcdp, void* userthis, uint32_t code);
    static void traceFull(VerilatedVcd* vcdp, void* userthis, uint32_t code);
    static void traceChg(VerilatedVcd* vcdp, void* userthis, uint32_t code);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard

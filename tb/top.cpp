// #include "verilated.h"
#include "Vtop.h"            // From Verilating "top.v"
#include "verilated_vcd_c.h" //VCD file

using namespace std;     //"using" directive

vluint64_t main_time=0;  // Current simulation time

int main(int argc, char** argv, char** env){   // Initialize Verilators variables
    Verilated::commandArgs(argc, argv);        // Remember args
    
    Vtop* my_top = new Vtop;                   // Create an instance of our module under test

    VerilatedVcdC* vcd_file=0;
  
     const char *vcd = Verilated::commandArgsPlusMatch("vcd="); //--vdc use then only it will activate
         if (vcd[0]) {
          Verilated::traceEverOn(true);
          vcd_file = new VerilatedVcdC;  // Create our trace.vcd output
          my_top->trace (vcd_file, 99);  // Trace all of the adder signals for the duration of the run
          vcd_file->open ("trace.vcd");  // Output the trace.vcd file
        }

vluint64_t timeout = 0;
	const char *arg_timeout = Verilated::commandArgsPlusMatch("timeout=");
	if (arg_timeout[0]) {
	  timeout = atoi(arg_timeout+9);
  }

vluint64_t vcd_start = 0;
	const char *arg_vcd_start = Verilated::commandArgsPlusMatch("vcd_start=");
	if (arg_vcd_start[0]) {
	  vcd_start = atoi(arg_vcd_start+11);
  }

    bool dump = false;
    my_top->clk = 0;
    my_top->reset = 0;         // Set some inputs
    while (!Verilated::gotFinish()) {

        if (vcd_file && !dump && (main_time > vcd_start)) {
            dump = true;
          }

        if (main_time > 300) {
            my_top->reset = 1;  // Deassert reset
        }
        
        my_top->eval();     // Evaluate any changes triggered by the falling edge

        if (dump) {
            vcd_file->dump(main_time);
        }
        
        if (timeout && (main_time >= timeout)) {
	        printf("Timeout: Exiting at time %lu\n", main_time);
	    }
        my_top->clk = !my_top->clk;    
        main_time += 2;       // Time passes...
    }

    if (vcd_file)
	  vcd_file->close();        // Close the trace.vcd 
    exit(0);                  // Testbench complete
}
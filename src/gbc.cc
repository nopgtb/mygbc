#include "gbc.h"//GBC

namespace mygbc{

    /// @brief Inits the gbc internals.
    /// @return 
    Status GBC::init(){
        run_flag_.store(true);
    }


    /// @brief Runs the main loop of the GBC.
    /// @return Exit status of the GBC.
    Status GBC::main_loop(){
        StatusOr<uint8_t> instruction_emulation;
        while(run_flag_.load()){
            instruction_emulation = processing_unit.fetch_decode_execute(memory_controller_);
            if(instruction_emulation.ok()){

            }
            return instruction_emulation.status();
        }
    }

    /// @brief Grants access to the processing unit and its internals.
    /// @return Processing unit.
    LR35902& GBC::get_processing_unit(){
        return processing_unit;
    }

    /// @brief Grants access to the memory controller and its internals.
    /// @return Memory controller.
    MemoryController& GBC::get_memory(){
        return memory_controller_;
    }
}


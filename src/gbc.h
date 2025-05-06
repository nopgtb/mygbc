#ifndef GBC_H
#define GBC_H

#include <atomic>
#include "components/memory_controller.h" //MemoryController
#include "components/lr35902.h" //LR35902

namespace mygbc{

    /// @brief Executes the functions of a GBC.
    class GBC{
        
        public:

        /// @brief Inits the gbc internals.
        /// @return 
        Status init();

        /// @brief Starts executing the GBC in its own thread.
        void run();

        /// @brief Stops the execution of the GBC
        void stop();

        /// @brief Runs the main loop of the GBC.
        /// @return Exit status of the GBC.
        Status main_loop();

        /// @brief Grants access to the processing unit and its internals.
        /// @return Processing unit.
        LR35902& get_processing_unit();

        /// @brief Grants access to the memory controller and its internals.
        /// @return Memory controller.
        MemoryController& get_memory();

        private:

        std::atomic<bool> run_flag_;

        //Components of the GBC
        MemoryController memory_controller_;
        LR35902 processing_unit;
    };

}

#endif
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream> //std::ostream
#include <memory> //std::shared_ptr
#include <vector> //std::vector

namespace mygbc{
    
    /// @brief Logger class, provides static logging utilities.
    class Logger{
        
        public:
        /// @brief Gets a static instance of the logger
        /// @return Static instance of the logger
        static Logger& instance();

        /// @brief Adds a output stream for the logging
        /// @param stream New output stream for the logging 
        static void init_stream(std::shared_ptr<std::ostream> stream);

        /// @brief Adds a output stream for the logging
        /// @param stream New output stream for the logging 
        void add_stream(std::shared_ptr<std::ostream> stream);

        /// @brief Logs and flushes message to output streams
        /// @param msg message to log
        static void send_message(const std::string& msg);

        /// @brief Logs and flushes message to output streams
        /// @param msg message to log
        static void send_message(const std::string&& msg);

        /// @brief Logs and flushes message to output streams
        /// @param msg message to log
        void log_flush(const std::string& msg);

        /// @brief Logs and flushes message to output streams
        /// @param msg message to log
        void log_flush(const std::string&& msg);

        private:
            std::vector<std::shared_ptr<std::ostream>> output_streams_;
    };
} //namespace_mygbc

#endif
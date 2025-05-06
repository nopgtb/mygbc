#include "logger.h" //Logger

namespace mygbc{
    /// @brief Gets a static instance of the logger
    /// @return Static instance of the logger
    Logger& Logger::instance(){
        static Logger log;
        return log;
    }

    /// @brief Adds a output stream for the logging
    /// @param stream New output stream for the logging 
    void Logger::init_stream(std::shared_ptr<std::ostream> stream){
        instance().add_stream(stream);
    }

    /// @brief Adds a output stream for the logging
    /// @param stream New output stream for the logging 
    void Logger::add_stream(std::shared_ptr<std::ostream> stream){
        output_streams_.push_back(stream);
    }

    /// @brief Logs and flushes message to output streams
    /// @param msg message to log
    void Logger::send_message(const std::string& msg){
        instance().log_flush(msg);
    }

    /// @brief Logs and flushes message to output streams
    /// @param msg message to log
    void Logger::send_message(const std::string&& msg){
        instance().log_flush(std::forward<const std::string&&>(msg));
    }

    /// @brief Logs and flushes message to output streams
    /// @param msg message to log
    void Logger::log_flush(const std::string& msg){
        for(auto stream : output_streams_){
            *stream << msg;
            stream->flush();
        }
    }

    /// @brief Logs and flushes message to output streams
    /// @param msg message to log
    void Logger::log_flush(const std::string&& msg){
        for(auto stream : output_streams_){
            *stream << msg;
            stream->flush();
        }
    }
}//namespace_mygbc
#include "log_message.h" //LogMessage
#include "../util.h" //Util
#include "logger.h" //Logger

namespace mygbc{

    /// @brief Initial creation of the log message with basic information 
    /// @param severity Severity of log
    /// @param file File from where log coming from
    /// @param function Function from where log coming from
    /// @param line Line from where log coming from
    LogMessage::LogMessage(LogMessage::LogSeverity severity):severity_(severity){
        message_stream_ << "[" << log_severity_to_str(severity) << "] ";
        message_stream_ << Util::get_unix_timestamp() << ": ";
    }

    /// @brief Logs and flushes the message to logger
    /// @details If severity is FATAL calls std::abort to kill program after log
    LogMessage::~LogMessage(){
        message_stream_ << "\n";
        Logger::send_message(message_stream_.str());
        if(severity_ == LogMessage::LogSeverity::FATAL){
            std::abort();
        }
    }

    /// @brief Returns reference to the internal stringstream
    /// @return StringStream for message logging
    std::ostream& LogMessage::stream(){
        return message_stream_;
    }

    /// @brief Convert given LogSeverity into str representation of the name.
    /// @param severity Severity of the log event
    /// @return str representation of the name.
    constexpr const char* LogMessage::log_severity_to_str(const LogMessage::LogSeverity severity){
        switch (severity)
        {
        case LogMessage::LogSeverity::INFO:
            return "INFO";
        case LogMessage::LogSeverity::WARNING:
            return "WARNING";
        case LogMessage::LogSeverity::ERROR:
            return "ERROR";
        case LogMessage::LogSeverity::FATAL:
            return "FATAL";
        default:
            return "UNKOWN";
        }
    }

}//namespace_mygbc
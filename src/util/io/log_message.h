#ifndef LOG_MESSAGE_H
#define LOG_MESSAGE_H

#include <sstream> //std::ostream

//glog inspired log macro
#define LOG(SEVERITY) \
    mygbc::LogMessage(mygbc::LogMessage::LogSeverity::SEVERITY).stream()

namespace mygbc{

    /// @brief Provides defered logging features.
    class LogMessage{
        public:
            /// @brief Severity of the logged message
            enum class LogSeverity{
                INFO = 0,
                WARNING = 1,
                ERROR = 2,
                FATAL = 3
            };

            /// @brief Initial creation of the log message with basic information 
            /// @param severity Severity of log
            /// @param file File from where log coming from
            /// @param function Function from where log coming from
            /// @param line Line from where log coming from
            LogMessage(LogMessage::LogSeverity severity);

            /// @brief Logs and flushes the message to logger
            /// @details If severity is FATAL calls std::abort to kill program after log
            ~LogMessage();

            /// @brief Returns reference to the internal stringstream
            /// @return StringStream for message logging
            std::ostream& stream();

        private:

            /// @brief Convert given LogSeverity into str representation of the name.
            /// @param severity Severity of the log event
            /// @return str representation of the name.
            constexpr const char* log_severity_to_str(const LogMessage::LogSeverity severity);

            std::stringstream message_stream_;
            LogSeverity severity_;
    };

}//namespace_mygbc

#endif
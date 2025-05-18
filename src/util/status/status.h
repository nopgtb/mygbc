#ifndef STATUS_H
#define STATUS_H

#include <string> // std::string

namespace mygbc{

    /// @brief Status class to indicate status of operations
    class Status{
        public:
            //Available statuses
            enum class StatusType{
                UNKOWN = 0,
                OK = 1,
                IO_ERROR = 2,
                PROTECTED_MEMORY_SET_ERROR = 3,
                INVALID_INDEX_ERROR = 4,
                INVALID_BINARY_ERROR = 5,
                INVALID_INPUT_ERROR = 6,
                INVALID_OPCODE_ERROR = 7,
                INVALID_REGISTER_ID_ERROR = 8,
                INVALID_MEMORY_RANGE_ERROR = 9,
            };

            /// @brief Builds a UNKOWN status as default.
            /// @details Sets type_ to StatusType::UNKOWN and message to empty string.
            Status();

            /// @brief Builds a status with the given parameters.
            /// @details Sets type_ to given status and message to given status message.
            /// @param type StatusType::Type of the current status.
            /// @param message Status message for the current status.
            Status(Status::StatusType type, const std::string& message);

            /// @brief Is the current status (No error)?
            /// @details Checks if the current status matches StatusType::OK
            /// @return current status matches StatusType::OK?
            const bool ok() const noexcept;

            /// @brief Returns the assosiated status message string.
            /// @details Getter for message_
            /// @return assosiated status message string.
            const std::string& message() const noexcept;

            /// @brief Returns the type of the status
            /// @details Getter for type_
            /// @return Type of the status (See Status::StatusType)  
            const Status::StatusType& code() const noexcept;

            /// @brief Builds a status with OK type.
            /// @details Status is returned with OK type.
            /// @return OK status object.
            static Status ok_status();

            /// @brief Builds a status with UNKOWN type and with given message.
            /// @details Status is returned with UNKOWN type and given message.
            /// @param message Status message.
            /// @return UNKOWN status object.
            static Status unkown_error(const std::string& message);

            /// @brief Builds a status with IOERROR type and with given message.
            /// @details Status is returned with IOERROR type and given message.
            /// @param message Status message.
            /// @return IOERROR status object.
            static Status io_error(const std::string& message);

            /// @brief Builds a status with PROTECTED_MEMORY_SET_ERROR type and with given message.
            /// @details Status is returned with PROTECTED_MEMORY_SET_ERROR type and given message.
            /// @param message Status message.
            /// @return PROTECTED_MEMORY_SET_ERROR status object.
            static Status protected_memory_set_error(const std::string& message);

            /// @brief Builds a status with INVALID_INDEX_ERROR type and with given message.
            /// @details Status is returned with INVALID_INDEX_ERROR type and given message.
            /// @param message Status message.
            /// @return INVALID_INDEX_ERROR status object.
            static Status invalid_index_error(const std::string& message);

            /// @brief Builds a status with INVALID_BINARY_ERROR type and with given message.
            /// @details Status is returned with INVALID_BINARY_ERROR type and given message.
            /// @param message Status message.
            /// @return INVALID_BINARY_ERROR status object.
            static Status invalid_binary_error(const std::string& message);

            /// @brief Builds a status with INVALID_INPUT_ERROR type and with given message.
            /// @details Status is returned with INVALID_INPUT_ERROR type and given message.
            /// @param message Status message.
            /// @return INVALID_INPUT_ERROR status object.
            static Status invalid_input_error(const std::string& message);

            /// @brief Builds a status with INVALID_OPCODE_ERROR type and with given message.
            /// @details Status is returned with INVALID_OPCODE_ERROR type and given message.
            /// @param message Status message.
            /// @return INVALID_OPCODE_ERROR status object.
            static Status invalid_opcode_error(const std::string& message);

            /// @brief Builds a status with INVALID_REGISTER_ID_ERROR type and with given message.
            /// @details Status is returned with INVALID_REGISTER_ID_ERROR type and given message.
            /// @param message Status message.
            /// @return INVALID_REGISTER_ID_ERROR status object.
            static Status invalid_register_id_error(const std::string& message);

            /// @brief Builds a status with INVALID_MEMORY_RANGE_ERROR type and with given message.
            /// @details Status is returned with INVALID_MEMORY_RANGE_ERROR type and given message.
            /// @param message Status message.
            /// @return INVALID_MEMORY_RANGE_ERROR status object.
            static Status invalid_memory_range_error(const std::string& message);

        private:
            StatusType type_;
            std::string message_;
    };
    
}//namespace_mygbc

#endif
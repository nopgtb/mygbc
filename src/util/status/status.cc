#include "status.h" //Status
#include <utility> //std::forward

/// @brief Builds a OK status as default.
/// @details Sets type_ to StatusType::OK and message to empty string.
Status::Status():type_(Status::StatusType::UNKOWN), message_(""){
}

/// @brief Builds a status with the given parameters.
/// @details Sets type_ to given status and message to given status message.
/// @param type StatusType::Type of the current status.
/// @param message Status message for the current status.
Status::Status(Status::StatusType type, const std::string& message):type_(type), message_(message){
}

/// @brief Is the current status (No error)?
/// @details Checks if the current status matches StatusType::OK
/// @return current status matches StatusType::OK?
const bool Status::ok() const noexcept{
    return type_ == Status::StatusType::OK;
}

/// @brief Returns the assosiated status message string.
/// @return assosiated status message string.
const std::string& Status::get_message() const noexcept{
    return message_;
}

/// @brief Returns the type of the status
/// @details Getter for type_
/// @return Type of the status (See Status::StatusType)  
const Status::StatusType& Status::get_type() const noexcept{
    return type_;
}

/// @brief Builds a status with OK type.
/// @details Status is returned with OK type.
/// @return OK status object.
Status Status::ok_status(){
    return Status(Status::StatusType::OK, "");
}

/// @brief Builds a status with UNKOWN type and with given message.
/// @details Status is returned with UNKOWN type and given message.
/// @param message Status message.
/// @return UNKOWN status object.
Status Status::unkown_error(const std::string& message){
    return Status(Status::StatusType::UNKOWN, std::forward<const std::string>(message));
}

/// @brief Builds a status with IOERROR type and with given message.
/// @details Status is returned with IOERROR type and given message.
/// @param message Status message.
/// @return IOERROR status object.
Status Status::io_error(const std::string& message){
    return Status(Status::StatusType::IO_ERROR, std::forward<const std::string>(message));
}

/// @brief Builds a status with PROTECTED_MEMORY_SET_ERROR type and with given message.
/// @details Status is returned with PROTECTED_MEMORY_SET_ERROR type and given message.
/// @param message Status message.
/// @return PROTECTED_MEMORY_SET_ERROR status object.
Status Status::protected_memory_set_error(const std::string& message){
    return Status(Status::StatusType::PROTECTED_MEMORY_SET_ERROR, std::forward<const std::string>(message));
}
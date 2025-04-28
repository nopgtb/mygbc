#include "bad_status_or_access.h"

namespace mygbc{

    /// @brief Default constructor for exception.
    /// @details Stores message for later use.
    /// @param msg Exception message.
    BadStatusOrAccess::BadStatusOrAccess(std::string msg):message_(msg){
    }

    /// @brief Returns message contents of the exception.
    /// @details Returns message given in constructor.
    /// @return Exception message.
    const char* BadStatusOrAccess::what() const noexcept{
        return message_.c_str();
    }

}//namespace_mygbc
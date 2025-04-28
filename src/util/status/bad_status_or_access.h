#ifndef BAD_STATUS_OR_ACCESS_H
#define BAD_STATUS_OR_ACCESS_H

#include <exception> //std::exception
#include <string> //std::string

namespace mygbc{

    /// @brief Exception type for accesing value on non ok StatusOr
    class BadStatusOrAccess : public std::exception{
        public:
            /// @brief Default constructor for exception.
            /// @details Stores message for later use.
            /// @param msg Exception message.
            explicit BadStatusOrAccess(std::string msg);

            /// @brief Returns message contents of the exception.
            /// @details Returns message given in constructor.
            /// @return Exception message.
            const char* what() const noexcept override;

        private:
            std::string message_;
    };
    
}//namespace_mygbc
#endif
#ifndef STATUS_OR_H
#define STATUS_OR_H

#include <concepts> //std::constructible_from, std::is_same
#include "status.h" //Status
#include "bad_status_or_access.h" //BadStatusOrAccess

namespace mygbc{

    /// @brief Templated StatusOr class. Enables returning of status or value.
    /// @tparam T Base type of the value.
    template <typename T>
    class StatusOr{

        public:

            /// @brief Default constructor.
            /// @details Builds status unkown.
            explicit StatusOr():status_(Status::StatusType::UNKOWN, ""){}

            /// @brief Constructor builds a status ok, statusor with value.
            /// @details Copies value from val.
            /// @tparam U Expects U to be constructable from T.
            /// @param val Value is copied from val.
            template <typename U>
            requires std::constructible_from<T, U>
            StatusOr(const U& val):status_(Status::StatusType::OK, ""), value_(val){
            }

            /// @brief Moves the value into internal variable.
            /// @details std::move is used to process the variable.
            /// @tparam U Expects U to be constructable from T.
            /// @param val Variable moved with std::move.
            template <typename U>
            requires std::constructible_from<T, U>
            StatusOr(U&& val):status_(Status::StatusType::OK, ""), value_(std::move(val)){
            }

            /// @brief Copy constructor, builds from given status
            /// @details Copies Status from val.
            /// @tparam U Expects U to be of class Status.
            /// @param val Status is copied from val.
            template <typename U>
            requires std::is_same_v<U, Status>
            StatusOr(const U& val):status_(val){
            }

            /// @brief Moves status from given status
            /// @details Moves Status from val.
            /// @tparam U Expects U to be of class Status.
            /// @param val Status is moved from val using std::move.
            template <typename U>
            requires std::is_same_v<U, Status>
            StatusOr(U&& val):status_(std::move(val)){
            }

            /// @brief Returns the value of the statusor as const reference.
            /// @details Callable on const lvalue. Status that is not ok, results in a std::logic_error throw. Check ok() before access!
            /// @return Const reference to the value of the statusor.
            /// @throw BadStatusOrAccess if the status is not OK and value is being accessed.
            const T& value() const &{
                if(ok()){
                    return value_;
                }
                throw BadStatusOrAccess("Tried to access value when status was not OK!");
            }

            /// @brief Returns the value of the statusor as const reference.
            /// @details Callable on lvalue. Status that is not ok, results in a std::logic_error throw. Check ok() before access!
            /// @return Const reference to the value of the statusor.
            /// @throw BadStatusOrAccess if the status is not OK and value is being accessed.
            T& value() &{
                if(ok()){
                    return value_;
                }
                throw BadStatusOrAccess("Tried to access value when status was not OK!");
            }

            /// @brief rvalue move return for taking ownership of the statusor value. Invalidates any further value access.
            /// @details callable on rvalue. uses the std::move(statusor).value() pattern to grant ownership.
            /// @return StatusOr value with ownership.
            /// @throw BadStatusOrAccess if the status is not OK and value is being accessed.
            T&& value() &&{
                if(ok()){
                    return std::move(value_);
                }
                throw BadStatusOrAccess("Tried to access value when status was not OK!");
            }

            /// @brief rvalue move return for taking ownership of the statusor value. Invalidates any further value access.
            /// @details callable on const rvalue.
            /// @return StatusOr value with ownership.
            /// @throw BadStatusOrAccess if the status is not OK and value is being accessed.
            const T&& value() const&&{
                if(ok()){
                    return std::move(value_);
                }
                throw BadStatusOrAccess("Tried to access value when status was not OK!");
            }

            /// @brief Checks wheter the status of the statusor is ok.
            /// @details Calls the ok function of the internal status.
            /// @return const bool value dependent on if the internal status is ok.
            const bool ok() const noexcept{
                return status_.ok();
            }
            
            /// @brief Returns the internal status.
            /// @details Const reference to the internal status object. 
            /// @return internal status object
            const Status& status() noexcept{
                return status_;
            }

        private:
            //Value and status
            T value_;
            Status status_; 
    };

}//namespace_mygbc


#endif
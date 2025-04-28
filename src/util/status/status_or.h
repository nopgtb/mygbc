#ifndef STATUS_OR_H
#define STATUS_OR_H

#include "status.h" //Status
#include "bad_status_or_access.h" //BadStatusOrAccess
#include <concepts> //std::constructible_from, std::is_same

namespace mygbc{

    /// @brief Templated StatusOr class. Enables returning of status or value.
    /// @tparam T Base type of the value.
    template <typename T>
    class StatusOr{
        public:
            /// @brief Default constructor.
            /// @details Builds status unkown.
            explicit StatusOr():status_(Status::StatusType::UNKOWN, ""), value_(){}

            /// @brief Copy constructor builds a status ok, statusor with value.
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
            StatusOr(const U& val):status_(val), value_(){
            }

            /// @brief Moves status from given status
            /// @details Moves Status from val.
            /// @tparam U Expects U to be of class Status.
            /// @param val Status is moved from val using std::move.
            template <typename U>
            requires std::is_same_v<U, Status>
            StatusOr(U&& val):status_(std::move(val)), value_(){
            }

            /// @brief Explicit constructor for status, build statusor with the given status.
            /// @details Explicit to avoid mistakes in return statements.
            /// @param status Status of the StatusOr.
            explicit StatusOr(const Status& status):status_(status), value_(){
            }

            /// @brief Explicit constructor for status, build statusor with the given status.
            /// @details Explicit to avoid mistakes in return statements.
            /// @param status Status of the StatusOr.
            explicit StatusOr(Status&& status):status_(std::move(status)), value_(){
            }

            /// @brief Returns the value of the statusor.
            /// @details Status that is not ok, results in a std::logic_error throw. Check ok() before access!
            /// @return Const reference to the value of the statusor.
            /// @throw BadStatusOrAccess if the status is not OK and value is being accessed.
            T value() const{
                if(ok()){
                    return value_;
                }
                throw BadStatusOrAccess("Tried to access value when status was not OK!");
            }

            /// @brief Returns the value of the statusor or given U.
            /// @details Status that is not ok, results in given U being returned. T must be able to be constructed from U.
            /// @tparam U Expects U to be constructable from T.
            /// @return value of the statusor.
            template <typename U>
            requires std::constructible_from<T, U>
            T value_or(U&& dval) const noexcept{
                if(ok()){
                    return value_;
                }
                return std::forward<U>(dval);
            }

            /// @brief Returns the value of the statusor or given U.
            /// @details Status that is not ok, results in given U being returned. T must be able to be constructed from U.
            /// @tparam U Expects U to be constructable from T.
            /// @return value of the statusor.
            template <typename U>
            requires std::constructible_from<T, U>
            T value_or(const U& dval) const noexcept{
                if(ok()){
                    return value_;
                }
                return dval;
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
            const Status& get_status() noexcept{
                return status_;
            }

        private:
            //Value and status
            T value_;
            Status status_; 
    };

}//namespace_mygbc


#endif
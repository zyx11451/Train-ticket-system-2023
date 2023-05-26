//
// Created by 123 on 2023/5/9.
//

#ifndef VECTOR_HPP_USERS_HPP
#define VECTOR_HPP_USERS_HPP
#include "myString.hpp"
#include "mapDatabase.hpp"
class User{
private:
    my_string<21> user_ID;
    my_string<31> password;
    my_string<16> name;
    my_string<31> mail_address;
    short privilege;
public:

};
#endif //VECTOR_HPP_USERS_HPP

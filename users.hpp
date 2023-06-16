//
// Created by 123 on 2023/5/9.
//

#ifndef VECTOR_HPP_USERS_HPP
#define VECTOR_HPP_USERS_HPP
#include "myString.hpp"
#include "mapDatabase.hpp"
#include <string>
class UsersInformation{
    //该类为维护用户信息的类。
public:
    struct User{
    public:
        struct key{
            my_string<21> user_ID;
            bool operator==(const key& other) const{
                return user_ID==other.user_ID;
            }
            bool operator<(const key& other) const{
                return user_ID<other.user_ID;
            }
            bool operator>(const key& other) const{
                return user_ID>other.user_ID;
            }
            bool operator>=(const key& other) const{
                return user_ID>=other.user_ID;
            }
            bool operator<=(const key& other) const{
                return user_ID<=other.user_ID;
            }
            bool operator!=(const key& other) const{
                return user_ID!=other.user_ID;
            }
            key()=default;
            key(std::string& b){
                user_ID=b;
            }
        };
    public:
        struct information{
            my_string<31> password;
            my_string<16> name;
            my_string<31> mail_address;
            int privilege;
        };
    };
    sjtu::MapDatabase<User::key,User::information> data;
public:
    UsersInformation(std::string a="users",std::string b="users_index"):data(a,b){};
    ~UsersInformation()=default;
    void insert(const User::key& key_, User::information& inf){
        data.insert(key_,inf);
    }
    User::information find(const User::key &k){
        return data.find(k);
    }
    bool exist(const User::key &k){
        return data.exist(k);
    }
    void modify(User::key &k,User::information &inf){
        data.change(k,inf);
    }
    bool empty(){
        return data.empty();
    }
    void clean(){
        data.clear();
    }
};
#endif //VECTOR_HPP_USERS_HPP

//
// Created by 123 on 2023/5/9.
//

#ifndef VECTOR_HPP_COMMAND_HPP
#define VECTOR_HPP_COMMAND_HPP
#include <string>
#include <cstring>
#include <iostream>
#include "users.hpp"
#include "trains.hpp"
class CommandScanner{
public:
    enum Mode{
        DivideBySpaces,DivideByOr
    };
    //一个简单的读入并分解指令的类
private:

    const char * buf= nullptr;
    const char *end= nullptr;
    Mode mode=DivideBySpaces;
public:
    void setInput(std::string &tar){
        buf=tar.c_str();
        end=buf+tar.length();
    }
    void changeMode(Mode m){
        mode=m;
    }
    void skipSpaces(){
        while(*buf==' ') ++buf;
    }
    void skipOr(){
        while(*buf=='|') ++buf;
    }
    bool hasMoreTokens(){
        return buf!=end;
    }
    std::string getToken(){
        std::string ans;
        if(mode==DivideBySpaces) {
            skipSpaces();
            while(hasMoreTokens()&&*buf!=' ') {
                ans+=*buf;
                ++buf;
            }
        }
        else if(mode==DivideByOr){
            skipSpaces();
            skipOr();
            while(hasMoreTokens()&&*buf!='|'&&*buf!=' ') {
                ans+=*buf;
                ++buf;
            }
        }
        return ans;
    }
    int getInt(){
        int x = 0, f = 1;
        while(*buf < '0' || *buf > '9')
        {
            if(*buf == '-') f = -1;
            ++buf;
        }
        while('0' <= *buf && *buf <= '9')
        {
            x = x * 10 + *buf - '0';
            ++buf;
        }
        if(*buf==']') ++buf;
        return x * f;
    }
    char getKey(){
        while(*buf != '-') ++buf;
        ++buf;
        return *buf;
    };
};
class Command{
private:
    int timeStamp;
    CommandScanner nowCommand;
    UsersInformation u;//用户信息
    sjtu::map<UsersInformation::User::key,int> log_in;//已登录用户和其privilege;
    void add_user(){
        UsersInformation::User::key new_k;
        UsersInformation::User::information new_inf;
        std::string user;
        std::string c;
        std::string p;
        std::string n;
        std::string m;
        int g;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k) {
                case 'c':
                    c=nowCommand.getToken();
                    break;
                case 'u':
                    user=nowCommand.getToken();
                    break;
                case 'p':
                    p=nowCommand.getToken();
                    break;
                case 'n':
                    n=nowCommand.getToken();
                    break;
                case 'm':
                    m=nowCommand.getToken();
                    break;
                case 'g':
                    g=nowCommand.getInt();
                    break;
                default:
                    break;
            }
        }
        if(u.empty()){
            g=10;
        }
        bool flag= false;
        auto it=log_in.find(c);
        if(it!=log_in.end()){
            if(it->second>g) flag= true;
        }
        if(u.exist(user)) flag= false;
        if(flag){
            message("0");
            new_k.user_ID=user;
            new_inf.name=n;
            new_inf.password=p;
            new_inf.mail_address=m;
            new_inf.privilege=g;
            u.insert(new_k,new_inf);
        }else{
            message("-1");
        }
    };
    void modify_user(){

    };
    void login(){
        std::string user;
        std::string p;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k) {
                case 'u':
                    user=nowCommand.getToken();
                    break;
                case 'p':
                    p=nowCommand.getToken();
                    break;
                default:
                    break;
            }
        }
        bool flag=true;
        auto it=log_in.find(user);
        if(it!=log_in.end()||!u.exist(user)) flag= false;
        if(flag){
            std::string true_p(u.find(user).password);
            if(strcmp(true_p.c_str(),p.c_str())!=0) flag= false;
        }
        if(flag){
            message("0");
            sjtu::pair<UsersInformation::User::key,int> new_pair(user,u.find(user).privilege);
            log_in.insert(new_pair);
        }else{
            message("1");
        }

    };
    void logout(){
        std::string user;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k) {
                case 'u':
                    user=nowCommand.getToken();
                    break;
                default:
                    break;
            }
        }
        bool flag=true;
        auto it=log_in.find(user);
        if(it==log_in.end()) flag= false;
        if(flag){
            message("0");
            log_in.erase(it);
        }else{
            message("-1");
        }
    };
    void query_profile(){
        std::string c;
        std::string user;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k) {
                case 'u':
                    user=nowCommand.getToken();
                    break;
                case 'c':
                    c=nowCommand.getToken();
                    break;
                default:
                    break;
            }
        }
        bool flag=true;
        auto it=log_in.find(c);
        if(it==log_in.end()) flag= false;
        if(!u.exist(user)) flag= false;
        if(flag){
            UsersInformation::User::information inf=u.find(user);
            if(it->second<=inf.privilege){
                if(c!=user) flag= false;
            }
            if(flag){
                std::string ans;
                ans+=user;
                ans+=' ';
                ans+=std::string(inf.name);
                ans+=' ';
                ans+=std::string(inf.mail_address);
                ans+=' ';
                ans+=char ('0'+inf.privilege);
                message(ans);
                return;
            }
        }
        message("-1");
    };
    void modify_profile(){
        std::string user;
        std::string c;
        std::string p;
        std::string n;
        std::string m;
        int g;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k) {
                case 'c':
                    c=nowCommand.getToken();
                    break;
                case 'u':
                    user=nowCommand.getToken();
                    break;
                case 'p':
                    p=nowCommand.getToken();
                    break;
                case 'n':
                    n=nowCommand.getToken();
                    break;
                case 'm':
                    m=nowCommand.getToken();
                    break;
                case 'g':
                    g=nowCommand.getInt();
                    break;
                default:
                    break;
            }
        }

    };
    void add_train();
    void delete_train();
    void release_train();
    void query_train();
    void query_ticket();
    void query_transfer();
    void query_order();
    void buy_ticket();
    void refund_ticket();
    void clean(){

    };
    void exit(){
        message("bye");
        std::exit(0);
    };
    void message(const std::string &ans) const{
        std::cout<<'['<<timeStamp<<']'<<' '<<ans<<'\n';
    }
public:
    Command(){
        timeStamp=0;
    }
    explicit Command(std::string &tar){
        nowCommand.setInput(tar);
        timeStamp=nowCommand.getInt();
    }
    void setCommand(std::string &tar){
        nowCommand.setInput(tar);
        timeStamp=nowCommand.getInt();
    }
    void execute(){
        std::string cmdType=nowCommand.getToken();
    }
};
#endif //VECTOR_HPP_COMMAND_HPP

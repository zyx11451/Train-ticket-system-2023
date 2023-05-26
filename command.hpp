//
// Created by 123 on 2023/5/9.
//

#ifndef VECTOR_HPP_COMMAND_HPP
#define VECTOR_HPP_COMMAND_HPP
#include <string>
#include <iostream>
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
        std::string ans="";
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
};
class Command{
private:
    int timeStamp;
    CommandScanner nowCommand;
    void add_user(){

    };
    void modify_user();
    void login();
    void logout();
    void query_profile();
    void modify_profile();
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
        std::cout<<"bye"<<'\n';
        std::exit(0);
    };
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

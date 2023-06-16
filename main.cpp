#include <iostream>
#include <string>
#include "command.hpp"
#include "bpt.hpp"
#include "mapDatabase.hpp"
#include "users.hpp"
#include <algorithm>//测试用
using namespace std;
int main() {
    Command c;
    bool clean_mode= false;//测试用
    if(!clean_mode){
        while(true){
            std::string input;
            getline(std::cin,input);
            c.setCommand(input);
            c.execute();
        }
    }else{
        c.clean();
    }
}

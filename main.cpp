#include <iostream>
#include <string>
#include "command.hpp"
#include "bpt.hpp"
#include "mapDatabase.hpp"
#include "users.hpp"
using namespace std;
int main() {
    //freopen("1.in","r",stdin);
    //freopen("a.out","w",stdout);
    Command c;
    bool clean_mode= false;//测试用
    if(!clean_mode){
        while(!c.exit_flag){
            std::string input;
            getline(std::cin,input);
            c.setCommand(input);
            c.execute();
        }
        return 0;
    }else{
        c.clean();
    }
}

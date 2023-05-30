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
#include "orders.hpp"
#include "vector.hpp"
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
    bool endOfToken(){
        return buf==end||*buf==' ';
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
    char getLetter(){
        char c;
        while(*buf<'A'||*buf>'Z') ++buf;
        c=*buf;
        ++buf;
        return c;
    }
    char nextChar(){
        char c=*(buf+1);
        return c;
    }
};
class Command{
private:
    int timeStamp;
    CommandScanner nowCommand;
    UsersInformation u;//用户信息
    TrainInformationSystem tr;
    OrderInformationSystem ord;
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
        bool flag= false;
        if(u.empty()){
            g=10;
            flag= true;
        }else{
            auto it=log_in.find(c);
            if(it!=log_in.end()){
                if(it->second>g) flag= true;
            }
            if(u.exist(user)) flag= false;
        }
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
            message("-1");
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
        int g=-1;
        bool p_changed= false;
        bool n_changed= false;
        bool m_changed= false;
        bool g_changed= false;
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
                    p_changed= true;
                    break;
                case 'n':
                    n=nowCommand.getToken();
                    n_changed=true;
                    break;
                case 'm':
                    m=nowCommand.getToken();
                    m_changed=true;
                    break;
                case 'g':
                    g=nowCommand.getInt();
                    g_changed=true;
                    break;
                default:
                    break;
            }
        }
        bool flag= true;
        auto it=log_in.find(c);
        if(it==log_in.end()) flag= false;
        if(!u.exist(user)) flag= false;
        if(flag){
            UsersInformation::User::information inf=u.find(user);
            if(it->second<=inf.privilege){
                if(c!=user) flag= false;
            }
            if(it->second<=g) flag= false;
            if(flag){
                if(p_changed) inf.password=p;
                if(n_changed) inf.name=n;
                if(m_changed) inf.mail_address=m;
                if(g_changed) inf.privilege=g;
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
    void add_train(){
        //突然发现add好像不用先设额外的string之类的变量
        TrainInformationSystem::key k_;
        TrainInformationSystem::train_basic_information inf;
        std::string ti;
        std::string begin_date;
        std::string end_date;
        int mon,date,hour,min;
        int i=0;
        int price_sum=0;
        int now_price;
        char ch;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k) {
                case 'i':
                    k_.train_ID=nowCommand.getToken();
                    break;
                case 'n':
                    inf.station_num=nowCommand.getInt();
                    break;
                case 'm':
                    inf.seat_num=nowCommand.getInt();
                    break;
                case 's':
                    nowCommand.changeMode(CommandScanner::DivideByOr);
                    i=1;
                    while(!nowCommand.endOfToken()){
                        inf.station_names[i]=nowCommand.getToken();
                        ++i;
                    }
                    nowCommand.changeMode(CommandScanner::DivideBySpaces);
                    break;
                case 'p':
                    nowCommand.changeMode(CommandScanner::DivideByOr);
                    i=1;
                    price_sum=0;
                    inf.total_prices[1]=0;
                    ++i;
                    while(!nowCommand.endOfToken()){
                      now_price=nowCommand.getInt();
                      price_sum+=now_price;
                      inf.total_prices[i]=price_sum;
                      ++i;
                    }
                    nowCommand.changeMode(CommandScanner::DivideBySpaces);
                    break;
                case 'x':
                    ti=nowCommand.getToken();
                    hour=(ti[0]-'0')*10+(ti[1]-'0');
                    min=(ti[3]-'0')*10+(ti[4]-'0');
                    inf.start_time=Time(hour,min);
                    break;
                case 't':
                    nowCommand.changeMode(CommandScanner::DivideByOr);
                    i=1;
                    while(!nowCommand.endOfToken()){
                        inf.travel_times[i]=nowCommand.getInt();
                        ++i;
                    }
                    nowCommand.changeMode(CommandScanner::DivideBySpaces);
                    break;
                case 'o':
                    nowCommand.changeMode(CommandScanner::DivideByOr);
                    i=1;
                    nowCommand.skipSpaces();
                    ch=nowCommand.nextChar();
                    if(ch!='_'){
                        while(!nowCommand.endOfToken()){
                            ++i;
                            inf.stop_over_times[i]=nowCommand.getInt();
                        }
                    }
                    nowCommand.changeMode(CommandScanner::DivideBySpaces);
                    break;
                case 'd':
                    nowCommand.changeMode(CommandScanner::DivideByOr);
                    begin_date=nowCommand.getToken();
                    end_date=nowCommand.getToken();
                    mon=(begin_date[0]-'0')*10+(begin_date[1]-'0');
                    date=(begin_date[3]-'0')*10+(begin_date[4]-'0');
                    inf.sale_date_begin=Calendar(mon,date);
                    mon=(end_date[0]-'0')*10+(end_date[1]-'0');
                    date=(end_date[3]-'0')*10+(end_date[4]-'0');
                    inf.sale_date_end=Calendar(mon,date);
                    nowCommand.changeMode(CommandScanner::DivideBySpaces);
                    break;
                case 'y':
                    inf.type=nowCommand.getLetter();
                    break;
                default:
                    break;
            }
        }
        if(tr.exist(k_)){
            message("-1");
        }else{
            tr.add(k_,inf);
            message("0");
        }
    };
    void delete_train(){
        TrainInformationSystem::key k_;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k){
                case 'i':
                    k_.train_ID=nowCommand.getToken();
                    break;
                default:
                    break;
            }
        }
        if(!tr.exist(k_)){
            message("-1");
        }else{
            if(tr.released(k_)) message("-1");
            else {
                tr.delete_train(k_);
                message("0");
            }
        }
    };
    void release_train(){
        TrainInformationSystem::key k_;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k){
                case 'i':
                    k_.train_ID=nowCommand.getToken();
                    break;
                default:
                    break;
            }
        }
        if(!tr.exist(k_)){
            message("-1");
        }else{
            if(tr.released(k_)) message("-1");
            else {
                tr.release_train(k_);

                message("0");
            }
        }
    };
    void query_train(){
        TrainInformationSystem::key k_;
        std::string da;
        int mon,d;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k){
                case 'i':
                    k_.train_ID=nowCommand.getToken();
                    break;
                case 'd':
                    da=nowCommand.getToken();
                    mon=(da[0]-'0')*10+(da[1]-'0');
                    d=(da[3]-'0')*10+(da[4]-'0');
                    break;
                default:
                    break;
            }
        }
        TrainInformationSystem::day_key dk_;
        dk_.train_ID=k_.train_ID;
        dk_.day=Calendar(mon,d);
        if(!tr.exist(k_)){
            message("-1");
            return;
        }
        TrainInformationSystem::train_basic_information tbi=tr.train_inf.find(k_);
        TrainInformationSystem::remained_seat rs=tr.remained_s.find(dk_);
        std::string ans;
        ans+=std::string(k_.train_ID);
        ans+=' ';
        ans+=tbi.type;
        message(ans);
        if(rs.station_num==0){
            //未release
            std::cout<<tbi.station_names[1]<<" xx-xx xx:xx -> "<<dk_.day<<' '<<tbi.start_time<<' '<<tbi.total_prices[1]<<' '<<tbi.seat_num<<'\n';
            Calendar ca=dk_.day;
            Time t=tbi.start_time;
            for(int i=2;i<=tbi.station_num-1;++i){
                t+=tbi.travel_times[i-1];
                ca+=t.show_day();
                t.clean_day();
                std::cout<<tbi.station_names[i]<<' '<<ca<<' '<<t<<" -> ";
                t+=tbi.stop_over_times[i];
                ca+=t.show_day();
                t.clean_day();
                std::cout<<ca<<' '<<t<<' '<<tbi.total_prices[i]<<' '<<tbi.seat_num<<'\n';
            }
            t+=tbi.travel_times[tbi.station_num-1];
            ca+=t.show_day();
            t.clean_day();
            std::cout<<tbi.station_names[tbi.station_num]<<' '<<ca<<' '<<t<<" -> xx-xx xx:xx "<<tbi.total_prices[tbi.station_num]<<" x\n";
        }else{
            std::cout<<tbi.station_names[1]<<" xx-xx xx:xx -> "<<dk_.day<<' '<<tbi.start_time<<' '<<tbi.total_prices[1]<<' '<<rs.remain[1]<<'\n';
            Calendar ca=dk_.day;
            Time t=tbi.start_time;
            for(int i=2;i<=tbi.station_num-1;++i){
                t+=tbi.travel_times[i-1];
                ca+=t.show_day();
                t.clean_day();
                std::cout<<tbi.station_names[i]<<' '<<ca<<' '<<t<<" -> ";
                t+=tbi.stop_over_times[i];
                ca+=t.show_day();
                t.clean_day();
                std::cout<<ca<<' '<<t<<' '<<tbi.total_prices[i]<<' '<<rs.remain[i]<<'\n';
            }
            t+=tbi.travel_times[tbi.station_num-1];
            ca+=t.show_day();
            t.clean_day();
            std::cout<<tbi.station_names[tbi.station_num]<<' '<<ca<<' '<<t<<" -> xx-xx xx:xx "<<tbi.total_prices[tbi.station_num]<<" x\n";
        }
    };
    void query_ticket(){
        TrainInformationSystem::station_key from;
        TrainInformationSystem::station_key to;
        std::string ca;
        std::string mode;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k){
                case 's':
                    from.station_ID=nowCommand.getToken();
                    break;
                case 'd':
                    ca=nowCommand.getToken();
                    break;
                case 't':
                    to.station_ID=nowCommand.getToken();
                    break;
                case 'p':
                    mode=nowCommand.getToken();
                    break;
                default:
                    break;
            }
        }
        Calendar c;
        c.month=(ca[0]-'0')*10+(ca[1]-'0');
        c.date=(ca[3]-'0')*10+(ca[4]-'0');
        c.date_to_dis();
        sjtu::vector<TrainInformationSystem::stop> f=tr.stops.dis(from);
        sjtu::vector<TrainInformationSystem::stop> t=tr.stops.dis(to);
        sjtu::vector<TrainInformationSystem::ticket> ans;
        auto i=f.begin();
        auto j=t.begin();
        while(i!=f.end()){
            while(j!=t.end()&&(*j)<(*i)) {
                ++j;
            }
            if(j==t.end()) break;
            if((*j)>(*i)){
                ++i;
                continue;
            }
            //现在i与j必定是同一辆车
            TrainInformationSystem::stop from_stop=*i;
            TrainInformationSystem::stop to_stop=*j;
            if(from_stop.station_cur>to_stop.station_cur){
                ++i;
                continue;
            }
            if(c<from_stop.begin_sale_date||c>from_stop.end_sale_date){
                ++i;
                continue;
            }
            //todo 接下来均为合理的
        }
    };
    void query_transfer(){

    };
    void query_order(){

    };
    void buy_ticket(){

    };
    void refund_ticket(){

    };
    void clean(){

    };
    void exit(){
        log_in.clear();
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

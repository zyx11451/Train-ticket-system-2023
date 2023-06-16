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
        const char *useless=end-1;
        while(*useless=='\r'||*useless=='\n'){
            --end;
            --useless;
        }
    }
    void changeMode(Mode m){
        mode=m;
        skipSpaces();
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
        return *(buf++);
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
                if(inf.privilege<10) ans+=char ('0'+inf.privilege);
                else ans+="10";
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
                UsersInformation::User::key us;
                us.user_ID=user;
                u.modify(us,inf);
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
        if(dk_.day<tbi.sale_date_begin||dk_.day>tbi.sale_date_end){
            message("-1");
            return;
        }
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
        std::string mode="cost";
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
                //车不能反着开
                ++i;
                continue;
            }
            if(c<from_stop.begin_sale_date+from_stop.departure_time.show_day()||c>from_stop.end_sale_date+from_stop.departure_time.show_day()){
                ++i;
                continue;
            }
            // 接下来均为合理的
            TrainInformationSystem::key k_;
            k_.train_ID=from_stop.train_ID;
            TrainInformationSystem::train_basic_information tbi=tr.train_inf.find(k_);
            TrainInformationSystem::ticket answer;
            answer.train_ID=k_.train_ID;
            answer.price=tbi.total_prices[to_stop.station_cur]-tbi.total_prices[from_stop.station_cur];
            answer.begin_time=from_stop.departure_time;
            answer.end_time=to_stop.arrival_time;
            Calendar st_c=c-answer.begin_time.show_day();
            TrainInformationSystem::day_key dk;
            dk.train_ID=k_.train_ID;
            dk.day=st_c;
            TrainInformationSystem::remained_seat rs=tr.remained_s.find(dk);
            answer.max_seat=rs.max_available(from_stop.station_cur,to_stop.station_cur);
            ans.push_back(answer);
            ++i;
        }
        if(mode=="cost") sjtu::sort(ans,0,ans.size()-1,TrainInformationSystem::ticket::cmp_cost);
        else sjtu::sort(ans,0,ans.size()-1,TrainInformationSystem::ticket::cmp_time);
        std::cout<<'['<<timeStamp<<']'<<' '<<ans.size()<<'\n';
        for(int ii=0;ii<ans.size();++ii){
            std::cout<<ans[ii].train_ID<<' '<<from.station_ID<<' '<<c<<' '<<std::string(ans[ii].begin_time)<<" -> "
            <<to.station_ID<<' '<<c+ans[ii].end_time.show_day()-ans[ii].begin_time.show_day()<<' '<<std::string (ans[ii].end_time)
            <<' '<<ans[ii].price<<' '<<ans[ii].max_seat<<'\n';
        }
    };
    void query_transfer(){
        TrainInformationSystem::station_key from;
        TrainInformationSystem::station_key to;
        std::string ca;
        std::string mode="cost";
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

        sjtu::map<TrainInformationSystem::station_key,Time> trans_s;
        //c+time.show_day就是具体日期
        sjtu::vector<TrainInformationSystem::stop> train1=tr.stops.dis(from);
        sjtu::vector<TrainInformationSystem::stop> train2=tr.stops.dis(to);
        bool successful= false;
        ConcreteTime start1,end1;
        ConcreteTime start2,end2;
        int total_cost,cost1,cost2;
        int total_time,now_total_time;
        int seat_num_1,seat_num_2;
        TrainInformationSystem::key train_a_1,now_train_a_1;
        TrainInformationSystem::key train_a_2,now_train_a_2;
        TrainInformationSystem::station_key transfer_station;
        for(auto it=train1.begin();it!=train1.end();++it){
            TrainInformationSystem::stop tmp=*it;
            if(c<tmp.begin_sale_date+tmp.departure_time.show_day()||c>tmp.end_sale_date+tmp.departure_time.show_day()) continue;
            TrainInformationSystem::key tk1;
            tk1.train_ID=tmp.train_ID;
            TrainInformationSystem::train_basic_information tbi1=tr.released_train_inf.find(tk1);
            Time now=tmp.departure_time;
            now.clean_day();
            for(int i=tmp.station_cur+1;i<=tbi1.station_num;++i){
                now+=tbi1.travel_times[i-1];
                sjtu::pair<TrainInformationSystem::station_key,Time> new_pair(tbi1.station_names[i],now);
                trans_s.insert(new_pair);
                now+=tbi1.stop_over_times[i];
            }
            for(auto it2=train2.begin();it2!=train2.end();++it2){
                TrainInformationSystem::key tk2;
                TrainInformationSystem::stop tmp2=*it2;
                tk2.train_ID=tmp2.train_ID;
                if(tk1==tk2) continue;
                TrainInformationSystem::train_basic_information tbi2=tr.released_train_inf.find(tk2);
                int station2=tmp2.station_cur;
                Time time_sum2=tmp2.arrival_time;
                int train2_time=0;
                for(int i=station2-1;i>=1;--i){
                    //求当前站的出发时间
                    if(i!=station2-1) time_sum2-=tbi2.stop_over_times[i+1];
                    time_sum2-=tbi2.travel_times[i];
                    if(i==station2-1) train2_time+=tbi2.travel_times[i];
                    else train2_time+=(tbi2.stop_over_times[i+1]+tbi2.travel_times[i]);
                    TrainInformationSystem::station_key now_station;
                    now_station.station_ID=tbi2.station_names[i];
                    if(trans_s.find(now_station)!=trans_s.end()){
                        Time arrive_time1=trans_s[now_station];
                        ConcreteTime c_arrive_time(c,arrive_time1);
                        ConcreteTime c_last_train2(tmp2.end_sale_date,time_sum2);
                        if(c_last_train2<c_arrive_time) continue;
                        ConcreteTime c_first_available_train2(tmp2.begin_sale_date,time_sum2);
                        if(c_first_available_train2<c_arrive_time){
                             c_first_available_train2.date=c_arrive_time.date;
                        }
                        if(c_first_available_train2<c_arrive_time){
                            ++c_first_available_train2.date;
                        }
                        int now_cost_1,now_cost_2;
                        ConcreteTime now_start_1,now_end_1;
                        ConcreteTime now_start_2,now_end_2;
                        int j;
                        for(j=tmp.station_cur+1;j<=tbi1.station_num;++j){
                            if(tbi1.station_names[j]==tbi2.station_names[i]){
                                now_cost_1=tbi1.total_prices[j]-tbi1.total_prices[tmp.station_cur];
                                break;
                            }
                        }
                        now_cost_2=tbi2.total_prices[station2]-tbi2.total_prices[i];
                        now_start_1= ConcreteTime(c-(tmp.departure_time.show_day()),tmp.departure_time);
                        now_end_1= ConcreteTime(c,arrive_time1);
                        now_start_2= c_first_available_train2;
                        now_end_2=ConcreteTime(c_first_available_train2.date,c_first_available_train2.t+train2_time);
                        now_total_time=now_end_2-now_start_1;
                        now_train_a_1.train_ID=tmp.train_ID;
                        now_train_a_2.train_ID=tmp2.train_ID;
                        if(!successful){
                            successful=true;
                            start1=now_start_1;
                            start2=now_start_2;
                            end1=now_end_1;
                            end2=now_end_2;
                            cost1=now_cost_1;
                            cost2=now_cost_2;
                            total_cost=cost1+cost2;
                            train_a_1.train_ID=tmp.train_ID;
                            train_a_2.train_ID=tmp2.train_ID;
                            total_time=now_total_time;
                            transfer_station.station_ID=now_station.station_ID;
                            TrainInformationSystem::day_key dk1;
                            dk1.day=c-(tmp.departure_time.show_day());
                            dk1.train_ID=train_a_1.train_ID;
                            TrainInformationSystem::remained_seat rs1=tr.remained_s.find(dk1);
                            seat_num_1=rs1.max_available(tmp.station_cur,j);
                            TrainInformationSystem::day_key dk2;
                            dk2.day=end2.date-(tmp2.departure_time.show_day());
                            dk2.train_ID=train_a_2.train_ID;
                            TrainInformationSystem::remained_seat rs2=tr.remained_s.find(dk2);
                            seat_num_2=rs2.max_available(i,station2);
                        }else{
                            if(mode=="cost"){
                                if(now_cost_1+now_cost_2> total_cost) continue;
                                if(now_cost_1+now_cost_2==total_cost) {
                                    if(now_total_time>total_time) continue;
                                    if(now_total_time==total_time){
                                        if(now_train_a_1>train_a_1) continue;
                                        if(now_train_a_1==train_a_1){
                                            if(now_train_a_2>train_a_2) continue;
                                        }
                                    }
                                }
                            }else{
                                if(now_total_time>total_time) continue;
                                if(now_total_time==total_time) {
                                    if(now_cost_1+now_cost_2> total_cost) continue;
                                    if(now_cost_1+now_cost_2==total_cost){
                                        if(now_train_a_1>train_a_1) continue;
                                        if(now_train_a_1==train_a_1){
                                            if(now_train_a_2>train_a_2) continue;
                                        }
                                    }
                                }
                            }
                            start1=now_start_1;
                            start2=now_start_2;
                            end1=now_end_1;
                            end2=now_end_2;
                            cost1=now_cost_1;
                            cost2=now_cost_2;
                            total_cost=cost1+cost2;
                            train_a_1=now_train_a_1;
                            train_a_2=now_train_a_2;
                            total_time=now_total_time;
                            transfer_station.station_ID=now_station.station_ID;
                            TrainInformationSystem::day_key dk1;
                            dk1.day=c-(tmp.departure_time.show_day());
                            dk1.train_ID=train_a_1.train_ID;
                            TrainInformationSystem::remained_seat rs1=tr.remained_s.find(dk1);
                            seat_num_1=rs1.max_available(tmp.station_cur,j);
                            TrainInformationSystem::day_key dk2;
                            dk2.day=end2.date-(tmp2.departure_time.show_day());
                            dk2.train_ID=train_a_2.train_ID;
                            TrainInformationSystem::remained_seat rs2=tr.remained_s.find(dk2);
                            seat_num_2=rs2.max_available(i,station2);
                        }
                    }
                }
            }
            trans_s.clear();
        }
        if(!successful) message("0");
        else{
            std::cout<<'['<<timeStamp<<']'<<' ';
            std::cout<<train_a_1.train_ID<<' '<<from.station_ID<<' '<<start1<<" -> "<<transfer_station.station_ID<<' '<<end1<<' '<<cost1<<' '<<seat_num_1<<'\n';
            std::cout<<train_a_2.train_ID<<' '<<transfer_station.station_ID<<' '<<start2<<" -> "<<to.station_ID<<' '<<end2<<' '<<cost2<<' '<<seat_num_2<<'\n';
        }
    };
    void buy_ticket(){
        std::string user;
        std::string i;
        std::string d;
        TrainInformationSystem::station_key from;
        TrainInformationSystem::station_key to;
        int n;
        std::string q="false";
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k) {
                case 'i':
                    i=nowCommand.getToken();
                    break;
                case 'u':
                    user=nowCommand.getToken();
                    break;
                case 'd':
                    d=nowCommand.getToken();
                    break;
                case 'f':
                    from.station_ID=nowCommand.getToken();
                    break;
                case 't':
                    to.station_ID=nowCommand.getToken();
                    break;
                case 'n':
                    n=nowCommand.getInt();
                    break;
                case 'q':
                    q=nowCommand.getToken();
                    break;
                default:
                    break;
            }
        }
        auto it=log_in.find(user);
        if(it==log_in.end()){
            message("-1");
            return;
        }
        if(!tr.released(i)){
            message("-1");
            return;
        }
        UsersInformation::User::key us;
        us.user_ID=user;
        Calendar c;
        c.month=(d[0]-'0')*10+(d[1]-'0');
        c.date=(d[3]-'0')*10+(d[4]-'0');
        c.date_to_dis();
        TrainInformationSystem::key tk;
        tk.train_ID=i;
        TrainInformationSystem::train_basic_information tbi=tr.released_train_inf.find(tk);
        if(tbi.seat_num<n){
            message("-1");
            return;
        }
        TrainInformationSystem::day_key dk;
        dk.train_ID=tk.train_ID;
        dk.day=c;
        int start=0,end=0,price_of_each=0,total_time=0,travel_t=0;
        TrainInformationSystem::remained_seat rs;
        for(int ii=1;ii<=tbi.station_num;++ii){
            if(tbi.station_names[ii]==from.station_ID){
                start=ii;
                dk.day-=(tbi.start_time+total_time).show_day();//是否跨天
                if(dk.day<tbi.sale_date_begin||dk.day>tbi.sale_date_end){
                    message("-1");
                    return;
                }
                rs=tr.remained_s.find(dk);
            }
            if(tbi.station_names[ii]==to.station_ID){
                end=ii;
                price_of_each=tbi.total_prices[end]-tbi.total_prices[start];
                travel_t-=tbi.stop_over_times[ii];
                break;
            }
            //下一站的出发时间
            if(start==0){
                total_time+=tbi.travel_times[ii];
                total_time+=tbi.stop_over_times[ii+1];
            }
            travel_t+=tbi.travel_times[ii];
            travel_t+=tbi.stop_over_times[ii+1];
        }
        if(price_of_each<=0) {
            message("-1");
            return;
        }
        int remain_s=rs.max_available(start,end);
        if(remain_s>=n){
            rs.modify(start,end-1,-n);
            tr.remained_s.change(dk,rs);
            OrderInformationSystem::order od;
            od.timestamp=timeStamp;
            od.user_ID=user;
            od.start_time=ConcreteTime(dk.day,tbi.start_time+total_time);
            od.end_time=ConcreteTime(dk.day,tbi.start_time+travel_t);
            od.train_ID=dk.train_ID;
            od.price=price_of_each;
            od.num=n;
            od.order_status=OrderInformationSystem::success;
            od.from_station=from.station_ID;
            od.to_station=to.station_ID;
            od.from_station_num=start;
            od.to_station_num=end;
            od.train_day=dk.day;
            ord.all_orders.insert(us,od);
            long long total_price=price_of_each;
            total_price*=n;
            std::cout<<'['<<timeStamp<<']'<<' '<<total_price<<'\n';
        }else{
            if(q=="false"){
                message("-1");
                return;
            }else{
                OrderInformationSystem::order od;
                od.timestamp=timeStamp;
                od.user_ID=user;
                od.start_time=ConcreteTime(dk.day,tbi.start_time+total_time);
                od.end_time=ConcreteTime(dk.day,tbi.start_time+travel_t);
                od.train_ID=dk.train_ID;
                od.price=price_of_each;
                od.num=n;
                od.order_status=OrderInformationSystem::pending;
                od.from_station=from.station_ID;
                od.to_station=to.station_ID;
                od.from_station_num=start;
                od.to_station_num=end;
                ord.all_orders.insert(us,od);
                ord.pending_orders.insert(dk,od);
                message("queue");
                return;
            }
        }

    };
    void refund_ticket(){
        UsersInformation::User::key user;
        int n=1;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k){
                case 'u':
                    user.user_ID=nowCommand.getToken();
                    break;
                case 'n':
                    n=nowCommand.getInt();
                default:
                    break;
            }
        }
        auto it=log_in.find(user);
        if(it==log_in.end()){
            message("-1");
            return;
        }
        sjtu::vector<OrderInformationSystem::order> all=ord.all_orders.dis(user);
        if(n>all.size()||all[n-1].order_status==OrderInformationSystem::refunded){
            message("-1");
            return;
        }
        OrderInformationSystem::order to_be_refunded=all[n-1];
        if(to_be_refunded.order_status==OrderInformationSystem::success){
            TrainInformationSystem::day_key dk;
            dk.train_ID=to_be_refunded.train_ID;
            dk.day=to_be_refunded.train_day;
            TrainInformationSystem::remained_seat rs=tr.remained_s.find(dk);
            rs.modify(to_be_refunded.from_station_num,to_be_refunded.to_station_num-1,to_be_refunded.num);
            sjtu::vector<OrderInformationSystem::order> waiting=ord.pending_orders.dis(dk);//从后往前检查。
            for(int i=waiting.size()-1;i>=0;--i){
                OrderInformationSystem::order now_waiting=waiting[i];
                int now_remain=rs.max_available(now_waiting.from_station_num,now_waiting.to_station_num);
                if(now_remain>=now_waiting.num){
                    rs.modify(now_waiting.from_station_num,now_waiting.to_station_num-1,-now_waiting.num);
                    ord.pending_orders.erase(dk,now_waiting);
                    UsersInformation::User::key nw_user;
                    nw_user.user_ID=now_waiting.user_ID;
                    ord.all_orders.erase(nw_user,now_waiting);
                    now_waiting.order_status=OrderInformationSystem::success;
                    ord.all_orders.insert(nw_user,now_waiting);
                    //此处可进一步优化(为bpt添加更改操作而非先删除再插入)

                }
            }
            tr.remained_s.change(dk,rs);
        }else{
            TrainInformationSystem::day_key dk;
            dk.train_ID=to_be_refunded.train_ID;
            dk.day=to_be_refunded.train_day;
            ord.pending_orders.erase(dk,to_be_refunded);
        }
        ord.all_orders.erase(user,to_be_refunded);
        to_be_refunded.order_status=OrderInformationSystem::refunded;
        ord.all_orders.insert(user,to_be_refunded);
        message("0");
    };
    void query_order(){
        UsersInformation::User::key user;
        while(nowCommand.hasMoreTokens()){
            char k=nowCommand.getKey();
            switch (k){
                case 'u':
                    user.user_ID=nowCommand.getToken();
                    break;
                default:
                    break;
            }
        }
        auto it=log_in.find(user);
        if(it==log_in.end()){
            message("-1");
            return;
        }
        sjtu::vector<OrderInformationSystem::order> all=ord.all_orders.dis(user);
        std::cout<<'['<<timeStamp<<']'<<' '<<all.size()<<'\n';
        for(int i=0;i<all.size();++i){
            OrderInformationSystem::order now_order=all[i];
            if(now_order.order_status==OrderInformationSystem::pending) std::cout<<"[pending] ";
            else if(now_order.order_status==OrderInformationSystem::success) std::cout<<"[success] ";
            else if(now_order.order_status==OrderInformationSystem::refunded) std::cout<<"[refunded] ";
            std::cout<<now_order.train_ID<<' '<<now_order.from_station<<' '<<now_order.start_time<<" -> "
            <<now_order.to_station<<' '<<now_order.end_time<<' '<<now_order.price<<' '<<now_order.num<<'\n';
        }
    };
public:
    bool exit_flag= false;
    void clean(){
        ord.clean();
        tr.clean();
        u.clean();
        message("0");
    };
    void s_exit(){
        log_in.clear();
        message("bye");
        exit_flag=true;
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
        if(cmdType=="add_user") add_user();
        else if(cmdType=="login") login();
        else if(cmdType=="logout") logout();
        else if(cmdType=="query_profile") query_profile();
        else if(cmdType=="modify_profile") modify_profile();
        else if(cmdType=="add_train") add_train();
        else if(cmdType=="delete_train") delete_train();
        else if(cmdType=="release_train") release_train();
        else if(cmdType=="query_train") query_train();
        else if(cmdType=="query_ticket") query_ticket();
        else if(cmdType=="query_transfer") query_transfer();
        else if(cmdType=="buy_ticket") buy_ticket();
        else if(cmdType=="query_order") query_order();
        else if(cmdType=="refund_ticket") refund_ticket();
        else if(cmdType=="clean") clean();
        else if(cmdType=="exit") s_exit();
    }
};
#endif //VECTOR_HPP_COMMAND_HPP

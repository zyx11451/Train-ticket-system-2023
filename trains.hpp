#ifndef TRAIN_HPP
#define TRAIN_HPP
#include "bpt.hpp"
#include "mapDatabase.hpp"
#include "myString.hpp"
#include <string>
class TrainInformationSystem{
public:
    struct key{
        my_string<21> train_ID;
        bool operator==(const key& other) const{
            return train_ID==other.train_ID;
        }
        bool operator<(const key& other) const{
            return train_ID<other.train_ID;
        }
        bool operator>(const key& other) const{
            return train_ID>other.train_ID;
        }
        bool operator>=(const key& other) const{
            return train_ID>=other.train_ID;
        }
        bool operator<=(const key& other) const{
            return train_ID<=other.train_ID;
        }
        bool operator!=(const key& other) const{
            return train_ID!=other.train_ID;
        }
        key()=default;
        key(std::string& b){
            train_ID=b;
        }
    };
public:
    struct train_basic_information{
        //用于火车信息的查询
        int station_num;
        my_string<31> station_names[101];
        int seat_num;
        int total_prices[101];//记录从始发站tp（1）=0到第i站的！总 ！价格
        Time start_time;
        int travel_times[101];
        int stop_over_times[101];
        Calendar sale_date_begin;
        Calendar sale_date_end;
        char type;
    };
public:
    struct stop{
        my_string<21> train_ID;
        Calendar begin_sale_date;
        Calendar end_sale_date;//方便查票
        Time arrival_time;
        Time departure_time;
        int station_cur;//快速判别站间次序、间隔。
        bool operator<(const stop& other) const{
            return train_ID<other.train_ID;
        }
        bool operator>(const stop& other) const{
            return train_ID>other.train_ID;
        }
        bool operator<=(const stop& other) const{
            return train_ID<other.train_ID;
        }
        bool operator>=(const stop& other) const{
            return train_ID>other.train_ID;
        }
        bool operator==(const stop& other) const{
            return train_ID==other.train_ID;
        }
        bool operator!=(const stop& other) const{
            return !(train_ID==other.train_ID);
        }
    };
public:
    struct day_key{
        my_string<21> train_ID;
        Calendar day;
        day_key()=default;
        day_key(std::string &a,int m,int d):train_ID(a),day(m,d){};
        day_key(const my_string<21> &a,const Calendar &c){
            train_ID=a;
            day=c;
        }
        bool operator<(const day_key& other) const{
            if(train_ID == other.train_ID) return day<other.day;
            return train_ID<other.train_ID;
        }
        bool operator>(const day_key& other) const{
            if(train_ID == other.train_ID) return day>other.day;
            return train_ID>other.train_ID;
        }
        bool operator<=(const day_key& other) const{
            if(train_ID == other.train_ID) return day<=other.day;
            return train_ID<other.train_ID;
        }
        bool operator>=(const day_key& other) const{
            if(train_ID == other.train_ID) return day>=other.day;
            return train_ID>other.train_ID;
        }
        bool operator==(const day_key& other) const{
            return train_ID==other.train_ID&&day==other.day;
        }
        bool operator!=(const day_key& other) const{
            return !(train_ID==other.train_ID&&day==other.day);
        }
    };
    struct station_key{
        my_string<31> station_ID;
        station_key()=default;
        station_key(std::string &a): station_ID(a){};
        station_key(my_string<31> &a) {
            station_ID=a;
        };
        bool operator<(const station_key& other) const{
            return station_ID<other.station_ID;
        }
        bool operator>(const station_key& other) const{
            return station_ID>other.station_ID;
        }
        bool operator<=(const station_key& other) const{
            return station_ID<other.station_ID;
        }
        bool operator>=(const station_key& other) const{
            return station_ID>other.station_ID;
        }
        bool operator==(const station_key& other) const{
            return station_ID==other.station_ID;
        }
        bool operator!=(const station_key& other) const{
            return !(station_ID==other.station_ID);
        }
    };
public:
    struct remained_seat{
        //若tle则考虑线段树
        int seat_num=0;
        int station_num=0;
        int remain[101];
        remained_seat()=default;
        remained_seat(int se_n,int sta_n):seat_num(se_n),station_num(sta_n){
            for(int i=1;i<=sta_n;++i){
                remain[i]=seat_num;
            }
        }
        void modify(int l,int r,int v){
            //算r
            for(int i=l;i<=r;++i){
                remain[i]+=v;
            }
        }
        int max_available(int l,int r){
            //第l站到第r站（因此不算r）
            int ans=remain[l];
            for(int i=l+1;i<r;++i){
                ans= std::min(ans,remain[i]);
            }
            return ans;
        }
    };
    struct ticket{
        my_string<21> train_ID;
        int price;
        int max_seat;
        Time begin_time;
        Time end_time;
        static bool cmp_cost(TrainInformationSystem::ticket &l, TrainInformationSystem::ticket &r){
            if(l.price!=r.price) return l.price<r.price;
            else return l.train_ID<r.train_ID;
        }
        static bool cmp_time(TrainInformationSystem::ticket &l, TrainInformationSystem::ticket &r){
            if((l.end_time-l.begin_time)!=(r.end_time-r.begin_time)) return (l.end_time-l.begin_time)<(r.end_time-r.begin_time);
            else return l.train_ID<r.train_ID;
        }
    };
    struct station_and_time{
        my_string<31> station_ID;
        Time t;
        bool operator<(const station_and_time& other) const{
            if(station_ID!=other.station_ID) return station_ID<other.station_ID;
            else return t<other.t;
        }
        bool operator<=(const station_and_time& other) const{
            if(station_ID!=other.station_ID) return station_ID<other.station_ID;
            else return t<=other.t;
        }
        bool operator>(const station_and_time& other) const{
            if(station_ID!=other.station_ID) return station_ID>other.station_ID;
            else return t>other.t;
        }
        bool operator>=(const station_and_time& other) const{
            if(station_ID!=other.station_ID) return station_ID>other.station_ID;
            else return t>=other.t;
        }
        bool operator==(const station_and_time& other) const{
            return station_ID==other.station_ID&&t==other.t;
        }
        bool operator!=(const station_and_time& other) const{
            return !(station_ID==other.station_ID&&t==other.t);
        }
    };
    struct transfer{
        my_string<21> train_ID_1;
        my_string<21> train_ID_2;
        int price;
        int max_seat;
        Calendar begin_date;
        Calendar end_date;
        Time begin;
        Time end;
    };
public:
    sjtu::MapDatabase<TrainInformationSystem::key,TrainInformationSystem::train_basic_information> train_inf;
    sjtu::MapDatabase<TrainInformationSystem::key,TrainInformationSystem::train_basic_information> released_train_inf;
    sjtu::MapDatabase<TrainInformationSystem::day_key,TrainInformationSystem::remained_seat> remained_s;
    //可能导致mle，若如此则换其它方式存，如一个key与value分离的BPT。
    sjtu::BPT<TrainInformationSystem::station_key,TrainInformationSystem::stop> stops;
    TrainInformationSystem(): train_inf("train_inf","train_inf_index"),
                              released_train_inf("released_train_inf","released_train_inf_index"),
                              stops("stops_node","stops_block"),
                              remained_s("remained_s_node","remained_s_block"){};
    void add(const TrainInformationSystem::key& k, train_basic_information &inf){
        //只需添加火车的相关信息
        train_inf.insert(k,inf);
    }
    bool exist(const TrainInformationSystem::key& k){
        return train_inf.exist(k);
    }
    bool released(const TrainInformationSystem::key& k){
        return released_train_inf.exist(k);
    }
    void release_train(const TrainInformationSystem::key& k){
        TrainInformationSystem::train_basic_information inf=train_inf.find(k);
        released_train_inf.insert(k,inf);
        TrainInformationSystem::remained_seat rs(inf.seat_num,inf.station_num);
        TrainInformationSystem::day_key dk(k.train_ID,inf.sale_date_begin);
        TrainInformationSystem::station_key sk(inf.station_names[1]);
        TrainInformationSystem::stop st;
        st.train_ID=k.train_ID;
        st.station_cur=1;
        st.begin_sale_date=inf.sale_date_begin;
        st.end_sale_date=inf.sale_date_end;
        st.arrival_time=inf.start_time;
        st.departure_time=inf.start_time;
        while (dk.day<=inf.sale_date_end){
            remained_s.insert(dk,rs);
            ++dk.day;
        }
        stops.insert(sk,st);
        for(int i=2;i<inf.station_num;++i){
            sk.station_ID=inf.station_names[i];
            st.station_cur=i;
            st.arrival_time=st.departure_time+inf.travel_times[i-1];
            st.departure_time=st.arrival_time+inf.stop_over_times[i];
            stops.insert(sk,st);
            //此处time的hour可以大于23;
        }
        ++st.station_cur;
        sk.station_ID=inf.station_names[inf.station_num];
        st.arrival_time=st.departure_time+inf.travel_times[inf.station_num-1];
        st.departure_time=st.arrival_time;
        stops.insert(sk,st);
    }
    void delete_train(const TrainInformationSystem::key& k){
        train_inf.erase(k);
    }
    void clean(){
        train_inf.clear();
        released_train_inf.clear();
        remained_s.clear();
        stops.clear_file();
    }
};
#endif //TRAIN_HPP



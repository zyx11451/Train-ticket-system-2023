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
        int prices[101];//记录从始发站（1）到第i站的！总 ！价格
        Time start_time;
        int travel_times[101];
        int stop_over_times[101];
        Calendar sale_date_begin;
        Calendar sale_date_end;
        char type;
    };
public:
    struct stop{
        my_string<31> station_name;
        my_string<21> train_ID;
        Calendar begin_sale_date;
        Calendar end_sale_date;
        Time arrival_time;
        Time departure_time;
        int station_num;//快速判别站间次序、间隔。
    };
public:
    struct day_key{
        my_string<21> train_ID;
        Calendar day;
        day_key()=default;
        day_key(std::string &a,int m,int d):train_ID(a),day(m,d){};
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
public:
    struct remained_seat{
        //若tle则考虑线段树
        int seat_num;
        int station_num;
        int remain[101];
        remained_seat()=default;
        remained_seat(int se_n,int sta_n):seat_num(se_n),station_num(sta_n){
            for(int i=1;i<=sta_n;++i){
                remain[i]=seat_num;
            }
        }
        void modify(int l,int r,int v){
            for(int i=l;i<=r;++i){
                remain[i]+=v;
            }
        }
    };
public:
    sjtu::MapDatabase<TrainInformationSystem::key,TrainInformationSystem::train_basic_information> train_inf;
    sjtu::MapDatabase<TrainInformationSystem::key,TrainInformationSystem::train_basic_information> released_train_inf;
    sjtu::BPT<TrainInformationSystem::day_key,TrainInformationSystem::stop> stops;
    sjtu::BPT<TrainInformationSystem::day_key,TrainInformationSystem::remained_seat> remained_s;
    TrainInformationSystem(): train_inf("train_inf","train_inf_index"),
                              released_train_inf("released_train_inf","released_train_inf_index"),
                              stops("stops_node","stops_block"),
                              remained_s("remained_s_node","remained_s_block"){}


};
#endif //TRAIN_HPP



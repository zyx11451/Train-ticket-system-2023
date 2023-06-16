//
// Created by 123 on 2023/5/30.
//

#ifndef COMMAND_HPP_ORDERS_HPP
#define COMMAND_HPP_ORDERS_HPP
#include "bpt.hpp"
#include "trains.hpp"
#include "users.hpp"
class OrderInformationSystem{
public:
    enum status{
        pending,success,refunded
    };
    struct order{
        status order_status;
        ConcreteTime start_time;
        ConcreteTime end_time;
        Calendar train_day;//列车从始发站出发时间
        my_string<31> from_station;
        my_string<31> to_station;
        my_string<21> train_ID;
        my_string<21> user_ID;
        int timestamp;
        int price;
        int num;
        int from_station_num;
        int to_station_num;
        //为了实现从新到旧所以反过来
        bool operator<(const order& other) const{
            return timestamp>other.timestamp;
        }
        bool operator>(const order& other) const{
            return timestamp<other.timestamp;
        }
        bool operator<=(const order& other) const{
            return timestamp>=other.timestamp;
        }
        bool operator>=(const order& other) const{
            return timestamp<=other.timestamp;
        }
        bool operator==(const order& other) const{
            return timestamp==other.timestamp;
        }
        bool operator!=(const order& other) const{
            return timestamp!=other.timestamp;
        }
    };
public:
    sjtu::BPT<TrainInformationSystem::day_key,OrderInformationSystem::order> pending_orders;
    sjtu::BPT<UsersInformation::User::key,OrderInformationSystem::order> all_orders;
    OrderInformationSystem(): pending_orders("pending_nodes","pending_blocks"), all_orders("all_orders_nodes","all_orders_blocks"){};
    void clean(){
        pending_orders.clear_file();
        all_orders.clear_file();
    }
};
#endif //COMMAND_HPP_ORDERS_HPP

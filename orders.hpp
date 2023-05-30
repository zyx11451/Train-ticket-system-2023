//
// Created by 123 on 2023/5/30.
//

#ifndef COMMAND_HPP_ORDERS_HPP
#define COMMAND_HPP_ORDERS_HPP
#include "bpt.hpp"
#include "trains.hpp"
class OrderInformationSystem{
public:
    enum status{
        pending,success,refunded
    };
    struct order{
        status order_status;
        ConcreteTime start_time;
        ConcreteTime end_time;
        my_string<31> from_station;
        my_string<31> to_station;
        my_string<21> train_ID;
        int timestamp;
        int price;
        int num;
        int from_station_num;
        int to_station_num;
    };
public:
    sjtu::BPT<TrainInformationSystem::day_key,OrderInformationSystem::order> pending_orders;
    sjtu::BPT<TrainInformationSystem::day_key,OrderInformationSystem::order> all_orders;
};
#endif //COMMAND_HPP_ORDERS_HPP

#include "bpt.hpp"
#include "mapDatabase.hpp"
#include "myString.hpp"
struct train{
    my_string<21> train_ID;
    int station_num;
    my_string<31> station_names[101];
    int seat_num;
    int prices[101];
    Time start_time;
    int travel_times[101];
    int stop_over_times[101];
    Calendar sale_date_begin;
    Calendar sale_date_end;
    char type;
};
struct stop{

};


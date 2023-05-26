//
// Created by 123 on 2023/5/9.
//
//包含my_string,calendar(月-日),time(时-分)结构。
#ifndef VECTOR_HPP_MYSTRING_HPP
#define VECTOR_HPP_MYSTRING_HPP
#include <string>
#include <iostream>
template<int length>
class my_string{
private:
    char c[length];
public:
    my_string(){
        c[0]='\0';
    }

    my_string(const std::string &tar){
        strcpy(c,tar.c_str());
    }

    my_string(const my_string& other){
        strcpy(c,other.c);
    }

    ~my_string()=default;

    explicit operator std::string (){
        std::string ans(strlen(c),c[0]);
        for(int i=0;i< strlen(c);++i){
            ans[i]=c[i];
        }
        return ans;
    }

    explicit operator char*(){
        return c;
    }

    my_string& operator=(const my_string& other){
        if(&other== this) return *this;
        strcpy(c,other.c);
        return *this;
    }
    bool operator>(const my_string &b) const{
        return (strcmp(c,b.c)>0);
    }

    bool operator<(const my_string &b) const{
        return (strcmp(c,b.c)<0);
    }

    bool operator==(const my_string &b) const{
        return (strcmp(c,b.c)==0);
    }

    bool operator!=(const my_string &b) const{
        return (strcmp(c,b.c)!=0);
    }
    bool operator>=(const my_string &b) const{
        return (strcmp(c,b.c)>=0);
    }

    bool operator<=(const my_string &b) const{
        return (strcmp(c,b.c)<=0);
    }
};

class Calendar{
    int date;
    int month;
    //int year;
    //int day;
    int dis;
    int number_of_day[13];
public:
    void date_to_dis(){
        for(int i=1;i<=month-1;i++){
            dis+=number_of_day[i];
        }
        dis+=date-1;
    }
    void dis_to_day(){
        int temp;
        temp=dis;
        int month_count=1;
        while(temp>=number_of_day[month_count]){
            temp-=number_of_day[month_count];
            month_count++;
        }
        month=month_count;
        date=1+temp;
    }
    Calendar(){
        date=1;
        month=1;
        dis=0;
        number_of_day[1]=31;
        number_of_day[2]=28;
        number_of_day[3]=31;
        number_of_day[4]=30;
        number_of_day[5]=31;
        number_of_day[6]=30;
        number_of_day[7]=31;
        number_of_day[8]=31;
        number_of_day[9]=30;
        number_of_day[10]=31;
        number_of_day[11]=30;
        number_of_day[12]=31;
    }
    Calendar( int m, int d){
        month=m;
        date=d;
        number_of_day[1]=31;
        number_of_day[2]=28;
        number_of_day[3]=31;
        number_of_day[4]=30;
        number_of_day[5]=31;
        number_of_day[6]=30;
        number_of_day[7]=31;
        number_of_day[8]=31;
        number_of_day[9]=30;
        number_of_day[10]=31;
        number_of_day[11]=30;
        number_of_day[12]=31;
        date_to_dis();
    }
    bool operator==(const Calendar &a){
        if(dis==a.dis){
            return  true;
        }else{
            return false;
        }
    }
    bool operator>(const Calendar &a){
        if(dis>a.dis){
            return true;
        }else{
            return false;
        }
    }
    bool operator<(const Calendar &a){
        if(dis<a.dis){
            return true;
        } else{
            return false;
        }
    }
    bool operator>=(const Calendar &a){
        if(dis>=a.dis){
            return true;
        }else{
            return false;
        }
    }
    bool operator<=(const Calendar &a){
        if(dis<=a.dis){
            return  true;
        }else{
            return false;
        }
    }
    bool operator!=(const Calendar &a){
        if(dis!=a.dis){
            return true;
        }else{
            return false;
        }
    }
    Calendar &operator+=(int a){
        dis+=a;
        dis_to_day();
        return *this;
    }
    Calendar &operator-=(int a){
        dis-=a;
        dis_to_day();
        return *this;
    }
    Calendar operator+(int a){
        Calendar ans;
        ans.dis=dis+a;
        ans.dis_to_day();
        return ans;
    }
    Calendar operator-(int a){
        Calendar ans;
        ans.dis=dis-a;
        ans.dis_to_day();
        return ans;
    }
    int operator-(const Calendar &a){
        return dis-a.dis;
    }
    Calendar operator++(){
        dis++;
        dis_to_day();
        return *this;
    }
    Calendar operator++(int){
        Calendar ans;
        ans.dis=dis;
        ans.dis_to_day();
        dis++;
        dis_to_day();
        return ans;
    }
    Calendar operator--(){
        dis--;
        dis_to_day();
        return *this;
    }
    Calendar operator--(int){
        Calendar ans;
        ans.dis=dis;
        ans.dis_to_day();
        dis--;
        dis_to_day();
        return ans;
    }
    Calendar &operator=(const Calendar &a){
        if(&a== this) return *this;
        dis=a.dis;
        month=a.month;
        date=a.date;
        return *this;
    }
    operator std::string()const{
        std::string a="00-00";
        a[1]=char(month%10+'0');
        a[0]=char(month/10+'0');
        a[4]=char(date%10+'0');
        a[3]=char(date/10+'0');
        return a;
    }
    friend std::ostream &operator <<(std::ostream &out,const Calendar&a){
        out<<std::string(a);
        return out;
    };
};

class Time{
private:
    int total_minutes;
public:
    Time(){
        total_minutes=0;
    }
    Time(int h,int m){
        total_minutes=h*60+m;
    }
    Time& operator+(int min){
        total_minutes+=min;
        return *this;
    }
    int operator-(const Time &other){
        return total_minutes-other.total_minutes;
    }
    Time& operator=(const Time &other){
        if(&other== this) return *this;
        total_minutes=other.total_minutes;
        return *this;
    }
    int show_day(){
        return total_minutes/1440;
    }
    int show_hour(){
        return total_minutes/60;
    }
    int show_minute(){
        return total_minutes%60;
    }

};
#endif //VECTOR_HPP_MYSTRING_HPP

//
// Created by 123 on 2023/5/9.
//
//包含my_string,calendar(月-日),time(时-分)结构。
#ifndef VECTOR_HPP_MYSTRING_HPP
#define VECTOR_HPP_MYSTRING_HPP
#include <string>
#include <iostream>
#include <cstring>
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

    explicit operator std::string () const{
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
    friend std::ostream &operator <<(std::ostream &out,const my_string&a){
        out<<std::string(a);
        return out;
    };
};

class Calendar{
public:
    int date;
    int month;
    int dis;
    static const int number_of_day[13];

    void date_to_dis(){
        dis=0;
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
    }
    Calendar( int m, int d){
        month=m;
        date=d;
        date_to_dis();
    }
    bool operator==(const Calendar &a) const{
        if(dis==a.dis){
            return  true;
        }else{
            return false;
        }
    }
    bool operator>(const Calendar &a) const{
        if(dis>a.dis){
            return true;
        }else{
            return false;
        }
    }
    bool operator<(const Calendar &a) const{
        if(dis<a.dis){
            return true;
        } else{
            return false;
        }
    }
    bool operator>=(const Calendar &a) const{
        if(dis>=a.dis){
            return true;
        }else{
            return false;
        }
    }
    bool operator<=(const Calendar &a) const{
        if(dis<=a.dis){
            return  true;
        }else{
            return false;
        }
    }
    bool operator!=(const Calendar &a) const{
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
    Calendar operator+(int a) const{
        Calendar ans;
        ans.dis=dis+a;
        ans.dis_to_day();
        return ans;
    }
    Calendar operator-(int a) const{
        Calendar ans;
        ans.dis=dis-a;
        ans.dis_to_day();
        return ans;
    }
    int operator-(const Calendar &a) const{
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
const int Calendar::number_of_day[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};;
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
    Time& operator+=(int min){
        total_minutes+=min;
        return *this;
    }
    Time& operator-=(int min){
        total_minutes-=min;
        return *this;
    }
    Time operator+(const int min) const{
        Time a;
        a.total_minutes=total_minutes+min;
        return a;
    }
    int operator-(const Time &other) const{
        return total_minutes-other.total_minutes;
    }
    Time& operator=(const Time &other){
        if(&other== this) return *this;
        total_minutes=other.total_minutes;
        return *this;
    }
    bool operator<(const Time &other) const{
        return total_minutes<other.total_minutes;
    }
    bool operator<=(const Time &other) const{
        return total_minutes<=other.total_minutes;
    }
    bool operator>(const Time &other) const{
        return total_minutes>other.total_minutes;
    }
    bool operator>=(const Time &other) const{
        return total_minutes>=other.total_minutes;
    }
    bool operator==(const Time &other) const{
        return total_minutes==other.total_minutes;
    }
    bool operator!=(const Time &other) const{
        return total_minutes!=other.total_minutes;
    }
    int show_day() const{
        return total_minutes/1440;
    }
    int show_hour() const{
        return (total_minutes%1440)/60;
    }
    int show_minute() const{
        return total_minutes%60;
    }
    void clean_day(){
        total_minutes%=1440;
    }
    operator std::string()const{
        std::string a="00:00";
        int h=show_hour();
        int m=show_minute();
        a[1]=char(h%10+'0');
        a[0]=char(h/10+'0');
        a[4]=char(m%10+'0');
        a[3]=char(m/10+'0');
        return a;
    }
    friend std::ostream &operator <<(std::ostream &out,const Time&a){
        out<<std::string(a);
        return out;
    };
};
class ConcreteTime{
public:
    Calendar date;
    Time t;
public:
    ConcreteTime()=default;
    ConcreteTime(Calendar c,Time ti){
        date=c+ti.show_day();
        ti.clean_day();
        t=ti;
    }
    ConcreteTime(int m,int d,int h,int min): date(m,d),t(h,min){};
    ConcreteTime& operator+=(int min){
        t+=min;
        if(t.show_day()>=1){
            date+=t.show_day();
            t.clean_day();
        }
    }
    bool operator<(const ConcreteTime& other) const{
        if(date!=other.date) return date<other.date;
        else return t<other.t;
    }
    bool operator>(const ConcreteTime& other) const{
        if(date!=other.date) return date>other.date;
        else return t>other.t;
    }
    bool operator<=(const ConcreteTime& other) const{
        if(date!=other.date) return date<other.date;
        else return t<=other.t;
    }
    bool operator>=(const ConcreteTime& other) const{
        if(date!=other.date) return date>other.date;
        else return t>=other.t;
    }
    bool operator==(const ConcreteTime& other) const{
        return date==other.date&&t==other.t;
    }
    bool operator!=(const ConcreteTime& other) const{
        return !(date==other.date&&t==other.t);
    }
    int operator-(const ConcreteTime& other) const{
        int day1=date.dis;
        int day2=other.date.dis;
        return (day2-day1)*1440+(t-other.t);
    }
    friend std::ostream &operator <<(std::ostream &out,const ConcreteTime&a){
        out<<a.date<<' '<<a.t;
        return out;
    };
};
#endif //VECTOR_HPP_MYSTRING_HPP

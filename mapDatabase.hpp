//
// Created by 123 on 2023/5/25.
//
//一个靠map在内存里存储索引的类，用于user和train的存储
#include "map.hpp"
#include "vector.hpp"
#include <fstream>
#ifndef BPT_HPP_MAPDATABASE_HPP
#define BPT_HPP_MAPDATABASE_HPP
namespace sjtu{
    template<class T>
    void sort(sjtu::vector<T> &tar,int begin,int end,bool(*cmp)(T&,T&)){
        //快速排序
        //end是最后一个元素位置
        if(end<=begin) return;
        int i=begin;
        int j=end;
        int e=begin+((end-begin+1)>>1);
        T divide=tar[e];
        while(true){
            while(cmp(tar[i],divide)) ++i;
            while(cmp(divide,tar[j])) --j;
            if(i>=j) break;
            std::swap(tar[i],tar[j]);
        }
        sort(tar,begin,j-1,cmp);
        sort(tar,j+1,end,cmp);
    }
    template<class key,class information>
    class MapDatabase{
    private:
        map<key,int> index;
        std::string file_name;
        std::fstream file;
        std::string index_file_name;
        std::fstream index_file;
        int len;
        int amount;
    public:
        void read_index(){
            index_file.seekg(0);
            index_file.read(reinterpret_cast<char *>(&len), sizeof(int));
            index_file.seekg(sizeof(int));
            index_file.read(reinterpret_cast<char *>(&amount), sizeof(int));
            for(int i=0;i<len;++i){
                pair<const key,int> p;
                index_file.seekg(2*sizeof(int)+i* sizeof(pair<const key,int>));
                index_file.read(reinterpret_cast<char *>(&p), sizeof(pair<const key,int>));
                index.insert(p);
            }
        }
        void write_index(){
            index_file.seekp(0);
            index_file.write(reinterpret_cast<char *>(&len), sizeof(int));
            index_file.seekp(sizeof(int));
            index_file.write(reinterpret_cast<char *>(&amount), sizeof(int));
            int i=0;
            for(auto it=index.begin();it!=index.end();++it){
                pair<const key,int> p(it->first,it->second);
                index_file.seekp(2*sizeof(int)+i* sizeof(pair<const key,int>));
                index_file.write(reinterpret_cast<char *>(&p), sizeof(pair<const key,int>));
                ++i;
            }
        }
        void write_inf(int loc, information &inf){
            file.seekp((loc-1)* sizeof(information));
            file.write(reinterpret_cast<char *>(&inf), sizeof(information));
        }
        void read_inf(int loc,information &inf){
            file.seekg((loc-1)* sizeof(information));
            file.read(reinterpret_cast<char *>(&inf), sizeof(information));
        }
        MapDatabase(std::string name,std::string index_name){
            file_name=name;
            index_file_name=index_name;
            file.open(file_name);
            if(!file){
                file.open(file_name,std::ios::out);
                file.close();
                file.open(file_name);
                index_file.open(index_file_name,std::ios::out);
                index_file.close();
                index_file.open(index_file_name);
                len=0;
                amount=0;
                write_index();
            }else{
                index_file.open(index_file_name);
                read_index();
            }
            //std::cout<<"successful"<<'\n';
        }
        ~MapDatabase(){
            write_index();
            //std::cout<<"successful"<<'\n';
        }
        void insert(const key &k, information &inf){
            ++len;
            ++amount;
            pair<const key,int> p(k,amount);
            index.insert(p);
            write_inf(amount,inf);
        }
        void erase(const key k){
            --len;
            auto it=index.find(k);
            if(it!=index.end()) index.erase(it);
        }
        bool exist(const key k){
            auto it=index.find(k);
            if(it!=index.end()){
                return true;
            }
            return false;
        }
        information find(const key k){
            auto it=index.find(k);
            if(it!=index.end()){
                int i=it->second;
                information ans;
                read_inf(i,ans);
                return ans;
            }
            return information();
        }
        void change( const key k, information inf){
            auto it=index.find(k);
            if(it==index.end()) return;
            write_inf(it->second,inf);
        }
        void clear(){
            len=0;
            amount=0;
            remove(file_name.c_str());
            remove(index_file_name.c_str());
        }
        bool empty(){
            return len==0;
        }
    };
}
#endif //BPT_HPP_MAPDATABASE_HPP

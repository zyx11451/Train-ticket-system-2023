//
// Created by 123 on 2023/5/9.
//

#ifndef ZYX_TRAIN_TICKET_SYSTEM_VECTOR_HPP
#define ZYX_TRAIN_TICKET_SYSTEM_VECTOR_HPP

#include "exceptions.hpp"
#include <climits>
#include <cstddef>
namespace sjtu
{
    template<typename T>
    class vector
    {
    private:
        T* data;
        int len;
        int capacity;
        void doubleSpace(){
            capacity*=2;
            //加1是为了留出end（），下同
            T* temp=static_cast<T*>(malloc((capacity+1)* sizeof(T)));
            for(int i=0;i<len;++i){
                new(temp+i) T(*(data+i));
                (data+i)->~T();
            }
            free(data);
            data=temp;
        };
    public:
        class const_iterator;
        class iterator
        {
            // The following code is written for the C++ type_traits library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly. In particular, without the following code,
            // @code{std::sort(iter, iter1);} would not compile.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::output_iterator_tag;

        private:
            vector<T>* belongs_to_which_vector;

        public:
            T* pt;
            iterator(T* pt_,vector<T>* x){
                pt=pt_;
                belongs_to_which_vector=x;
            }
            iterator operator+(const int &n) const
            {
                iterator ans(pt+n,belongs_to_which_vector);
                return ans;
            }
            iterator operator-(const int &n) const
            {
                iterator ans(pt-n,belongs_to_which_vector);
                return ans;
            }
            int operator-(const iterator &rhs) const
            {
                if(rhs.belongs_to_which_vector!=belongs_to_which_vector) throw invalid_iterator();
                return pt-rhs.pt;
            }
            iterator& operator+=(const int &n)
            {
                pt+=n;
                return *this;
            }
            iterator& operator-=(const int &n)
            {
                pt-=n;
                return *this;
            }
            iterator operator++(int) {
                iterator temp(pt,belongs_to_which_vector);
                pt++;
                return temp;
            }
            iterator& operator++() {
                pt++;
                return *this;
            }
            iterator operator--(int) {
                iterator temp(pt,belongs_to_which_vector);
                pt--;
                return temp;
            }
            iterator& operator--() {
                pt--;
                return *this;
            }
            T& operator*() const{
                return *pt;
            }
            bool operator==(const iterator &rhs) const {
                return (pt==rhs.pt)&&(belongs_to_which_vector==rhs.belongs_to_which_vector);
            }
            bool operator==(const const_iterator &rhs) const {
                return (pt==rhs.pt)&&(belongs_to_which_vector==rhs.belongs_to_which_vector);
            }
            bool operator!=(const iterator &rhs) const {
                return !((pt==rhs.pt)&&(belongs_to_which_vector==rhs.belongs_to_which_vector));
            }
            bool operator!=(const const_iterator &rhs) const {
                return !((pt==rhs.pt)&&(belongs_to_which_vector==rhs.belongs_to_which_vector));
            }
        };
        class const_iterator
        {
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::output_iterator_tag;

        private:

        public:
            const T*  pt;
            const vector<T>* belongs_to_which_vector;

            const_iterator operator+(const int &n) const
            {

                const_iterator ans;
                ans.pt=pt+n;
                ans.belongs_to_which_vector=belongs_to_which_vector;
                return ans;
            }
            const_iterator operator-(const int &n) const
            {

                const_iterator ans;
                ans.pt=pt-n;
                ans.belongs_to_which_vector=belongs_to_which_vector;
                return ans;
            }
            int operator-(const const_iterator &rhs) const
            {

                if(rhs.belongs_to_which_vector!=belongs_to_which_vector) throw invalid_iterator();
                return pt-rhs.pt;
            }
            const_iterator& operator+=(const int &n)
            {

                pt+=n;
                return *this;
            }
            const_iterator& operator-=(const int &n)
            {

                pt-=n;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator temp;
                temp.pt=pt;
                temp.belongs_to_which_vector=belongs_to_which_vector;
                pt++;
                return temp;
            }

            const_iterator& operator++() {
                pt++;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator temp;
                temp.pt=pt;
                temp.belongs_to_which_vector=belongs_to_which_vector;
                return temp;
            }

            const_iterator& operator--() {
                pt--;
                return *this;
            }

            const T& operator*() const{
                return *pt;
            }

            bool operator==(const iterator &rhs) const {
                return (pt==rhs.pt)&&(belongs_to_which_vector==rhs.belongs_to_which_vector);
            }
            bool operator==(const const_iterator &rhs) const {
                return (pt==rhs.pt)&&(belongs_to_which_vector==rhs.belongs_to_which_vector);
            }

            bool operator!=(const iterator &rhs) const {
                return !((pt==rhs.pt)&&(belongs_to_which_vector==rhs.belongs_to_which_vector));
            }
            bool operator!=(const const_iterator &rhs) const {
                return !((pt==rhs.pt)&&(belongs_to_which_vector==rhs.belongs_to_which_vector));
            }
        };

        vector() {
            capacity=1;
            len=0;
            data=static_cast<T *>(malloc(2*sizeof(T)));
        }
        vector(const vector &other) {
            capacity=other.capacity;
            len=other.len;
            data=static_cast<T *>(malloc((capacity+1)*sizeof(T)));
            for(int i=0;i<len;++i){
                new(data+i) T(*(other.data+i));
            }
        }

        ~vector() {
            for(int i=0;i<len;++i){
                (data+i)->~T();
            }
            free(data);
        }

        vector &operator=(const vector &other) {
            if(other.data==data) return *this;
            free(data);
            capacity=other.capacity;
            len=other.len;

            data=static_cast<T *>(malloc((capacity+1)*sizeof(T)));
            for(int i=0;i<len;++i){
                new(data+i) T(*(other.data+i));
            }
            return *this;
        }

        T & at(const size_t &pos) {
            if(pos<0||pos>=len) throw index_out_of_bound();
            return *(data+pos);
        }
        const T & at(const size_t &pos) const {
            if(pos<0||pos>=len) throw index_out_of_bound();
            return *(data+pos);
        }

        T & operator[](const size_t &pos) {
            if(pos<0||pos>=len) throw index_out_of_bound();
            return *(data+pos);
        }
        const T & operator[](const size_t &pos) const {
            if(pos<0||pos>=len) throw index_out_of_bound();
            return *(data+pos);
        }

        const T & front() const {
            if(len==0) throw container_is_empty();
            return *data;
        }

        const T & back() const {
            if(len==0) throw container_is_empty();
            return *(data+len-1);
        }

        iterator begin() {
            iterator ans(data, this);
            return ans;
        }
        const_iterator cbegin() const {
            const_iterator ans;
            ans.pt=data;
            ans.belongs_to_which_vector= this;
            return ans;
        }

        iterator end() {
            iterator ans(data+len, this);
            return ans;
        }
        const_iterator cend() const {
            const_iterator ans;
            ans.pt=data+len;
            ans.belongs_to_which_vector= this;
            return ans;
        }

        bool empty() const {
            return len==0;
        }

        size_t size() const {
            return len;
        }

        void clear() {
            for(int i=0;i<len;++i){
                (data+i)->~T();
            }
            free(data);
            capacity=1;
            len=0;
            data=static_cast<T *>(malloc(2*sizeof(T)));

        }

        iterator insert(iterator pos, const T &value) {
            if(capacity==len) doubleSpace();
            T* temp=data+len-1;
            T* before_temp=data+len-2;
            T* target=pos.pt;

            this->push_back(*temp);
            while(temp!=target&&temp!=data){
                *temp=*before_temp;
                temp--;
                before_temp--;
            }
            *temp=value;
            return iterator(temp, this);
        }

        iterator insert(const size_t &ind, const T &value) {
            if(ind>len) throw index_out_of_bound();
            if(capacity==len) doubleSpace();
            T* temp=data+len-1;
            T* before_temp=data+len-2;
            T* target=data+ind;
            this->push_back(*temp);
            temp--;
            while(temp!=target&&temp!=data){
                *temp=*before_temp;
                temp--;
                before_temp--;
            }
            *temp=value;
            return iterator(temp, this);
        }

        iterator erase(iterator pos) {
            T* temp=pos.pt;
            T* next=temp+1;
            while (next!=data+len){
                *temp=*next;
                temp++;
                next++;
            }
            this->pop_back();
            return iterator(pos.pt,this);
        }

        iterator erase(const size_t &ind) {
            if(ind>=len) throw index_out_of_bound();
            T* temp=data+ind;
            T* next=temp+1;
            while (next!=data+len){
                *temp=*next;
                temp++;
                next++;
            }
            this->pop_back();
            return iterator(data+ind,this);
        }

        void push_back(const T &value) {
            if(capacity==len) doubleSpace();
            new(data+len) T(value);
            len++;
        }

        void pop_back() {
            if(len==0) throw container_is_empty();
            len--;
            (data+len)->~T();
            //if(len<capacity/3&&len!=0) cutSpace();
        }
    };


}
#endif //ZYX_TRAIN_TICKET_SYSTEM_VECTOR_HPP

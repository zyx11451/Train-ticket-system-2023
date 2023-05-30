//
// Created by 123 on 2023/5/25.
//

#ifndef BPT_HPP_MAP_HPP
#define BPT_HPP_MAP_HPP
#include <functional>
#include <cstddef>
#include "exceptions.hpp"
namespace sjtu {
    template<class T1, class T2>
    class pair {
    public:
        T1 first;
        T2 second;
        constexpr pair() : first(), second() {}
        pair(const pair &other) = default;
        pair(pair &&other) = default;
        pair(const T1 &x, const T2 &y) : first(x), second(y) {}
        template<class U1, class U2>
        pair(U1 &&x, U2 &&y) : first(x), second(y) {}
        template<class U1, class U2>
        pair(const pair<U1, U2> &other) : first(other.first), second(other.second) {}
        template<class U1, class U2>
        pair(pair<U1, U2> &&other) : first(other.first), second(other.second) {}
    };
    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    >
    class map {
    public:

        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;
        Compare cmp = Compare();
        enum Colour {
            red, black
        };

        class RedBlackNode {
        public:

            Colour colour;
            value_type *data;
            RedBlackNode *left_son;
            RedBlackNode *right_son;
            RedBlackNode *father;

            RedBlackNode(Key key_, T T_) {
                father = nullptr;
                left_son = nullptr;
                right_son = nullptr;
                data = new value_type(key_, T_);
                colour = red;
            }

            RedBlackNode(RedBlackNode &father_, Key key_, T T_) {
                //此函数未更新father_的儿子
                father = &father_;
                left_son = nullptr;
                right_son = nullptr;
                data = new value_type(key_, T_);
                colour = black;
            }

            RedBlackNode(const RedBlackNode &other) {
                data = new value_type(other.data->first, other.data->second);
                colour = other.colour;
                father = nullptr;
                right_son = nullptr;
                left_son = nullptr;
            }

            RedBlackNode(RedBlackNode *other) {
                //专用于exchange
                //不应拷贝data，否则在tmp析构时a里的data也会死
                right_son = other->right_son;
                left_son = other->left_son;
                colour = other->colour;
                father = other->father;
            }

            ~RedBlackNode() {
                //未删儿子，故不配合其它操作会leak
                delete data;
            }

            void copy_son(RedBlackNode *other) {
                if (other->left_son != nullptr) {
                    left_son = new RedBlackNode(*(other->left_son));
                    left_son->father= this;
                    left_son->copy_son(other->left_son);
                }
                if (other->right_son != nullptr) {
                    right_son = new RedBlackNode(*(other->right_son));
                    right_son->father= this;
                    right_son->copy_son(other->right_son);
                }
            }

            void kill_son() {
                if (left_son != nullptr) {
                    left_son->kill_son();
                    delete left_son;
                }
                if (right_son != nullptr) {
                    right_son->kill_son();
                    delete right_son;
                }

            }
        };

        RedBlackNode *root;
        RedBlackNode *head;
        int len;

        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
        class const_iterator;

        class iterator {
        public:
            RedBlackNode *t;
            map<Key,T,Compare>* home;
            /**
             * TODO add data members
             *   just add whatever you want.
             */
        public:
            static RedBlackNode *left_youngest_son(RedBlackNode *target) {
                RedBlackNode *temp = target;
                while (temp->left_son != nullptr) {
                    temp = temp->left_son;
                }
                return temp;
            }

            static RedBlackNode *right_youngest_son(RedBlackNode *target) {
                RedBlackNode *temp = target;
                while (temp->right_son != nullptr) {
                    temp = temp->right_son;
                }
                return temp;
            }
            iterator(){
                t= nullptr;
                home= nullptr;
            }
            iterator(map<Key,T,Compare> *home_) {
                // TODO
                t = nullptr;
                home=home_;
            }

            iterator(const iterator &other) {
                // TODO
                t = other.t;
                home=other.home;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {
                if(t== home->head) throw invalid_iterator();
                iterator temp(*this);
                if (t->right_son != nullptr) {
                    t = left_youngest_son(t->right_son);
                } else {
                    RedBlackNode *f = t->father;
                    while (t != f->left_son) {
                        t = f;
                        if (t == nullptr) break;
                        f = t->father;
                    }
                    t = f;
                }
                return temp;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++() {
                if(t== home->head) throw invalid_iterator();
                if (t->right_son != nullptr) {
                    t = left_youngest_son(t->right_son);
                } else {
                    RedBlackNode *f = t->father;
                    while (t != f->left_son) {
                        t = f;
                        if (t == nullptr) break;
                        f = t->father;
                    }
                    t = f;
                }
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int) {
                if(t== nullptr) throw invalid_iterator();
                iterator temp(*this);
                if (t->left_son != nullptr) {
                    t = right_youngest_son(t->left_son);
                } else {
                    RedBlackNode *f = t->father;
                    while (t != f->right_son) {
                        t = f;
                        if (t == nullptr) break;
                        f = t->father;
                        if(f== nullptr) throw invalid_iterator();
                    }
                    t = f;
                }
                return temp;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                if(t == nullptr) throw invalid_iterator();
                if (t->left_son != nullptr) {
                    t = right_youngest_son(t->left_son);
                } else {
                    RedBlackNode *f = t->father;
                    while (t != f->right_son) {
                        t = f;
                        if (t == nullptr) break;
                        f = t->father;
                        if(f== nullptr) throw invalid_iterator();
                    }
                    t = f;
                }
                return *this;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type &operator*() const {
                return *(t->data);
            }

            bool operator==(const iterator &rhs) const {
                return t == rhs.t;
            }

            bool operator==(const const_iterator &rhs) const {
                return t == rhs.t;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return t != rhs.t;
            }

            bool operator!=(const const_iterator &rhs) const {
                return t != rhs.t;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type *operator->() const noexcept {
                return t->data;
            }
        };

        class const_iterator {
            // it should have similar member method as iterator.
            //  and it should be able to construct from an iterator.
        public:
            // data members.
            const RedBlackNode *t;
            const map<Key,T,Compare>* home;
        public:
            static RedBlackNode *left_youngest_son(RedBlackNode *target) {
                RedBlackNode *temp = target;
                while (temp->left_son != nullptr) {
                    temp = temp->left_son;
                }
                return temp;
            }

            static RedBlackNode *right_youngest_son(RedBlackNode *target) {
                RedBlackNode *temp = target;
                while (temp->right_son != nullptr) {
                    temp = temp->right_son;
                }
                return temp;
            }
            const_iterator(){
                t= nullptr;
                home= nullptr;
            }
            const_iterator(const map<Key,T,Compare>* home_) {
                // TODO
                t = nullptr;
                home=home_;
            }

            const_iterator(const const_iterator &other) {
                // TODO
                t = other.t;
                home=other.home;
            }

            const_iterator(const iterator &other) {
                // TODO
                t = other.t;
                home=other.home;
            }

            // And other methods in iterator.
            const_iterator operator++(int) {
                if(t== home->head) throw invalid_iterator();
                const_iterator temp(*this);
                if (t->right_son != nullptr) {
                    t = left_youngest_son(t->right_son);
                } else {
                    RedBlackNode *f = t->father;
                    while (t != f->left_son) {
                        t = f;
                        if (t == nullptr) break;
                        f = t->father;
                    }
                    t = f;
                }
                return temp;
            }

            const_iterator &operator++() {
                if(t== home->head) throw invalid_iterator();
                if (t->right_son != nullptr) {
                    t = left_youngest_son(t->right_son);
                } else {
                    RedBlackNode *f = t->father;
                    while (t != f->left_son) {
                        t = f;
                        if (t == nullptr) break;
                        f = t->father;
                    }
                    t = f;
                }
                return *this;
            }

            const_iterator operator--(int) {
                if(t== nullptr) throw invalid_iterator();
                const_iterator temp(*this);
                if (t->left_son != nullptr) {
                    t = right_youngest_son(t->left_son);
                } else {
                    RedBlackNode *f = t->father;
                    while (t != f->right_son) {
                        t = f;
                        if (t == nullptr) break;
                        f = t->father;
                        if(f== nullptr) throw invalid_iterator();
                    }
                    t = f;
                }
                return temp;
            }

            /**
             * TODO --iter
             */
            const_iterator &operator--() {
                if(t== nullptr) throw invalid_iterator();
                if (t->left_son != nullptr) {
                    t = right_youngest_son(t->left_son);
                } else {
                    RedBlackNode *f = t->father;
                    while (t != f->right_son) {
                        t = f;
                        if (t == nullptr) break;
                        f = t->father;
                        if(f== nullptr) throw invalid_iterator();
                    }
                    t = f;
                }
                return *this;
            }

            value_type &operator*() const {
                return *(t->data);
            }

            bool operator==(const iterator &rhs) const {
                return t == rhs.t;
            }

            bool operator==(const const_iterator &rhs) const {
                return t == rhs.t;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return t != rhs.t;
            }

            bool operator!=(const const_iterator &rhs) const {
                return t != rhs.t;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type *operator->() const noexcept {
                return t->data;
            }
            // And other methods in iterator.
            // And other methods in iterator.
        };

        map() {
            head = nullptr;
            root = nullptr;
            len = 0;
        }

        map(const map &other) {
            if(other.len==0){
                head = nullptr;
                root = nullptr;
                len = 0;
            }else{
                head = new RedBlackNode(*other.head);
                root = new RedBlackNode(*other.root);
                head->left_son = root;
                root->father = head;
                root->copy_son(other.root);
                len = other.len;
            }

        }

        map &operator=(const map &other) {
            if (&other == this) return *this;
            if(other.len==0){
                head = nullptr;
                root = nullptr;
                len = 0;
                return *this;
            }
            if (root != nullptr) {
                root->kill_son();
                delete root;
                delete head;
            }
            len = other.len;
            head = new RedBlackNode(*other.head);
            root = new RedBlackNode(*other.root);
            head->left_son = root;
            root->father = head;
            root->copy_son(other.root);
            return *this;
        }

        inline bool is_root(RedBlackNode *obj) {
            return obj->father == head;
        }

        inline bool is_left_son(RedBlackNode *obj) {
            return obj->father->left_son == obj;
        }

        inline bool is_right_son(RedBlackNode *obj) {
            return obj->father->right_son == obj;
        }

        inline RedBlackNode *uncle(RedBlackNode *obj) {
            //必须保证祖父存在
            if (is_left_son(obj->father)) {
                return obj->father->father->right_son;
            } else {
                return obj->father->father->left_son;
            }
        }

        ~map() {
            if (root != nullptr) {
                root->kill_son();
                delete root;
                delete head;
            }

        }

        RedBlackNode *my_find(const Key &key) const {
            RedBlackNode *now_searching = root;
            while (now_searching != nullptr) {
                if (cmp(now_searching->data->first, key)) now_searching = now_searching->right_son;
                else {
                    if (cmp(key, now_searching->data->first)) now_searching = now_searching->left_son;
                    else return now_searching;
                    //既不小于也不大于推出等于。
                }
            }
            return nullptr;
        }

        T &at(const Key &key) {
            RedBlackNode *ans = my_find(key);
            if (ans == nullptr) throw index_out_of_bound();
            else return ans->data->second;
        }

        const T &at(const Key &key) const {
            RedBlackNode *ans = my_find(key);
            if (ans == nullptr) throw index_out_of_bound();
            else return ans->data->second;
        }


        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T &operator[](const Key &key) const {
            RedBlackNode *ans = my_find(key);
            if (ans == nullptr) throw index_out_of_bound();
            else return ans->data->second;
        }

        void LL(RedBlackNode *target) {
            RedBlackNode *s = target->left_son;
            if (is_left_son(target)) target->father->left_son = s;
            else target->father->right_son = s;
            s->father = target->father;
            target->left_son = s->right_son;
            if(s->right_son!= nullptr) s->right_son->father = target;
            s->right_son = target;
            target->father = s;
            if(target==root) root=s;
        }

        void RR(RedBlackNode *target) {
            RedBlackNode *s = target->right_son;
            if (is_left_son(target)) target->father->left_son = s;
            else target->father->right_son = s;
            s->father = target->father;
            target->right_son = s->left_son;
            if(s->left_son!= nullptr) s->left_son->father = target;
            s->left_son = target;
            target->father = s;
            if(target==root) root=s;
        }


        /**
         * return a iterator to the beginning
         */
        iterator begin() {
            iterator ans(this);
            if(len==0) ans.t= nullptr;
            else{
                RedBlackNode *tmp = iterator::left_youngest_son(root);
                ans.t = tmp;
            }
            return ans;
        }

        const_iterator cbegin() const {
            const_iterator ans(this);
            if(len==0) ans.t= nullptr;
            else{
                RedBlackNode *tmp = iterator::left_youngest_son(root);
                ans.t = tmp;
            }
            return ans;
        }

        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {
            iterator ans(this);
            ans.t = head;
            return ans;
        }

        const_iterator cend() const {
            const_iterator ans(this);
            ans.t = head;
            return ans;
        }

        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {
            return len == 0;
        }

        /**
         * returns the number of elements.
         */
        size_t size() const {
            return len;
        }

        /**
         * clears the contents
         */
        void clear() {
            if(root!= nullptr) root->kill_son();
            delete root;
            delete head;
            head = nullptr;
            root = nullptr;
            len = 0;
        }

        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        void insert_adjust(RedBlackNode *target) {
            //插入一个红色的叶子节点后的调整
            if (is_root(target)) {
                target->colour = black;
                return;
            } else if (is_root(target->father)) {
                target->father->colour = black;
                return;
            } else {
                //父亲为黑色时不用动
                if(target->father->colour== black) return;
                RedBlackNode *u = uncle(target);
                if (u != nullptr &&
                    u->colour == red &&
                    target->father->colour == red) {
                    u->colour = black;
                    target->father->colour = black;
                    target->father->father->colour = red;
                    insert_adjust(target->father->father);
                } else if (is_left_son(target->father) && is_right_son(target)) {
                    RR(target->father);
                    LL(target->father);
                    target->colour = black;
                    target->right_son->colour = red;
                } else if (is_right_son(target->father) && is_left_son(target)) {
                    LL(target->father);
                    RR(target->father);
                    target->colour = black;
                    target->left_son->colour = red;
                } else {
                    if (is_left_son(target)) {
                        LL(target->father->father);
                        target->father->colour = black;
                        target->father->right_son->colour = red;
                    } else {
                        RR(target->father->father);
                        target->father->colour = black;
                        target->father->left_son->colour = red;
                    }
                }
            }
        }

        pair<iterator, bool> insert(const value_type &value) {
            if (root == nullptr) {
                root = new RedBlackNode(value.first, value.second);
                head = new RedBlackNode(value.first, value.second);
                head->left_son = root;
                root->father = head;
                root->colour = black;
                len++;
                iterator ans(this);
                ans.t = root;
                return pair<iterator, bool>(ans, true);
            } else {
                RedBlackNode *to_insert = root;
                while (true) {
                    if (cmp(value.first, to_insert->data->first)) {
                        if (to_insert->left_son != nullptr) to_insert = to_insert->left_son;
                        else {
                            RedBlackNode *n = new RedBlackNode(value.first, value.second);
                            to_insert->left_son = n;
                            n->father = to_insert;
                            insert_adjust(n);
                            len++;
                            iterator ans(this);
                            ans.t = n;
                            return pair<iterator, bool>(ans, true);
                        }
                    } else {
                        if (cmp(to_insert->data->first, value.first)) {
                            if (to_insert->right_son != nullptr) to_insert = to_insert->right_son;
                            else {
                                RedBlackNode *n = new RedBlackNode(value.first, value.second);
                                to_insert->right_son = n;
                                n->father = to_insert;
                                insert_adjust(n);
                                len++;
                                iterator ans(this);
                                ans.t = n;
                                return pair<iterator, bool>(ans, true);
                            }
                        } else {
                            iterator ans(this);
                            ans.t = to_insert;
                            return pair<iterator, bool>(ans, false);
                        }
                    }
                }

            }
        }

        /**
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T &operator[](const Key &key) {
            RedBlackNode *ans = my_find(key);
            if (ans == nullptr) {
                T temp=T();
                value_type new_element(key,temp);
                this->insert(new_element);
                ans= my_find(key);
            }
            return ans->data->second;
        }

        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        inline void left_exchange(RedBlackNode *a, RedBlackNode *b) {
            //a和其左孩子b在树上的地位完全更换，但a,b对应的数没变
            if(a==root) root=b;
            RedBlackNode* tf=a->father;
            RedBlackNode* tr=a->right_son;
            Colour tc=a->colour;
            b->father=a->father;
            if(is_left_son(a)) tf->left_son=b;
            else tf->right_son=b;
            a->left_son=b->left_son;
            a->right_son=b->right_son;
            if(b->left_son!= nullptr) b->left_son->father=a;
            if(b->right_son!= nullptr) b->right_son->father=a;
            b->right_son=tr;
            if(tr!= nullptr) tr->father=b;
            b->left_son=a;
            a->father=b;
            a->colour=b->colour;
            b->colour=tc;
        }
        inline void right_exchange(RedBlackNode *a,RedBlackNode *b){
            if(a==root) root=b;
            RedBlackNode* tf=a->father;
            RedBlackNode* tl=a->left_son;
            Colour tc=a->colour;
            b->father=a->father;
            if(is_left_son(a)) tf->left_son=b;
            else tf->right_son=b;
            a->left_son=b->left_son;
            a->right_son=b->right_son;
            if(b->left_son!= nullptr) b->left_son->father=a;
            if(b->right_son!= nullptr) b->right_son->father=a;
            b->left_son=tl;
            if(tl!= nullptr) tl->father=b;
            b->right_son=a;
            a->father=b;
            a->colour=b->colour;
            b->colour=tc;
        }
        inline void simple_exchange(RedBlackNode* a,RedBlackNode* b){
            if(a==root) root=b;
            RedBlackNode* tf=a->father;
            RedBlackNode* tl=a->left_son;
            RedBlackNode* tr=a->right_son;
            Colour tc=a->colour;
            if(is_left_son(a)) tf->left_son=b;
            else tf->right_son=b;
            a->left_son=b->left_son;
            a->right_son=b->right_son;
            if(b->left_son!= nullptr) b->left_son->father=a;
            if(b->right_son!= nullptr) b->right_son->father=a;
            b->left_son=tl;
            if(tl!= nullptr) tl->father=b;
            b->right_son=tr;
            if(tr!= nullptr) tr->father=b;
            if(is_left_son(b)) b->father->left_son=a;
            else b->father->right_son=a;
            a->father=b->father;
            b->father=tf;
            a->colour=b->colour;
            b->colour=tc;
        }
        inline RedBlackNode *brother(RedBlackNode *obj) {
            if (is_left_son(obj)) return obj->father->right_son;
            else return obj->father->left_son;
        }

        void erase_adjust(RedBlackNode *target) {
            if (target == root) {
                target->colour = black;
                return;
            }
            if (brother(target)->colour == red) {
                if (is_left_son(target)) RR(target->father);
                else LL(target->father);
                target->father->father->colour = black;
                target->father->colour = red;
                erase_adjust(target);
            } else {
                RedBlackNode *bt = brother(target);
                if ((bt->left_son == nullptr || bt->left_son->colour == black) &&
                    (bt->right_son == nullptr || bt->right_son->colour == black)) {
                    if (target->father->colour == red) {
                        target->father->colour = black;
                        bt->colour = red;
                    } else {
                        bt->colour = red;
                        erase_adjust(target->father);
                    }
                } else {
                    if (is_left_son(target)) {
                        if (bt->left_son!= nullptr&&bt->left_son->colour == red) {
                            LL(bt);
                            bt->colour = red;
                            bt->father->colour = black;
                        }
                        RR(target->father);
                        Colour temp_colour = target->father->colour;
                        target->father->colour = black;
                        target->father->father->colour = temp_colour;
                        target->father->father->right_son->colour = black;
                    } else {
                        if (bt->right_son!= nullptr && bt->right_son->colour == red) {
                            RR(bt);
                            bt->colour = red;
                            bt->father->colour = black;
                        }
                        LL(target->father);
                        Colour temp_colour = target->father->colour;
                        target->father->colour = black;
                        target->father->father->colour = temp_colour;
                        target->father->father->left_son->colour = black;
                    }
                }
            }
        }

        void erase(iterator pos) {
            if (pos.t == head) throw invalid_iterator();
            if(pos.t== nullptr) throw invalid_iterator();
            if (pos.home!=this) throw invalid_iterator();
            if (len == 1) {
                delete head;
                delete root;
                head= nullptr;
                root= nullptr;
                len = 0;
                return;
            }
            //为了不让迭代器失效只能慢慢换指针
            RedBlackNode *target = pos.t;
            RedBlackNode *ts = target->left_son;
            RedBlackNode *to_exchange= nullptr;
            if(ts != nullptr){
                to_exchange=ts->right_son;
                if(to_exchange!= nullptr){
                    while(to_exchange->right_son!= nullptr) to_exchange=to_exchange->right_son;
                    simple_exchange(target,to_exchange);
                }else{
                    left_exchange(target,ts);
                }
            }
            //现在target一定最多只有一个非空子节点且为红。
            if(target->right_son!= nullptr){
                right_exchange(target,target->right_son);
            }
            if(target->left_son!= nullptr){
                left_exchange(target,target->left_son);
            }
            if (target->colour == red) {
                if (is_left_son(target)) target->father->left_son = nullptr;
                else target->father->right_son = nullptr;
                delete target;
            } else {
                erase_adjust(target);
                if (is_left_son(target)) target->father->left_son = nullptr;
                else target->father->right_son = nullptr;
                delete target;
            }
            len--;
        }

        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const {
            if (my_find(key) != nullptr) return 1;
            else return 0;
        }

        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key) {
            if (my_find(key) != nullptr) {
                iterator ans(this);
                ans.t = my_find(key);
                return ans;
            } else {
                return end();
            }
        }

        const_iterator find(const Key &key) const {
            if (my_find(key) != nullptr) {
                const_iterator ans(this);
                ans.t = my_find(key);
                return ans;
            } else {
                return cend();
            }
        }
    };


}
#endif //BPT_HPP_MAP_HPP

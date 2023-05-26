//
// Created by 123 on 2023/5/13.
//
//一个稍微改良版的bpt：因ID不可重复，故node中能只保存key，存更多信息
#ifndef MAIN_CPP_BPT_HPP
#define MAIN_CPP_BPT_HPP

#include "vector.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <string>

namespace sjtu {
    template<class T>
    T *upper_bound(T *first, T *last, T key) {
        if (first == last)return first;
        last--;
        if (key >= *last)return last + 1;
        while (first < last) {
            T *mid = first + (last - first) / 2;
            if (key < *mid) {
                last = mid;
            } else {
                first = mid + 1;
            }
        }
        return first;
    }

    template<class T>
    T *lower_bound(T *first, T *last, T key) {
        if (first == last)return first;
        last--;
        if (key > *last)return last + 1;
        while (first < last) {
            T *mid = first + (last - first) / 2;
            if (*mid < key) {
                first = mid + 1;
            } else {
                last = mid;
            }
        }
        return first;
    }

    template<class key, class value>
    struct datatype {
    public:
        key k;
        value v;

        datatype() {};

        datatype(key k_, value v_) {
            k = k_;
            v = v_;
        };

        datatype &operator=(const datatype &other) {
            if (&other == this) return *this;
            k = other.k;
            v = other.v;
            return *this;
        }

        bool operator>(datatype &b) {
            if (k != b.k) return k > b.k;
            else return v > b.v;
        }

        bool operator<(datatype &b) {
            if (k != b.k) return k < b.k;
            else return v < b.v;
        }

        bool operator==(datatype &b) {
            return k == b.k && v == b.v;
        }

        bool operator!=(datatype &b) {
            return !(k == b.k && v == b.v);
        }

        bool operator>=(datatype &b) {
            if (k != b.k) return k > b.k;
            else return v >= b.v;
        }

        bool operator<=(datatype &b) {
            if (k != b.k) return k < b.k;
            else return v <= b.v;
        }
    };

    template<class key, class value>
    class BPT;

    template<class key, class value>
    class Node {
        //存储子节点或块的元素的最大值
    public:
        typedef datatype<key, value> s;
        static const int size = 4000/(sizeof(key)+ sizeof(value)+ sizeof(int));
        int Node_ID;//1,2,3...
        int father;
        int len;
        s d[size] = {};
        bool is_father_of_block;
        friend class BPT<key, value>;
        int son[size + 1] = {};
    public:


        Node() {
            father = 0;
            len = 0;
            is_father_of_block = true;
        };

        Node &operator=(const Node &other) {
            if (&other == this) return *this;
            Node_ID = other.Node_ID;
            father = other.father;
            len = other.len;
            is_father_of_block = other.is_father_of_block;
            for (int i = 0; i < size; ++i) {
                d[i] = other.d[i];
                son[i] = other.son[i];
            }
            son[size] = other.son[size];
            return *this;
        }
    };

    template<class key, class value>
    class Block {
        static const int size = 4000/(sizeof(key)+ sizeof(value));
        int Block_ID;//1,2,3...
        int father;
        int len;
        typedef datatype<key, value> s;
        s d[size] = {};

        friend class BPT<key, value>;

    public:
        int next;

        Block() {
            father = 0;
            len = 0;
            next = -1;
        };
    };

    template<class key, class value>
    class BPT {
        typedef Node<key, value> node_type;
        typedef Block<key, value> block_type;
        const int end_pos = -1;
        int node_number = 0;
        int block_number = 0;
        std::string node_file_name;
        std::fstream node_data;
        std::string block_file_name;
        std::fstream block_data;

        typedef datatype<key, value> s;
        struct head {
            int nn;
            int bn;
            int root_ID;
        };
    public:
        int root_node_ID;

        void read_head(head &t) {
            block_data.seekg(0);
            block_data.read(reinterpret_cast<char *>(&t), sizeof(head));
        }

        void write_head(head &t) {
            block_data.seekp(0);
            block_data.write(reinterpret_cast<char *>(&t), sizeof(head));
        }

        void read_node(int index, node_type &target) {
            node_data.seekg((index - 1) * sizeof(node_type));
            node_data.read(reinterpret_cast<char *>(&target), sizeof(node_type));
        }

        void write_node(int index, node_type &target) {
            node_data.seekp((index - 1) * sizeof(node_type));
            node_data.write(reinterpret_cast<char *>(&target), sizeof(node_type));
        }

        void read_block(int index, block_type &target) {
            block_data.seekg(sizeof(head) + (index - 1) * sizeof(block_type));
            block_data.read(reinterpret_cast<char *>(&target), sizeof(block_type));
        }

        void write_block(int index, block_type &target) {
            block_data.seekp(sizeof(head) + (index - 1) * sizeof(block_type));
            block_data.write(reinterpret_cast<char *>(&target), sizeof(block_type));
        }
        BPT(const std::string &node_name = "nodes", const std::string &block_name = "blocks") {
            node_file_name = node_name;
            block_file_name = block_name;
            node_data.open(node_file_name);
            if (!node_data) {

                node_data.open(node_file_name, std::ios::out);
                node_data.close();
                node_data.open(node_file_name);
                block_data.open(block_file_name, std::ios::out);
                block_data.close();
                block_data.open(block_file_name);
                head h;
                write_head(h);
                ++node_number;
                //root.Node_ID=node_number;
                root_node_ID = 1;
                node_type root;
                root.Node_ID = root_node_ID;
                root.len = 0;
                block_type first_block;
                first_block.next = end_pos;
                ++block_number;
                first_block.Block_ID = block_number;
                root.father = -1;
                root.son[0] = 1;
                first_block.len = 0;
                first_block.father = root.Node_ID;
                write_node(1, root);
                write_block(1, first_block);
            } else {
                block_data.open(block_file_name);
                head h;
                read_head(h);
                root_node_ID = h.root_ID;
                node_number = h.nn;
                block_number = h.bn;
            }
        };

        ~BPT() {
            head h;
            h.bn = block_number;
            h.nn = node_number;
            h.root_ID = root_node_ID;
            write_head(h);
        }

        void merge_node(node_type &now_son_node, node_type &father_node) {
            int t;
            for (t = 0; t <= father_node.len; ++t)
                if (father_node.son[t] == now_son_node.Node_ID) break;
            if (t >= father_node.len) return;
            node_type next_node;
            read_node(father_node.son[t + 1], next_node);
            if (now_son_node.father != next_node.father) return;
            if (now_son_node.len + next_node.len >= (now_son_node.size - 2) ) return;
            now_son_node.d[now_son_node.len] = father_node.d[t];
            now_son_node.len++;
            if (father_node.Node_ID == root_node_ID) root_node_ID = father_node.Node_ID;
            if (now_son_node.is_father_of_block) {
                for (int i = 0; i < next_node.len; ++i) {
                    now_son_node.d[now_son_node.len + i] = next_node.d[i];
                    now_son_node.son[now_son_node.len + i] = next_node.son[i];
                    update_block_father(next_node.son[i], now_son_node.Node_ID);
                }
                now_son_node.son[now_son_node.len + next_node.len] = next_node.son[next_node.len];
                update_block_father(next_node.son[next_node.len], now_son_node.Node_ID);
            } else {
                for (int i = 0; i < next_node.len; ++i) {
                    now_son_node.d[now_son_node.len + i] = next_node.d[i];
                    now_son_node.son[now_son_node.len + i] = next_node.son[i];
                    update_node_father(next_node.son[i], now_son_node.Node_ID);
                }
                now_son_node.son[now_son_node.len + next_node.len] = next_node.son[next_node.len];
                update_node_father(next_node.son[next_node.len], now_son_node.Node_ID);
            }
            now_son_node.len = now_son_node.len + next_node.len;


            node_erase(father_node.d[t], father_node);
            write_node(father_node.Node_ID, father_node);
            if (now_son_node.father == root_node_ID) {
                if (father_node.len == 0) {
                    now_son_node.father = -1;
                    root_node_ID = now_son_node.Node_ID;
                }
            }
            write_node(now_son_node.Node_ID, now_son_node);
        };
        void node_erase(s &target, node_type &now_node) {
            int t = lower_bound(now_node.d, now_node.d + now_node.len, target) - now_node.d;
            for (int i = t + 1; i < now_node.len; ++i) {
                now_node.d[i - 1] = now_node.d[i];
                now_node.son[i] = now_node.son[i + 1];
            }
            --now_node.len;
            if (now_node.father != -1 && now_node.len < now_node.size / 2) {
                node_type father;
                read_node(now_node.father, father);
                merge_node(now_node, father);
            }
        };

        void special_node_erase(int block_ID, node_type &now_node) {
            int t = 0;
            for (t = 0; t <= now_node.len; ++t) {
                if (now_node.son[t] == block_ID) break;
            }
            if (t >= now_node.len) return;//其实不会出现这种状况，因为删的block必不是最后一个。
            for (int i = t + 1; i < now_node.len; ++i) {
                now_node.d[i - 1] = now_node.d[i];
                now_node.son[i] = now_node.son[i + 1];
            }
            --now_node.len;
            if (now_node.father != -1 && now_node.len < now_node.size / 2) {
                node_type father;
                read_node(now_node.father, father);
                merge_node(now_node, father);
            }
        }

        void merge_block(int Block_ID_1, int Block_ID_2) {
            if (Block_ID_2 == -1 || Block_ID_1 == -1) return;
            block_type block_1, block_2;
            read_block(Block_ID_1, block_1);
            read_block(Block_ID_2, block_2);
            if (block_1.father != block_2.father) return;
            if (block_1.len + block_2.len >= block_2.size ) return;
            for (int i = 0; i < block_2.len; ++i) block_1.d[block_1.len + i] = block_2.d[i];
            block_1.next = block_2.next;
            block_1.len += block_2.len;
            node_type father;
            write_block(Block_ID_1, block_1);
            read_node(block_1.father, father);
            special_node_erase(block_1.Block_ID, father);
            write_node(block_1.father, father);
        };


        void node_insert(int target_ID, s &target, int new_son_ID) {
            node_type target_node;
            read_node(target_ID, target_node);
            int t = upper_bound(target_node.d, target_node.d + target_node.len, target) - target_node.d;
            for (int i = target_node.len; i > t; --i) {
                target_node.d[i] = target_node.d[i - 1];
                target_node.son[i + 1] = target_node.son[i];
            }
            target_node.d[t] = target;
            target_node.son[t + 1] = new_son_ID;
            ++target_node.len;
            write_node(target_ID, target_node);
            if (target_ID == root_node_ID) root_node_ID = target_node.Node_ID;
            if (target_node.len == target_node.size) split_node(target_ID);

        };

        void update_block_father(int Block_ID, int father_ID) {
            block_type temp;
            read_block(Block_ID, temp);
            temp.father = father_ID;
            write_block(Block_ID, temp);
        }

        void update_node_father(int Node_ID, int father_ID) {
            node_type temp;
            read_node(Node_ID, temp);
            temp.father = father_ID;
            write_node(Node_ID, temp);
        }

        void split_node(int target_ID) {
            node_type target_node;
            node_type new_node;
            ++node_number;
            new_node.Node_ID = node_number;
            read_node(target_ID, target_node);
            if (target_node.is_father_of_block) {
                for (int i = 0; i < target_node.len / 2; ++i) {
                    new_node.d[i] = target_node.d[i + target_node.len - target_node.len / 2];
                    new_node.son[i] = target_node.son[i + target_node.len - target_node.len / 2];
                    update_block_father(target_node.son[i + target_node.len - target_node.len / 2], new_node.Node_ID);
                }
                new_node.son[target_node.len / 2] = target_node.son[target_node.len];
                update_block_father(target_node.son[target_node.len], new_node.Node_ID);
            } else {
                for (int i = 0; i < target_node.len / 2; ++i) {
                    new_node.d[i] = target_node.d[i + target_node.len - target_node.len / 2];
                    new_node.son[i] = target_node.son[i + target_node.len - target_node.len / 2];
                    update_node_father(target_node.son[i + target_node.len - target_node.len / 2], new_node.Node_ID);
                }
                new_node.son[target_node.len / 2] = target_node.son[target_node.len];
                update_node_father(target_node.son[target_node.len], new_node.Node_ID);
            }


            new_node.len = target_node.len / 2;
            target_node.len -= (new_node.len + 1);
            new_node.father = target_node.father;
            new_node.is_father_of_block = target_node.is_father_of_block;

            if (target_node.father != -1) {
                write_node(target_node.Node_ID, target_node);
                write_node(new_node.Node_ID, new_node);
                node_insert(target_node.father, target_node.d[target_node.len], new_node.Node_ID);
            } else {
                node_type new_root;
                ++node_number;
                new_root.Node_ID = node_number;
                new_root.d[0] = target_node.d[target_node.len];
                new_root.son[0] = target_node.Node_ID;
                new_root.son[1] = new_node.Node_ID;
                new_root.father = -1;
                target_node.father = new_root.Node_ID;
                new_node.father = new_root.Node_ID;
                new_root.len = 1;
                new_root.is_father_of_block = false;
                root_node_ID = new_root.Node_ID;
                write_node(target_node.Node_ID, target_node);
                write_node(new_node.Node_ID, new_node);
                write_node(new_root.Node_ID, new_root);
                return;
            }

        };

        void split_block(int target_ID) {
            block_type target_block;
            block_type new_block;
            ++block_number;
            new_block.Block_ID = block_number;
            read_block(target_ID, target_block);
            for (int i = 0; i < target_block.len / 2; ++i) {
                new_block.d[i] = target_block.d[i + target_block.len - target_block.len / 2];
            }
            new_block.len = target_block.len / 2;
            target_block.len -= new_block.len;
            new_block.next = target_block.next;
            target_block.next = new_block.Block_ID;
            new_block.father = target_block.father;
            write_block(target_block.Block_ID, target_block);
            write_block(new_block.Block_ID, new_block);
            node_insert(target_block.father, target_block.d[target_block.len - 1], new_block.Block_ID);

        };


        void insert(key k_, value v_) {
            block_type target_block;
            s target(k_, v_);
            int t = find_l(target, root_node_ID, target_block);
            for (int i = target_block.len; i > t; --i) {
                target_block.d[i] = target_block.d[i - 1];
            }
            target_block.d[t] = target;
            target_block.len++;
            write_block(target_block.Block_ID, target_block);
            if (target_block.len == target_block.size) split_block(target_block.Block_ID);
        };

        int find_l(s &target, int now_node_ID, block_type &pos) {
            node_type temp;
            read_node(now_node_ID, temp);
            int t = lower_bound(temp.d, temp.d + temp.len, target) - temp.d;
            if (temp.is_father_of_block) {
                read_block(temp.son[t], pos);
                if (pos.len == 0) return 0;
                return lower_bound(pos.d, pos.d + pos.len, target) - pos.d;
            } else {
                return find_l(target, temp.son[t], pos);
            }
        }

        void erase(key k_, value v_) {
            s target(k_, v_);
            block_type now_block;
            int t = find_l(target, root_node_ID, now_block);
            if (now_block.len==0||now_block.d[t] != target||t>=now_block.len) {
                return;
            }
            for (int i = t + 1; i < now_block.len; ++i) {
                now_block.d[i - 1] = now_block.d[i];
            }
            --now_block.len;
            write_block(now_block.Block_ID, now_block);
            if (now_block.len < now_block.size / 2) {
                merge_block(now_block.Block_ID, now_block.next);
            }

        };

        int key_lower_bound(s *first, s *last, key k_) {
            s *f = first;
            if (first == last)return first - f;
            last--;
            if (k_ > last->k)return last + 1 - f;
            while (first < last) {
                s *mid = first + (last - first) / 2;
                if (mid->k < k_) {
                    first = mid + 1;
                } else {
                    last = mid;
                }
            }
            return first - f;
        }
        int binary_search(key k_, int now_node_ID, block_type &target_block) {
            node_type temp;
            read_node(now_node_ID, temp);
            int t = key_lower_bound(temp.d, temp.d + temp.len, k_);
            if (temp.is_father_of_block) {
                read_block(temp.son[t], target_block);
                if (target_block.len == 0) return 0;
                return key_lower_bound(target_block.d, target_block.d + target_block.len, k_);
            } else {
                return binary_search(k_, temp.son[t], target_block);
            }
        }
        vector<value> dis(key k_) {
            vector<value> ans;
            block_type target_block;
            int t = binary_search(k_, root_node_ID, target_block);
            bool end = false;
            for (int i = t; i < target_block.len; ++i) {
                if (target_block.d[i].k < k_) continue;
                if (target_block.d[i].k > k_) {
                    end = true;
                    break;
                }
                ans.push_back(target_block.d[i].v);
            }
            while ((!end) && target_block.next != -1) {
                read_block(target_block.next, target_block);
                for (int i = 0; i < target_block.len; ++i) {
                    if (target_block.d[i].k > k_) {
                        end = true;
                        break;
                    }
                    ans.push_back(target_block.d[i].v);
                }
            }
            return ans;
        };

        void clear_file() {
            remove(node_file_name.c_str());
            remove(block_file_name.c_str());
        }
    };


}


#endif //MAIN_CPP_BPT_HPP

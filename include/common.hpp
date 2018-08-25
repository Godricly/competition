#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <string>
#include <map>

using namespace std;

class Tree {
public:
    string dst_id_;
    map<string, float> srcs_; 

    Tree() {}
    Tree(string src_id, string dst_id, float percent) {
        dst_id_ = dst_id;
        srcs_[src_id] = percent;
    }

    void display() {
        for(auto p: srcs_) {
            cout<<p.first<<":"<<p.second<<endl;
        }
        cout<<endl;
    }
};

#endif

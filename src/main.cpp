#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include <chrono>
#include <vector>

#include "common.hpp"

using namespace std;

void find_boss(Tree* tree, string& companyid, vector<string>& controller, vector<float>& percent) {
    float maxval = 0.0f;
    for(auto s: tree->srcs_) {
        if(s.second>maxval) {
            maxval = s.second;
        }
    }
    for(auto s: tree->srcs_) {
        if(s.second == maxval) {
            controller.push_back(s.first);
            percent.push_back(s.second);
        }
    }
    cout<<companyid<<","<<int(maxval*10000)/10000.0<<",";
    cout<<"[";
    for(size_t i=0; i<controller.size(); ++i) {
        if( i!=controller.size()-1 ) cout<<controller[i]<<",";
        else cout<<controller[i];
    }
    cout<<"]"<<endl;
}

//@Deprecated
Tree* search(map<string, Tree*>& trees, string dst_id) {

    if(trees.find(dst_id)==trees.end() ) {
        // cout<<"no more boss, return NULL"<<endl;
        return NULL;
    }

    Tree* t = trees[dst_id];

    // for(auto s: t->srcs_) {
    //     Tree* ret = search(trees, s.first);
    //     if(ret==NULL) 
    //         continue;
    //     for(auto r: ret->srcs_) {
    //         if(t->srcs_.find(r.first) == t->srcs_.end() ) {
    //             t->srcs_[r.first] = r.second;
    //         } else {
    //             t->srcs_[r.first] *= r.second;
    //         }
    //     }
    // }
    // t->display();
    return t;
}


Tree* search_recursive(map<string, Tree*>& trees, const string& dst_id, map<string, Tree*>& cache, unordered_map<string, float>& path) {

    if(trees.find(dst_id)==trees.end()) {
        // cout<<"no more boss, return NULL"<<endl;
        return NULL;
    }

    if(cache.find(dst_id) != cache.end()) {
        // cout<<"Hit Cache: "<<cache.size()<<endl;
        return cache[dst_id];
    }

    Tree* current = trees[dst_id];
    Tree* flatten = new Tree(dst_id);
    for(auto s: current->srcs_) { // 从current的直接投资人里面递归找展开树
        string cont_id = s.first;
        float cont_pr = s.second;
        if(path.find(cont_id) != path.end()) {
            flatten->srcs_[cont_id] = cont_pr * path[dst_id] / path[cont_id];
            continue;
        }
        unordered_map <string, float> cont_path(path);
        // cont_path.insert(cont_id);
        //change weights
        if (cont_path.empty()) {
            cont_path[cont_id] = cont_pr;
        } else {
            cont_path[cont_id] = cont_path[dst_id] * cont_pr;
        }

        Tree* ret_flatten = search_recursive(trees, cont_id, cache, cont_path);// 投资公司的展开树
        if(ret_flatten==NULL) { // 为NULL，是个大公司，不需要别人投资，例如 华为..
            flatten->srcs_[cont_id] = cont_pr;
            continue;
        } else {                // 否则，把展开树转移给小公司, 控股权连乘
            for(auto ret: ret_flatten->srcs_) {
                if(flatten->srcs_.find(ret.first) == flatten->srcs_.end() ) {
                    flatten->srcs_[ret.first] = cont_pr * ret.second;
                } else {
                    flatten->srcs_[ret.first] += cont_pr * ret.second;
                }
            }
        }
    }
    cache[dst_id] = flatten;
    return flatten;
}

int main(int argc, char ** argv) {

    string inpath = argv[1];
    ifstream infile(inpath);
    string src_id;
    string dst_id;
    float percent;

    /*****
    * building tree nodes
    *****/
    map<string, Tree*> trees;
    map<string, bool> companies;

auto bstart = std::chrono::system_clock::now();
    while(infile>>src_id>>dst_id>>percent) {
        // cout<<src_id<<","<<dst_id<<","<<percent<<endl;

        if(trees.find(dst_id) == trees.end() ) {    // 如果没有这个树节点，new一个
            trees[dst_id] = new Tree(src_id, dst_id, percent);
        } else {                                    // 
            trees[dst_id]->srcs_[src_id] = percent;
        }
        companies[src_id] = true;
        companies[dst_id] = true;
    }

auto bend = std::chrono::system_clock::now();
std::chrono::duration<float> build_elapsed = bend - bstart;
float build_time = static_cast<float>(1000*build_elapsed.count());
// cout<<"Building Tree Time: "<<build_time<<endl;

    // cout<<"Tree Building Finished. "<<trees.size()<<endl;

    /*****
    # recursively search and get a flatten tree
    *****/
    map<string, Tree*> cache;
    for(auto c: companies) {

auto sstart = std::chrono::system_clock::now();

        string companyid = c.first;
        unordered_map <string, float> path;
        Tree* finaltree = search_recursive(trees, companyid, cache, path);

        if(finaltree!=NULL) {
            vector<string> controller;
            vector<float> percent;
            find_boss(finaltree, companyid, controller, percent);
        }

auto send = std::chrono::system_clock::now();
std::chrono::duration<float> findmax_elapsed = send - sstart;
float findmax_time = static_cast<float>(1000*findmax_elapsed.count());
//cout<<"Search Time: "<<findmax_time<<endl;

    }


    return 0;
}
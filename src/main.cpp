#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include "common.hpp"

using namespace std;

void find_boss(Tree* tree, string& companyid, string& controller, float& percent) {
    for(auto s: tree->srcs_) {
        if(s.second>percent) {
            percent = s.second;
            controller = s.first;
        }
    }
    cout<<companyid<<","<<percent<<",["<<controller<<"]"<<endl;
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


Tree* search_recursive(map<string, Tree*>& trees, const string& dst_id, map<string, Tree*>& cache, const unordered_set<string>& path) {

    if(trees.find(dst_id)==trees.end()) {
        // cout<<"no more boss, return NULL"<<endl;
        return NULL;
    }

    if(cache.find(dst_id) != cache.end()) {
        return cache[dst_id];
    }

    Tree* current = trees[dst_id];
    Tree* flatten = new Tree(dst_id);
    for(auto s: current->srcs_) { // 从current的直接投资人里面递归找展开树
        string cont_id = s.first;
        float cont_pr = s.second;
        if(path.find(cont_id) != path.end())
            continue;
        unordered_set <string> cont_path(path);
        cont_path.insert(cont_id);
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

    ifstream infile("geek_data_processed.csv");
    string src_id;
    string dst_id;
    float percent;

    map<string, Tree*> trees;
    map<string, bool> companies;

    while(infile>>src_id>>dst_id>>percent) {
        // cout<<src_id<<","<<dst_id<<","<<percent<<endl;

        if(trees.find(dst_id) == trees.end() ) {
            trees[dst_id] = new Tree(src_id, dst_id, percent);
        } else {
            trees[dst_id]->srcs_[src_id] = percent;
        }
        companies[src_id] = true;
        companies[dst_id] = true;   
    }
    // cout<<"Tree Building Finished. "<<trees.size()<<endl;

    map<string, Tree*> cache;
    for(auto c: companies) {
        string companyid = c.first;
        unordered_set <string> path;
        Tree* finaltree = search_recursive(trees, companyid, cache, path);
        if(finaltree!=NULL) {
            string controller = "";
            float percent = 0.0f;
            find_boss(finaltree, companyid, controller, percent);
        }
    }

    return 0;
}
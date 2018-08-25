#include <string>
#include <iostream>
#include <fstream>

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

Tree* search(map<string, Tree*> trees, string dst_id) {

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

int main(int argc, char ** argv) {

    ifstream infile("sample_geek.csv");
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

    for(auto c: companies) {
        string companyid = c.first;
        Tree* finaltree = search(trees, companyid);
        if(finaltree!=NULL) {
            string controller = "";
            float percent = 0.0f;
            find_boss(finaltree, companyid, controller, percent);
        }
    }

    return 0;
}
// bplustree.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <string.h>
using namespace std;
// 定义阶数
#define ORDER 5
class BplusTree
{
private:
    // 定义b+树结构体
    struct BPlusTreeNode {
        BPlusTreeNode* parent;  // 父节点
        bool is_leaf;   // 是否为叶子节点
        int key_num;    // 关键字的数量
        int key[ORDER - 1];     // 关键字数组
        string name[ORDER - 1];
        BPlusTreeNode* child[ORDER];    // 存放孩子指针的数组
        BPlusTreeNode* next;            // 指向下一个兄弟节点
    };
public:
    BPlusTreeNode* root=new BPlusTreeNode;
    // 构造函数
    BplusTree() { root->is_leaf = true; root->parent = nullptr; root->key_num = 0; root->next = nullptr; }

public:
    // 分割操作
    void split(BPlusTreeNode* node);
    // 插入操作
    void insert(BPlusTreeNode* node, int key,string name);
    // 查找操作
    void find(BPlusTreeNode* node, int key);
};

// TODO: 在此处引用程序需要的其他标头。

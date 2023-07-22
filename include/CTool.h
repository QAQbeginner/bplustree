#pragma once
#include "bplustree.h"

// 加载节点
BPlusTreeNode* load_node(string path);

string read_one(int &fp);

int* read_many(int &fp,int n);

string* read_name(int &fp,int n);


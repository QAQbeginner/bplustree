// bplustree.cpp: 定义应用程序的入口点。
//
#include "bplustree.h"


void BplusTree::split(BPlusTreeNode* node)
{
    // 创建新结点用于存储右半部分的数值
    BPlusTreeNode* new_node = new BPlusTreeNode();
    new_node->is_leaf = true;
    new_node->next = node->next;
    node->next = new_node;
    int mid = (ORDER - 1) / 2;
    for (int i = mid; i < ORDER - 1; i++) {
        new_node->key[i - mid] = node->key[i];
    }
    new_node->key_num = ORDER - 1 - mid;
    // 将node作为分割后的左半部分进行修改
    node->key_num = mid;
    new_node->parent = node->parent;
    // 若分割前node本身为根节点，则创建新的根节点
    if (node == root) {
        BPlusTreeNode* new_root = new BPlusTreeNode();
        new_root->is_leaf = false;
        root = new_root;
        root->child[0] = node;
        node->parent = root;
    }
    // 获取node在其父节点所属的子节点的位置
    int i = 0;
    while (node->parent->child[i] != node) {
        i++;
    }
    // 将该位置后续元素进行后移，并插入node节点
    for (int j = node->parent->key_num; j > i; j--) {
        node->parent->child[j + 1] = node->parent->child[j];
        node->parent->key[j] = node->parent->key[j - 1];
    }
    node->parent->child[i + 1] = new_node;
    node->parent->key[i] = new_node->key[0];
    node->parent->key_num++;
    // 若父节点经过插入后，其数量达到饱和，则递归进行分割
    if (node->parent->key_num == ORDER - 1)
    {
        split(node->parent);
    }
}


void BplusTree::insert(BPlusTreeNode* node, int key, string name)
{
    // 如果node为叶子节点则进行查找并插入
    if (node->is_leaf) {
        int i = 0;
        // 获取要插入的元素位置
        while (i < node->key_num && node->key[i] < key) {
            i++;
        }
        // 将元素进行后移
        for (int j = node->key_num; j > i; j--) {
            node->key[j] = node->key[j - 1];
        }
        // 赋值
        node->key[i] = key;
        node->name[i] = name;
        node->key_num++;
        // 数量超过的话，进行分割
        if (node->key_num == ORDER - 1) {
            split(node);
        }
    }
    // 如果根节点不是叶子节点，则进行递归
    else
    {
        int i = 0;
        // 查找该结点的范围
        while (i < node->key_num && node->key[i] < key) {
            i++;
        }
        insert(node->child[i], key,name);
    }
}

void BplusTree::find(BPlusTreeNode* node, int key)
{
    // 如果node为叶子节点则进行查找并插入
    if (node->is_leaf) {
        int i = 0;
        // 获取要插入的元素位置
        while (i < node->key_num) {
            if (node->key[i] == key)
            {
                cout << node->name[i] << endl;
                return;
            }
            i++;
        }
    }
    // 如果根节点不是叶子节点，则进行递归
    else
    {
        int i = 0;
        // 查找该结点的范围
        while (i < node->key_num && node->key[i] < key) {
            i++;
        }
        find(node->child[i], key);
    }
}



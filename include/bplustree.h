#pragma once
#include"CTool.h"
// 定义阶数
#define ORDER 71
// 定义b+树结构体
struct BPlusTreeNode {
    BPlusTreeNode* parent=nullptr;  // 父节点
    bool is_leaf;   // 是否为叶子节点
    int key_num=0;    // 关键字的数量
    int key[ORDER] = {-1};     // 关键字数组
    string* name[ORDER] = {nullptr};
    BPlusTreeNode* child[ORDER+1] = {nullptr};    // 存放孩子指针的数组
    BPlusTreeNode* next; // 指向下一个兄弟节点
    int id=-1;  
    int p_id=-1;
    int b_id = -1;
    int c_id[ORDER+1]={-1};
};
struct info {
    BPlusTreeNode* node=nullptr;    // 找到的叶子节点
    int loc;                //	查找的值在该节点的位置
};
// 定义结构体用于反序列化操作
typedef struct NodeInfo
{
    BPlusTreeNode* node=nullptr;
    bool is_loaded=false;// 是否被加载
    bool is_changed=false;// 是否被修改
    bool is_removed=false;
}NodeInfo,*PNodeInfo;
class BplusTree
{
public:
    // 查找叶子节点操作
    info* find(BPlusTreeNode* node, int key,vector<PNodeInfo> &node_arr);
private:
    // 获取该节点在其父亲节点的位置
    int get_loc(BPlusTreeNode* p_node, BPlusTreeNode* c_node);
    // 自下而上修改父节点的相关值
    void p_change(BPlusTreeNode* p_node, int key, int c_key,vector<PNodeInfo> &node_arr);
    // 自上而下修改子节点的相关值
    void c_change(BPlusTreeNode* c_node, BPlusTreeNode* p_node,vector<PNodeInfo> &node_arr);
    // 遍历一层的数值
    void s_ceng(BPlusTreeNode* p_node,vector<PNodeInfo> &node_arr);
     // 找到key值在父节点的位置
    int find_key(BPlusTreeNode* p_node, int key);
public:
    // 根节点
    BPlusTreeNode* root = new BPlusTreeNode();
    // 节点数量
    int node_num = 1;
    // 默认构造函数
    BplusTree() { root->is_leaf = true; root->parent = nullptr; root->key_num = 0; root->next = nullptr; root->id = 1; }
    // xigouganhsu
public:
    // 分割操作
    void split(BPlusTreeNode* node,vector<PNodeInfo> &node_arr);
    // 插入操作
    void insert(BPlusTreeNode* node, int key, string* name,vector<PNodeInfo> &node_arr);
    // 查询操作
    void search(BPlusTreeNode* node, int key,vector<PNodeInfo> &node_arr);
    // 查看树的结构
    void all(BPlusTreeNode* node,vector<PNodeInfo> &node_arr);
    // 修改
    void change(BPlusTreeNode* node, int key,vector<PNodeInfo> &node_arr);  
    
    // 序列化操作
    // 保存单个节点
    void save(BPlusTreeNode* node);

    void save_changed(BPlusTreeNode* node,vector<PNodeInfo> &node_arr);

    void save_new(BPlusTreeNode* node);


    // 反序列化操作
    static BPlusTreeNode* load_node(int t_id,vector<PNodeInfo> &node_arr);
    static BPlusTreeNode* n_load_node(int t_id,vector<PNodeInfo> &node_arr);

    int get_node_loc(BPlusTreeNode* node,BPlusTreeNode* p_node);
    // 非叶子节点的合并,r_node合并到l_node
    void combine(BPlusTreeNode* l_node,BPlusTreeNode* r_node,vector<PNodeInfo> &node_arr);
    // 非叶子节点
    // 左边向右边借
    void borrow_l(BPlusTreeNode* l_node,BPlusTreeNode* r_node,vector<PNodeInfo> &node_arr);
    // 右边向左边借
    void borrow_r(BPlusTreeNode* r_node,BPlusTreeNode* l_node,vector<PNodeInfo> &node_arr);
    // 非叶子节点
    void deal_node(BPlusTreeNode* node,vector<PNodeInfo> &node_arr);
    // 叶子节点
    void deal_y_node(BPlusTreeNode* node,vector<PNodeInfo> &node_arr,int key,int key_l);
    void combine_y_node(BPlusTreeNode* l_node,BPlusTreeNode* r_node,vector<PNodeInfo> &node_arr,int key_l);
    void delect(BPlusTreeNode* node,int key,vector<PNodeInfo> &node_arr);
};
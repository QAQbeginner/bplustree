#include "bplustree.h"

// 分割（已改）
void BplusTree::split(BPlusTreeNode* node,vector<PNodeInfo> &node_arr)
{
    // 创建新结点用于存储右半部分的数值
    BPlusTreeNode* new_node = new BPlusTreeNode();
    new_node->is_leaf = node->is_leaf;

    if(node->b_id!=-1&&node_arr[node->b_id]->is_loaded!=true)
        node->next=load_node(node->b_id,node_arr);
    new_node->next = node->next;
    new_node->b_id=node->b_id;
    
    // 序列化新增
    new_node->id = ++node_num;
    new_node->parent = node->parent;

    // 新加的用于序列化的操作
    new_node->p_id = node->p_id;
    
    node->b_id = new_node->id;
    node->next = new_node;
    int mid = (ORDER) / 2;
    for (int i = mid; i < ORDER; i++) {
        new_node->key[i - mid] = node->key[i];
        if(new_node->is_leaf)
        	new_node->name[i - mid] = node->name[i];
    }
    new_node->key_num = ORDER - mid;
    // 将node作为分割后的左半部分进行修改
    node->key_num = mid;
    // add the new node to the arr
    PNodeInfo t_node_info=new NodeInfo();
    t_node_info->node=new_node;
    t_node_info->is_loaded=true;
    t_node_info->is_changed=true;
    node_arr.push_back(t_node_info);

    if(!node->is_leaf)
    	// 把node节点的右半部分孩子节点的父节点指向新创建的节点
    	c_change(node->child[(ORDER-1)/2+1],new_node,node_arr);
    // 若分割前node本身为根节点，则创建新的根节点
    if (node == root) {
        BPlusTreeNode* new_root = new BPlusTreeNode();
        new_root->is_leaf = false;
        root = new_root;
        root->p_id=-1;
        root->child[0] = node; 
        root->child[1] = new_node;
        //root->key[0] = new_node->key[0];
        //root->key_num = 1;
        node->parent = root;
        new_node->parent=root;
        // 序列化操作
        root->id = ++node_num;
        root->c_id[0] = node->id;
        root->c_id[1] = new_node->id;
        node->p_id = root->id;
        new_node->p_id=root->id;
        PNodeInfo r_node_info=new NodeInfo();
        r_node_info->node=new_root;
        r_node_info->is_loaded=true;
        r_node_info->is_changed=true;
        node_arr.push_back(r_node_info);
        //return;
    }
    
    // 获取node在其父节点所属的子节点的位置
    int i = 0;
    while (node->parent->c_id[i] != node->id) {
        i++;
    }
    
    // 将该位置后续元素进行后移，并插入new_node节点
    for (int j = node->parent->key_num; j > i; j--) {
        node->parent->child[j+1] = node->parent->child[j];
        node->parent->key[j] = node->parent->key[j - 1];
        // 序列化新增
        node->parent->c_id[j + 1] = node->parent->c_id[j];
    }
    node->parent->key_num++;
    node->parent->child[i + 1] = new_node;
    node->parent->key[i] = new_node->key[0];
    // 序列化新增
    node->parent->c_id[i + 1] = new_node->id;

	// 若父节点经过插入后，其数量达到饱和，则递归进行分割
    if (node->parent->key_num == ORDER)
    {
        split(node->parent,node_arr);
        // 把原父节点的右半部分孩子节点指向新创建的父亲节点
        // c_change(node->parent->child[(ORDER - 1)/2+1], node->parent->next,node_arr);
    }
	 // 若分割节点不为叶子节点，则将刚添加至父节点的节点删除
    if (!new_node->is_leaf)
    {
        for (int i = 0; i < new_node->key_num-1; i++)
        {
            new_node->key[i] = new_node->key[i + 1];
        }
        new_node->key_num--;
    }
}
// 插入（已改）
void BplusTree::insert(BPlusTreeNode* node, int key, string* name,vector<PNodeInfo> &node_arr)
{

    if(node_arr.size()>28&&node_arr[28]->is_loaded>1)
        cout<<"problem"<<endl;
    // string* p_name = new string(name);
    // p_name->append(name);
    // 如果node为叶子节点则进行查找并插入
    if (node->is_leaf) {
        BPlusTreeNode* t_node = node;
        if(t_node->b_id>-1&&node_arr[t_node->b_id]->is_loaded==false)
            t_node->next=load_node(t_node->b_id,node_arr);
        while (t_node && t_node->next)
        {
            if (key > t_node->next->key[0])
            {
                if(t_node->b_id>-1&&node_arr[t_node->b_id]->is_loaded==false)
                     t_node->next=load_node(t_node->b_id,node_arr);
                t_node = t_node->next;
            }
            else
                break;
        }
        int i = 0;
        // 获取要插入的元素位置
        while (i < t_node->key_num && t_node->key[i] < key) {
            i++;
        }
        //若插入的关键字比当前结点中的最大值还大，破坏了B+树中从根结点到当前结点的所有索引值，此时需要及时修正后，再做其他操作。
        /*if (i == t_node->key_num)
            p_change(t_node->parent, t_node->key[t_node->key_num - 1], key);*/
        // 将元素进行后移
        node_arr[t_node->id]->is_changed=true;
        for (int j = t_node->key_num; j > i; j--) {
            t_node->key[j] = t_node->key[j - 1];
            t_node->name[j]=t_node->name[j-1];
        }
        //cout<<"sb"<<endl;
        // 赋值
        t_node->key[i] = key;
        t_node->name[i] = name;
        t_node->key_num++;
        // 数量超过的话，进行分割
        if (t_node->key_num == ORDER) {
            split(t_node,node_arr);
            if(node_arr.size()>28&&node_arr[28]->is_loaded>1)
                cout<<"problem"<<endl;
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
        int c_id=node->c_id[i];
        if(node_arr[c_id]->is_loaded==true)
        {
            node->child[i]=node_arr[c_id]->node;
            insert(node->child[i], key,name,node_arr);
            if(node_arr.size()>28&&node_arr[28]->is_loaded>1)
                cout<<"problem"<<endl;
        }
        else
        {
            node->child[i]=load_node(c_id,node_arr);
            insert(node->child[i], key,name,node_arr);
        }
    }
}
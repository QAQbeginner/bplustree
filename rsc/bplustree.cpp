// bplustree.cpp: 定义应用程序的入口点。
//
#include "bplustree.h"

// 查找并返回叶子节点（已改）
info* BplusTree::find(BPlusTreeNode* node, int key,vector<PNodeInfo> &node_arr)
{
    // 如果node为叶子节点则进行查找
    if (node->is_leaf>0) {
        int i = 0;
        // 获取要插入的元素位置
        while (i < node->key_num) {
            if (node->key[i] == key)
            {
                // cout << node->name[i] << endl;
                info* f_info = new info;
                f_info->node = node;
                f_info->loc = i;
                return f_info;
            }
            i++;
        }
        return nullptr;
    }
    // 如果根节点不是叶子节点，则进行递归
    else
    {
        int i = 0;
        // 查找该结点的范围
        while (i < node->key_num && node->key[i] <=key) {
            i++;
        }
        // add a funtion: if node is not loaded then loaded the node and do the next thing
        int c_id=node->c_id[i];
        if(node_arr[c_id]->is_loaded==true)
        {
            node->child[i]=node_arr[c_id]->node;
            find(node->child[i], key,node_arr);
        }
        else
        {
            node->child[i]=n_load_node(c_id,node_arr);
            find(node->child[i], key,node_arr);
        }
    }
}
// 查询操作（已改）
void BplusTree::search(BPlusTreeNode* node, int key,vector<PNodeInfo> &node_arr)
{
    info* f_info = find(node, key,node_arr);
    if(f_info==nullptr)
    {
        cout<<"can't find this data,please check your input"<<endl;
        return ;
    }
    cout << "the search value is:" << *(f_info->node->name[f_info->loc]) << endl;
    delete f_info;
    f_info=nullptr;
}
// 获取该节点在其父亲节点的位置(还没改)
int BplusTree::get_loc(BPlusTreeNode* p_node, BPlusTreeNode* c_node)
{
    int i = 0;
    for (i = 0; i < p_node->key_num; i++)
    {
        if (p_node->child[i] == c_node)
            return i;
    }
    return i;
}
// 自下而上修改父节点的相关值（已改）
void BplusTree::p_change(BPlusTreeNode* p_node, int key, int c_key,vector<PNodeInfo> &node_arr)
{
    BPlusTreeNode* tp_node = p_node;
    // 当删除某结点中最大或者最小的关键字，就会涉及到更改其双亲结点一直到根结点中所有索引值的更改。
    while (tp_node)
    {
        for (int i = 0; i < tp_node->key_num; i++)
        {
            if (key == tp_node->key[i])
            {
                tp_node->key[i] = c_key;
                node_arr[tp_node->id]->is_changed=true;
                break;
            }
        }
        if(tp_node==root)
            break;
        tp_node = tp_node->parent;
    }

}
// 自上而下修改子节点的相关值(还没改)(ok)
void BplusTree::c_change(BPlusTreeNode* c_node, BPlusTreeNode* p_node,vector<PNodeInfo> &node_arr)
{
    BPlusTreeNode* cd_node = c_node;
    int i = 0;
    while(i<(ORDER-1)/2+1)
    {
        p_node->child[i] = cd_node;
        p_node->c_id[i]=cd_node->id;
        cd_node->parent = p_node;
        cd_node->p_id = p_node->p_id;
        if(cd_node->b_id==-1)
            return;
        if(node_arr[cd_node->b_id]->is_loaded==true)
        {
            cd_node->next=node_arr[cd_node->b_id]->node;
        }
        else
        {
            // cout<<node_arr[cd_node->b_id]->is_loaded<<endl;
            cd_node->next=n_load_node(cd_node->b_id,node_arr);
        }
            cd_node = cd_node->next;
        i++;
    }
}
// 遍历一层的数值(还没改)（已改）
void BplusTree::s_ceng(BPlusTreeNode* p_node,vector<PNodeInfo> &node_arr)
{
    BPlusTreeNode* temp = p_node;
    while (temp)
    {
        cout<<"(";
        for (int i = 0; i < temp->key_num; i++)
        {
            cout << temp->key[i] << " ";
        }
        cout<<")";
        if(temp->b_id!=-1)
        {
            if(node_arr[temp->b_id]->is_loaded==true)
            {
                temp->next=node_arr[temp->b_id]->node;
                temp = temp->next;
            }
            else
            {
                temp->next=n_load_node(temp->b_id,node_arr);
                temp=temp->next;
            }
        }
        else
            temp=nullptr;
        cout << "     ";
    }
}
// 查看树的结构(还没改)（已改）
void BplusTree::all(BPlusTreeNode* node,vector<PNodeInfo> &node_arr)
{
    BPlusTreeNode* temp = node;
    while (temp)
    {
        s_ceng(temp,node_arr);
        cout << endl;
        if(temp->c_id[0]!=-1)
        {
            if(node_arr[temp->c_id[0]]->is_loaded==true)
            {
                temp->child[0]=node_arr[temp->c_id[0]]->node;
                temp = temp->child[0];
            }
            else
            {
                temp->child[0]=n_load_node(temp->c_id[0],node_arr);
                temp = temp->child[0];
            }
        }
        else
            temp=nullptr;
    }
}
// 修改
void BplusTree::change(BPlusTreeNode* node, int key,vector<PNodeInfo> &node_arr)
{
    info* f_info = find(node, key,node_arr);
    string n_name;
    cout<<"please input your value that you are ready to change"<<endl;
    cin>>n_name;
    string* t_str=new string(n_name);
    f_info->node->name[f_info->loc]=t_str;
    node_arr[f_info->node->id]->is_changed=true;
    cout<<"change success"<<endl;
    delete f_info;
    f_info=nullptr;
}  

int BplusTree::find_key(BPlusTreeNode* p_node, int key)
{
    for (int i = 0; i <p_node->key_num; i++)
    {
        if (key == p_node->key[i])
            return i;
    }
    return -1;
}



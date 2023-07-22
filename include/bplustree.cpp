// bplustree.cpp: 定义应用程序的入口点。
//
#include "bplustree.h"

// 分割
void BplusTree::split(BPlusTreeNode* node)
{
    // 创建新结点用于存储右半部分的数值
    BPlusTreeNode* new_node = new BPlusTreeNode();
    new_node->is_leaf = node->is_leaf;
    //new_node->next = node->next;
    node->next = new_node;
    int mid = (ORDER) / 2;
    for (int i = mid; i < ORDER; i++) {
        new_node->key[i - mid] = node->key[i];
        new_node->name[i - mid] = node->name[i];
    }
    new_node->key_num = ORDER - mid;
    // 将node作为分割后的左半部分进行修改
    node->key_num = mid;

    // 序列化新增
    new_node->id = ++node_num;

    // 若分割前node本身为根节点，则创建新的根节点
    if (node == root) {
        BPlusTreeNode* new_root = new BPlusTreeNode();
        new_root->is_leaf = false;
        root = new_root;
        root->child[0] = node; 
        root->child[1] = new_node;
       // root->key[0] = new_node->key[0];
       //root->key_num = 1;
        node->parent = root;
        // 序列化操作
        root->id = ++node_num;
        root->c_id[0] = node->id;
        root->c_id[1] = new_node->id;
        node->p_id = root->id;
    }
    new_node->parent = node->parent;

    // 新加的用于序列化的操作
    new_node->p_id = node->p_id;
    node->b_id = new_node->id;

    // 获取node在其父节点所属的子节点的位置
    int i = 0;
    while (node->parent->child[i] != node) {
        i++;
    }

    // 将该位置后续元素进行后移，并插入new_node节点
    for (int j = node->parent->key_num; j > i; j--) {
        node->parent->child[j + 1] = node->parent->child[j];
        node->parent->key[j] = node->parent->key[j - 1];
        // 序列化新增
        node->parent->c_id[j + 1] = node->parent->c_id[j];
    }
    node->parent->child[i + 1] = new_node;
    node->parent->key[i] = new_node->key[0];
    node->parent->key_num++;
    // 序列化新增
    node->parent->c_id[i + 1] = new_node->id;
    // 若分割节点不为叶子节点，则将刚添加至父节点的节点删除
    if (!new_node->is_leaf)
    {
        for (int i = 0; i < new_node->key_num-1; i++)
        {
            new_node->key[i] = new_node->key[i + 1];
        }
        new_node->key_num--;
    }
    // 若父节点经过插入后，其数量达到饱和，则递归进行分割
    if (node->parent->key_num == ORDER)
    {
        split(node->parent);
        c_change(node->parent->child[(ORDER + 1) / 2], node->parent->next);
    }
}
// 插入
void BplusTree::insert(BPlusTreeNode* node, int key, string* name)
{
    // string* p_name = new string(name);
   // p_name->append(name);
    // 如果node为叶子节点则进行查找并插入
    if (node->is_leaf) {
        BPlusTreeNode* t_node = node;
        while (t_node && t_node->next)
        {
            if (key > t_node->next->key[0])
                t_node = t_node->next;
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
        for (int j = t_node->key_num; j > i; j--) {
            t_node->key[j] = t_node->key[j - 1];
        }
        //cout<<"sb"<<endl;
        // 赋值
        t_node->key[i] = key;
        t_node->name[i] = name;
        t_node->key_num++;
        // 数量超过的话，进行分割
        if (t_node->key_num == ORDER) {
            split(t_node);
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
        insert(node->child[i], key, name);
    }
}
// 查找并返回叶子节点
info* BplusTree::find(BPlusTreeNode* node, int key)
{
    // 如果node为叶子节点则进行查找
    if (node->is_leaf) {
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
    }
    // 如果根节点不是叶子节点，则进行递归
    else
    {
        int i = 0;
        // 查找该结点的范围
        while (i < node->key_num && node->key[i] <=key) {
            i++;
        }
        find(node->child[i], key);
    }
}
// 查询操作
void BplusTree::search(BPlusTreeNode* node, int key)
{
    info* f_info = find(node, key);
    cout << "the search value is:" << *(f_info->node->name[f_info->loc]) << endl;
}
// 获取该节点在其父亲节点的位置
int BplusTree::get_loc(BPlusTreeNode* p_node, BPlusTreeNode* c_node)
{
    for (int i = 0; i < p_node->key_num; i++)
    {
        if (p_node->child[i] == c_node)
            return i;
    }
    return 0;
}
// 自下而上修改父节点的相关值
void BplusTree::p_change(BPlusTreeNode* p_node, int key, int c_key)
{
    BPlusTreeNode* tp_node = p_node;
    // 当删除某结点中最大或者最小的关键字，就会涉及到更改其双亲结点一直到根结点中所有索引值的更改。
    while (tp_node)
    {
        for (int i = 0; i < tp_node->key_num; i++)
        {
            if (key == p_node->key[i])
            {
                p_node->key[i] = c_key;
                break;
            }
        }
        tp_node = tp_node->parent;
    }

}
// 自上而下修改子节点的相关值
void BplusTree::c_change(BPlusTreeNode* c_node, BPlusTreeNode* p_node)
{
    BPlusTreeNode* cd_node = c_node;
    int i = 0;
    while (cd_node)
    {
        p_node->child[i] = cd_node;
        cd_node->parent = p_node;
        cd_node = cd_node->next;
        i++;
    }

}
// 删除操作(没写完)
void BplusTree::delect(BPlusTreeNode* node, int key)
{
    info* f_info = find(node, key);
    BPlusTreeNode* f_node = f_info->node;
    BPlusTreeNode* p_node = f_node->parent;
    int loc = f_info->loc;
    for (int i = loc; i < f_node->key_num; i++)
    {
        f_node->key[i] = f_node->key[i + 1];
        f_node->name[i] = f_node->name[i + 1];
    }
    // 当结点中关键字个数大于⌈M/2⌉，做删除操作不会破坏 B+树，则可以直接删除。
    if (f_node->key_num > ceil(ORDER / 2))
    {
        // 当删除某结点中最大或者最小的关键字，就会涉及到更改其双亲结点一直到根结点中所有索引值的更改。
        if (loc == (f_node->key_num - 1) || loc == 0)
        {
            int c_key = 0;
            if (loc == 0)
                c_key = f_node->key[1];
            else
                c_key = f_node->key[loc - 1];
            p_change(p_node, key, c_key);
        }
        f_node->key_num--;
        return;
    }
    //当删除该关键字，导致当前结点中关键字个数小于 ⌈M/2⌉，若其兄弟结点中含有多余的关键字，可以从兄弟结点中借关键字完成删除操作
    else if (f_node->key_num <= ceil(ORDER / 2))
    {
        BPlusTreeNode* pl_node = f_node->parent;
        //若其兄弟结点中含有多余的关键字，可以从兄弟结点中借关键字完成删除操作
        int c_loc = get_loc(f_node->parent, f_node);
        // 若左兄弟节点有多余的值可以借用，则将左兄弟的最大值借用
        if (pl_node->child[c_loc - 1] && pl_node->child[c_loc - 1]->key_num > ceil(ORDER / 2))
        {
            BPlusTreeNode* b_node = pl_node->child[c_loc - 1];
            for (int i = f_node->key_num - 1; i > 0; i--)
            {
                f_node->key[i + 1] = f_node->key[i];
                f_node->name[i + 1] = f_node->name[i];
            }
            f_node->key[0] = b_node->key[b_node->key_num - 1];
            f_node->name[0] = b_node->name[b_node->key_num - 1];
            b_node->key_num--;
            p_change(b_node->parent, key, f_node->key[0]);
        }
        // 若右兄弟节点有多余的值可以借用，则将右兄弟的最小值借用
        else if (pl_node->child[c_loc + 1] && pl_node->child[c_loc - 1]->key_num > ceil(ORDER / 2))
        {
            BPlusTreeNode* g_node = pl_node->child[c_loc + 1];
            int temp = g_node->key[0];
            string* s_temp = new string(*g_node->name[0]);
            for (int i = 0; i < g_node->key_num - 1; i++)
            {
                g_node->key[i] = g_node->key[i + 1];
            }
            f_node->key[f_node->key_num - 1] = temp;
            f_node->name[f_node->key_num - 1] = s_temp;
            g_node->key_num--;
            p_change(g_node->parent, key, g_node->key[0]);
        }
        // 如果都没有可以借用的，就合并【还没写】
        else
        {
            BPlusTreeNode* b_node = pl_node->child[c_loc - 1];
        }
    }
}
// 遍历一层的数值
void BplusTree::s_ceng(BPlusTreeNode* p_node)
{
    BPlusTreeNode* temp = p_node;
    while (temp)
    {
        for (int i = 0; i < temp->key_num; i++)
        {
            cout << temp->key[i] << " ";
        }
        temp = temp->next;
        cout << "   ";
    }
}
// 查看树的结构
void BplusTree::all(BPlusTreeNode* node)
{
    BPlusTreeNode* temp = node;
    while (temp)
    {
        s_ceng(temp);
        cout << endl;
        temp = temp->child[0];
    }
}

// 单个节点序列化并保存操作[写完了，有待优化]
void BplusTree::save(BPlusTreeNode* node)
{
    // E:\vs programe\bplustree\x64\Debug\data
    string path = "./data//node";
    path.append(to_string(node->id));
    path.append(".txt");
    int fp;
    fp=open(path.c_str(),O_RDWR|O_CREAT,0777);
    // 保存相关值
    // id
    // cout<<"sizeofnod id"<<sizeof(to_string(node->id).c_str())<<endl;
    write(fp,to_string(node->id).c_str(),to_string(node->id).size());
    write(fp,"\n", 1);
    // is_leaf
    write(fp,to_string(node->is_leaf).c_str(),to_string(node->is_leaf).size());
    write(fp,"\n", 1);
    // pid
    write(fp,to_string(node->p_id).c_str(), to_string(node->p_id).size());
    write(fp,"\n", 1);
    // bid
    write(fp,to_string(node->b_id).c_str(), to_string(node->b_id).size());
    write(fp,"\n", 1);
    // key_num
    write(fp,to_string(node->key_num).c_str(), to_string(node->key_num).size());
    write(fp,"\n", 1);
    // name
    if (node->is_leaf)
    {
        string leaf_str;
        for (int i = 0; i < node->key_num; i++)
        {
            leaf_str.append(node->name[i]->c_str());
            leaf_str.append(" ");
        }
        leaf_str.erase(leaf_str.end() - 1);
        write(fp,leaf_str.c_str(), leaf_str.size());
        write(fp,"\n", 1);
    }
    // key
    string k_str;
    for (int i = 0; i < node->key_num;i++)
    {
        k_str.append(to_string(node->key[i]).c_str());
        k_str.append(" ");
    }
    k_str.erase(k_str.end() - 1);
    write(fp,k_str.c_str(), k_str.size());
    write(fp,"\n", 1);
    // child_id
    string cld_str;
    for (int i = 0; i < ORDER; i++)
    {
        if (node->c_id[i] == -1)
            break;
        cld_str.append(to_string(node->c_id[i]));
        cld_str.append(" ");
    }
    cld_str.erase(cld_str.end() - 1);
    write(fp,cld_str.c_str(), cld_str.size());
    write(fp,"\n", 1);
    close(fp);
    cout << path << endl;
}
// 保存一层节点
void BplusTree::save_ceng(BPlusTreeNode* node)
{
    BPlusTreeNode* temp = node;
    while (temp)
    {
        save(temp);
        temp = temp->next;
        //cout << "   ";
    }
}
// 保存全部
void BplusTree::save_all(BPlusTreeNode* node)
{
    // 先把根节点的id和总共的数量保存至之root空间
    string path = "./root/node";
    path.append(".txt");
    cout<<path<<endl;
    int fp;
    fp=open(path.c_str(), O_RDWR|O_CREAT,0777);
    write(fp,to_string(node->id).c_str(), to_string(node->id).size());
    write(fp,"\n", 1);
    write(fp,to_string(node_num).c_str(), to_string(node_num).size());
    close(fp);
    BPlusTreeNode* temp = node;
    while (temp)
    {
        save_ceng(temp);
        temp = temp->child[0];
    }
}

// bplustree.cpp: 定义应用程序的入口点。
//
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
            node->child[i]=load_node(c_id,node_arr);
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
            if (key == p_node->key[i])
            {
                p_node->key[i] = c_key;
                node_arr[p_node->id]->is_changed=true;
                break;
            }
        }
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
            cd_node->next=load_node(cd_node->b_id,node_arr);
        }
            cd_node = cd_node->next;
        i++;
    }
}
// 删除操作(没写完)(写完)(还没改)(ok)
void BplusTree::delect(BPlusTreeNode* node, int key,vector<PNodeInfo> &node_arr)
{
    info* f_info = find(node, key,node_arr);
    if(f_info==nullptr)
    {
        cout<<"can't find this data,please check your input"<<endl;
        return ;
    }
    node_arr[f_info->node->id]->is_changed=true;
    BPlusTreeNode* f_node = f_info->node;
    BPlusTreeNode* p_node = f_node->parent;
    int loc = f_info->loc;
    int key_l = f_node->key[0];
    for (int i = loc; i < f_node->key_num; i++)
    {
        f_node->key[i] = f_node->key[i + 1];
        f_node->name[i] = f_node->name[i + 1];
    }
    // 当结点中关键字个数大于⌈M/2⌉，做删除操作不会破坏 B+树，则可以直接删除。
    if (f_node->key_num > (ORDER-1)/2+1)
    {
        // 当删除某结点中最大或者最小的关键字，就会涉及到更改其双亲结点一直到根结点中所有索引值的更改。
        if (loc == 0)
        {
            int c_key = 0;
            if (loc == 0)
                c_key = f_node->key[0];
            else
                c_key = f_node->key[loc - 1];
            p_change(p_node, key, c_key,node_arr);
        }
        f_node->key_num--;
        return;
    }
    //当删除该关键字，导致当前结点中关键字个数小于 ⌈M/2⌉，若其兄弟结点中含有多余的关键字，可以从兄弟结点中借关键字完成删除操作
    else if (f_node->key_num <= (ORDER-1)/2+1)
    {
        BPlusTreeNode* pl_node = f_node->parent;
        //若其兄弟结点中含有多余的关键字，可以从兄弟结点中借关键字完成删除操作
        int c_loc = get_loc(f_node->parent, f_node);
        // 加载左节点
        int l_loc=pl_node->c_id[c_loc-1];
        if(l_loc>-1&&node_arr[l_loc]->is_loaded==false)
            pl_node->child[c_loc - 1]=load_node(l_loc,node_arr); 
        // 加载右节点
        int r_loc=pl_node->c_id[c_loc+1];
        if(r_loc>0&&node_arr[r_loc]->is_loaded==false)
            pl_node->child[c_loc + 1]=load_node(r_loc,node_arr); 
        // 若左兄弟节点有多余的值可以借用，则将左兄弟的最大值借用
        if (pl_node->child[c_loc - 1] && pl_node->child[c_loc - 1]->key_num >(ORDER-1)/2+1)
        {
            node_arr[l_loc]->is_changed=true;
            BPlusTreeNode* b_node = pl_node->child[c_loc - 1];
            for (int i = f_node->key_num - 2; i >= 0; i--)
            {
                f_node->key[i + 1] = f_node->key[i];
                f_node->name[i + 1] = f_node->name[i];
            }
            f_node->key[0] = b_node->key[b_node->key_num - 1];
            f_node->name[0] = b_node->name[b_node->key_num - 1];
            b_node->key_num--;
            p_change(b_node->parent, key, f_node->key[0],node_arr);
        }
        // 若右兄弟节点有多余的值可以借用，则将右兄弟的最小值借用
        else if (pl_node->child[c_loc + 1] && pl_node->child[c_loc + 1]->key_num > (ORDER-1)/2+1)
        {
            node_arr[c_loc+1]->is_changed=true;
            BPlusTreeNode* g_node = pl_node->child[c_loc + 1];
            int temp = g_node->key[0];
            string* s_temp = new string(*g_node->name[0]);
            for (int i = 0; i < g_node->key_num - 2; i++)
            {
                g_node->key[i] = g_node->key[i + 1];
            }
            f_node->key[f_node->key_num - 1] = temp;
            f_node->name[f_node->key_num - 1] = s_temp;
            g_node->key_num--;
            p_change(g_node->parent, key, g_node->key[0],node_arr);
        }
        // 如果都没有可以借用的，就合并【还没写】
        else
        {
            // 优先合并左边的
            if (pl_node->child[c_loc - 1])
            {
                node_arr[pl_node->child[c_loc - 1]->id]->is_changed=true;
                BPlusTreeNode* l_node = pl_node->child[c_loc - 1];
                int num = l_node->key_num;
                // 先将节点的值全部移植到左边
                for (int i = 0; i < f_node->key_num - 1; i++)
                {
                    l_node->key[num +i] = f_node->key[i];
                    if (l_node->is_leaf)
                        l_node->name[num +i]= f_node->name[i];
                    l_node->key_num++;
                }
                //[后续还要更新左节点的next]
                l_node->next = f_node->next;
                l_node->b_id = f_node->b_id;
                // 且如果父节点key值只有一个，则删除父节点，root指向该节点
                if (pl_node->key_num == 1)
                {
                    root = l_node;
                    //rm_node_arr.push_back(pl_node->id);
                    node_arr[pl_node->id]->is_removed=true;
                    delete(pl_node);
                }
                // 如果不止一个，就只需要删除父节点中的相关值即可（1.key值2.孩子节点）
                else
                {
                    int loc = get_loc(pl_node, f_node);
                    BPlusTreeNode* loc_node = pl_node->child[loc];
                    for (int i = loc; i < pl_node->key_num + 1; i++)
                    {
                        pl_node->child[i] = pl_node->child[i + 1];
                        pl_node->c_id[i] = pl_node->c_id[i + 1];
                    }
                    int key_loc = find_key(pl_node, key_l);
                    for (int i = key_loc; i < pl_node->key_num; i++)
                    {
                        pl_node->key[i] = pl_node->key[i + 1];
                    }
                    pl_node->key_num--;
                }
                // 最后删除掉该节点
                node_arr[f_node->id]->is_removed=true;
                //rm_node_arr.push_back(f_node->id);
                delete(f_node);
            }
            // 若右兄弟节点有多余的值可以借用(说明只有两个节点，则直接合并为根节点，不用写那么多了)
            else
            {
                node_arr[pl_node->child[c_loc + 1]->id]->is_changed=true;
                BPlusTreeNode* r_node = pl_node->child[c_loc + 1];
                int temp_keynum=f_node->key_num;
                // 先将节点的值全部移植到左边
                for (int i = 0; i < r_node->key_num; i++)
                {
                    f_node->key[temp_keynum -1+ i] = r_node->key[i];
                    if (f_node->is_leaf)
                        f_node->name[temp_keynum-1 + i] = r_node->name[i];
                    f_node->key_num++;
                }
                f_node->key_num--;
                //[后续还要更新左节点的next]
                f_node->next = r_node->next;
                f_node->b_id = r_node->b_id;
                // 且如果父节点key值只有一个，则删除父节点，root指向该节点
                if (pl_node->key_num == 1)
                {
                    root = f_node;
                    node_arr[pl_node->id]->is_removed=true;
                    //rm_node_arr.push_back(pl_node->id);
                    delete(pl_node);
                }
                // 如果不止一个，就只需要删除父节点中的相关值即可（1.key值2.孩子节点）
                else
                {
                    int loc = get_loc(pl_node, r_node);
                    BPlusTreeNode* loc_node = pl_node->child[loc];
                    for (int i = loc; i < pl_node->key_num + 1; i++)
                    {
                        pl_node->child[i] = pl_node->child[i + 1];
                        pl_node->c_id[i] = pl_node->c_id[i + 1];
                    }
                    int key_loc = find_key(pl_node, r_node->key[0]);
                    for (int i = key_loc; i < pl_node->key_num; i++)
                    {
                        pl_node->key[i] = pl_node->key[i + 1];
                    }
                    pl_node->key_num--;
                }
                // 最后删除掉该节点
                node_arr[r_node->id]->is_removed=true;
                // rm_node_arr.push_back(r_node->id);
                delete(r_node);
            }

        }
    }
}
// 遍历一层的数值(还没改)（已改）
void BplusTree::s_ceng(BPlusTreeNode* p_node,vector<PNodeInfo> &node_arr)
{
    BPlusTreeNode* temp = p_node;
    while (temp)
    {
        for (int i = 0; i < temp->key_num; i++)
        {
            cout << temp->key[i] << " ";
        }
        if(temp->b_id!=-1)
        {
            if(node_arr[temp->b_id]->is_loaded==true)
            {
                temp->next=node_arr[temp->b_id]->node;
                temp = temp->next;
            }
            else
            {
                temp->next=load_node(temp->b_id,node_arr);
                temp=temp->next;
            }
        }
        else
            temp=nullptr;
        cout << "   ";
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
                temp->child[0]=load_node(temp->c_id[0],node_arr);
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
}  


// 单个节点序列化并保存操作[写完了，有待优化]
void BplusTree::save(BPlusTreeNode* node)
{
    // E:\vs programe\bplustree\x64\Debug\data
    string path="/home/wanghao/study_source/Bplustree/build/data/node";
    //string path = "./data//node";
    path.append(to_string(node->id));
    path.append(".txt");
    int fp;
    fp=open(path.c_str(),O_RDWR|O_CREAT,0777);
     /* 清空文件 */
    ftruncate(fp,0);

    /* 重新设置文件偏移量 */
    lseek(fp,0,SEEK_SET);
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
// 保存一层节点(还没改)
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
    //string path = "./root/node";
    string path="/home/wanghao/study_source/Bplustree/build/root/node";
    path.append(".txt");
    cout<<path<<endl;
    int fp;
    fp=open(path.c_str(), O_RDWR|O_CREAT,0777);
    write(fp,to_string(node->id).c_str(), to_string(node->id).size());
    write(fp,"\n", 1);
    write(fp,to_string(node_num).c_str(), to_string(node_num).size());
    cout<<to_string(node_num)<<endl;
    close(fp);
    BPlusTreeNode* temp = node;
    while (temp)
    {
        save_ceng(temp);
        temp = temp->child[0];
    }
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

// 加载节点
BPlusTreeNode* BplusTree::load_node(int t_id,vector<PNodeInfo>& node_arr)
{
    //string path = "./data/node";
    string path="/home/wanghao/study_source/Bplustree/build/data/node";
    path.append(to_string(t_id));
    path.append(".txt");
	int fp;
	fp=open(path.c_str(),O_RDONLY|O_SYNC);
    if(fp==-1)
    {
        cout<<"open file false:node"<<t_id<<endl;
        return nullptr;
    }
	// id
	int id=stoi(read_one(fp));
	// is_leaf
	bool is_leaf=stoi(read_one(fp));
	// pid
	int pid=stoi(read_one(fp));
	// bid
	int bid=stoi(read_one(fp));
	// key_num
	int key_num=stoi(read_one(fp));
	// name
	string *name;
	if(is_leaf)
	{
		name=read_name(fp,key_num);
		//cout<<name[0]<<endl;
	}
	// key
	int* k_arr=read_many(fp,key_num);
	//cout<<k_arr[0]<<endl;
	// child_id
    int* c_arr;
	if(!is_leaf) 
	{
		c_arr=read_many(fp,key_num+1);
		//cout<<c_arr[0]<<endl;
	}
	// build the node and give value to it
	BPlusTreeNode* new_node=new BPlusTreeNode();
	new_node->b_id=bid;
	new_node->p_id=pid;
	new_node->id=id;
	if(pid!=-1)
		new_node->parent=node_arr[pid]->node;
	new_node->is_leaf=is_leaf;
	new_node->key_num=key_num;
	for(int i=0;i<key_num;i++)
	{
		new_node->key[i]=k_arr[i];
	}
	if(is_leaf)
	{
		for(int i=0;i<key_num;i++)
			new_node->name[i]=&name[i];
	}
    if(!is_leaf)
    {
        for(int i=0;i<=key_num;i++)
            new_node->c_id[i]=c_arr[i];
    }
    // cout<<node_arr[id]->is_changed<<endl;
	node_arr[id]->node=new_node;
    node_arr[id]->is_loaded=true;
    node_arr[id]->is_changed=false;
    close(fp);
	return new_node;
}

void BplusTree::save_changed(BPlusTreeNode* node,vector<PNodeInfo> &node_arr)
{
    // 先把根节点的id和总共的数量保存至之root空间
    //string path = "./root/node";
    string path="/home/wanghao/study_source/Bplustree/build/root/node";
    path.append(".txt");
    cout<<path<<endl;
    int fp;
    fp=open(path.c_str(), O_RDWR|O_CREAT,0777);
    write(fp,to_string(node->id).c_str(), to_string(node->id).size());
    write(fp,"\n", 1);
    write(fp,to_string(node_num).c_str(), to_string(node_num).size());
    cout<<to_string(node_num)<<endl;
    close(fp);
    for(int i=0;i<node_arr.size();i++)
    {
        if(node_arr[i]->is_changed==true)
        {
            if(node_arr[i]->is_removed==true)
            {
                string path="/home/wanghao/study_source/Bplustree/build/data/node";
                path.append(to_string(i));
                path.append(".txt");
                remove(path.c_str());
            }
            else
                save(node_arr[i]->node);
        }
    }
}
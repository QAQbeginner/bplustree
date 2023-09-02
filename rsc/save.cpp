#include "bplustree.h"

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

BPlusTreeNode* BplusTree::n_load_node(int t_id,vector<PNodeInfo> &node_arr)
{
    // open file
    int file_loc=(t_id%10000)*8;
    int file_id=t_id/10000;
    string path="/home/wanghao/study_source/Bplustree/build/data/node";
    path.append(to_string(file_id));
    path.append(".txt");
	int fp;
	fp=open(path.c_str(),O_RDONLY|O_SYNC);
    if(fp==-1)
    {
        cout<<"open file false:node"<<t_id<<endl;
        return nullptr;
    }
    set_line(fp,file_loc);
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
    else{
        set_line(fp,1);
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
    return nullptr;

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
    if(node->id==11024)
        cout<<node->key_num<<endl;
    write(fp,k_str.c_str(), k_str.size());
    write(fp,"\n", 1);
    // child_id
    string cld_str;
    for (int i = 0; i <= node->key_num; i++)
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
    // cout << path << endl;
}

void BplusTree::save_changed(BPlusTreeNode* node,vector<PNodeInfo> &node_arr)
{
    // 先把根节点的id和总共的数量保存至之root空间
    std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
    //string path = "./root/node";
    string path="/home/wanghao/study_source/Bplustree/build/root/node";
    path.append(".txt");
    // cout<<path<<endl;
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
                save_new(node_arr[i]->node);
        }
    }
    std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();
	cout<<std::chrono::duration_cast<chrono::microseconds>(t2-t1).count()<<"微秒"<<endl;
}

void BplusTree::save_new(BPlusTreeNode* node)
{
    int file_id=node->id/10000;
    string path="/home/wanghao/study_source/Bplustree/build/data/node";
    //string path = "./data//node";
    path.append(to_string(file_id));
    path.append(".txt");
    int fp;
    fp=open(path.c_str(),O_RDWR|O_CREAT,0777);
    
    // add node data to the bottle

    /* 重新设置文件偏移量 */
    lseek(fp,0,SEEK_END);
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
    else{write(fp,"\n", 1);}
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
    for (int i = 0; i <=node->key_num; i++)
    {
        if (node->c_id[i] == -1)
            break;
        cld_str.append(to_string(node->c_id[i]));
        cld_str.append(" ");
    }
    cld_str.erase(cld_str.end() - 1);
    if(cld_str.size()==0)
         write(fp,"\n", 1);
    else
        write(fp,cld_str.c_str(), cld_str.size());
    write(fp,"\n", 1);
    close(fp);
    // cout << path << endl;
}
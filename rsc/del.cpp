#include "bplustree.h"
int BplusTree::get_node_loc(BPlusTreeNode* node,BPlusTreeNode* p_node)
{
	int loc=0;
	while(p_node->c_id[loc]!=node->id)
		loc++;
	return loc;
}

// 非叶子节点的合并,r_node合并到l_node
void BplusTree::combine(BPlusTreeNode* l_node,BPlusTreeNode* r_node,vector<PNodeInfo> &node_arr)
{
	BPlusTreeNode* p_node=l_node->parent;
	int loc=get_node_loc(r_node,p_node);
	for(int i=loc;i<p_node->key_num;i++)
	{
		p_node->child[i]=p_node->child[i+1];
		p_node->c_id[i]=p_node->c_id[i+1];

	}
	int num=l_node->key_num;
	l_node->key[num]=p_node->key[loc-1];
	l_node->next=r_node->next;
	l_node->b_id=r_node->b_id;
	l_node->key_num++;
	for(int i=0;i<r_node->key_num;i++)
	{
		l_node->key[num+i+1]=r_node->key[i];
		l_node->key_num++;
	}
	for(int i=0;i<=r_node->key_num;i++)
	{
		l_node->c_id[num+i+1]=r_node->c_id[i];
		l_node->child[num+i+1]=r_node->child[i];
		l_node->child[num+i+1]->parent=l_node;
		l_node->child[num+i+1]->p_id=l_node->id;
	}
	if(l_node->key_num==ORDER)
		split(l_node,node_arr);
	if(p_node->key_num==1)
	{
		root=l_node;
		node_arr[p_node->id]->is_removed=true;
		delete p_node;
		p_node=nullptr;
	}
	else
	{
		delect(p_node,p_node->key[loc-1],node_arr);
	}

	node_arr[r_node->id]->is_removed=true;
	delete r_node;
	r_node=nullptr;
}

// 非叶子节点
// 左边向右边借
void BplusTree::borrow_l(BPlusTreeNode* l_node,BPlusTreeNode* r_node,vector<PNodeInfo> &node_arr)
{
	BPlusTreeNode* p_node=l_node->parent;
	int loc=get_node_loc(r_node,p_node);
	int num=l_node->key_num;
	l_node->key[num]=p_node->key[loc-1];
	l_node->c_id[num+1]=r_node->c_id[0];
	l_node->child[num+1]=r_node->child[0];

	l_node->child[num+1]->parent=l_node;
	l_node->child[num+1]->p_id=l_node->id;


	l_node->key_num++;
	p_node->key[loc-1]=r_node->key[0];
	for(int i=0;i<r_node->key_num;i++)
	{
		r_node->key[i]=r_node->key[i+1];
	}
	for(int i=0;i<=r_node->key_num;i++)
	{
		r_node->c_id[i]=r_node->c_id[i+1];
		r_node->child[i]=r_node->child[i+1];
	}
	r_node->key_num--;
}
// 右边向左边借
void BplusTree::borrow_r(BPlusTreeNode* r_node,BPlusTreeNode* l_node,vector<PNodeInfo> &node_arr)
{
	BPlusTreeNode* p_node=l_node->parent;
	int loc=get_node_loc(r_node,p_node);
	int num=r_node->key_num;
	for(int i=r_node->key_num;i>0;i--)
	{
		r_node->key[i]=r_node->key[i-1];
	}
	for(int i=r_node->key_num+1;i>0;i--)
	{
		r_node->c_id[i]=r_node->c_id[i-1];
		r_node->child[i]=r_node->child[i-1];
	}
	r_node->key[0]=p_node->key[loc-1];
	p_node->key[loc-1]=l_node->key[l_node->key_num-1];
	r_node->c_id[0]=l_node->c_id[l_node->key_num];
	r_node->child[0]=l_node->child[l_node->key_num];

	r_node->child[0]->parent=r_node;
	r_node->child[0]->p_id=r_node->id;
	l_node->key_num--;
	r_node->key_num++;
}

// 非叶子节点
void BplusTree::deal_node(BPlusTreeNode* node,vector<PNodeInfo> &node_arr)
{
	BPlusTreeNode* p_node=node->parent;
	int loc=get_node_loc(node,p_node);
	if(loc>0)
	{
		BPlusTreeNode* l_node=p_node->child[loc-1];
		if(l_node->key_num<=(ORDER)/2)
		{
			 combine(l_node,node,node_arr);
		}
		else
		{
			borrow_r(node,l_node,node_arr);
		}
	}
	else
	{
		BPlusTreeNode* r_node=p_node->child[loc+1];
		if(r_node->key_num<=(ORDER)/2)
		{
			combine(node,r_node,node_arr);
		}
		else
		{
			borrow_l(node,r_node,node_arr);
		}
	}
}

// 叶子节点
void BplusTree::deal_y_node(BPlusTreeNode* node,vector<PNodeInfo> &node_arr,int key,int key_l)
{
	BPlusTreeNode* p_node = node->parent;
	int loc=get_node_loc(node,p_node);
    // int key_l=node->key[0];
	// 加载左节点
    int l_loc=p_node->c_id[loc-1];
    if(l_loc>-1&&node_arr[l_loc]->is_loaded==false)
    	p_node->child[loc - 1]=load_node(l_loc,node_arr); 
    // 加载右节点
    int r_loc=p_node->c_id[loc+1];
    if(r_loc>0&&node_arr[r_loc]->is_loaded==false)
    	p_node->child[loc + 1]=load_node(r_loc,node_arr); 
    // 若左兄弟节点有多余的值可以借用，则将左兄弟的最大值借用
    if (p_node->child[loc - 1] && p_node->child[loc - 1]->key_num >(ORDER)/2)
    {
    	node_arr[l_loc]->is_changed=true;
    	BPlusTreeNode* b_node = p_node->child[loc - 1];
    	for (int i = node->key_num - 1; i >= 0; i--)
    	{
    		node->key[i+1]=node->key[i];
    		node->name[i + 1] = node->name[i];
    	}
    	node->key[0] = b_node->key[b_node->key_num - 1];
        node->name[0] = b_node->name[b_node->key_num - 1];
        b_node->key_num--;
		node->key_num++;
        p_change(b_node->parent, key_l, node->key[0],node_arr);
    }
    else if(loc<p_node->key_num&&p_node->child[loc + 1] && p_node->child[loc + 1]->key_num > (ORDER)/2)
    {
    	node_arr[loc+1]->is_changed=true;
        BPlusTreeNode* g_node = p_node->child[loc + 1];
        int temp = g_node->key[0];
        string* s_temp = new string(*g_node->name[0]);
        for (int i = 0; i < g_node->key_num - 1; i++)
        {
        	g_node->key[i] = g_node->key[i + 1];
			g_node->name[i]=g_node->name[i+1];
        }
        node->key[node->key_num] = temp;
        node->name[node->key_num] = s_temp;
        g_node->key_num--;
		node->key_num++;
        p_change(g_node->parent, temp, g_node->key[0],node_arr);
		if(key_l!=node->key[0])
			p_change(node->parent,key_l,node->key[0],node_arr);
    }
    // 如果都没有可以借用的，就合并
    else
    {
    	// 优先合并左边的
    	if(p_node->child[loc - 1])
    	{
    		combine_y_node(p_node->child[loc - 1],node,node_arr,key_l);
    	}
    	else
    	{
			if(key_l!=node->key[0])
			p_change(node->parent,key_l,node->key[0],node_arr);
    		combine_y_node(node,p_node->child[loc + 1],node_arr,p_node->child[loc + 1]->key[0]);
    	}
    }
}

void BplusTree::combine_y_node(BPlusTreeNode* l_node,BPlusTreeNode* r_node,vector<PNodeInfo> &node_arr,int key_l)
{
	// int key_l=r_node->key[0];
	BPlusTreeNode* p_node=l_node->parent;
	node_arr[l_node->id]->is_changed=true;
    int num=l_node->key_num;
	// 先将节点的值全部移植到左边
	for (int i = 0; i < r_node->key_num; i++)
    {
    	l_node->key[num +i] = r_node->key[i];
      	l_node->name[num +i]= r_node->name[i];
        l_node->key_num++;
    }
    //[后续还要更新左节点的next]
    l_node->next = r_node->next;
    l_node->b_id = r_node->b_id;
    // 且如果父节点key值只有一个，则删除父节点，root指向该节点
    if (p_node->key_num == 1)
    {
    	root = l_node;
     	node_arr[p_node->id]->is_removed=true;
     	delete p_node;
		p_node=nullptr;
    }
    // 如果不止一个，就只需要删除父节点中的相关值即可（1.key值2.孩子节点）
    else
    {
    	int loc = get_node_loc(r_node, p_node);
        BPlusTreeNode* loc_node = p_node->child[loc];
        // 找到被合并右节点在父节点的位置
        for (int i = loc; i < p_node->key_num + 1; i++)
        {
        	p_node->child[i] = p_node->child[i + 1];
        	p_node->c_id[i] = p_node->c_id[i + 1];
        }
        // 右节点被合并后，其父节点中的相关值也要被删除
        int key_loc = find_key(p_node, key_l);
        for (int i = key_loc; i < p_node->key_num; i++)
        {
        	p_node->key[i] = p_node->key[i + 1];
        }
        p_node->key_num--;
        if(p_node->key_num<ORDER/2&&p_node!=root)
        	deal_node(p_node,node_arr);
    }
    // 最后删除掉该节点
    node_arr[r_node->id]->is_removed=true;
	delete r_node;
	r_node=nullptr;
}





void BplusTree::delect(BPlusTreeNode* node,int key,vector<PNodeInfo> &node_arr)
{
	int loc=0;
    int key_l=node->key[0];
	while(node->key[loc]!=key)
		loc++;
	if(node->is_leaf)
	{
		delete node->name[loc];
		node->name[loc]=nullptr;
	}
	for(int i=loc;i<node->key_num;i++)
	{
		node->key[i]=node->key[i+1];
		if(node->is_leaf==true)
		{
			node->name[i] = node->name[i + 1];
		}
	}
	if(node->key_num>ORDER/2)
	{
		if (loc == 0&&node->is_leaf==true&&root!=node)
        {
            int c_key = 0;
            if (loc == 0)
                c_key = node->key[0];
            else
                c_key = node->key[loc - 1];
            // 自下而上修改值
            p_change(node->parent, key, c_key,node_arr);
        }
        node->key_num--;
        return;
	}
	else if(node->key_num<=ORDER/2)
	{

        if(node!=root)
		{
			node->key_num--;
			if(node->is_leaf)
				deal_y_node(node,node_arr,key,key_l);
			else
				deal_node(node,node_arr);
		}
        else
            node->key_num--;
	}
}


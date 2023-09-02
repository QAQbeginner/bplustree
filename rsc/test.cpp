#include"test.h"

void test_insert(BplusTree *m_tree,vector<PNodeInfo> &node_arr,int n)
{
	for(int i=1;i<=n;i++)
	{
		string name=to_string(i)+"test";
		string *c_name=new string(name);
		m_tree->insert(m_tree->root, i, c_name,node_arr);
	}
}

void random_insert(BplusTree *m_tree,vector<PNodeInfo> &node_arr,int n)
{
	vector<int> temp;
	for (int i = 0; i < n; ++i)
	{
		temp.push_back(i + 1);
	}
	random_shuffle(temp.begin(), temp.end());
	std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
	for (int i = 0; i < temp.size(); i++)
	{
		//cout << temp[i] << " ";
		string name=to_string(temp[i])+"test";
		string *c_name=new string(name);
		m_tree->insert(m_tree->root, temp[i], c_name,node_arr);
	}
	std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();
	cout<<std::chrono::duration_cast<chrono::microseconds>(t2-t1).count()<<"微秒"<<endl;
}

void random_search(BplusTree *m_tree,vector<PNodeInfo> &node_arr,int n)
{
    vector<int> temp;
    for (int i = 0; i < n; ++i)
	{
		temp.push_back(i + 1);
	}
    random_shuffle(temp.begin(), temp.end());
    std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
    for (int i = 0; i < temp.size(); i++)
	{
		m_tree->find(m_tree->root,temp[i],node_arr);
	}
    std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();
	cout<<std::chrono::duration_cast<chrono::microseconds>(t2-t1).count()<<"微秒"<<endl;
}

void random_delete(BplusTree *m_tree,vector<PNodeInfo> &node_arr,int n)
{
    vector<int> temp;
    for (int i = 0; i < n; ++i)
	{
		temp.push_back(i + 1);
	}
    random_shuffle(temp.begin(), temp.end());




    std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
    for (int i = 0; i < temp.size(); i++)
	{
		info* f_info = m_tree->find(m_tree->root, temp[i],node_arr);
    	if(f_info==nullptr)
    	{
        	cout<<"can't find this data,please check your input"<<endl;
			continue;
   		}
    	node_arr[f_info->node->id]->is_changed=true;
    	BPlusTreeNode* f_node = f_info->node;
    	BPlusTreeNode* p_node = f_node->parent;
		m_tree->delect(f_node,temp[i],node_arr);
	}
    std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();
	cout<<std::chrono::duration_cast<chrono::microseconds>(t2-t1).count()<<"微秒"<<endl;
}
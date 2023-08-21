
#include "test.h"
void xunhuan(BplusTree *m_tree,vector<PNodeInfo> &node_arr);
int main()
{
	int root_node_id=1;
	int node_num=1;
	bool is_kong=true;
	init(root_node_id,node_num,is_kong);
	vector<PNodeInfo> node_arr;
	for(int i=0;i<=node_num;i++)
	{ 
		PNodeInfo t_node_info=new NodeInfo();
		node_arr.push_back(t_node_info);
	}
	BplusTree *m_tree=new BplusTree();
	if(is_kong==true)
	{
		delete m_tree->root;
		node_arr[1]->node=new BPlusTreeNode();
		node_arr[1]->node->is_leaf=true;
		node_arr[1]->node->id=node_num;
		node_arr[1]->is_loaded=true;
		node_arr[1]->is_changed=true;
		m_tree->root=node_arr[1]->node;
	}
	else
	{
		delete m_tree->root;
		m_tree->root=BplusTree::load_node(root_node_id,node_arr);
	}
	m_tree->node_num=node_num;
	xunhuan(m_tree,node_arr);
	for(int i=0;i<node_arr.size();i++)
	{
		if(node_arr[i]->node!=nullptr)
		{
			if(node_arr[i]->node->is_leaf==true)
			{
				for(int j=0;j<node_arr[i]->node->key_num;j++)
					delete node_arr[i]->node->name[j];
			}
			delete node_arr[i]->node;
		}
	}
	for(int i=0;i<node_arr.size();i++)
	{
		delete node_arr[i];
	}
	delete m_tree;
	return 0;
}
void xunhuan(BplusTree *m_tree,vector<PNodeInfo> &node_arr)
{
	while (true)
	{
		//system("clear");
		char choose = 0;
		cout << "please input your choose" << endl;
		cout << "1:insert 2:find 3:all 4:save 5:change 6:delect 7:rm all files 8:insert n datas 9:insert random datas" << endl;
		cout << "q:quit 10:random_search 11:random_delete "<<endl;
		choose = getchar();
		switch (choose)
		{
		case '1':
		{
			system("clear");
			int score;
			cout << "please input your score" << endl;
			cin >> score;
			if(m_tree->find(m_tree->root,score,node_arr)!=nullptr)
			{
				cout<<"the score is aready asistent"<<endl;
				continue;
			}
			string name;
			cout << "please input your name" << endl;
			cin >> name;
			string* c_name = new string(name);
			m_tree->insert(m_tree->root, score, c_name,node_arr);
		}break;
		case '2':
		{
			system("clear");
			int score;
			cout << "please input your find_score" << endl;
			cin >> score;
			m_tree->search(m_tree->root, score,node_arr);
		}break;
		case '3':
		{ 
			system("clear");
			m_tree->all(m_tree->root,node_arr);
		}break;
		case '4':
		{
			system("clear");
			m_tree->save_changed(m_tree->root,node_arr);
		}break;
		case '5':
		{
			system("clear");
			int key;
			cout<<"please input your key"<<endl;
			cin>>key;
			m_tree->change(m_tree->root,key,node_arr);
		}break;
		case '6':
		{
			system("clear");
			int key;
			cout<<"input your key"<<endl;
			cin>>key;
			info* f_info = m_tree->find(m_tree->root, key,node_arr);
    		if(f_info==nullptr)
    		{
        		cout<<"can't find this data,please check your input"<<endl;
				continue;
   			}
    		node_arr[f_info->node->id]->is_changed=true;
    		BPlusTreeNode* f_node = f_info->node;
    		BPlusTreeNode* p_node = f_node->parent;

			m_tree->delect(f_node,key,node_arr);
		}break;
		case '7':
		{
			rm_files();
			return ;
		}break;
		case '8':
		{
			int n;
			cout<<"please input the num of insert datas"<<endl;
			cin>>n;
			test_insert(m_tree,node_arr,n);
		}break;
		case '9':
		{
			int n;
			cout<<"please input the num of insert datas"<<endl;
			cin>>n;
			random_insert(m_tree,node_arr,n);
			// system("pause");
		}break;
		case 'q':
		{
			return ;
		}break;
		case 'w':
		{
			int n;
			cout<<"please input the num of search datas"<<endl;
			cin>>n;
			random_search(m_tree,node_arr,n);
		}break;
		}
	}
}

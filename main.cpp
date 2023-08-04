#include "bplustree.h"

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
	// vector<PNodeInfo> node_arr(node_num+1,new NodeInfo());
	// vector<PNodeInfo>* node_arr=new vector<PNodeInfo>[node_num];
	BplusTree *m_tree=new BplusTree();
	if(is_kong==true)
	{
		node_arr[1]->node=new BPlusTreeNode();
		node_arr[1]->node->is_leaf=true;
		node_arr[1]->node->id=node_num;
		node_arr[1]->is_loaded=true;
		m_tree->root=node_arr[1]->node;
	}
	else
		m_tree->root=BplusTree::load_node(root_node_id,node_arr);
	m_tree->node_num=node_num;
	xunhuan(m_tree,node_arr);
}
void xunhuan(BplusTree *m_tree,vector<PNodeInfo> &node_arr)
{
	while (true)
	{
		//system("clear");
		char choose = 0;
		cout << "please input your choose" << endl;
		cout << "1:insert 2:find 3:all 4:save 5:change 6:delect" << endl;
		choose = getchar();
		switch (choose)
		{
		case '1':
		{
			system("clear");
			int score;
			string name;
			cout << "please input your score" << endl;
			cin >> score;
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
			m_tree->save_all(m_tree->root);
		}break;
		case '5':
		{
			system("clear");
			int key;
			cin>>key;
			m_tree->change(m_tree->root,key,node_arr);
		}break;
		case '6':
		{
			system("clear");
			int key;
			cout<<"input your key"<<endl;
			cin>>key;
			m_tree->delect(m_tree->root,key,node_arr);
		}
		}
	}
}

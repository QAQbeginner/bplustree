#include "bplustree.h"

void xunhuan(BplusTree *m_tree,vector<PNodeInfo> &node_arr);
int main()
{
	int root_node_id=0;
	int node_num=0;
	init(root_node_id,node_num);
	vector<PNodeInfo> node_arr;
	for(int i=0;i<=node_num;i++)
	{
		PNodeInfo t_node_info=new NodeInfo();
		node_arr.push_back(t_node_info);
	}
	// vector<PNodeInfo> node_arr(node_num+1,new NodeInfo());
	// vector<PNodeInfo>* node_arr=new vector<PNodeInfo>[node_num];
	BplusTree *m_tree=new BplusTree();
	m_tree->root=BplusTree::load_node(root_node_id,node_arr);
	xunhuan(m_tree,node_arr);
}
void xunhuan(BplusTree *m_tree,vector<PNodeInfo> &node_arr)
{
	while (true)
	{
		char choose = 0;
		cout << "please input your choose" << endl;
		cout << "1:insert 2:find 3:all" << endl;
		choose = getchar();
		switch (choose)
		{
		case '1':
		{
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
			int score;
			cout << "please input your find_score" << endl;
			cin >> score;
			m_tree->search(m_tree->root, score,node_arr);
		}break;
		case '3':
		{
			m_tree->all(m_tree->root);
		}break;
		case '4':
		{
			m_tree->save_all(m_tree->root);
		}break;
		case '5':
		{
			// int fp=open("./data/node1.txt",O_RDWR);
			string path="./data/node1.txt";
			// load_node(path);
			return ;
		}
		case '6':
		{
			int key;
			cout<<"input your key"<<endl;
			cin>>key;
			m_tree->delect(m_tree->root,key,node_arr);
		}
		}
	}
}

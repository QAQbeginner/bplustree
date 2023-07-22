#include "bplustree.h"
#include "CTool.h"


int main()
{
	BplusTree* m_tree=new BplusTree;
	string* st = new string("a");
	for (int i = 1; i <= 5; i++)
	{
		m_tree->insert(m_tree->root, i, st);
	}
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
			m_tree->insert(m_tree->root, score, c_name);
		}break;
		case '2':
		{
			int score;
			cout << "please input your find_score" << endl;
			cin >> score;
			m_tree->search(m_tree->root, score);
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
			load_node(path);
			return 0;
		}
		}
	}
}
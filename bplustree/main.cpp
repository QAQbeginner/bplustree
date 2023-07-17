#include "bplustree.h"

int main()
{
	BplusTree m_tree;
	while (true)
	{
		int choose=0;
        cout<<"please input your choose"<<endl;
        cout<<"1:insert 2:find"<<endl;
        cin>>choose;
		switch(choose)
			{
			case 1:
			{
				int score;
				string name;
                cout<<"please input your score"<<endl;
				cin >> score;
                cout<<"please input your name"<<endl;
				cin >> name;
				m_tree.insert(m_tree.root,score, name);
                getchar();
			}break;
			case 2:
			{
				int score;
                cout<<"please input your find_score"<<endl;
				cin >> score;
				m_tree.find(m_tree.root,score);
                getchar();
			}break;
            case 3:
            {
                return 0;
            }
            }
	}
}
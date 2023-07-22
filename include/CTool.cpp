#include "CTool.h"

// 加载节点
BPlusTreeNode* load_node(string path)
{
	int fp;
	fp=open(path.c_str(),O_RDONLY|O_CREAT|O_SYNC);
	// id
	int id=stoi(read_one(fp));
	cout<<id<<endl;
	// is_leaf
	bool is_leaf=stoi(read_one(fp));
	cout<<is_leaf<<endl;
	// pid
	int pid=stoi(read_one(fp));
	cout<<pid<<endl;
	// bid
	int bid=stoi(read_one(fp));
	cout<<bid<<endl;
	// key_num
	int key_num=stoi(read_one(fp));
	cout<<key_num<<endl;
	// name
	if(is_leaf)
	{
		string* name=read_name(fp,key_num);
		cout<<name[1]<<endl;
	}
	// key
	int* k_arr=read_many(fp,key_num);
	cout<<k_arr[0]<<endl;
	// child_id
	// int* c_arr=read_many(fp,key_num+1);
	// cout<<c_arr[0]<<endl;
	return nullptr;
}

string read_one(int &fp)
{
	// 记录读取到的字符
		char temp;
		string ret_str;
		// 用于区分当前存储key还是value（遇到空格就是下一个值，即flag切换为1）
		int flag = 0;
		while (read(fp, &temp, 1) > 0)
		{
			if (temp == '\n')
			return ret_str;
			else if (temp == ' ')
			{
				//printf("have kongge\n");
				flag = 1;
				continue;
			}
     	   else
     	       ret_str+=temp;
		
		}
		return ret_str;
}
int* read_many(int &fp,int n)
{
	int* arr=new int[n];
	char temp;
	string str;
	int loc=0;
	while(read(fp, &temp, 1) > 0)
	{
		if(temp=='\n')
		{
			arr[loc]=stoi(str);
			return arr;
		}
		else if(temp==' ')
		{
			arr[loc]=stoi(str);
			loc++;
			str.clear();
			continue;
		}
		else
		{
			str+=temp;
		}
	}
}

string* read_name(int &fp,int n)
{
	string* n_str=new string[n];
	char temp;
	string str;
	int loc=0;
	while(read(fp, &temp, 1) > 0)
	{
		if(temp=='\n')
		{
			n_str[loc]=str;
			return n_str;
		}
		else if(temp==' ')
		{
			n_str[loc]=str;
			// cout<<str;
			loc++;
			str.clear();
			//continue;
		}
		else
		{
			str+=temp;
		}
	}
	return n_str;
}
#include "CTool.h"

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

void init(int &root_id,int &node_num)
{
	//string path="./root/node.txt";
	string path="/home/wanghao/study_source/Bplustree/build/root/node.txt";
	int fp;
	fp=open(path.c_str(),O_RDONLY|O_CREAT|O_SYNC);
	root_id=stoi(read_one(fp).c_str());
	node_num=stoi(read_one(fp).c_str());
	close(fp);
	return;
}
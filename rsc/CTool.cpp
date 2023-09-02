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

void init(int &root_id,int &node_num,bool &is_kong)
{
	//string path="./root/node.txt";
	is_kong=false;
	string path="/home/wanghao/study_source/Bplustree/build/root/node.txt";
	int fp;
	fp=open(path.c_str(),O_RDONLY|O_SYNC);
	if(fp==-1)
	{
		is_kong=true;
		return;
	}
	root_id=stoi(read_one(fp).c_str());
	node_num=stoi(read_one(fp).c_str());
	close(fp);
	return;
}

void rm_files()
{
    //在目录后面加上"\\*.*"进行第一次搜索
	string dir="/home/wanghao/study_source/Bplustree/build/data";
	struct dirent *direntp;
	DIR *dirp = opendir(dir.c_str());
	if (dirp != NULL) 
	{
		while ((direntp = readdir(dirp)) != NULL)
		{
			if(strcmp(direntp->d_name,"..")==0 || strcmp(direntp->d_name,".")==0)
					continue;
			string temp_path=dir+"/"+direntp->d_name;
			//printf("%s\n", temp_path.c_str());
			int ret=remove(temp_path.c_str());
			// cout<<ret<<endl;
		}
	}
	closedir(dirp);
	int ret;
	ret=remove("/home/wanghao/study_source/Bplustree/build/root/node.txt");
	return ;
}

string getTime()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep));
    return tmp;
}


void set_line(int &fp,int n)
{
		int loc=0;
		// 记录读取到的字符
		char temp;
		// 用于区分当前存储key还是value（遇到空格就是下一个值，即flag切换为1）
		while (loc!=n&&read(fp, &temp, 1) > 0)
		{
			//cout<<temp<<endl;
			if (temp == '\n')
				loc++;
		}
}
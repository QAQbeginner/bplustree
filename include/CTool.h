#pragma once
#include<iostream>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<math.h>
#include<vector>
#include<dirent.h>
#include<time.h>
#include<setjmp.h>
#include <stdbool.h>
using namespace std;
static jmp_buf buf;

string read_one(int &fp);

int* read_many(int &fp,int n);

string* read_name(int &fp,int n);

void init(int &root_id,int &node_num,bool &is_kong);

void rm_files();

string getTime();

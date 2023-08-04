#pragma once
#include <iostream>
#include <string>
#include<fcntl.h>
#include<unistd.h>
#include<math.h>
#include<vector>
using namespace std;


string read_one(int &fp);

int* read_many(int &fp,int n);

string* read_name(int &fp,int n);

void init(int &root_id,int &node_num,bool &is_kong);


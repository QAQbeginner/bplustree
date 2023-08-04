## B+树

```
有k个子结点的结点必然有k个关键码；
非叶结点仅具有索引作用，跟记录有关的信息均存放在叶结点中。
树的所有叶结点构成一个有序链表，可以按照关键码排序的次序遍历全部记录。
```

```
B+树的插入逻辑如下：
	如果B+树为空，则新生成一个结点，作为B+树的根结点，并将值插入结束。
	如果B+树不为空，则查找当前数值应该插入的位置，定位到需要插入到的叶子结点。
	插入到叶子结点后，如果该结点未达到最大数量，则结束。
	如果该结点已满，则将该结点分裂成两个结点，并将中间的关键字插入到父节点中。如果父节点已满，则递归分裂父节点，直到不需要分裂为止。
```

```
B+树的删除逻辑：
	当结点中关键字个数大于⌈M/2⌉，做删除操作不会破坏 B+树，则可以直接删除。
	当删除某结点中最大或者最小的关键字，就会涉及到更改其双亲结点一直到根结点中所有索引值的更改。
	当删除该关键字，导致当前结点中关键字个数小于 ⌈M/2⌉，若其兄弟结点中含有多余的关键字，可以从兄弟结点中借关键字完成删除操作
	如果都没有可以借用的，就合并
```

```
B+树的查找逻辑：
	通过树的性质，逐层递归查找。
struct info {
    BPlusTreeNode* node; // 找到的叶子节点
    int loc;			//	查找的值在该节点的位置
};
```

```
B+树的修改逻辑：
	同查找。
```

```
B+树的序列化和反序列化逻辑：
大致思想：
	1.由于在存储的时候，指针地址是无法存储的，因为每次加载，其指针地址都是会变的，所以引入id值进行转化存储。
	2.当数据过多时，若一次性加载所有节点，会导致加载时间过长，且往往很多时候，用户只需要查询或修改个别值，并不需要加载所有节点，因此，在加载时，只需要加载根节点即可，剩余的节点在需要使用时在进行动态加载。
B+树结构体
// 定义b+树结构体
struct BPlusTreeNode {
    BPlusTreeNode* parent;  	// 父节点
    bool is_leaf;   			// 是否为叶子节点
    int key_num=0;    			// 关键字的数量
    int key[ORDER] = {-1};      // 关键字数组
    string* name[ORDER] = {nullptr};			// 值数组
    BPlusTreeNode* child[ORDER] = {nullptr};    // 存放孩子指针的数组
    BPlusTreeNode* next; 	// 指向下一个兄弟节点
    int id=-1;  			// 用于序列化时代替该节点指针存入
    int p_id=-1;			// 用于序列化时代替父节点指针存入
    int b_id = -1;			// 用于序列化时代替兄弟节点指针存入
    int c_id[ORDER]={-1};	// 用于序列化时代替孩子节点指针存入
};
具体逻辑：
	1.可以将各个节点保存为一个txt文件（已完成）
	2.在序列化时，将每个节点的孩子节点指针和父亲指针转换为相应的id（已完成）
	3.需要定义一个结构体和一个node_arr容器，用于初始化存储的节点和判断该节点是否被加载或修改，其中容器的下标就是节点所对应的id
		// 定义结构体用于反序列化操作
        typedef struct NodeInfo
        {
            BPlusTreeNode* node;
            bool is_loaded=false;// 是否被加载
            bool is_changed=false;// 是否被修改
        }NodeInfo,*PNodeInfo;
     4.定义load_node函数加装用于split函数和find函数中用于反序列化时的懒惰加载（已完成）
     5.在修改节点或者分割节点时或插入值时，需要将is_changed修改为true（可要可不要，后续看时间安排）
	 6.在进行保存时，需要将is_changed为true的节点全部重新保存（周五任务，可要可不要）
```

```
中途遇到的一些问题：
	1.在使用vector容器进行初始化时：vector<PNodeInfo> node_arr(node_num+1,new NodeInfo());这样是不行的，因为一旦更改了一个元素的值，后续的元素值全部被修改。
	2.在插入元素函数中，若定义插入的节点为：BPlusTreenode new_node;而不是BPlusTreenode* new_node=new BPlusTreenode();则在插入结束后，new_node会被自动释放，导致插入失败，只有new或者malloc的变量在函数执行完后，其变量仍然存在。
	3.在类内定义的结构体不可定义为static用于声明类内函数的返回值类型。
	4.goto语句不能用于跳往函数外部，只能在函数内部使用
```

```
B+树定义的类：
class BplusTree
{
private:
    // 查找叶子节点操作
    info* find(BPlusTreeNode* node, int key,vector<PNodeInfo> &node_arr);
    // 获取该节点在其父亲节点的位置
    int get_loc(BPlusTreeNode* p_node, BPlusTreeNode* c_node);
    // 自下而上修改父节点的相关值
    void p_change(BPlusTreeNode* p_node, int key, int c_key);
    // 自上而下修改子节点的相关值
    void c_change(BPlusTreeNode* c_node, BPlusTreeNode* p_node);
    // 遍历一层的数值
    void s_ceng(BPlusTreeNode* p_node,vector<PNodeInfo> &node_arr);
     // 找到key值在父节点的位置
    int find_key(BPlusTreeNode* p_node, int key);
public:
    // 根节点
    BPlusTreeNode* root = new BPlusTreeNode();
    // 节点数量
    int node_num = 1;
    // 默认构造函数
    BplusTree() { root->is_leaf = true; root->parent = nullptr; root->key_num = 0; root->next = nullptr; root->id = 1; }
    // 不要了
    BplusTree(char* path);
public:
    // 分割操作
    void split(BPlusTreeNode* node,vector<PNodeInfo> &node_arr);
    // 插入操作
    void insert(BPlusTreeNode* node, int key, string* name,vector<PNodeInfo> &node_arr);
    // 查询操作
    void search(BPlusTreeNode* node, int key,vector<PNodeInfo> &node_arr);
    // 删除操作
    void delect(BPlusTreeNode* node, int key,vector<PNodeInfo> &node_arr);
    // 查看树的结构
    void all(BPlusTreeNode* node,vector<PNodeInfo> &node_arr);
    // 修改
    void change(BPlusTreeNode* node, int key,vector<PNodeInfo> &node_arr);  
    
    // 序列化操作
    // 保存单个节点
    void save(BPlusTreeNode* node);
    // 保存一层节点
    void save_ceng(BPlusTreeNode* node);
    // 保存全部
    void save_all(BPlusTreeNode* node);

    // 反序列化操作
    static BPlusTreeNode* load_node(int t_id,vector<PNodeInfo> &node_arr);
};

```

```
数据测试：
1.十万加数据插入并随机查询，删除，修改（可行）
2.增加数据的三种方式：	
					1.直接增加（可行）
					2.增加后叶子结点分割（可行）
					3.增加后叶子结点以及父节点都要分割（可行）
3.删除数据的三种形式：
					1.直接删除（可行）
					2.删除后需要借左右兄弟节点（可行）
					3.删除后需要合并（可行）（右节点合并有问题）（已改）
					4.删除后合并且合并父节点 （可行）
4.修改数据（可行）
```


# bplustree
增加数据：已完成
删除数据：已完成
查询和修改：已完成

#序列化
1.可以将各个节点保存为一个txt文件（已完成）
2.在序列化时，将每个节点的孩子节点指针和父亲指针转换为相应的id（已完成）
序列化需要定义的函数：
	1.将节点转换为相应的格式数据（已改）
	2.将txt数据写入文件
反序列化需要定义的函数：
定义结构体用于反序列化操作
def struct Node_info
{
	Btreenode* node;
	bool is_loaded=false;// 是否被加载
	bool is_changed=false;// 是否被修改
}
	将txt文件中的数据转化为节点：只写入相对应的id而不写入指针
	

后续更改：
	需要再main函数或者b+树类中创建一个可以容纳Node_info*的容器b_vector
	其中容器的下标就是节点所对应的id
	load_node函数要加装在split函数和find函数中用于反序列化时的懒惰加载
	find函数要重新增加一个参数：b_vector,用于在递归往下时，判断是否加载节点并更新b_vector（周四任务）
	split分割函数同理，需要在分割的时候将新的节点保存至容器内（周四任务）
	即在main函数加载时，通过读取root文件夹中保存的根节点信息和节点数量创建等数量的容器，并将根节点保存进去（周四任务）
	delect同理，需要在访问兄弟节点或者子节点时进行判断该节点是否被加载，若没有，则加载后访问（周四任务）
	在修改节点或者分割节点时或插入值时，需要将is_changed修改为true（可要可不要，后续看时间安排）
	在进行保存时，需要将is_changed为true的节点全部重新保存（周五任务）

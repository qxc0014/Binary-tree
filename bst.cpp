#include<iostream>
#include<vector>
using namespace std;
class Node{
    public:
        int value;
        int key;
        bool init = false;
        Node* left = nullptr;//小于root
        Node* right = nullptr;//大于root
        Node(int v,int k,bool initstate):value(v),key(k),init(initstate){}
};
class distindex{
    public:
        distindex(int dist_,int index_):dist(dist_),index(index_){}
        int dist;
        int index;
};
class result{
    public:
    result(int worst_dis_):worst_dis(worst_dis_){}//用于搜索一个近邻点
    result(int worst_dis_,int k):worst_dis(worst_dis_),worst_dis_cap(vector<distindex>(k,distindex(worst_dis_,-1))),size(k){ 
    }
    int worst_dis;
    int index;
    int num=0;
    int size;
    vector<distindex> worst_dis_cap;
    void add_point(int bias,int node_index);
};
void result::add_point(int bias,int node_index){
        if(num != this->size) num++;//已插入值的个数
        if(bias >= worst_dis_cap[this->size-1].dist) return;//大于最大值直接跳出
        int i = num-1;//已经插入最大值的index
        while(i>0){
             if(bias < worst_dis_cap[i-1].dist){
                this->worst_dis_cap[i] = worst_dis_cap[i-1];
                i--;
             }else{
                break;
             }
        }
        worst_dis_cap[i].dist = bias;
        worst_dis_cap[i].index = node_index;
        worst_dis =  worst_dis_cap[this->size-1].dist;
}
/*建树*/
Node* insert(Node* root,int points,int index){
    if(root == nullptr){//没有初始化根节点
        root = new Node(index,points,true);
    }else if(points > root->key){
        root->right = insert(root->right,points,index);
    }else if(points < root->key){
        root->left = insert(root->left,points,index);
    }
    return root;
}
/*查找对应的数*/
Node* search_key(Node* root,int key){
    Node* now_node = root;
    while(now_node != nullptr){
       // cout << now_node->key << endl;
        if(now_node->key == key){
            return now_node;
        }else if(now_node->key > key){
            now_node = now_node->left;
        }else if(now_node->key < key){
            now_node = now_node->right;
        }
    }
    return now_node;
}
/*找一个最近点*/
bool one_neighbor_search(Node* node,result &a,int key){
    if(node == nullptr) return false;//判断是否为叶子节点
    if((node->key - key) < a.worst_dis) a.index = node->value;
    a.worst_dis = abs(node->key - key) < a.worst_dis?abs(node->key - key):a.worst_dis;//先计算是否需要更新最坏距离
    if(a.worst_dis == 0) return true;//如果刚好找到了这个值
    if(node->key > key){//如果要找的值小于这个节点
        if(one_neighbor_search(node->left,a,key)){ //如果在左枝找到了，就不用找右枝了
            return true;//首先判断坐
        }else if(abs(node->key - key)<a.worst_dis){
            return one_neighbor_search(node->right,a,key);
        }
        return false;
    }else{//如果要找的值大于这个节点
        if(one_neighbor_search(node->right,a,key)){//往右
            return true;//往右找
        }else if(abs(node->key - key)<a.worst_dis){//
             return one_neighbor_search(node->left,a,key);
        }
        return false;
    }
}
/*找k个最近点*/
bool k_neighbor_search(Node* node,result &b,int key){
    if(node == nullptr) return false;//判断是否为叶子节点
    b.add_point(abs(node->key - key),node->value);
    if(b.worst_dis == 0) return true;//如果刚好找到了这个值
    if(node->key > key){//如果要找的值小于这个节点
        if(k_neighbor_search(node->left,b,key)){ //如果在左枝找到了，就不用找右枝了
            return true;//首先判断坐
        }else if(abs(node->key - key)<b.worst_dis){
            return k_neighbor_search(node->right,b,key);
        }
        return false;
    }else{//如果要找的值大于这个节点
        if(k_neighbor_search(node->right,b,key)){//往右
            return true;//往右找
        }else if(abs(node->key - key)<b.worst_dis){//
             return k_neighbor_search(node->left,b,key);
        }
        return false;
    }
}
int main(int argc, char const *argv[])
{
    int random[]={45,43,12,5437,743,65,23,7,23,9};
    Node* root = nullptr;//避免出现野指针
    /*存放二叉树*/
    for(int i = 0;i < sizeof(random)/sizeof(int);i++){
        root = insert(root,random[i],i);
    }
    int x;
    cout << "请输入一个需要寻找的整数:";
    cin >> x;
    Node* search_node = search_key(root,x);
    if(search_node == nullptr){
        cout << "没找到！" << endl;
    }else{
        cout << "找到了，节点index=" << search_node->value << endl;
    }
    result a(2e6);
    one_neighbor_search(root,a,x);
    result b(2e6,3);
    k_neighbor_search(root,b,x);
    cout << "最近邻的点的index=" << a.index << endl;
    cout << "最近邻的点的3个index=" << b.worst_dis_cap[0].index
                                  << b.worst_dis_cap[1].index
                                << b.worst_dis_cap[2].index  
                                << endl;
    return 0;
}

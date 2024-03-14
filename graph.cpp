#include <iostream>//包含了用于输入输出的流类和对象，例如 cin、cout 
#include <fstream>//包含了用于文件输入输出的流类和对象，例如 ifstream、ofstream 
#include <vector>//包含了向量（动态数组）的定义和相关函数
#include <string>//包含了字符串的定义和相关函数
#include <algorithm>//包含了各种常见算法的定义和实现，例如排序、查找
#include <cmath>//包含了数学函数的定义，例如 sin、cos、sqrt 
#include <cstdlib>//包含了标准库中的通用工具函数，例如内存分配、随机数生成
#include <ctime>//包含了时间处理函数的定义，例如获取当前时间、延时

using namespace std;

//深度优先遍历递归实现
struct TreeNode{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x):val(x),left(nullptr),right(nullptr){}
};
class Solution{
public:
    void inorder(TreeNode *root,vector<int>& res){
        if(!root){
            return;
        }
        inorder(root->left,res);
        res.push_back(root->val);
        inorder(root->right,res);
    }

    vector<int>inorderTraversal(TreeNode *root){
        vector<int> res;
        inorder(root,res);
        return res;
    }
};
int main(){
    // 构建一个简单的二叉树
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    // 创建 Solution 对象
    Solution sol;

    // 调用中序遍历函数
    vector<int> result = sol.inorderTraversal(root);

    // 输出中序遍历结果
    cout << "Inorder traversal: ";
    for (int val : result) {
        cout << val << " ";
    }
    cout << endl;

    // 释放内存
    delete root->left->left;
    delete root->left->right;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
} 

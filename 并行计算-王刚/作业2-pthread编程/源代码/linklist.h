#ifndef LINKLIST_H
#define LINKLIST_H

#include <iostream>

using namespace std;

template <class T>
class ListNode
{
public:
    ListNode<T> *next;
    ListNode<T> *prev;
    T data;
};

template <class T>
class LinkList
{
public:
    LinkList();
    LinkList(const LinkList& ln);//拷贝构造函数
    ~LinkList();               //析构函数
    void add(T e);             //向链表添加数据
    void ascSort();            //升序排序
    void remove(T index);      //移除某个结点
    T find(int index);         //查找结点
    bool isEmpty();            //判断是否为空
    int size();                //链表长度
    void show();               //显示链表
    void resShow();            //链表反向显示
    void removeAll();          //删除全部结点
private:
    ListNode<T> *head;
    ListNode<T> *tail;
    int length;
};

#endif // LINKLIST_H

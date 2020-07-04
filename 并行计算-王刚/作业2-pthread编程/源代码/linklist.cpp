#include "linklist.h"

template <typename T>
LinkList<T>::LinkList()
{
    head = new ListNode<T>;
    tail = new ListNode<T>;
    head->next = tail;
    head->prev = nullptr;
    tail->next = nullptr;
    tail->prev = head;
    length = 0;
}

//拷贝构造函数
template <typename T>
LinkList<T>::LinkList(const LinkList &ln)
{
    head = new ListNode<T>;
    head->prev = nullptr;
    tail = new ListNode<T>;
    head->next = tail;
    tail->prev = head;
    length = 0;
    ListNode<T>* temp = ln.head;
    while (temp->next != ln.tail)
    {
        temp = temp->next;
        tail->data = temp->data;
        ListNode<T> *p = new ListNode<T>;
        p->prev = tail;
        tail->next = p;
        tail = p;
        length++;
    }
    tail->next = nullptr;
}

//析构函数
template <typename T>
LinkList<T>::~LinkList()
{
    if (length == 0)
    {
        delete head;
        delete tail;
        head = nullptr;
        tail = nullptr;
        return;
    }
    while (head->next != nullptr)
    {
        ListNode<T> *temp = head;
        head = head->next;
        delete temp;
    }
    delete head;
    head = nullptr;
}

//向链表添加数据
template <typename T>
void LinkList<T>::add(T e)
{
    ListNode<T>* temp = this->tail;
    tail->data = e;
    tail->next = new ListNode<T>;
    ListNode<T> *p = tail;
    tail = tail->next;
    tail->prev = p;
    tail->next = nullptr;
    length++;
}

//查找结点
template <typename T>
T LinkList<T>::find(int index)
{
    if (length == 0)
    {
        std::cout << "myList is empty";
        return NULL;
    }
    if (index >= length)
    {
        std::cout << "Out of bounds";
        return NULL;
    }
    int x = 0;
    T data;
    ListNode<T> *p;
    if (x < length / 2)
    {
        p = head->next;
        while (p->next != nullptr && x++ != index)
        {
            p = p->next;
        }
    }
    else {
        p = tail->prev;
        while (p->prev != nullptr && x++ != index)
        {
            p = p->prev;
        }
    }
    return p->data;
}

//删除结点
template <typename T>
void LinkList<T>::remove(T index)
{
    if (length == 0)
    {
        std::cout << "myList is empty";
        return;
    }
    ListNode<T> *p = head;
    while (p->next != nullptr)
    {
        p = p->next;
        if (p->data == index)
        {
            ListNode<T> *temp = p->prev;
            temp->next = p->next;
            p->next->prev = temp;
            delete p;
            length--;
            return;
        }
    }
}

//删除所有结点
template <typename T>
void LinkList<T>::removeAll()
{
    if (length == 0)
    {
        return;
    }
    ListNode<T> *p = head->next;
    while (p != tail)
    {
        ListNode<T>* temp = p;
        p = p->next;
        delete temp;
    }
    head->next = tail;
    tail->prev = head;
    length = 0;
}

//升序排序
template <typename T>
void LinkList<T>::ascSort()
{
    if (length <= 1) return;
    ListNode<T> *p = head->next;
    for (int i = 0; i < length - 1; i++)
    {
        ListNode<T> *q = p->next;
        for (int j = i + 1; j < length; j++)
        {
            if (p->data > q->data)
            {
                T temp = q->data;
                q->data = p->data;
                p->data = temp;
            }
            q = q->next;
        }
        p = p->next;
    }
}

//判断是否为空
template <typename T>
bool LinkList<T>::isEmpty()
{
    if (length == 0)
    {
        return true;
    }
    else {
        return false;
    }
}

//链表长度
template <typename T>
int LinkList<T>::size()
{
    return length;
}

//输出链表
template <typename T>
void LinkList<T>::show()
{
    if (length == 0)
    {
        std::cout << "myList is empty" << std::endl;
        return;
    }
    ListNode<T> *p = head->next;
    while (p != tail)
    {
        std::cout << p->data << " ";
        p = p->next;
    }
    std::cout << std::endl;
}

//反向输出链表
template <typename T>
void LinkList<T>::resShow()
{
    if (length == 0)return;
    ListNode<T> *p = tail->prev;
    while (p != head)
    {
        std::cout << p->data << " ";
        p = p->prev;
    }
    std::cout << std::endl;
}

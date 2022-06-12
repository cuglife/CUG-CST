#ifndef MYLIST_H
#define MYLIST_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QMessageBox>
const int defaultSize=100;

template <class T>
class myList {
public:
    virtual void insert(int i,const T& item)=0;
    virtual void append(const T& item)=0;
    virtual void clear()=0;
    virtual bool isEmpty()=0;
    virtual void removeAt(int i)=0;
    virtual int length() const =0;
    virtual T& at(int i)=0;
};




template <class T>
class SeqList :public myList<T> {
public:
    SeqList(int size=defaultSize);
    ~SeqList();
    SeqList(SeqList<T>& L);

    void insert(int i,const T& item);
    void insert(const T& item,T& value);
    void insertAll(const T& item,T& value);
    void _insert(int i,const T& item);
    void _insert(const T& item,T& value);
    void _insertAll(const T& item,T& value);
    void append(const T& item);

    void removeAt(int i);
    void removeValue(T& item);
    void removeValueAll(T& item);
    void clear();

    T& at(int i);

    bool sort();
    bool inversion();

    bool isEmpty(){return listLength==0;}
    bool isFull(){return listLength==maxSize;}
    bool expan();
    int length() const{return listLength;}
    int size() const{return maxSize;}

private:
    T* listArray;
    int maxSize;
    int listLength;
};

template <class T>
SeqList<T>::SeqList(int size) {
    if (size > 0) {
        maxSize = size;
        listLength = 0;
        listArray = new T[maxSize];
        if (listArray == NULL)
            qDebug()<<"listArray内存分配失败";
    }
    else {
        qDebug()<<"构造函数序号非法！";
    }
}

template <class T>
SeqList<T>::~SeqList() {
    delete [] listArray;
}

template <class T>
SeqList<T>::SeqList(SeqList<T>& L) {
    maxSize = L.maxSize();
    listLength = L.length();
    T value;
    listArray = new T[maxSize];
    if (listArray == NULL)
        qDebug()<<"listArray内存分配失败";
    for (int i = 0; i < L.length(); i++) {
        listArray[i] = L.at(i);
    }
}

template <class T>
void SeqList<T>::insert(int i,const T& item){
    if (isFull())
        expan();

    if (i >= 0 && i <= listLength) {
        if(listLength==0)
            listArray[0] = item;
        else {
            for (int j = listLength; j > i; j--)
                listArray[j] = listArray[j-1];
            listArray[i+1] = item;
        }
        listLength++;
    }
    else
        qDebug()<<"插入序号非法！";
}

template <class T>
void SeqList<T>::insert(const T& item,T& value){
    if (isFull())
        expan();

    for (int i=0; i < listLength; i++){
            if(listArray[i]==item){
                for (int j=listLength; j > i; j--)
                    listArray[j] = listArray[j-1];
                listArray[i+1]=value;
                listLength++;
                return;
            }
    }
}

template <class T>
void SeqList<T>::insertAll(const T& item,T& value){
    if (isFull())
        expan();

    for (int i=0; i < listLength; i++){
            if(listArray[i]==item){
                for (int j=listLength; j > i; j--)
                    listArray[j] = listArray[j-1];
                listArray[i+1]=value;
                listLength++;
                i++;//跳过已插入的项
            }
    }
}

template <class T>
void SeqList<T>::_insert(int i,const T& item){
    if (isFull())
        expan();

    if (i >= 0 && i <= listLength) {
        if(listLength==0)
            listArray[0] = item;
        else {
            for (int j = listLength; j > i; j--)
                listArray[j] = listArray[j-1];
            listArray[i] = item;
        }
        listLength++;
    }
    else
        qDebug()<<"插入序号非法！";
}

template <class T>
void SeqList<T>::_insert(const T& item,T& value){
    if (isFull())
        expan();

    for (int i=0; i <= listLength; i++){
        if(listArray[i]==item){
            for (int j=listLength; j >= i; j--)
                listArray[j+1] = listArray[j];
            listArray[i]=value;
            listLength++;
            return;
            }
    }
}

template <class T>
void SeqList<T>::_insertAll(const T& item,T& value){
    if (isFull())
        expan();

    for (int i=0; i <= listLength; i++){
        if(listArray[i]==item){
            for (int j=listLength; j >= i; j--)
                listArray[j+1] = listArray[j];
            listArray[i]=value;
            listLength++;
            i++;
        }
    }
}

template <class T>
bool SeqList<T>::expan(){
    int newSize = 50 * (maxSize / 50 + 1);
    T* expanlistArray = new T[newSize];
    for (int i = 0; i < listLength; i++)
        expanlistArray[i] = listArray[i];
    listArray = expanlistArray;
    maxSize = newSize;
    delete [] expanlistArray;
    return true;
}

template <class T>
void SeqList<T>::append(const T& item){
    if (isFull())
        expan();
    listArray[listLength] = item;
}

template <class T>
void SeqList<T>::clear(){
    delete [] listArray;
    listArray=new T[maxSize];
    listLength = 0;
}

template <class T>
void SeqList<T>::removeAt(int i){
    if(isEmpty())
        qDebug()<<"表空无法移除！";
    else if (i>=0){
        for (; i < listLength; i++)
            listArray[i] = listArray[i+1];
        listLength--;
    }
}

template <class T>
void SeqList<T>::removeValue(T& value){
    if(isEmpty())
        qDebug()<<"表空无法移除！";
    for (int i=0; i < listLength; i++){
        if(listArray[i]==value){
            for (; i < listLength; i++)
                listArray[i] = listArray[i+1];
            listLength--;
            return;
        }
    }
}

template <class T>
void SeqList<T>::removeValueAll(T& value){
    if(isEmpty())
        qDebug()<<"表空无法移除！";
    for (int i=0; i < listLength; i++){
        if(listArray[i]==value){
            for (int j=i; j < listLength; j++)
                listArray[j] = listArray[j+1];
            listLength--;
            i--;
        }
    }
}

template <class T>
T& SeqList<T>::at(int i){
    if (i <= listLength)
        return listArray[i];
    else{
        qDebug()<<"序号越界！";
        return listArray[0];
    }
}

template <class T>
bool SeqList<T>::sort(){
    int i, j;
    QString temp;
    for (i = 0; i < listLength-1; i++) {
        for (j = 0; j < listLength - 1 - i; j++) {
            QString str_j=listArray[j];
            QString str_jplus=listArray[j + 1];


            int int_j=str_j.toInt();
            int int_jplus=str_jplus.toInt();

            if (int_j > int_jplus) {
                temp = listArray[j];
                listArray[j] = listArray[j + 1];
                listArray[j + 1] = temp;
            }
        }
    }
    return true;
}

template <class T>
bool SeqList<T>::inversion(){
    //虽然这个好像没啥意义
    for(int i=0;i<listLength/2;i++){
        T temp=listArray[i];
        listArray[i]=listArray[listLength-1-i];
        listArray[listLength-1-i]=temp;
    }
    return true;
}



template <class T>
class LinkList;

template<class T>
class LinkNode {                //结点类
friend class LinkList<T>;       //声明List类为友元类 
public:
T data;                     //数据域
LinkNode<T> * next;         //指针域
LinkNode(LinkNode<T>* ptr=NULL){next=ptr;}
LinkNode(const T& item, LinkNode<T>* ptr=NULL){data=item, next=ptr;}
};



template <class T>
class LinkList :public myList<T> {
public:
    LinkList(){head=current=new LinkNode<T>;listLength=0;}
    LinkList(const T& x){head=new LinkNode<T>(x);}
    LinkList(LinkList<T>& L);
    ~LinkList(){clear();}

    void insert(int i,const T& item);
    void insert(const T& item,T& value);
    void insertAll(const T& item,T& value);
    void _insert(int i,const T& item);
    void _insert(const T& item,T& value);
    void _insertAll(const T& item,T& value);
    void append(const T& item);

    void removeAt(int i);
    void removeValue(T& item);
    void removeValueAll(T& item);

    T& at(int i);
    void setdata(int i,const T& item);
    int currPos()const;
    bool moveTo(int pos);
    bool search(T x);

    bool sort(LinkList &L);
    bool inversion();

    void clear();

    int length() const{return listLength;}
    bool isEmpty(){return listLength==0;}
    LinkList<T>& operator=(LinkList<T>& L);
protected:
    LinkNode<T>* head;
    LinkNode<T>* current;

private:
    int listLength;
};

template <class T>
LinkList<T>::LinkList(LinkList<T>& L){
    T value;
    LinkNode<T> *srcptr=L.head;
    LinkNode<T> *destptr=head=new LinkNode<T>;
    while (srcptr->next!=NULL) {
        value=srcptr->next->data;
        destptr->next=new LinkNode<T>(value);
        destptr=destptr->next;
        srcptr=srcptr->next;
    }
    destptr->next=NULL;
}

template <class T>
bool LinkList<T>::search(T x){
    current=head;
    while (current!=NULL) {
        if(current->data==x)
            return true;
        else
            current=current->next;
    }
    return false;
}

template <class T>
bool LinkList<T>::moveTo(int pos){
    if(pos < 0||pos>listLength){
        qDebug()<<"序号非法";
        return false;
    }
    current = head;
    if(pos==0)
        return true;
    else{
        int k=0;
        while (k<pos&&current!=NULL) {
            current=current->next;
            k++;
        }
        return true;
    }
}

template <class T>
void LinkList<T>::insert(int i,const T& item){
    LinkNode<T> * newNode = new LinkNode<T>(item);
    if(newNode==NULL){
        qDebug()<<"newNode储存分配错误";
    }

    if(head==NULL||i==0){
        newNode->next=head->next;
        head->next=newNode;
        listLength++;
    }
    else{
        moveTo(i);
        if(current == NULL){
            qDebug()<<"无效的插入位置";
        }
        else {
            newNode->next = current->next;
            current->next = newNode;
            listLength++;
        }
    }
}

template <class T>
void LinkList<T>::insert(const T& item,T& value){
    LinkNode<T> * newNode = new LinkNode<T>(value);
    if(newNode==NULL){
        qDebug()<<"newNode储存分配错误";
    }
    if(search(item)){
        newNode->next = current->next;
        current->next = newNode;
        listLength++;
    }
    else
        qDebug()<<"未查询到";
}

template <class T>
void LinkList<T>::insertAll(const T& item,T& value){
    current=head;
    if(head->data==item){
        LinkNode<T> * newNode = new LinkNode<T>(value);
        newNode->next=head;
        head=newNode;
        listLength++;
        current=current->next;

    }

        LinkNode<T> * preNode =current;
        current=current->next;

    while (current!=NULL) {
        if(current->data==item){
            LinkNode<T> * newNode = new LinkNode<T>(value);

            newNode->next = current;
            preNode=
            current->next = newNode;
            listLength++;
            current=current->next->next;

        }
        else
            current=current->next;
    }
}

template <class T>
void LinkList<T>::_insert(int i,const T& item){
    LinkNode<T> * newNode = new LinkNode<T>(item);
    if(newNode==NULL){
        qDebug()<<"newNode储存分配错误";
    }

    if(head==NULL||i==0){
        newNode->next=head;
        head=newNode;
        listLength++;
    }
    else{
        moveTo(i-1);
        if(current == NULL){
            qDebug()<<"无效的插入位置";
        }
        else {
            newNode->next = current->next;
            current->next = newNode;
            listLength++;
        }
    }
}

template <class T>
void LinkList<T>::_insert(const T& item,T& value){
    LinkNode<T> * newNode = new LinkNode<T>(value);
    if(newNode==NULL)
        qDebug()<<"newNode储存分配错误";

    current=head;
    if(head->data==item){
        newNode->next=head;
        head=newNode;
        listLength++;
        return;
    }
    LinkNode<T> * PreNode = current;
    current= current->next;
    while (current!=NULL) {
        if(current->data==item){
            newNode->next = current;
            PreNode->next = newNode;
            listLength++;
            break;
        }
        else{
            PreNode=current;
            current=current->next;
        }
    }
}

template <class T>
void LinkList<T>::_insertAll(const T& item,T& value){
    current=head;
    if(head->data==item){
        LinkNode<T> * newNode = new LinkNode<T>(value);
        newNode->next=head;
        head=newNode;
        listLength++;
    }
    LinkNode<T> * PreNode = current;
    current=current->next;
    while (current!=NULL) {
        if(current->data==item){
            LinkNode<T> * newNode = new LinkNode<T>(value);
            newNode->next = current;
            PreNode->next = newNode;
            listLength++;
            PreNode=current;
            current=current->next;

        }
        else{
            PreNode=current;
            current=current->next;
        }
    }
}

template <class T>
void LinkList<T>::append(const T& item){
    moveTo(listLength-1);
    current->next=new LinkNode<T>(item,NULL);
    listLength++;
}


template <class T>
void LinkList<T>::removeAt(int i){
    moveTo(i-1);
    LinkNode<T>* temp=current->next;
    current->next=current->next->next;
    delete temp;
    listLength-- ;
}

template <class T>
void LinkList<T>::removeValue(T& item){
    current=head;
    if(head->data==item){
        LinkNode<T>* temp=head;
        head=head->next;
        delete temp;
        listLength--;
    }
    else{
        LinkNode<T> * PreNode = current;
        current=current->next;
        while (current!=NULL) {
            if(current->data==item){
                PreNode->next=current->next;
                delete current;
                current=PreNode->next->next;
                listLength--;
                break;
            }
            else{
                PreNode=current;
                current=current->next;
            }
        }
    }
}

template <class T>
void LinkList<T>::removeValueAll(T& item){
    while (head->data==item) {
        LinkNode<T>* temp=head;
        head=head->next;
        delete temp;
        listLength--;
    }
    current=head;
    LinkNode<T> * PreNode = current;
    current=current->next;
    while (current!=NULL) {
        if(current->data==item){
            LinkNode<T>* temp=current;
            PreNode->next=current->next;
            current=current->next;
            delete temp;
            listLength--;
        }
        else{
            PreNode=current;
            current=current->next;
        }
    }
}

template <class T>
bool LinkList<T>::sort(LinkList &L){
    int i, j;
    QString temp;
    for (i = 0; i < listLength-1; i++) {
        for (j = 0; j < listLength - 1 - i; j++) {
            QString str_j=L.at(j);
            QString str_jplus=L.at(j+1);
            int int_j=str_j.toInt();
            int int_jplus=str_jplus.toInt();

            if (int_j > int_jplus) {
                temp = L.at(j);
                L.setdata(j,str_jplus);
                L.setdata(j+1,temp);
            }
        }
    }
    return true;
}

template <class T>
bool LinkList<T>::inversion(){
    LinkNode<T> *preNode=head;
    LinkNode<T> *traversNode=head->next;
    LinkNode<T> *SuccNode=NULL;
    head->next=NULL;
    while (traversNode!=NULL) {
        SuccNode=traversNode->next;
        traversNode->next=preNode;
        preNode=traversNode;
        traversNode=SuccNode;
    }
    head=preNode->next;
    return true;
}

template <class T>
T& LinkList<T>::at(int i){
    moveTo(i);
    if(current == NULL)
        qDebug()<<"无效的位置";
    return current->data;
}

template <class T>
void LinkList<T>::setdata(int i,const T& item){
    moveTo(i);
    current->data=item;
}

template <class T>
int LinkList<T>::currPos()const
{
    LinkNode<T>* temp=head;
    int i;
    for(i=0;current!=temp;i++)
        temp=temp->next;
    return i;
}

template <class T>
void LinkList<T>::clear(){
    while(head!=NULL){
        current=head;
        head=head->next;
        delete current;
    }
    head=current=new LinkNode<T>;
    listLength=0;
}

template <class T>
LinkList<T>& LinkList<T>::operator=(LinkList<T>& L){
    T value;
    LinkNode<T> *srcptr=L.getHead();
    LinkNode<T> *destptr=head=new LinkNode<T>;
    while (srcptr->next!=NULL) {
        value=srcptr->next->data;
        destptr->next=new LinkNode<T>(value);
        destptr=destptr->next;
        srcptr=srcptr->next;
    }
    destptr->next=NULL;
    return *this;
}

#endif // MYLIST_H


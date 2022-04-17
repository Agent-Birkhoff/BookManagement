#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

struct user
{
	char username[11];//用户名最大10字符
	char password[19];//密码最大18位
	char realName[101];//真实姓名最大100字符
	char phoneNumber[12];//手机号码11位
	char ID[19];//ID号18位
	int credit;//信用值
	char bksBrwd[100][101];//最多借100本书 bksBrwd[x][101] x指定借阅的100本书中的哪一本 x范围应与whichOne同步
	int whichOne[100];//指定借阅的是同种书中的哪一本 whichOne[x] x指定借阅的100本书中的哪一本 x范围应与bksBrwd同步
	struct user* next;
};

struct book
{
	char name[101];//书名最大100字符
	char author[101];//作者名最大100字符
	char category[11];//书籍类别最大10字符5汉字
	char publisher[101];//出版社名最大100字符
	char username[100][11];//最多100本相同的书 username[x][y] x指定同种书的哪一本 x应与rentDate同步 y为借阅用户名最大10字符
	double price;//价格
	int num;//同种书的剩余量 0<=num<=100
	int rentDate[100][3];//最多100本相同的书 日期yyyy/mm/dd rentDate[x][3] x应与username同步
	int creditLimit;//借阅所需最低信用值
	struct book* next;
};

extern struct user* crtUser(int length);//创建失败则返回NULL length为0时只新建头
extern void modUser(struct user* list, int order, struct user temp);//传入节点错误则无作为
extern void delUser(struct user* list, int order);//传入节点错误则无作为
extern void insUser(struct user* list, int order, struct user temp);//传入节点错误则无作为
extern int lenUser(struct user* list);//链表错误则返回-1 只有头则返回0
extern struct user* viewUser(struct user* list, int order);//查看指定节点返回该节点指针 错误则返回NULL
extern int searchUser(struct user* list, int start, char keyWord[11]);//start为起始位置 按username的关键字查找 找不到或错误则返回-1 如匹配则返回该节点在链表中的顺序
extern int searchUser_Acc(struct user* list, char username[11]);//按username全字匹配 找不到或错误则返回-1 如匹配则返回该节点在链表中的顺序
extern struct book* crtBook(int length);//创建失败则返回NULL length为0时只新建头
extern void modBook(struct book* list, int order, struct book temp);//传入节点错误则无作为
extern void delBook(struct book* list, int order);//传入节点错误则无作为
extern void insBook(struct book* list, int order, struct book temp);//传入节点错误则无作为
extern int lenBook(struct book* list);//链表错误则返回-1 只有头则返回0
extern struct book* viewBook(struct book* list, int order);//查看指定节点返回该节点指针 错误则返回NULL
extern int searchBook(struct book* list, int start, char keyWord[101]);//start为起始位置 按bookname或author的关键字查找 找不到或错误则返回-1 如匹配则返回该节点在链表中的顺序
extern int searchBook_Acc(struct book* list, char bookname[101]);//按bookname全字匹配 找不到或错误则返回-1 如匹配则返回该节点在链表中的顺序
extern void delAll(struct user* userList, struct book* bookList);//删除所有链表 回收内存
extern void bubbleSortBook(struct book* list, char type, int mode);//如果节点或比较模式错误则无作为 依赖lenBook()与viewBook()
																											//type=='p'时按价格排序 type=='n'时按储量排序 type=='c'时按信用值要求排序 mode==1时从大到小 mode==2时从小到大
extern int is_rented(struct book* list, int bookOrder);//是否已被租出 已被租出返回1 未被租出返回0 依赖viewBook()
extern void returnBook_U(struct book* bookList, struct user* userList, int userOrder);//归零用户借阅记录 依赖viewUser() viewBook()与searchBook_Acc()
extern int rentCount(struct book* bookList, int bookOrder);//返回指定书籍被租借次数 依赖viewBook()
extern struct book* rank(struct book* list);//按被借阅次数排序 返回排序后新链表首地址 警告 用后需要手动删除返回的链表
																   //依赖crtBook() lenBook() insBook() viewBook() rentCount()

#endif

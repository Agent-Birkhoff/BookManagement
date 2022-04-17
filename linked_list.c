#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4996)//禁用强制strcpy_s
#pragma warning( disable : 6031)//禁用讨厌的scanf返回值警告
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include"date.h"

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

struct user* crtUser(int length)//创建失败则返回NULL length为0时只新建头
{
	struct user* start, * node, * end;
	start = (struct user*)malloc(sizeof(struct user));
	end = start;
	if (end)
	{
		int i;
		for (i = 1; i <= length; i++)
		{
			node = (struct user*)malloc(sizeof(struct user));
			if (node)//注意 元素未初始化
			{
				end->next = node;
				end = node;
			}
			else
				i--;//重试
		}
		end->next = NULL;
	}
	return start;
}

void modUser(struct user* list, int order, struct user temp)//传入节点错误则无作为
{
	struct user* target = list;
	if (target)
	{
		int i;
		for (i = 0; i < order; i++)
		{
			if (target->next)
				target = target->next;
			else
				return;
		}

		*target = temp;//复制结构体
	}
}

void delUser(struct user* list, int order)//传入节点错误则无作为
{
	struct user* former = list;
	if (former == NULL)
		return;
	struct user* target = former->next;
	int i;
	for (i = 0; i < order - 1; i++)
	{
		if (former->next)
			former = former->next;
		else
			return;
	}
	target = former->next;
	if (target)
	{
		former->next = target->next;
		free(target);
	}
}

void insUser(struct user* list, int order, struct user temp)//传入节点错误则无作为
{
	struct user* former = list;
	if (former == NULL)
		return;
	struct user* node = (struct user*)malloc(sizeof(struct user));
	if (node)
	{
		int i;
		for (i = 0; i < order - 1; i++)
		{
			if (former->next)
				former = former->next;
			else
			{
				free(node);
				return;
			}
		}
		node->next = former->next;
		former->next = node;

		*node = temp;//复制结构体
	}
}

int lenUser(struct user* list)//链表错误则返回-1 只有头则返回0
{
	int n = 0;
	struct user* p = list;
	if (p)
	{
		while (p->next)
		{
			n++;
			p = p->next;
		}
	}
	else
		return -1;
	return n;
}

struct user* viewUser(struct user* list, int order)//查看指定节点返回该节点指针 错误则返回NULL
{
	struct user* target = list;
	if (target)
	{
		int i;
		for (i = 0; i < order; i++)
		{
			if (target->next)
				target = target->next;
			else
				return NULL;
		}
	}
	return target;
}

int searchUser(struct user* list, int start, char keyWord[11])//按username查找 找不到或错误则返回-1 如匹配则返回该节点在链表中的顺序
{
	if (list != NULL && start >= 1)
	{
		int i;
		struct user* temp = list;
		for (i = 0; i < start; i++)
		{
			if (temp == NULL)
				return -1;
			temp = temp->next;
		}

		while (temp)
		{
			if (strstr(temp->username, keyWord))
				return i;
			i++;
			temp = temp->next;
		}
	}
	return -1;
}

int searchUser_Acc(struct user* list, char username[11])//按username全字匹配 找不到或错误则返回-1 如匹配则返回该节点在链表中的顺序
{
	if (list)
	{
		int i = 1;
		struct user* temp = list->next;
		while (temp)
		{
			if (strcmp(username, temp->username) == 0)
				return i;
			i++;
			temp = temp->next;
		}
	}
	return -1;
}

struct book* crtBook(int length)//创建失败则返回NULL length为0时只新建头
{
	struct book* start, * node, * end;
	start = (struct book*)malloc(sizeof(struct book));
	end = start;
	if (end)
	{
		int i;
		for (i = 0; i < length; i++)
		{
			node = (struct book*)malloc(sizeof(struct book));
			if (node)//注意 元素未初始化
			{
				end->next = node;
				end = node;
			}
			else
				i--;
		}
		end->next = NULL;
	}
	return start;
}

void modBook(struct book* list, int order, struct book temp)//传入节点错误则无作为
{
	struct book* target = list;
	if (target)
	{
		int i;
		for (i = 0; i < order; i++)
		{
			if (target->next)
				target = target->next;
			else
				return;
		}

		*target = temp;//结构体复制
	}
}

void delBook(struct book* list, int order)//传入节点错误则无作为
{
	struct book* former = list;
	if (former == NULL)
		return;
	struct book* target = former->next;
	int i;
	for (i = 0; i < order - 1; i++)
	{
		if (former->next)
			former = former->next;
		else
			return;
	}
	target = former->next;
	if (target)
	{
		former->next = target->next;
		free(target);
	}
}

void insBook(struct book* list, int order, struct book temp)//传入节点错误则无作为
{
	struct book* former = list;
	if (former == NULL)
		return;
	struct book* node = (struct book*)malloc(sizeof(struct book));
	if (node)
	{
		int i;
		for (i = 0; i < order - 1; i++)
		{
			if (former->next)
				former = former->next;
			else
			{
				free(node);
				return;
			}
		}
		node->next = former->next;
		former->next = node;

		*node = temp;//结构体复制
	}
}

int lenBook(struct book* list)//链表错误则返回-1 只有头则返回0
{
	int n = 0;
	struct book* p = list;
	if (p)
	{
		while (p->next)
		{
			n++;
			p = p->next;
		}
	}
	else
		return -1;
	return n;
}

struct book* viewBook(struct book* list, int order)//查看指定节点返回该节点指针 错误则返回NULL
{
	struct book* target = list;
	if (target)
	{
		int i;
		for (i = 0; i < order; i++)
		{
			if (target->next)
				target = target->next;
			else
				return NULL;
		}
	}
	return target;
}

int searchBook(struct book* list, int start, char keyWord[101])//start为起始位置 按bookname或author的关键字查找 找不到或错误则返回-1 如匹配则返回该节点在链表中的顺序
{
	if (list != NULL && start >= 1)
	{
		int i;
		struct book* temp = list;
		for (i = 0; i < start; i++)
		{
			if (temp == NULL)
				return -1;
			temp = temp->next;
		}

		while (temp)
		{
			if (strstr(temp->name, keyWord))
				return i;
			else if (strstr(temp->author, keyWord))
				return i;
			i++;
			temp = temp->next;
		}
	}
	return -1;
}

int searchBook_Acc(struct book* list, char bookname[101])//按bookname全字匹配 找不到或错误则返回-1 如匹配则返回该节点在链表中的顺序
{
	if (list)
	{
		int i = 1;
		struct book* temp = list->next;
		while (temp)
		{
			if (strcmp(bookname, temp->name) == 0)
				return i;
			i++;
			temp = temp->next;
		}
	}
	return -1;
}

void delAll(struct user* userList, struct book* bookList)//删除所有链表 回收内存
{
	struct user* list = userList;
	while (list)
	{
		struct user* temp = list->next;
		free(list);
		list = temp;
	}
	struct book* list2 = bookList;
	while (list2)
	{
		struct book* temp = list2->next;
		free(list2);
		list2 = temp;
	}
}

void bubbleSortBook(struct book* list, char type, int mode)//如果节点或比较模式错误则无作为 依赖lenBook()与viewBook()
{                                                                                              //type=='p'时按价格排序 type=='n'时按储量排序 type=='c'时按信用值要求排序 mode==1时从大到小 mode==2时从小到大
	int length = lenBook(list);
	int i, n, swap = 0;
	for (i = length - 1; i > 0; i--)
	{
		for (n = 1; n <= i; n++)
		{
			swap = 0;//默认不交换
			if (type == 'p' && mode == 1 && (viewBook(list, n)->price < viewBook(list, n + 1)->price)) //比较价格 从大到小
				swap = 1;
			else if (type == 'p' && mode == 2 && (viewBook(list, n)->price > viewBook(list, n + 1)->price)) //比较价格 从小到大
				swap = 1;
			else if (type == 'n' && mode == 1 && (viewBook(list, n)->num < viewBook(list, n + 1)->num)) //比较剩余量 从大到小
				swap = 1;
			else if (type == 'n' && mode == 2 && (viewBook(list, n)->num > viewBook(list, n + 1)->num)) //比较剩余量 从小到大
				swap = 1;
			else if (type == 'c' && mode == 1 && (viewBook(list, n)->creditLimit < viewBook(list, n + 1)->creditLimit)) //比较信用值限制 从大到小
				swap = 1;
			else if (type == 'c' && mode == 2 && (viewBook(list, n)->creditLimit > viewBook(list, n + 1)->creditLimit)) //比较信用值限制 从小到大
				swap = 1;
			else
				return;
			if (swap == 1)
			{
				struct book* temp = list;
				int j;
				for (j = 0; j < n - 1; j++)
					temp = temp->next;
				struct book* m = temp->next;//第n个
				temp->next = m->next;//n-1链接到n+1
				m->next = (temp->next)->next;//n链接到n+2或链表结尾NULL
				(temp->next)->next = m;//n+1链接到n
			}
		}
	}
}

int is_rented(struct book* list, int bookOrder)//是否已被租出 已被租出返回1 未被租出返回0 依赖viewBook()
{
	int n;
	struct book bookT = *viewBook(list, bookOrder);
	for (n = 0; n < 100; n++)
	{
		if (bookT.username[n][0] != '\0')
			return 1;
	}
	return 0;
}

void returnBook_U(struct book* bookList, struct user* userList, int userOrder)//归零用户借阅记录 依赖viewUser() viewBook()与searchBook_Acc()
{
	struct user userT = *viewUser(userList, userOrder);
	int n;
	for (n = 0; n < 100; n++)
	{
		if (userT.bksBrwd[n][0] == '\0')
			continue;
		int bookOrder = searchBook_Acc(bookList, userT.bksBrwd[n]);
		if (bookOrder != -1)
		{
			int i = userT.whichOne[n];
			viewBook(bookList, bookOrder)->username[i][0] = '\0';
			viewBook(bookList, bookOrder)->rentDate[i][0] = 0;
			viewBook(bookList, bookOrder)->rentDate[i][1] = 0;
			viewBook(bookList, bookOrder)->rentDate[i][2] = 0;
		}
	}
}

int rentCount(struct book* bookList, int bookOrder)//返回指定书籍被租借次数 依赖viewBook()
{
	struct book bookT = *viewBook(bookList, bookOrder);
	int n;
	int count = 0;
	for (n = 0; n < 100; n++)
	{
		if (bookT.username[n][0] != '\0')
			count++;
	}
	return count;
}

struct book* rank(struct book* list)//按被借阅次数排序 返回排序后新链表首地址 警告 用后需要手动删除返回的链表
{                                                      //依赖crtBook() lenBook() insBook() viewBook() rentCount()
	struct book* sort = crtBook(0);//复制链表
	int n;
	for (n = 1; n <= lenBook(list); n++)
		insBook(sort, n, *viewBook(list, n));

	int length = lenBook(sort);
	int i;
	for (i = length - 1; i > 0; i--)
	{
		for (n = 1; n <= i; n++)
		{
			if (rentCount(sort, n + 1) >= rentCount(sort, n))
			{
				struct book* temp = sort;
				int j;
				for (j = 0; j < n - 1; j++)
					temp = temp->next;

				struct book* m = temp->next;//第n个
				temp->next = m->next;//n-1链接到n+1
				m->next = (temp->next)->next;//n链接到n+2或链表结尾NULL
				(temp->next)->next = m;//n+1链接到n
			}
		}
	}

	return sort;
}

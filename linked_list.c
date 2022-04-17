#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4996)//����ǿ��strcpy_s
#pragma warning( disable : 6031)//���������scanf����ֵ����
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include"date.h"

struct user
{
	char username[11];//�û������10�ַ�
	char password[19];//�������18λ
	char realName[101];//��ʵ�������100�ַ�
	char phoneNumber[12];//�ֻ�����11λ
	char ID[19];//ID��18λ
	int credit;//����ֵ
	char bksBrwd[100][101];//����100���� bksBrwd[x][101] xָ�����ĵ�100�����е���һ�� x��ΧӦ��whichOneͬ��
	int whichOne[100];//ָ�����ĵ���ͬ�����е���һ�� whichOne[x] xָ�����ĵ�100�����е���һ�� x��ΧӦ��bksBrwdͬ��
	struct user* next;
};

struct book
{
	char name[101];//�������100�ַ�
	char author[101];//���������100�ַ�
	char category[11];//�鼮������10�ַ�5����
	char publisher[101];//�����������100�ַ�
	char username[100][11];//���100����ͬ���� username[x][y] xָ��ͬ�������һ�� xӦ��rentDateͬ�� yΪ�����û������10�ַ�
	double price;//�۸�
	int num;//ͬ�����ʣ���� 0<=num<=100
	int rentDate[100][3];//���100����ͬ���� ����yyyy/mm/dd rentDate[x][3] xӦ��usernameͬ��
	int creditLimit;//���������������ֵ
	struct book* next;
};

struct user* crtUser(int length)//����ʧ���򷵻�NULL lengthΪ0ʱֻ�½�ͷ
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
			if (node)//ע�� Ԫ��δ��ʼ��
			{
				end->next = node;
				end = node;
			}
			else
				i--;//����
		}
		end->next = NULL;
	}
	return start;
}

void modUser(struct user* list, int order, struct user temp)//����ڵ����������Ϊ
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

		*target = temp;//���ƽṹ��
	}
}

void delUser(struct user* list, int order)//����ڵ����������Ϊ
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

void insUser(struct user* list, int order, struct user temp)//����ڵ����������Ϊ
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

		*node = temp;//���ƽṹ��
	}
}

int lenUser(struct user* list)//��������򷵻�-1 ֻ��ͷ�򷵻�0
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

struct user* viewUser(struct user* list, int order)//�鿴ָ���ڵ㷵�ظýڵ�ָ�� �����򷵻�NULL
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

int searchUser(struct user* list, int start, char keyWord[11])//��username���� �Ҳ���������򷵻�-1 ��ƥ���򷵻ظýڵ��������е�˳��
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

int searchUser_Acc(struct user* list, char username[11])//��usernameȫ��ƥ�� �Ҳ���������򷵻�-1 ��ƥ���򷵻ظýڵ��������е�˳��
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

struct book* crtBook(int length)//����ʧ���򷵻�NULL lengthΪ0ʱֻ�½�ͷ
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
			if (node)//ע�� Ԫ��δ��ʼ��
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

void modBook(struct book* list, int order, struct book temp)//����ڵ����������Ϊ
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

		*target = temp;//�ṹ�帴��
	}
}

void delBook(struct book* list, int order)//����ڵ����������Ϊ
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

void insBook(struct book* list, int order, struct book temp)//����ڵ����������Ϊ
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

		*node = temp;//�ṹ�帴��
	}
}

int lenBook(struct book* list)//��������򷵻�-1 ֻ��ͷ�򷵻�0
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

struct book* viewBook(struct book* list, int order)//�鿴ָ���ڵ㷵�ظýڵ�ָ�� �����򷵻�NULL
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

int searchBook(struct book* list, int start, char keyWord[101])//startΪ��ʼλ�� ��bookname��author�Ĺؼ��ֲ��� �Ҳ���������򷵻�-1 ��ƥ���򷵻ظýڵ��������е�˳��
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

int searchBook_Acc(struct book* list, char bookname[101])//��booknameȫ��ƥ�� �Ҳ���������򷵻�-1 ��ƥ���򷵻ظýڵ��������е�˳��
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

void delAll(struct user* userList, struct book* bookList)//ɾ���������� �����ڴ�
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

void bubbleSortBook(struct book* list, char type, int mode)//����ڵ��Ƚ�ģʽ����������Ϊ ����lenBook()��viewBook()
{                                                                                              //type=='p'ʱ���۸����� type=='n'ʱ���������� type=='c'ʱ������ֵҪ������ mode==1ʱ�Ӵ�С mode==2ʱ��С����
	int length = lenBook(list);
	int i, n, swap = 0;
	for (i = length - 1; i > 0; i--)
	{
		for (n = 1; n <= i; n++)
		{
			swap = 0;//Ĭ�ϲ�����
			if (type == 'p' && mode == 1 && (viewBook(list, n)->price < viewBook(list, n + 1)->price)) //�Ƚϼ۸� �Ӵ�С
				swap = 1;
			else if (type == 'p' && mode == 2 && (viewBook(list, n)->price > viewBook(list, n + 1)->price)) //�Ƚϼ۸� ��С����
				swap = 1;
			else if (type == 'n' && mode == 1 && (viewBook(list, n)->num < viewBook(list, n + 1)->num)) //�Ƚ�ʣ���� �Ӵ�С
				swap = 1;
			else if (type == 'n' && mode == 2 && (viewBook(list, n)->num > viewBook(list, n + 1)->num)) //�Ƚ�ʣ���� ��С����
				swap = 1;
			else if (type == 'c' && mode == 1 && (viewBook(list, n)->creditLimit < viewBook(list, n + 1)->creditLimit)) //�Ƚ�����ֵ���� �Ӵ�С
				swap = 1;
			else if (type == 'c' && mode == 2 && (viewBook(list, n)->creditLimit > viewBook(list, n + 1)->creditLimit)) //�Ƚ�����ֵ���� ��С����
				swap = 1;
			else
				return;
			if (swap == 1)
			{
				struct book* temp = list;
				int j;
				for (j = 0; j < n - 1; j++)
					temp = temp->next;
				struct book* m = temp->next;//��n��
				temp->next = m->next;//n-1���ӵ�n+1
				m->next = (temp->next)->next;//n���ӵ�n+2�������βNULL
				(temp->next)->next = m;//n+1���ӵ�n
			}
		}
	}
}

int is_rented(struct book* list, int bookOrder)//�Ƿ��ѱ���� �ѱ��������1 δ���������0 ����viewBook()
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

void returnBook_U(struct book* bookList, struct user* userList, int userOrder)//�����û����ļ�¼ ����viewUser() viewBook()��searchBook_Acc()
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

int rentCount(struct book* bookList, int bookOrder)//����ָ���鼮�������� ����viewBook()
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

struct book* rank(struct book* list)//�������Ĵ������� ����������������׵�ַ ���� �ú���Ҫ�ֶ�ɾ�����ص�����
{                                                      //����crtBook() lenBook() insBook() viewBook() rentCount()
	struct book* sort = crtBook(0);//��������
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

				struct book* m = temp->next;//��n��
				temp->next = m->next;//n-1���ӵ�n+1
				m->next = (temp->next)->next;//n���ӵ�n+2�������βNULL
				(temp->next)->next = m;//n+1���ӵ�n
			}
		}
	}

	return sort;
}

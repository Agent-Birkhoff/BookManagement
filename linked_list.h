#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

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

extern struct user* crtUser(int length);//����ʧ���򷵻�NULL lengthΪ0ʱֻ�½�ͷ
extern void modUser(struct user* list, int order, struct user temp);//����ڵ����������Ϊ
extern void delUser(struct user* list, int order);//����ڵ����������Ϊ
extern void insUser(struct user* list, int order, struct user temp);//����ڵ����������Ϊ
extern int lenUser(struct user* list);//��������򷵻�-1 ֻ��ͷ�򷵻�0
extern struct user* viewUser(struct user* list, int order);//�鿴ָ���ڵ㷵�ظýڵ�ָ�� �����򷵻�NULL
extern int searchUser(struct user* list, int start, char keyWord[11]);//startΪ��ʼλ�� ��username�Ĺؼ��ֲ��� �Ҳ���������򷵻�-1 ��ƥ���򷵻ظýڵ��������е�˳��
extern int searchUser_Acc(struct user* list, char username[11]);//��usernameȫ��ƥ�� �Ҳ���������򷵻�-1 ��ƥ���򷵻ظýڵ��������е�˳��
extern struct book* crtBook(int length);//����ʧ���򷵻�NULL lengthΪ0ʱֻ�½�ͷ
extern void modBook(struct book* list, int order, struct book temp);//����ڵ����������Ϊ
extern void delBook(struct book* list, int order);//����ڵ����������Ϊ
extern void insBook(struct book* list, int order, struct book temp);//����ڵ����������Ϊ
extern int lenBook(struct book* list);//��������򷵻�-1 ֻ��ͷ�򷵻�0
extern struct book* viewBook(struct book* list, int order);//�鿴ָ���ڵ㷵�ظýڵ�ָ�� �����򷵻�NULL
extern int searchBook(struct book* list, int start, char keyWord[101]);//startΪ��ʼλ�� ��bookname��author�Ĺؼ��ֲ��� �Ҳ���������򷵻�-1 ��ƥ���򷵻ظýڵ��������е�˳��
extern int searchBook_Acc(struct book* list, char bookname[101]);//��booknameȫ��ƥ�� �Ҳ���������򷵻�-1 ��ƥ���򷵻ظýڵ��������е�˳��
extern void delAll(struct user* userList, struct book* bookList);//ɾ���������� �����ڴ�
extern void bubbleSortBook(struct book* list, char type, int mode);//����ڵ��Ƚ�ģʽ����������Ϊ ����lenBook()��viewBook()
																											//type=='p'ʱ���۸����� type=='n'ʱ���������� type=='c'ʱ������ֵҪ������ mode==1ʱ�Ӵ�С mode==2ʱ��С����
extern int is_rented(struct book* list, int bookOrder);//�Ƿ��ѱ���� �ѱ��������1 δ���������0 ����viewBook()
extern void returnBook_U(struct book* bookList, struct user* userList, int userOrder);//�����û����ļ�¼ ����viewUser() viewBook()��searchBook_Acc()
extern int rentCount(struct book* bookList, int bookOrder);//����ָ���鼮�������� ����viewBook()
extern struct book* rank(struct book* list);//�������Ĵ������� ����������������׵�ַ ���� �ú���Ҫ�ֶ�ɾ�����ص�����
																   //����crtBook() lenBook() insBook() viewBook() rentCount()

#endif

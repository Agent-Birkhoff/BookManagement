#define _CRT_SECURE_NO_WARNINGS//����SDL
#pragma warning( disable : 6031)//���������scanf����ֵ����
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"linked_list.h"
#include"file.h"
#include"date.h"

extern void admin(struct settings* preference, struct user* userList, struct book* bookList);
extern void user(int userOrder, struct settings preference, struct user* userList, struct book* bookList);

void login(struct settings* preference, struct user* userList, struct book* bookList);//��¼
void regist(struct user* userList);//ע��

int main(void)
{
	struct settings preference = readSettings();//��ȡ����
	struct user* userList = readUser();//��ȡ�û�����
	if (userList == NULL)//���������½�����ͷ
		userList = crtUser(0);
	struct book* bookList = readBook();//��ȡ�鼮����
	if (bookList == NULL)//���������½�����ͷ
		bookList = crtBook(0);

	char select = '0';//��ѡ��
	while (1)//�Զ�ˢ����Ļ
	{
		system("cls");//����
		printf("\33[4;34;47m��ӭʹ��ͼ�����ϵͳ!\n\n\33[0m");
		printf("���¹���:%s\n\n", preference.bulletin);
		printf("1. ��¼\n");
		printf("2. ע��\n");
		printf("0. �˳�\n\n");
		printf("\33[33m��ѡ��:\33[0m");
		rewind(stdin);//������뻺���� fflush()�޷���vs2019�������������� ��ʹ��rewind()����
		scanf("%c", &select);
		switch (select)
		{
		case '1':
			login(&preference, userList, bookList);
			break;
		case '2':
			regist(userList);
			break;
		case '0':
			delAll(userList, bookList);//ɾ����������ڴ�
			exit(0);
		case '\n':
			printf("\33[31m������ѡ�����!\n\33[0m");
			system("timeout /t 1");//��ͣ1����
			break;
		default:
			printf("\33[31m�������!\n\33[0m");
			system("timeout /t 1");//��ͣ1����
			break;
		}
	}
	return 0;
}

void login(struct settings* preference, struct user* userList, struct book* bookList)
{
	int tryTime = 0;
	while (1)
	{
		system("cls");
		printf("\33[7m��¼\n\n\33[0m");
		printf("�������û���(����0����):");
		char username[11] = { 0 };
		rewind(stdin);
		scanf_s("%s", username, 11);
		if (strcmp(username, "0\0") == 0)//����0�򷵻ؿ�ʼ����
			return;
		printf("����������(����0����):");
		char passwd[19] = { 0 };
		rewind(stdin);
		scanf_s("%s", passwd, 19);
		if (strcmp(passwd, "0\0") == 0)//����0�򷵻ؿ�ʼ����
			return;
		else if (strcmp(username, "admin\0") == 0)//����Ա�û���admin
		{
			if (strcmp(passwd, preference->admin_passwd) == 0)
			{
				tryTime = 0;
				timer = time(&timer);
				admin(preference, userList, bookList);
				return;
			}
			else
			{
				if (tryTime > 10)//�������ƽ� Ĭ���������10�δ�����
				{
					delAll(userList, bookList);//ɾ����������ڴ�
					exit(0);
				}
				tryTime++;
				printf("\33[31m�������!\33[0m");
				system("timeout /t 1");
				continue;
			}
		}
		else
		{
			int userOrder = searchUser_Acc(userList, username);
			if (userOrder != -1)
			{
				if (strcmp(passwd, viewUser(userList, userOrder)->password) == 0)
				{
					tryTime = 0;
					timer = time(&timer);
					user(userOrder, *preference, userList, bookList);//��ת����Ӧ�û�����
					return;
				}
				else
				{
					if (tryTime > 10)//�������ƽ� Ĭ���������10�δ�����
					{
						delAll(userList, bookList);//ɾ����������ڴ�
						exit(0);
					}
					tryTime++;
					printf("\33[31m�������!\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			else
			{
				printf("\33[31m�û�������!\33[0m");
				system("timeout /t 1");
				continue;
			}
		}
	}
}

void regist(struct user* userList)
{
	while (1)
	{
		system("cls");
		struct user userT = { {0},{0},{0},{0},{0}, 100 ,{0},{0}, NULL };
		printf("\33[3mע��\n\n\33[0m");

		printf("(����0�Է����ϲ�˵�)\n");
		printf("�������û���(�10λ):");
		rewind(stdin);
		scanf_s("%s", userT.username, 11);
		if (strcmp(userT.username, "0\0") == 0)//����0�򷵻ؿ�ʼ����
			return;
		if (searchUser_Acc(userList, userT.username) != -1 || strcmp(userT.username, "admin\0") == 0)
		{
			printf("\33[31m���û����ѱ�ռ��!\33[0m");
			system("timeout /t 1");
			continue;
		}
		if (userT.username[0] == '\0')
		{
			printf("\33[31m�����ַ�������!\33[0m");
			system("timeout /t 1");
			continue;
		}

		while (1)
		{
			printf("����������(8-18λ):");
			rewind(stdin);//ˢ�»�����
			scanf_s("%s", userT.password, 19);
			if (strcmp(userT.password, "0\0") == 0)//����0�򷵻ؿ�ʼ����
				return;
			if (strlen(userT.password) < 8)
			{
				printf("\33[31m����ӦΪ8-18λ!\n\33[0m");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("��������ʵ����:");
			rewind(stdin);//ˢ�»�����
			scanf_s("%s", userT.realName, 101);
			if (strcmp(userT.realName, "0\0") == 0)//����0�򷵻ؿ�ʼ����
				return;
			if (userT.realName[0] == '\0')
			{
				printf("\33[31m�����ַ�������!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("����ID����(���18λ):");
			rewind(stdin);//ˢ�»�����
			scanf_s("%s", userT.ID, 19);
			if (strcmp(userT.ID, "0\0") == 0)//����0�򷵻ؿ�ʼ����
				return;
			if (userT.ID[0] == '\0')
			{
				printf("\33[31m�����ַ�������!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("��������ϵ��ʽ(11λ�ֻ�����):");
			rewind(stdin);//ˢ�»�����
			scanf_s("%s", userT.phoneNumber, 12);
			if (strcmp(userT.phoneNumber, "0\0") == 0)//����0�򷵻ؿ�ʼ����
				return;
			if (strlen(userT.phoneNumber) != 11)
			{
				printf("\33[31m�ֻ�����ӦΪ11λ!\n\33[0m");
				continue;
			}
			break;
		}

		while (1)
		{
			system("cls");
			printf("\33[3m�����������Ϣ�Ƿ���ȷ\n\n\33[0m");
			printf("�û���:   %s\n", userT.username);
			printf("����:     %s\n", userT.password);
			printf("��ʵ����: %s\n", userT.realName);
			printf("��ϵ��ʽ: %s\n", userT.phoneNumber);
			printf("ID����:   %s\n\n", userT.ID);
			printf("\33[33m����yȷ��ע��,����n����ע��,����0����ע�Ტ����:\33[0m");
			char input;
			rewind(stdin);
			scanf("%c", &input);

			switch (input)
			{
			case 'y':
				insUser(userList, lenUser(userList) + 1, userT);//���û������β����ڵ�
				saveUser(userList);//д�����
				printf("\33[32mע��ɹ�!\33[0m");
				system("timeout /t 1");
				return;
			case 'n':
				break;
			case '0':
				return;
			default:
				printf("\33[31m�������!\33[0m");
				system("timeout /t 1");
				continue;
			}

			break;
		}
	}
}

#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 6031)//���������scanf����ֵ����
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"file.h"
#include"linked_list.h"
#include"date.h"

int listBksBrwd(int userOrder, struct settings preference, struct user* userList, struct book* bookList);//��ӡ�ѽ��鼮
int borrowLimit(int userOrder, struct settings preference, struct user* userList);//�����Խ輸���� �����򷵻س�������(����)
void returnBook(int userOrder, struct settings preference, struct user* userList, struct book* bookList);//����
void borrowBook(int userOrder, struct settings preference, struct user* userList, struct book* bookList);//����
void do_borrow(int userOrder, struct settings preference, struct user* userList, struct book* bookList, int bookOrder);//ִ�н������
void allBook(int userOrder, struct settings preference, struct user* userList, struct book* bookList);//�г������鼮
void modifyInfo(int userOrder, struct settings preference, struct user* userList, struct book* bookList);//�޸���Ϣ
extern int bookRank(struct book* list);//���ܻ�ӭ�鼮���а� ��ֱ����ʾ(����)ǰ���� �¼�����鼮���� ����0����ѡ�鼮��� ��admin.c��

void user(int userOrder, struct settings preference, struct user* userList, struct book* bookList)
{
	char select = '0';
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		printf("\33[7m��ӭ��,%s!\n\33[0m", viewUser(userList, userOrder)->username);
		int credit = remainCredit(preference, userList, bookList, userOrder);
		printf("��ǰ��������ֵΪ\33[36m%d\33[0m\n", credit);
		if (credit < 100)
			printf("\33[31m��������ֵ�Ѳ���100,�밴ʱ�黹�鼮����ϵ����Ա���ɷ���!\n\33[0m");
		printf("\n");
		printf("1. �黹�鼮\n");
		printf("2. ����(����)�鼮\n");
		printf("3. �鿴�����鼮\n");
		printf("4. �鿴���ܻ�ӭ�鼮���а�\n");
		printf("5. �޸ĸ�����Ϣ\n");
		printf("0. �˳���¼\n\n");
		printf("\33[33m��ѡ��:\33[0m");
		rewind(stdin);
		scanf("%c", &select);
		int bookOrder;
		struct book bookT;
		char input = 'n';
		switch (select)
		{
		case '1':
			returnBook(userOrder, preference, userList, bookList);
			break;
		case '2':
			borrowBook(userOrder, preference, userList, bookList);
			break;
		case '3':
			allBook(userOrder, preference, userList, bookList);
			break;
		case '4':
			while (1)
			{
				bookOrder = bookRank(bookList);
				if (bookOrder == 0)
					break;
				bookT = *viewBook(bookList, bookOrder);
				printf("\n����:%s ����:%s ���:%s ������:%s\n", bookT.name, bookT.author, bookT.category, bookT.publisher);
				printf("\33[33m����yȷ�Ͻ���,���������ַ�ȡ��:\33[0m");
				rewind(stdin);
				scanf("%c", &input);
				if (input == 'y')
				{
					do_borrow(userOrder, preference, userList, bookList, bookOrder);//����
					break;
				}
			}
			break;
		case '5':
			modifyInfo(userOrder, preference, userList, bookList);
			break;
		case '0':
			return;
		default:
			printf("\33[31m�������!\33[0m");
			system("timeout /t 1");
			break;
		}
	}
}

int listBksBrwd(int userOrder, struct settings preference, struct user* userList, struct book* bookList)
{
	int record = 0;
	struct user userT = *viewUser(userList, userOrder);//��ȡ����ӡ�û���Ϣ
	int n;
	for (n = 0; n < 100; n++)//��ӡ������Ϣ
	{
		if (userT.bksBrwd[n][0] == '\0')
			continue;
		record = 1;
		int bookOrder = searchBook_Acc(bookList, userT.bksBrwd[n]);
		if (bookOrder == -1)
			printf("���:\33[31m��\33[0m ����:%s ", userT.bksBrwd[n]);//�����ڵ����ޱ��
		else
			printf("���:\33[36m%d\33[0m ����:%s ", bookOrder, userT.bksBrwd[n]);
		if (bookOrder != -1)
		{
			int remainDays = isExpired(bookList, bookOrder, userT.whichOne[n], preference.config_rent[1]);
			if (remainDays > 0)
				printf("\33[32mʣ������:%d��\n\33[0m", remainDays);
			else if (remainDays == 0)
				printf("\33[33mʣ�����ڲ���һ��!\n\33[0m");
			else
				printf("\33[31m�ѳ�������%d��! ��ÿ��۳�%d����ֵ!\n\33[0m", -remainDays, creditPunish(preference, bookList, bookOrder));
		}
		else
			printf("\33[31m���鲻����(�ѱ�ɾ��),����ϵ����Ա!\n\33[0m");
	}
	return record;
}

int borrowLimit(int userOrder, struct settings preference, struct user* userList)
{
	struct user userT = *viewUser(userList, userOrder);
	int count = 0;
	int n;
	for (n = 0; n < 100; n++)
	{
		if (userT.bksBrwd[n][0] != '\0')
			count++;
	}
	return (preference.config_rent[0] - count);
}

void returnBook(int userOrder, struct settings preference, struct user* userList, struct book* bookList)
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		printf("\33[7m�黹�鼮\n\33[0m");
		struct user userT = *viewUser(userList, userOrder);//�����Ż�
		int credit = remainCredit(preference, userList, bookList, userOrder);
		printf("%s,��ǰ��������ֵΪ\33[36m%d\33[0m\n", userT.username, credit);
		if (credit < 100)
			printf("\33[31m��������ֵ�Ѳ���100,�밴ʱ�黹�鼮����ϵ����Ա���ɷ���!\n\33[0m");
		printf("\n");
		printf("Ŀǰ���ѽ��鼮:\n");
		if (listBksBrwd(userOrder, preference, userList, bookList) == 0)//��ʾ�ѽ��鼮��Ϣ
		{
			printf("\n\33[31mû�н��ļ�¼!\33[0m\n\n");
			system("pause");
			return;
		}
		printf("\n");
		int bookOrder;
		printf("\33[33m������Ҫ�黹�鼮�ı��(����0����):\33[0m");
		rewind(stdin);
		scanf("%d", &bookOrder);
		autoLogOut(timer);//�Զ�ע������
		if (bookOrder == 0)
			return;
		if (bookOrder<0 || bookOrder>lenBook(bookList))
		{
			printf("\33[31m���޴���!\33[0m");
			system("timeout /t 1");
			continue;
		}
		struct book bookT = *viewBook(bookList, bookOrder);
		int i, old = -1;
		for (i = 0; i < 100; i++)
		{
			if (strcmp(userT.bksBrwd[i], bookT.name) == 0)
			{
				old = i;//��ʼֵ
				break;
			}
		}
		if (old >= 0)
		{
			for (i = old + 1; i < 100; i++)
			{
				if (strcmp(userT.bksBrwd[i], bookT.name) == 0)
				{
					int y_i = bookT.rentDate[userT.whichOne[i]][0];
					int m_i = bookT.rentDate[userT.whichOne[i]][1];
					int d_i = bookT.rentDate[userT.whichOne[i]][2];
					int y_old = bookT.rentDate[userT.whichOne[old]][0];
					int m_old = bookT.rentDate[userT.whichOne[old]][1];
					int d_old = bookT.rentDate[userT.whichOne[old]][2];
					if (y_i < y_old)
						old = i;
					else if (y_i == y_old && m_i < m_old)
						old = i;
					else if (y_i == y_old && m_i == m_old && d_i <= d_old)
						old = i;
				}
			}

			int n = userT.whichOne[old];
			bookT.username[n][0] = '\0';
			bookT.rentDate[n][0] = 0;
			bookT.rentDate[n][1] = 0;
			bookT.rentDate[n][2] = 0;
			bookT.num++;
			userT.bksBrwd[old][0] = '\0';
			userT.whichOne[old] = 0;
			int remainDays = isExpired(bookList, bookOrder, n, preference.config_rent[1]);
			if (remainDays < 0)
				userT.credit += (int)(preference.config_credit[1] * bookT.price * remainDays);//�۳�����ֵ
			else
			{
				int pastDays = preference.config_rent[1] - remainDays;
				userT.credit += (int)(preference.config_credit[0] * pastDays);
				if (userT.credit > 100)
					userT.credit = 100;
				else if (userT.credit < 0)
					userT.credit = 0;
			}
			modBook(bookList, bookOrder, bookT);//�޸��鼮����
			saveBook(bookList);//д�����
			modUser(userList, userOrder, userT);
			saveUser(userList);//д�����
			printf("\33[32m�黹�ɹ�!\33[0m");
			system("timeout /t 1");
			continue;
		}
		else
		{
			printf("\33[31m��û�н����Ȿ��!\33[0m");
			system("timeout /t 1");
		}
	}
}

void borrowBook(int userOrder, struct settings preference, struct user* userList, struct book* bookList)
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		printf("\33[7m����(����)�鼮\n\33[0m");
		int credit = remainCredit(preference, userList, bookList, userOrder);
		printf("%s,��ǰ��������ֵΪ\33[36m%d\33[0m\n", viewUser(userList, userOrder)->username, credit);
		if (credit < 100)
			printf("\33[31m��������ֵ�Ѳ���100,�밴ʱ�黹�鼮����ϵ����Ա���ɷ���!\n\33[0m");
		int remainNum = borrowLimit(userOrder, preference, userList);
		printf("���ѽ���%d����,", preference.config_rent[0] - remainNum);
		if (remainNum <= 0)
			printf("\33[31m�ѵִ������������!\n\n\33[0m");
		else
			printf("\33[32m���ɽ���%d��\n\n\33[0m", remainNum);

		printf("\33[33m������(���������ߵ�)�ؼ���(����0����):\33[0m");
		char keyWord[101] = { 0 };
		rewind(stdin);
		scanf_s("%s", keyWord, 101);
		if (strcmp(keyWord, "0\0") == 0)
			return;

		printf("\n");
		printf("     ����                 ����                 ���       ������               ����ֵ���� ���\n");//��ʽ���Ż�
		int n;
		int is_none = 1;//��������Ƿ�Ϊ��
		for (n = 1; n <= lenBook(bookList); n++)
		{
			n = searchBook(bookList, n, keyWord);
			if (n != -1)
			{
				struct book bookT = *viewBook(bookList, n);
				if (bookT.num > 0)
					printf("\33[32m�ɽ� \33[0m");
				else
					printf("\33[31m���� \33[0m");
				printf("%-20s %-20s %-10s %-20s    %3d    \33[36m%3d\33[0m\n", bookT.name, bookT.author, bookT.category, bookT.publisher, bookT.creditLimit, n);
				is_none = 0;//���������Ϊ��
			}
			else
				break;
		}
		if (is_none == 1)
		{
			printf("\n\33[31mû���ҵ��κν��!\33[0m\n\n");
			system("pause");
			continue;
		}

		while (1)
		{
			printf("\n");
			printf("\33[33m�������������Ķ�Ӧ�鼮(����-1��������,����0����):\33[0m");
			int select;
			rewind(stdin);
			scanf("%d", &select);
			if (select == -1)
				break;
			if (select == 0)
				return;
			if (select < 0 || select>lenBook(bookList))
			{
				printf("\33[31m���޴���!\n\33[0m");
				system("timeout /t 1");
				continue;
			}
			else
			{
				do_borrow(userOrder, preference, userList, bookList, select);
				break;
			}
		}
	}
}

void do_borrow(int userOrder, struct settings preference, struct user* userList, struct book* bookList, int bookOrder)
{
	autoLogOut(timer);//�Զ�ע������
	struct book bookT = *viewBook(bookList, bookOrder);
	int i, available = -1;
	for (i = 0; i < 100; i++)
	{
		if (bookT.username[i][0] == '\0')
		{
			available = i;
			break;
		}
	}
	if (bookT.num <= 0 || available == -1)
	{
		printf("\n\33[31m�����ѱ�����,�޷�����!\33[0m\n");
		system("pause");
		return;
	}
	else if (borrowLimit(userOrder, preference, userList) <= 0)
	{
		printf("\n\33[31m���Ľ��������ѵִ�����,�޷�����!\33[0m\n");
		system("pause");
		return;
	}
	else if (remainCredit(preference, userList, bookList, userOrder) < bookT.creditLimit)
	{
		printf("\n\33[31m����ֵ����,�޷�����!\33[0m\n");
		system("pause");
		return;
	}
	else
	{
		struct user userT = *viewUser(userList, userOrder);//��Լ��Դ
		int n;
		for (n = 0; n < 100; n++)
		{
			if (userT.bksBrwd[n][0] == '\0')//�ҵ���λ��
			{
				strcpy(userT.bksBrwd[n], bookT.name);
				userT.whichOne[n] = available;
				strcpy(bookT.username[available], userT.username);
				getDate(&bookT.rentDate[available][0], &bookT.rentDate[available][1], &bookT.rentDate[available][2]);
				bookT.num--;
				modUser(userList, userOrder, userT);//�޸��û�����
				modBook(bookList, bookOrder, bookT);//�޸��鼮����
				saveUser(userList);//�������
				saveBook(bookList);//�������
				break;
			}
		}
		printf("\n\33[32m���ĳɹ�!\33[0m");
		system("timeout /t 1");
		return;
	}
}

void allBook(int userOrder, struct settings preference, struct user* userList, struct book* bookList)
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		printf("\33[7mȫ���鼮�б�:\n\33[0m");
		int credit = remainCredit(preference, userList, bookList, userOrder);
		printf("%s,��ǰ��������ֵΪ\33[36m%d\33[0m\n", viewUser(userList, userOrder)->username, credit);
		if (credit < 100)
			printf("\33[31m��������ֵ�Ѳ���100,�밴ʱ�黹�鼮����ϵ����Ա���ɷ���!\n\33[0m");
		int remainNum = borrowLimit(userOrder, preference, userList);
		printf("���ѽ���%d����,", preference.config_rent[0] - remainNum);
		if (remainNum <= 0)
			printf("\33[31m�ѵִ������������!\n\n\33[0m");
		else
			printf("\33[32m���ɽ���%d��\n\n\33[0m", remainNum);

		printf("     ����                 ����                 ���       ������               ����ֵ���� ���\n");//��ʽ���Ż�
		if (lenBook(bookList) == 0)
		{
			printf("\n\33[31m�������κ��鼮!\33[0m\n\n");
			system("pause");
			return;
		}
		int n;
		for (n = 1; n <= lenBook(bookList); n++)
		{
			struct book bookT = *viewBook(bookList, n);
			if (bookT.num > 0)
				printf("\33[32m�ɽ� \33[0m");
			else
				printf("\33[31m���� \33[0m");
			printf("%-20s %-20s %-10s %-20s    %3d    \33[36m%3d\33[0m\n", bookT.name, bookT.author, bookT.category, bookT.publisher, bookT.creditLimit, n);
		}

		while (1)
		{
			printf("\n");
			printf("\33[33m�������������Ķ�Ӧ�鼮(����0����):\33[0m");
			int select;
			rewind(stdin);
			scanf("%d", &select);
			if (select == 0)
				return;
			if (select < 0 || select>lenBook(bookList))
			{
				printf("\33[31m���޴���!\n\33[0m");
				system("timeout /t 1");
				continue;
			}
			else
			{
				do_borrow(userOrder, preference, userList, bookList, select);
				break;
			}
		}
	}
}

void modifyInfo(int userOrder, struct settings preference, struct user* userList, struct book* bookList)
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		printf("\33[7m�޸ĸ�����Ϣ\n\33[0m");
		struct user userT = *viewUser(userList, userOrder);
		int credit = remainCredit(preference, userList, bookList, userOrder);
		printf("%s,��ǰ��������ֵΪ\33[36m%d\33[0m\n", userT.username, credit);
		if (credit < 100)
			printf("\33[31m��������ֵ�Ѳ���100,�밴ʱ�黹�鼮����ϵ����Ա���ɷ���!\n\33[0m");

		printf("\n");
		printf("��ǰ���ĸ�����Ϣ����:\n");
		printf("1. ����: %s\n", userT.password);
		printf("2. ��ʵ����: %s\n", userT.realName);
		printf("3. ��ϵ��ʽ: %s\n", userT.phoneNumber);
		printf("4. ID����: %s\n", userT.ID);
		printf("\33[33m��ѡ��Ҫ�޸ĵ���Ŀ(����0����):\33[0m");
		char choice = '0';
		rewind(stdin);
		scanf("%c", &choice);
		autoLogOut(timer);//�Զ�ע������
		char newString[101] = { 0 };
		printf("\n");
		rewind(stdin);
		switch (choice)
		{
		case'1':
			while (1)
			{
				printf("������������(8-18λ):");
				scanf_s("%s", newString, 19);
				if (strlen(newString) < 8)
				{
					printf("\33[31m���볤�Ȳ���!����������!\33[0m");
					system("timeout /t 1");
					continue;
				}
				char repeat[19];
				printf("���ظ�����������:");
				rewind(stdin);
				scanf_s("%s", repeat, 19);
				if (strcmp(repeat, newString) == 0)
				{
					strcpy(viewUser(userList, userOrder)->password, newString);
					break;
				}
				else
				{
					printf("\33[31m������������벻һ��,��ȡ������!\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'2':
			printf("��������ȷ����ʵ����:");
			scanf_s("%s", newString, 101);
			if (newString[0] != '\0')
				strcpy(viewUser(userList, userOrder)->realName, newString);
			else
			{
				printf("\33[31m����Ϊ��!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'3':
			printf("�������µ���ϵ��ʽ(11λ):");
			scanf_s("%s", newString, 12);
			if (strlen(newString) == 11)
				strcpy(viewUser(userList, userOrder)->phoneNumber, newString);
			else
			{
				printf("\33[31m���ȴ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'4':
			printf("������ȷ��ID����(�18λ):");
			scanf_s("%s", newString, 19);
			if (newString[0] != '\0')
				strcpy(viewUser(userList, userOrder)->ID, newString);
			else
			{
				printf("\33[31m����Ϊ��!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'0':
			return;
		default:
			printf("\33[31m�������!\33[0m");
			system("timeout /t 1");
			continue;
		}
		saveUser(userList);//�������
		printf("\33[32m�޸ĳɹ�!\33[0m");
		system("timeout /t 1");
	}
}

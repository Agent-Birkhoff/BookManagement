#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 6031)//���������scanf����ֵ����
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"linked_list.h"
#include"file.h"
#include"date.h"

void overview(struct settings* preference, struct book* bookList, struct user* userList);//���ݸ���
void listBook(struct settings* preference, struct book* bookList, struct user* userList);//��ʾ�����鼮
int listUser(struct settings* preference, struct user* userList, struct book* bookList);//��ʾ�����û�
void addBook(struct settings* preference, struct book* list);//�����鼮��������Ŀ
void checkBook(struct settings* preference, struct book* bookList, struct user* userList, int bookOrder);//�����鼮
void bookDetails(struct settings* preference, struct book* bookList, struct user* userList, int bookOrder);//�޸�ɾ���鼮
void checkUser(struct settings* preference, struct user* userList, struct book* bookList, int userOrder);//�����û�
void userDetails(struct settings* preference, struct user* userList, struct book* bookList, int userOrder);//�޸�ɾ���û�
void config(struct settings* preference);//����
int bookRank(struct book* list);//���ܻ�ӭ�鼮���а� ��ֱ����ʾ(����)ǰ���� �¼�����鼮���� ����0����ѡ�鼮���

void admin(struct settings* preference, struct user* userList, struct book* bookList)
{
	char select = '0';
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		printf("\33[7m�������\n\33[0m");
		printf("�ϴθ�������: \33[36m%4d��%02d��%02d��\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);//��ʾ�ϴθ�������
		printf("1. ���ݸ���\n");
		printf("2. ����(������)�鼮\n");
		printf("3. ����(���޸�)�鼮\n");
		printf("4. ����(���޸�)�û�\n");
		printf("5. ����\n");
		printf("0. �˳���¼\n\n");
		printf("\33[33m��ѡ��:\33[0m");
		rewind(stdin);
		scanf("%c", &select);
		switch (select)
		{
		case'1':
			overview(preference, bookList, userList);
			break;
		case '2':
			addBook(preference, bookList);
			break;
		case '3':
			checkBook(preference, bookList, userList, 0);
			break;
		case '4':
			checkUser(preference, userList, bookList, 0);
			break;
		case '5':
			config(preference);
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

void overview(struct settings* preference, struct book* bookList, struct user* userList)
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		printf("\33[7m���ݸ���\n\33[0m");
		printf("�ϴθ�������: \33[36m%4d��%02d��%02d��\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);

		int i;
		int length = lenBook(bookList);
		int sumBook = 0, remainBook = 0;
		for (i = 1; i <= length; i++)//����ÿ���鼮
		{
			struct book bookT = *viewBook(bookList, i);
			int j;
			for (j = 0; j < 100; j++)//ͳ�Ƹ������ѱ����������
			{
				if (bookT.username[j][0] != '\0')
					sumBook += 1;
			}
			sumBook += bookT.num;
			remainBook += bookT.num;
		}
		printf("Ŀǰ���й�%d����,��%d��,ʣ��%d��\n", length, sumBook, remainBook);
		length = lenUser(userList);
		printf("Ŀǰ��ϵͳ��ע����%d���û�\n\n", length);

		char select = '0';
		int order = 0;
		printf("1. �鿴���ܻ�ӭ�鼮���а�\n");
		printf("2. �鿴�����鼮\n");
		printf("3. �鿴�����û�\n");
		printf("\33[33m��ѡ��(����0����):\33[0m");
		rewind(stdin);
		scanf("%c", &select);
		switch (select)
		{
		case'1':
			while (1)
			{
				order = bookRank(bookList);
				if (order != 0)
					checkBook(preference, bookList, userList, order);
				else
					break;
			}
			break;
		case '2':
			listBook(preference, bookList, userList);//���ò鿴�޸ĵȹ��ܱ����뺯������
			break;
		case '3':
			while (1)
			{
				order = listUser(preference, userList, bookList);
				if (order != 0)
					checkUser(preference, userList, bookList, order);
				else
					break;
			}
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

void listBook(struct settings* preference, struct book* bookList, struct user* userList)
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		char type;
		int mode;
		char select = '0';
		printf("\33[7m��ʾ�����鼮\n\n\33[0m");
		printf("1. ���۸�����\n");
		printf("2. ����������\n");
		printf("3. ������ֵҪ������\n");
		printf("\33[33m��ѡ������ģʽ(����0����):\33[0m");
		rewind(stdin);
		scanf("%c", &select);
		if (select == '1')
			type = 'p';
		else if (select == '2')
			type = 'n';
		else if (select == '3')
			type = 'c';
		else if (select == '0')
			return;
		else
		{
			printf("\33[31m�������!\33[0m");
			system("timeout /t 1");
			continue;
		}
		printf("\n");
		printf("1. �Ӵ�С\n");
		printf("2. ��С����\n");
		printf("\33[33m��ѡ������ʽ(����0����):\33[0m");
		rewind(stdin);
		scanf("%c", &select);
		if (select == '1')
			mode = 1;
		else if (select == '2')
			mode = 2;
		else if (select == '0')
			return;
		else
		{
			printf("\33[31m�������!\33[0m");
			system("timeout /t 1");
			continue;
		}

		while (1)
		{
			struct book* sort = crtBook(0);//��������
			int n;
			for (n = 1; n <= lenBook(bookList); n++)
				insBook(sort, n, *viewBook(bookList, n));
			bubbleSortBook(sort, type, mode);//���鼮��������

			system("cls");//����
			printf("\33[7m�鼮�嵥:\n\n\33[0m");
			printf("����                 ����                 ���       ������              ʣ���� ����ֵҪ�� �۸�   ���\n");//��ʽ���Ż�
			for (n = 1; n <= lenBook(sort); n++)
			{
				if (viewBook(sort, n) == NULL)
					continue;
				struct book bookT = *viewBook(sort, n);
				printf("%-20s %-20s %-10s %-20s  %d       %d     %.2lf \33[36m%3d\33[0m\n", bookT.name, bookT.author, bookT.category, bookT.publisher, bookT.num, bookT.creditLimit, bookT.price, n);
			}

			while (1)
			{
				printf("\n");
				printf("\33[33m���������鿴/�޸�/ɾ����Ӧ�鼮(����0����):\33[0m");
				int select = 0;
				rewind(stdin);
				scanf("%d", &select);
				if (select < 0 || select>lenBook(sort))
				{
					printf("\33[31m���޴���!\n\33[0m");
					system("timeout /t 1");
					continue;
				}
				else if (select == 0)
				{
					while (sort)//�ͷ��ڴ�
					{
						struct book* bookT = sort->next;
						free(sort);
						sort = bookT;
					}
					return;
				}
				else
				{
					select = searchBook_Acc(bookList, viewBook(sort, select)->name);//ͨ����������ʱ���ת��Ϊʵ�ʱ��
					checkBook(preference, bookList, userList, select);
					while (sort)//�������
					{
						struct book* bookT = sort->next;
						free(sort);
						sort = bookT;
					}
					break;
				}
			}
		}
	}
}

int listUser(struct settings* preference, struct user* userList, struct book* bookList)
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		printf("\33[7m�û��嵥:\n\n\33[0m");
		printf("�û���     ��ʵ����   ID                 ��ϵ��ʽ    ����ֵ ���\n");//��ʽ���Ż� ����������ϸ��Ϣ�в鿴
		int n;
		for (n = 1; n <= lenUser(userList); n++)
		{
			if (viewUser(userList, n) == NULL)
				continue;
			struct user userT = *viewUser(userList, n);
			printf("%-10s %-10s %-18s %-11s   %3d \33[36m%3d\33[0m\n", userT.username, userT.realName, userT.ID, userT.phoneNumber, userT.credit, n);
		}

		while (1)
		{
			printf("\n");
			printf("\33[33m���������鿴/�޸�/ɾ����Ӧ�û�(����0����):\33[0m");
			int select = 0;
			rewind(stdin);
			scanf("%d", &select);
			if (select < 0 || select>lenUser(userList))
			{
				printf("\33[31m���޴���!\n\33[0m");
				system("timeout /t 1");
				continue;
			}
			else
				return select;
		}
	}
}

void addBook(struct settings* preference, struct book* list)
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		struct book bookT = { {0},{0},{0},{0},{0}, 0.0 , 0 ,{0}, 0 , NULL };//��ʼ���ṹ��
		printf("\33[7m�����鼮\n\33[0m");
		printf("�ϴθ�������: \33[36m%4d��%02d��%02d��\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);//��ʾ�ϴθ�������

		printf("(����0�Է����ϲ�˵�)\n");
		printf("����������(�����»��ߴ���ո�):");
		rewind(stdin);
		scanf_s("%s", bookT.name, 101);
		if (strcmp(bookT.name, "0\0") == 0)//����0�򷵻ؿ�ʼ����
			return;
		else if (bookT.name[0] == '0')
		{
			printf("\33[31m�������!\33[0m");
			system("timeout /t 1");
			continue;
		}
		int whether_exist = searchBook_Acc(list, bookT.name);
		if (whether_exist != -1)
		{
			bookT = *viewBook(list, whether_exist);//���Ƹ�����Ϣ
			printf("\33[31m�����Ѵ���!\n\33[0m");
			printf("\33[33m������Ҫ���ӵ�����:\33[0m");
			int addNum;
			rewind(stdin);
			scanf("%d", &addNum);
			int sum = 0;
			int i;
			for (i = 0; i < 100; i++)//ͳ�Ƹ���������
			{
				if (bookT.username[i][0] != '\0')
					sum++;
			}
			sum += bookT.num;
			if (addNum > 0 && (addNum + sum) <= 100)
				bookT.num += addNum;
			else if ((addNum + sum) > 100)
			{
				printf("\33[31m���Ӻ󽫳����������(100��),δ������!\n\33[0m");
				printf("��ǰ��������:%d��", bookT.num);
				system("timeout /t 1");
				return;
			}
			else if (addNum == 0)
				return;
			else
			{
				printf("\33[31m����ֵ����,δ���κθ���!\33[0m");
				system("timeout /t 1");
				return;
			}
			modBook(list, whether_exist, bookT);
			saveBook(list);//�������
			getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
			saveSettings(*preference);//д�����һ�θ���ʱ��
			printf("\33[32m�����鼮�ɹ�!\33[0m");
			system("timeout /t 1");
			return;
		}

		while (1)
		{
			printf("������������(�����»��ߴ���ո�):");
			rewind(stdin);
			scanf_s("%s", bookT.author, 101);
			if (strcmp(bookT.author, "0\0") == 0)//����0�򷵻ؿ�ʼ����
				return;
			else if (bookT.author[0] == '0')
			{
				printf("\33[31m�������!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("������������(�����»��ߴ���ո�,���10�ַ���5����):");
			rewind(stdin);
			scanf_s("%s", bookT.category, 11);
			if (strcmp(bookT.category, "0\0") == 0)//����0�򷵻ؿ�ʼ����
				return;
			else if (bookT.category[0] == '0')
			{
				printf("\33[31m�������!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("�������������(�����»��ߴ���ո�):");
			rewind(stdin);
			scanf_s("%s", bookT.publisher, 101);
			if (strcmp(bookT.publisher, "0\0") == 0)//����0�򷵻ؿ�ʼ����
				return;
			else if (bookT.publisher[0] == '0')
			{
				printf("\33[31m�������!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("���������۸�(����):");
			rewind(stdin);
			scanf("%lf", &bookT.price);
			if (bookT.price == 0)//����0�򷵻ؿ�ʼ����
				return;
			else if (bookT.price <= 0)
			{
				printf("\33[31m�������!Ӧ��������!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("�������������(����,���100��):");
			rewind(stdin);
			scanf("%d", &bookT.num);
			if (bookT.num == 0)//����0�򷵻ؿ�ʼ����
				return;
			else if (bookT.num < 1 || bookT.num>100)
			{
				printf("\33[31m�������!���뷶ΧΪ1-100!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("���趨���ĸ��������������ֵ(����):");
			rewind(stdin);
			scanf("%d", &bookT.creditLimit);
			if (bookT.creditLimit == 0)//����0�򷵻ؿ�ʼ����
				return;
			else if (bookT.creditLimit < 0 || bookT.creditLimit > 100)
			{
				printf("\33[31m�������!���뷶ΧΪ0-100!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}
		printf("\n");

		insBook(list, lenBook(list) + 1, bookT);
		saveBook(list);//�������
		getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
		saveSettings(*preference);//д�����һ�θ���ʱ��
		printf("\33[32m�����鼮�ɹ�!\33[0m");
		system("timeout /t 1");
		return;
	}
}

void checkBook(struct settings* preference, struct book* bookList, struct user* userList, int bookOrder)//�����鼮
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		printf("\33[7m�����鼮\n\33[0m");
		printf("�ϴθ�������: \33[36m%4d��%02d��%02d��\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);//��ʾ�ϴθ�������

		if (bookOrder == 0)
		{
			printf("\33[33m������(���������ߵ�)�ؼ���(����0����):\33[0m");
			char keyWord[101] = { 0 };
			rewind(stdin);
			scanf_s("%s", keyWord, 101);
			if (strcmp(keyWord, "0\0") == 0)
				return;

			printf("\n");
			printf("����                 ����                 ���       ������              ʣ���� ����ֵҪ�� �۸�   ���\n");//��ʽ���Ż�
			int n;
			int is_none = 1;//��������Ƿ�Ϊ��
			for (n = 1; n <= lenBook(bookList); n++)
			{
				n = searchBook(bookList, n, keyWord);
				if (n != -1)
				{
					struct book bookT = *viewBook(bookList, n);
					printf("%-20s %-20s %-10s %-20s  %d       %d     %.2lf \33[36m%3d\33[0m\n", bookT.name, bookT.author, bookT.category, bookT.publisher, bookT.num, bookT.creditLimit, bookT.price, n);
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
				printf("\33[33m�����������鿴/�޸�/ɾ����Ӧ�鼮(����-1��������,����0����):\33[0m");
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
					bookDetails(preference, bookList, userList, select);
					break;
				}
			}
		}
		else
		{
			bookDetails(preference, bookList, userList, bookOrder);
			return;
		}
	}
}

void bookDetails(struct settings* preference, struct book* bookList, struct user* userList, int bookOrder)//�޸�ɾ���鼮
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		struct book bookT = *viewBook(bookList, bookOrder);
		printf("�鼮%s����Ϣ����:\n\n", bookT.name);

		int i;
		int sum = 0;
		for (i = 0; i < 100; i++)//ͳ�Ƹ���������
		{
			if (bookT.username[i][0] != '\0')
				sum++;
		}
		sum += bookT.num;

		printf("1. ����: %s\n", bookT.author);
		printf("2. ���: %s\n", bookT.category);
		printf("3. ������: %s\n", bookT.publisher);
		printf("4. �۸�: %.2lf\n", bookT.price);
		printf("5. ʣ������: %d (����%d)\n", bookT.num, sum);
		printf("6. ���������������ֵ: %d\n", bookT.creditLimit);
		printf("\33[31md. ɾ������\n\n\33[0m");//����d��ɾ������

		char select = '0';
		printf("\33[33m��ѡ��һ���޸�(����0����):\33[0m");
		rewind(stdin);
		scanf("%c", &select);
		autoLogOut(timer);//�Զ�ע������
		char newString[101] = { 0 };
		double newPrice = 0.0;
		int newValue = 0;
		rewind(stdin);
		switch (select)
		{
		case'1':
			printf("������������:");
			scanf_s("%s", newString, 101);
			if (newString[0] != '\0')
				strcpy(viewBook(bookList, bookOrder)->author, newString);
			else
			{
				printf("\33[31m�������,δ���κθ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'2':
			printf("�����������(�10�ַ���5����):");
			scanf_s("%s", newString, 11);
			if (newString[0] != '\0')
				strcpy(viewBook(bookList, bookOrder)->category, newString);
			else
			{
				printf("\33[31m�������,δ���κθ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'3':
			printf("�������³�����:");
			scanf_s("%s", newString, 101);
			if (newString[0] != '\0')
				strcpy(viewBook(bookList, bookOrder)->publisher, newString);
			else
			{
				printf("\33[31m�������,δ���κθ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'4':
			printf("�������¼۸�(����):");
			scanf("%lf", &newPrice);
			if (newPrice > 0.0)
				viewBook(bookList, bookOrder)->price = newPrice;
			else
			{
				printf("\33[31m�������,δ���κθ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'5':
			printf("��������ʣ������(0-100):");
			scanf("%d", &newValue);
			if (newValue >= 0 && newValue <= 100)
				viewBook(bookList, bookOrder)->num = newValue;
			else
			{
				printf("\33[31m�������,δ���κθ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'6':
			printf("������������ֵҪ��(0-100):");
			scanf("%d", &newValue);
			if (newValue >= 0 && newValue <= 100)
				viewBook(bookList, bookOrder)->creditLimit = newValue;
			else
			{
				printf("\33[31m�������,δ���κθ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'd':
			if (is_rented(bookList, bookOrder) == 1)
				printf("\33[31m����!ɾ�������Ӱ�쵽�û����ļ�¼!\n\33[0m");
			printf("\33[33mȷ��ɾ��?����yȷ��,���������ַ�ȡ��:\33[0m");
			char input = 'n';
			scanf("%c", &input);
			if (input == 'y')
			{
				delBook(bookList, bookOrder);
				saveBook(bookList);//д�����
				getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
				saveSettings(*preference);//д�����һ�θ���ʱ��
				printf("\33[32mɾ���ɹ�!\33[0m");
				system("timeout /t 1");
				return;
			}
			else
				continue;
		case'0':
			return;
		default:
			printf("\33[31m�������!\33[0m");
			system("timeout /t 1");
			continue;
		}
		saveBook(bookList);//�������
		getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
		saveSettings(*preference);//д�����һ�θ���ʱ��
		printf("\33[32m�޸ĳɹ�!\33[0m");
		system("timeout /t 1");
	}
}

void checkUser(struct settings* preference, struct user* userList, struct book* bookList, int userOrder)//�����û�
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		printf("\33[7m�����û�\n\33[0m");
		printf("�ϴθ�������: \33[36m%4d��%02d��%02d��\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);//��ʾ�ϴθ�������

		if (userOrder == 0)
		{
			printf("\33[33m�������û����ؼ���(����0����):\33[0m");
			char keyWord[11] = { 0 };
			rewind(stdin);
			scanf_s("%s", keyWord, 11);
			if (strcmp(keyWord, "0\0") == 0)
				return;
			if (strcmp(keyWord, "admin\0") == 0)
			{
				printf("\n");
				printf("\33[31m���û�Ϊ����Ա!\33[0m");
				system("timeout /t 1");
				continue;
			}

			printf("\n");
			printf("�û���     ��ʵ����   ID                 ��ϵ��ʽ    ����ֵ ���\n");//��ʽ���Ż� ����������ϸ��Ϣ�в鿴
			int n;
			int is_none = 1;//��������Ƿ�Ϊ��
			for (n = 1; n <= lenUser(userList); n++)
			{
				n = searchUser(userList, n, keyWord);
				if (n != -1)
				{
					struct user userT = *viewUser(userList, n);
					printf("%-10s %-10s %-18s %-11s   %3d \33[36m%3d\33[0m\n", userT.username, userT.realName, userT.ID, userT.phoneNumber, userT.credit, n);
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
				printf("\33[33m�����������鿴/�޸�/ɾ����Ӧ�û�(����-1��������,����0����):\33[0m");
				int select;
				rewind(stdin);
				scanf("%d", &select);
				autoLogOut(timer);//�Զ�ע������
				if (select == -1)
					break;
				if (select == 0)
					return;
				if (select < 0 || select>lenUser(userList))
				{
					printf("\33[31m���޴���!\n\33[0m");
					system("timeout /t 1");
					continue;
				}
				else
				{
					userDetails(preference, userList, bookList, select);
					break;
				}
			}
		}
		else
		{
			userDetails(preference, userList, bookList, userOrder);
			return;
		}
	}
}

void userDetails(struct settings* preference, struct user* userList, struct book* bookList, int userOrder)//�޸�ɾ���û�
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		struct user bookT = *viewUser(userList, userOrder);
		printf("�û�%s����Ϣ����:\n\n", bookT.username);

		printf("�ѽ��鼮:\n");
		int n, invalidBook = 0, record = 0;
		for (n = 0; n < 100; n++)//��ӡ������Ϣ
		{
			if (bookT.bksBrwd[n][0] == '\0')
				continue;
			record = 1;
			printf("����:%s ", bookT.bksBrwd[n]);
			int bookOrder = searchBook_Acc(bookList, bookT.bksBrwd[n]);
			if (bookOrder != -1)
			{
				int remainDays = isExpired(bookList, bookOrder, bookT.whichOne[n], preference->config_rent[1]);
				if (remainDays > 0)
					printf("\33[32mʣ������:%d��\n\33[0m", remainDays);
				else if (remainDays == 0)
					printf("\33[33mʣ�����ڲ���һ��!\n\33[0m");
				else
					printf("\33[31m�ѳ�������%d��! ��ÿ��۳�%d����ֵ!\n\33[0m", -remainDays, creditPunish(*preference, bookList, bookOrder));
			}
			else
			{
				printf("\33[31m���鲻����(�ѱ�ɾ��)!\n\33[0m");
				invalidBook = 1;//�����ѱ�ɾ�����鼮
			}
		}
		if (record == 0)
			printf("\n���û�û�н��ļ�¼!\n");

		printf("\n");
		printf("1. ����: %s\n", bookT.password);
		printf("2. ��ʵ����: %s\n", bookT.realName);
		printf("3. ID����: %s\n", bookT.ID);
		printf("4. ��ϵ��ʽ: %s\n", bookT.phoneNumber);
		printf("5. ����ֵ: %d\n", remainCredit(*preference, userList, bookList, userOrder));
		if (invalidBook == 1)
			printf("\33[32ma. �����鼮(�ϱ��з��ֲ������鼮)\n\33[0m");//����a�����鼮
		printf("\33[31md. ɾ�����û�\n\n\33[0m");//����d��ɾ�����û�

		char select = '0';
		printf("\33[33m��ѡ��һ���޸�(����0����):\33[0m");
		rewind(stdin);
		scanf("%c", &select);
		autoLogOut(timer);//�Զ�ע������
		char newString[101] = { 0 };
		int newCredit = 100;
		rewind(stdin);
		switch (select)
		{
		case'1':
			printf("������������(8-18λ):");
			scanf_s("%s", newString, 19);
			if (newString[0] != '\0' && strlen(newString) >= 8)
				strcpy(viewUser(userList, userOrder)->password, newString);
			else
			{
				printf("\33[31m�������,δ���κθ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'2':
			printf("��������ȷ��ʵ����:");
			scanf_s("%s", newString, 101);
			if (newString[0] != '\0')
				strcpy(viewUser(userList, userOrder)->realName, newString);
			else
			{
				printf("\33[31m�������,δ���κθ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'3':
			printf("��������ȷID(���18λ):");
			scanf_s("%s", newString, 19);
			if (newString[0] != '\0')
				strcpy(viewUser(userList, userOrder)->ID, newString);
			else
			{
				printf("\33[31m�������,δ���κθ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'4':
			printf("����������ϵ��ʽ(11λ):");
			scanf_s("%s", newString, 12);
			if (newString[0] != '\0' && strlen(newString) == 11)
				strcpy(viewUser(userList, userOrder)->phoneNumber, newString);
			else
			{
				printf("\33[31m�������,δ���κθ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'5':
			printf("������������ֵ(0-100):");
			scanf("%d", &newCredit);
			if (newCredit >= 0 && newCredit <= 100)
				viewUser(userList, userOrder)->credit = newCredit;
			else
			{
				printf("\33[31m�������,δ���κθ���!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'a':
			if (invalidBook == 1)
				addBook(preference, bookList);
			else
			{
				printf("\33[31m�������!\33[0m");
				system("timeout /t 1");
			}
			continue;
		case'd':
			printf("ȷ��ɾ��(���û��Ľ�����Ϣ���ᶪʧ)?\n����yȷ��,���������ַ�ȡ��:");
			char input = 'n';
			scanf("%c", &input);
			if (input == 'y')
			{
				returnBook_U(bookList, userList, userOrder);//ɾ�����û��Ľ��ļ�¼
				saveBook(bookList);
				delUser(userList, userOrder);
				saveUser(userList);//д�����
				getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
				saveSettings(*preference);//д�����һ�θ���ʱ��
				printf("\33[32mɾ���ɹ�!\33[0m");
				system("timeout /t 1");
				return;
			}
			else
				continue;
		case'0':
			return;
		default:
			printf("\33[31m�������!\33[0m");
			system("timeout /t 1");
			continue;
		}
		saveUser(userList);//�������
		getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
		saveSettings(*preference);//д�����һ�θ���ʱ��
		printf("\33[32m�޸ĳɹ�!\33[0m");
		system("timeout /t 1");
	}
}

void config(struct settings* preference)
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע������
		system("cls");
		printf("\33[7m����\n\33[0m");
		printf("�ϴθ�������: \33[36m%4d��%02d��%02d��\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);

		printf("��ǰϵͳ��������:\n");
		printf("1. ����Ա����: %s\n", preference->admin_passwd);
		printf("2. �û�����������: %d��\n", preference->config_rent[0]);
		printf("3. �û�����������: %d��\n", preference->config_rent[1]);
		printf("4. ����ֵ�ָ�����: %lf\n", preference->config_credit[0]);
		printf("5. ����ֵ�ͷ�����: %lf\n", preference->config_credit[1]);
		printf("6. ��ҳ����: %s\n", preference->bulletin);
		printf("ע:������ֻ���µ��û����Ч!\n");
		printf("    ����ֵ�ָ�����Ϊ��������ʱÿ����ÿ�����ӵ�����ֵ!\n");
		printf("    ����ֵ�ͷ�����Ϊÿ��ʱһ��۳�������ֵ�����۸�֮��!\n");
		printf("\33[33m��ѡ��Ҫ�޸ĵ���Ŀ(����0����):\33[0m");

		char select = '0';
		rewind(stdin);
		scanf("%c", &select);
		autoLogOut(timer);//�Զ�ע������
		char newString[101];
		int newRent;
		double newCredit;
		printf("\n");
		rewind(stdin);
		switch (select)
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
					strcpy(preference->admin_passwd, newString);
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
			while (1)
			{
				printf("��������ֵ(1-100):");
				scanf("%d", &newRent);
				if (newRent >= 1 && newRent <= 100)
				{
					preference->config_rent[0] = newRent;
					break;
				}
				else
				{
					printf("\33[31m�������!\n\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'3':
			while (1)
			{
				printf("��������ֵ(����):");
				scanf("%d", &newRent);
				if (newRent >= 1)
				{
					preference->config_rent[1] = newRent;
					break;
				}
				else
				{
					printf("\33[31m�������!\n\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'4':
			while (1)
			{
				printf("��������ֵ(����):");
				scanf("%lf", &newCredit);
				if (newCredit > 0.0)
				{
					preference->config_credit[0] = newCredit;
					break;
				}
				else
				{
					printf("\33[31m�������!\n\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'5':
			while (1)
			{
				printf("��������ֵ(����):");
				scanf("%lf", &newCredit);
				if (newCredit > 0.0)
				{
					preference->config_credit[1] = newCredit;
					break;
				}
				else
				{
					printf("\33[31m�������!\n\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'6':
			while (1)
			{
				printf("�������¹���(���100�ַ���50����):");
				scanf_s("%s", newString, 101);
				if (preference->bulletin[0] != '\0')
				{
					strcpy(preference->bulletin, newString);
					break;
				}
				else
				{
					printf("\33[31m������!����Ӧ��д����Ա��ϵ��ʽ!\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'0':
			return;
		default:
			printf("\33[31m�������!\n\33[0m");
			system("timeout /t 1");
			continue;
		}
		getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
		saveSettings(*preference);//�������
		printf("\33[32m�޸ĳɹ�!\33[0m");
		system("timeout /t 1");
	}
}

int bookRank(struct book* list)//���ܻ�ӭ�鼮���а� ��ֱ����ʾ(����)ǰ5�� �¼�����鼮���� ����0����ѡ�鼮���
{
	while (1)
	{
		autoLogOut(timer);//�Զ�ע�����
		system("cls");//����
		printf("\33[7m���ܻ�ӭ�鼮���а�\n\n\33[0m");
		if (list == NULL)//����������ֱ�ӷ���
			return 0;
		if (list->next == NULL)//���������� ��ʾ���沢�˳�
		{
			printf("\33[31m���κ��鼮!\n\n\33[0m");
			system("pause");
			return 0;
		}

		struct book* sort = rank(list);
		struct book* bookT;
		int realOrder[5] = { 0 };//����ʵ�ʱ�ŵ����� Ԫ�ظ�������ʾ���������
		printf("����   ����                 ����                 �������� ���\n");
		int n;
		for (n = 1; n <= 5; n++)//��ʾǰ������ ��Ϊǰ5��
		{
			if (viewBook(sort, n) == NULL)
				continue;
			bookT = viewBook(sort, n);
			realOrder[n - 1] = searchBook_Acc(list, viewBook(sort, n)->name);//ͨ����������ʱ���ת��Ϊʵ�ʱ��
			printf("��%2d�� %-20s %-20s    %2d   \33[36m%3d\33[0m\n", n, bookT->name, bookT->author, rentCount(sort, n), realOrder[n - 1]);
		}

		while (sort)//�ͷ��ڴ�
		{
			bookT = sort->next;
			free(sort);
			sort = bookT;
		}

		printf("\n");
		printf("\33[33m������Ҫ�������鼮�ı��(����0����):\33[0m");
		int select = 0;
		rewind(stdin);
		scanf("%d", &select);
		if (select != 0 && select != realOrder[0] && select != realOrder[1] && select != realOrder[2] && select != realOrder[3] && select != realOrder[4])
		{
			printf("\33[31m�ϱ����޴���!\n\33[0m");
			system("timeout /t 1");
			continue;
		}
		else
			return select;
	}
}

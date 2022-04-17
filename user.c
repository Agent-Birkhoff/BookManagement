#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 6031)//禁用讨厌的scanf返回值警告
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"file.h"
#include"linked_list.h"
#include"date.h"

int listBksBrwd(int userOrder, struct settings preference, struct user* userList, struct book* bookList);//打印已借书籍
int borrowLimit(int userOrder, struct settings preference, struct user* userList);//还可以借几本书 超限则返回超出几本(负数)
void returnBook(int userOrder, struct settings preference, struct user* userList, struct book* bookList);//还书
void borrowBook(int userOrder, struct settings preference, struct user* userList, struct book* bookList);//借书
void do_borrow(int userOrder, struct settings preference, struct user* userList, struct book* bookList, int bookOrder);//执行借书操作
void allBook(int userOrder, struct settings preference, struct user* userList, struct book* bookList);//列出所有书籍
void modifyInfo(int userOrder, struct settings preference, struct user* userList, struct book* bookList);//修改信息
extern int bookRank(struct book* list);//最受欢迎书籍排行榜 会直接显示(并列)前五名 新加入的书籍优先 返回0或所选书籍编号 在admin.c中

void user(int userOrder, struct settings preference, struct user* userList, struct book* bookList)
{
	char select = '0';
	while (1)
	{
		autoLogOut(timer);//自动注销功能
		system("cls");
		printf("\33[7m欢迎您,%s!\n\33[0m", viewUser(userList, userOrder)->username);
		int credit = remainCredit(preference, userList, bookList, userOrder);
		printf("当前您的信用值为\33[36m%d\33[0m\n", credit);
		if (credit < 100)
			printf("\33[31m您的信用值已不足100,请按时归还书籍或联系管理员缴纳罚金!\n\33[0m");
		printf("\n");
		printf("1. 归还书籍\n");
		printf("2. 查找(借阅)书籍\n");
		printf("3. 查看所有书籍\n");
		printf("4. 查看最受欢迎书籍排行榜\n");
		printf("5. 修改个人信息\n");
		printf("0. 退出登录\n\n");
		printf("\33[33m请选择:\33[0m");
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
				printf("\n书名:%s 作者:%s 类别:%s 出版社:%s\n", bookT.name, bookT.author, bookT.category, bookT.publisher);
				printf("\33[33m输入y确认借阅,输入其他字符取消:\33[0m");
				rewind(stdin);
				scanf("%c", &input);
				if (input == 'y')
				{
					do_borrow(userOrder, preference, userList, bookList, bookOrder);//借阅
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
			printf("\33[31m输入错误!\33[0m");
			system("timeout /t 1");
			break;
		}
	}
}

int listBksBrwd(int userOrder, struct settings preference, struct user* userList, struct book* bookList)
{
	int record = 0;
	struct user userT = *viewUser(userList, userOrder);//获取待打印用户信息
	int n;
	for (n = 0; n < 100; n++)//打印借书信息
	{
		if (userT.bksBrwd[n][0] == '\0')
			continue;
		record = 1;
		int bookOrder = searchBook_Acc(bookList, userT.bksBrwd[n]);
		if (bookOrder == -1)
			printf("编号:\33[31m无\33[0m 书名:%s ", userT.bksBrwd[n]);//不存在的书无编号
		else
			printf("编号:\33[36m%d\33[0m 书名:%s ", bookOrder, userT.bksBrwd[n]);
		if (bookOrder != -1)
		{
			int remainDays = isExpired(bookList, bookOrder, userT.whichOne[n], preference.config_rent[1]);
			if (remainDays > 0)
				printf("\33[32m剩余租期:%d天\n\33[0m", remainDays);
			else if (remainDays == 0)
				printf("\33[33m剩余租期不足一日!\n\33[0m");
			else
				printf("\33[31m已超出租期%d天! 将每天扣除%d信用值!\n\33[0m", -remainDays, creditPunish(preference, bookList, bookOrder));
		}
		else
			printf("\33[31m该书不存在(已被删除),请联系管理员!\n\33[0m");
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
		autoLogOut(timer);//自动注销功能
		system("cls");
		printf("\33[7m归还书籍\n\33[0m");
		struct user userT = *viewUser(userList, userOrder);//性能优化
		int credit = remainCredit(preference, userList, bookList, userOrder);
		printf("%s,当前您的信用值为\33[36m%d\33[0m\n", userT.username, credit);
		if (credit < 100)
			printf("\33[31m您的信用值已不足100,请按时归还书籍或联系管理员缴纳罚金!\n\33[0m");
		printf("\n");
		printf("目前您已借书籍:\n");
		if (listBksBrwd(userOrder, preference, userList, bookList) == 0)//显示已借书籍信息
		{
			printf("\n\33[31m没有借阅记录!\33[0m\n\n");
			system("pause");
			return;
		}
		printf("\n");
		int bookOrder;
		printf("\33[33m请输入要归还书籍的编号(输入0返回):\33[0m");
		rewind(stdin);
		scanf("%d", &bookOrder);
		autoLogOut(timer);//自动注销功能
		if (bookOrder == 0)
			return;
		if (bookOrder<0 || bookOrder>lenBook(bookList))
		{
			printf("\33[31m查无此书!\33[0m");
			system("timeout /t 1");
			continue;
		}
		struct book bookT = *viewBook(bookList, bookOrder);
		int i, old = -1;
		for (i = 0; i < 100; i++)
		{
			if (strcmp(userT.bksBrwd[i], bookT.name) == 0)
			{
				old = i;//初始值
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
				userT.credit += (int)(preference.config_credit[1] * bookT.price * remainDays);//扣除信用值
			else
			{
				int pastDays = preference.config_rent[1] - remainDays;
				userT.credit += (int)(preference.config_credit[0] * pastDays);
				if (userT.credit > 100)
					userT.credit = 100;
				else if (userT.credit < 0)
					userT.credit = 0;
			}
			modBook(bookList, bookOrder, bookT);//修改书籍链表
			saveBook(bookList);//写入磁盘
			modUser(userList, userOrder, userT);
			saveUser(userList);//写入磁盘
			printf("\33[32m归还成功!\33[0m");
			system("timeout /t 1");
			continue;
		}
		else
		{
			printf("\33[31m您没有借阅这本书!\33[0m");
			system("timeout /t 1");
		}
	}
}

void borrowBook(int userOrder, struct settings preference, struct user* userList, struct book* bookList)
{
	while (1)
	{
		autoLogOut(timer);//自动注销功能
		system("cls");
		printf("\33[7m查找(借阅)书籍\n\33[0m");
		int credit = remainCredit(preference, userList, bookList, userOrder);
		printf("%s,当前您的信用值为\33[36m%d\33[0m\n", viewUser(userList, userOrder)->username, credit);
		if (credit < 100)
			printf("\33[31m您的信用值已不足100,请按时归还书籍或联系管理员缴纳罚金!\n\33[0m");
		int remainNum = borrowLimit(userOrder, preference, userList);
		printf("您已借阅%d本书,", preference.config_rent[0] - remainNum);
		if (remainNum <= 0)
			printf("\33[31m已抵达借阅数量上限!\n\n\33[0m");
		else
			printf("\33[32m还可借阅%d本\n\n\33[0m", remainNum);

		printf("\33[33m请输入(书名或作者的)关键字(输入0返回):\33[0m");
		char keyWord[101] = { 0 };
		rewind(stdin);
		scanf_s("%s", keyWord, 101);
		if (strcmp(keyWord, "0\0") == 0)
			return;

		printf("\n");
		printf("     书名                 作者                 类别       出版社               信用值限制 编号\n");//格式可优化
		int n;
		int is_none = 1;//搜索结果是否为空
		for (n = 1; n <= lenBook(bookList); n++)
		{
			n = searchBook(bookList, n, keyWord);
			if (n != -1)
			{
				struct book bookT = *viewBook(bookList, n);
				if (bookT.num > 0)
					printf("\33[32m可借 \33[0m");
				else
					printf("\33[31m暂无 \33[0m");
				printf("%-20s %-20s %-10s %-20s    %3d    \33[36m%3d\33[0m\n", bookT.name, bookT.author, bookT.category, bookT.publisher, bookT.creditLimit, n);
				is_none = 0;//搜索结果不为空
			}
			else
				break;
		}
		if (is_none == 1)
		{
			printf("\n\33[31m没有找到任何结果!\33[0m\n\n");
			system("pause");
			continue;
		}

		while (1)
		{
			printf("\n");
			printf("\33[33m请输入编号来借阅对应书籍(输入-1重新搜索,输入0返回):\33[0m");
			int select;
			rewind(stdin);
			scanf("%d", &select);
			if (select == -1)
				break;
			if (select == 0)
				return;
			if (select < 0 || select>lenBook(bookList))
			{
				printf("\33[31m查无此书!\n\33[0m");
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
	autoLogOut(timer);//自动注销功能
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
		printf("\n\33[31m该书已被借完,无法借阅!\33[0m\n");
		system("pause");
		return;
	}
	else if (borrowLimit(userOrder, preference, userList) <= 0)
	{
		printf("\n\33[31m您的借阅数量已抵达上限,无法借阅!\33[0m\n");
		system("pause");
		return;
	}
	else if (remainCredit(preference, userList, bookList, userOrder) < bookT.creditLimit)
	{
		printf("\n\33[31m信用值不足,无法借阅!\33[0m\n");
		system("pause");
		return;
	}
	else
	{
		struct user userT = *viewUser(userList, userOrder);//节约资源
		int n;
		for (n = 0; n < 100; n++)
		{
			if (userT.bksBrwd[n][0] == '\0')//找到空位置
			{
				strcpy(userT.bksBrwd[n], bookT.name);
				userT.whichOne[n] = available;
				strcpy(bookT.username[available], userT.username);
				getDate(&bookT.rentDate[available][0], &bookT.rentDate[available][1], &bookT.rentDate[available][2]);
				bookT.num--;
				modUser(userList, userOrder, userT);//修改用户链表
				modBook(bookList, bookOrder, bookT);//修改书籍链表
				saveUser(userList);//存入磁盘
				saveBook(bookList);//存入磁盘
				break;
			}
		}
		printf("\n\33[32m借阅成功!\33[0m");
		system("timeout /t 1");
		return;
	}
}

void allBook(int userOrder, struct settings preference, struct user* userList, struct book* bookList)
{
	while (1)
	{
		autoLogOut(timer);//自动注销功能
		system("cls");
		printf("\33[7m全部书籍列表:\n\33[0m");
		int credit = remainCredit(preference, userList, bookList, userOrder);
		printf("%s,当前您的信用值为\33[36m%d\33[0m\n", viewUser(userList, userOrder)->username, credit);
		if (credit < 100)
			printf("\33[31m您的信用值已不足100,请按时归还书籍或联系管理员缴纳罚金!\n\33[0m");
		int remainNum = borrowLimit(userOrder, preference, userList);
		printf("您已借阅%d本书,", preference.config_rent[0] - remainNum);
		if (remainNum <= 0)
			printf("\33[31m已抵达借阅数量上限!\n\n\33[0m");
		else
			printf("\33[32m还可借阅%d本\n\n\33[0m", remainNum);

		printf("     书名                 作者                 类别       出版社               信用值限制 编号\n");//格式可优化
		if (lenBook(bookList) == 0)
		{
			printf("\n\33[31m库中无任何书籍!\33[0m\n\n");
			system("pause");
			return;
		}
		int n;
		for (n = 1; n <= lenBook(bookList); n++)
		{
			struct book bookT = *viewBook(bookList, n);
			if (bookT.num > 0)
				printf("\33[32m可借 \33[0m");
			else
				printf("\33[31m暂无 \33[0m");
			printf("%-20s %-20s %-10s %-20s    %3d    \33[36m%3d\33[0m\n", bookT.name, bookT.author, bookT.category, bookT.publisher, bookT.creditLimit, n);
		}

		while (1)
		{
			printf("\n");
			printf("\33[33m请输入编号来借阅对应书籍(输入0返回):\33[0m");
			int select;
			rewind(stdin);
			scanf("%d", &select);
			if (select == 0)
				return;
			if (select < 0 || select>lenBook(bookList))
			{
				printf("\33[31m查无此书!\n\33[0m");
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
		autoLogOut(timer);//自动注销功能
		system("cls");
		printf("\33[7m修改个人信息\n\33[0m");
		struct user userT = *viewUser(userList, userOrder);
		int credit = remainCredit(preference, userList, bookList, userOrder);
		printf("%s,当前您的信用值为\33[36m%d\33[0m\n", userT.username, credit);
		if (credit < 100)
			printf("\33[31m您的信用值已不足100,请按时归还书籍或联系管理员缴纳罚金!\n\33[0m");

		printf("\n");
		printf("当前您的个人信息如下:\n");
		printf("1. 密码: %s\n", userT.password);
		printf("2. 真实姓名: %s\n", userT.realName);
		printf("3. 联系方式: %s\n", userT.phoneNumber);
		printf("4. ID号码: %s\n", userT.ID);
		printf("\33[33m请选择要修改的项目(输入0返回):\33[0m");
		char choice = '0';
		rewind(stdin);
		scanf("%c", &choice);
		autoLogOut(timer);//自动注销功能
		char newString[101] = { 0 };
		printf("\n");
		rewind(stdin);
		switch (choice)
		{
		case'1':
			while (1)
			{
				printf("请输入新密码(8-18位):");
				scanf_s("%s", newString, 19);
				if (strlen(newString) < 8)
				{
					printf("\33[31m密码长度不足!请重新输入!\33[0m");
					system("timeout /t 1");
					continue;
				}
				char repeat[19];
				printf("请重复输入新密码:");
				rewind(stdin);
				scanf_s("%s", repeat, 19);
				if (strcmp(repeat, newString) == 0)
				{
					strcpy(viewUser(userList, userOrder)->password, newString);
					break;
				}
				else
				{
					printf("\33[31m两次输入的密码不一致,已取消操作!\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'2':
			printf("请输入正确的真实姓名:");
			scanf_s("%s", newString, 101);
			if (newString[0] != '\0')
				strcpy(viewUser(userList, userOrder)->realName, newString);
			else
			{
				printf("\33[31m不可为空!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'3':
			printf("请输入新的联系方式(11位):");
			scanf_s("%s", newString, 12);
			if (strlen(newString) == 11)
				strcpy(viewUser(userList, userOrder)->phoneNumber, newString);
			else
			{
				printf("\33[31m长度错误!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'4':
			printf("输入正确的ID号码(最长18位):");
			scanf_s("%s", newString, 19);
			if (newString[0] != '\0')
				strcpy(viewUser(userList, userOrder)->ID, newString);
			else
			{
				printf("\33[31m不可为空!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'0':
			return;
		default:
			printf("\33[31m输入错误!\33[0m");
			system("timeout /t 1");
			continue;
		}
		saveUser(userList);//存入磁盘
		printf("\33[32m修改成功!\33[0m");
		system("timeout /t 1");
	}
}

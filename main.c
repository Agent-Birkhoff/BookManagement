#define _CRT_SECURE_NO_WARNINGS//禁用SDL
#pragma warning( disable : 6031)//禁用讨厌的scanf返回值警告
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"linked_list.h"
#include"file.h"
#include"date.h"

extern void admin(struct settings* preference, struct user* userList, struct book* bookList);
extern void user(int userOrder, struct settings preference, struct user* userList, struct book* bookList);

void login(struct settings* preference, struct user* userList, struct book* bookList);//登录
void regist(struct user* userList);//注册

int main(void)
{
	struct settings preference = readSettings();//读取设置
	struct user* userList = readUser();//读取用户链表
	if (userList == NULL)//不存在则新建链表头
		userList = crtUser(0);
	struct book* bookList = readBook();//读取书籍链表
	if (bookList == NULL)//不存在则新建链表头
		bookList = crtBook(0);

	char select = '0';//所选项
	while (1)//自动刷新屏幕
	{
		system("cls");//清屏
		printf("\33[4;34;47m欢迎使用图书管理系统!\n\n\33[0m");
		printf("最新公告:%s\n\n", preference.bulletin);
		printf("1. 登录\n");
		printf("2. 注册\n");
		printf("0. 退出\n\n");
		printf("\33[33m请选择:\33[0m");
		rewind(stdin);//清空输入缓冲区 fflush()无法在vs2019环境下正常工作 故使用rewind()代替
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
			delAll(userList, bookList);//删除链表回收内存
			exit(0);
		case '\n':
			printf("\33[31m请输入选项序号!\n\33[0m");
			system("timeout /t 1");//暂停1秒钟
			break;
		default:
			printf("\33[31m输入错误!\n\33[0m");
			system("timeout /t 1");//暂停1秒钟
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
		printf("\33[7m登录\n\n\33[0m");
		printf("请输入用户名(输入0返回):");
		char username[11] = { 0 };
		rewind(stdin);
		scanf_s("%s", username, 11);
		if (strcmp(username, "0\0") == 0)//输入0则返回开始界面
			return;
		printf("请输入密码(输入0返回):");
		char passwd[19] = { 0 };
		rewind(stdin);
		scanf_s("%s", passwd, 19);
		if (strcmp(passwd, "0\0") == 0)//输入0则返回开始界面
			return;
		else if (strcmp(username, "admin\0") == 0)//管理员用户名admin
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
				if (tryTime > 10)//防暴力破解 默认最大允许10次错误尝试
				{
					delAll(userList, bookList);//删除链表回收内存
					exit(0);
				}
				tryTime++;
				printf("\33[31m密码错误!\33[0m");
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
					user(userOrder, *preference, userList, bookList);//跳转至对应用户界面
					return;
				}
				else
				{
					if (tryTime > 10)//防暴力破解 默认最大允许10次错误尝试
					{
						delAll(userList, bookList);//删除链表回收内存
						exit(0);
					}
					tryTime++;
					printf("\33[31m密码错误!\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			else
			{
				printf("\33[31m用户不存在!\33[0m");
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
		printf("\33[3m注册\n\n\33[0m");

		printf("(输入0以返回上层菜单)\n");
		printf("请输入用户名(最长10位):");
		rewind(stdin);
		scanf_s("%s", userT.username, 11);
		if (strcmp(userT.username, "0\0") == 0)//输入0则返回开始界面
			return;
		if (searchUser_Acc(userList, userT.username) != -1 || strcmp(userT.username, "admin\0") == 0)
		{
			printf("\33[31m该用户名已被占用!\33[0m");
			system("timeout /t 1");
			continue;
		}
		if (userT.username[0] == '\0')
		{
			printf("\33[31m输入字符数错误!\33[0m");
			system("timeout /t 1");
			continue;
		}

		while (1)
		{
			printf("请输入密码(8-18位):");
			rewind(stdin);//刷新缓冲区
			scanf_s("%s", userT.password, 19);
			if (strcmp(userT.password, "0\0") == 0)//输入0则返回开始界面
				return;
			if (strlen(userT.password) < 8)
			{
				printf("\33[31m密码应为8-18位!\n\33[0m");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("请输入真实姓名:");
			rewind(stdin);//刷新缓冲区
			scanf_s("%s", userT.realName, 101);
			if (strcmp(userT.realName, "0\0") == 0)//输入0则返回开始界面
				return;
			if (userT.realName[0] == '\0')
			{
				printf("\33[31m输入字符数错误!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("输入ID号码(最大18位):");
			rewind(stdin);//刷新缓冲区
			scanf_s("%s", userT.ID, 19);
			if (strcmp(userT.ID, "0\0") == 0)//输入0则返回开始界面
				return;
			if (userT.ID[0] == '\0')
			{
				printf("\33[31m输入字符数错误!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("请输入联系方式(11位手机号码):");
			rewind(stdin);//刷新缓冲区
			scanf_s("%s", userT.phoneNumber, 12);
			if (strcmp(userT.phoneNumber, "0\0") == 0)//输入0则返回开始界面
				return;
			if (strlen(userT.phoneNumber) != 11)
			{
				printf("\33[31m手机号码应为11位!\n\33[0m");
				continue;
			}
			break;
		}

		while (1)
		{
			system("cls");
			printf("\33[3m请检查输入的信息是否正确\n\n\33[0m");
			printf("用户名:   %s\n", userT.username);
			printf("密码:     %s\n", userT.password);
			printf("真实姓名: %s\n", userT.realName);
			printf("联系方式: %s\n", userT.phoneNumber);
			printf("ID号码:   %s\n\n", userT.ID);
			printf("\33[33m输入y确认注册,输入n重新注册,输入0放弃注册并返回:\33[0m");
			char input;
			rewind(stdin);
			scanf("%c", &input);

			switch (input)
			{
			case 'y':
				insUser(userList, lenUser(userList) + 1, userT);//在用户链表结尾插入节点
				saveUser(userList);//写入磁盘
				printf("\33[32m注册成功!\33[0m");
				system("timeout /t 1");
				return;
			case 'n':
				break;
			case '0':
				return;
			default:
				printf("\33[31m输入错误!\33[0m");
				system("timeout /t 1");
				continue;
			}

			break;
		}
	}
}

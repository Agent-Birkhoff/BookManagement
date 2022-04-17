#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 6031)//禁用讨厌的scanf返回值警告
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"linked_list.h"
#include"file.h"
#include"date.h"

void overview(struct settings* preference, struct book* bookList, struct user* userList);//数据概览
void listBook(struct settings* preference, struct book* bookList, struct user* userList);//显示所有书籍
int listUser(struct settings* preference, struct user* userList, struct book* bookList);//显示所有用户
void addBook(struct settings* preference, struct book* list);//新增书籍或增加数目
void checkBook(struct settings* preference, struct book* bookList, struct user* userList, int bookOrder);//查找书籍
void bookDetails(struct settings* preference, struct book* bookList, struct user* userList, int bookOrder);//修改删除书籍
void checkUser(struct settings* preference, struct user* userList, struct book* bookList, int userOrder);//查找用户
void userDetails(struct settings* preference, struct user* userList, struct book* bookList, int userOrder);//修改删除用户
void config(struct settings* preference);//设置
int bookRank(struct book* list);//最受欢迎书籍排行榜 会直接显示(并列)前五名 新加入的书籍优先 返回0或所选书籍编号

void admin(struct settings* preference, struct user* userList, struct book* bookList)
{
	char select = '0';
	while (1)
	{
		autoLogOut(timer);//自动注销功能
		system("cls");
		printf("\33[7m管理面板\n\33[0m");
		printf("上次更改数据: \33[36m%4d年%02d月%02d日\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);//显示上次更改日期
		printf("1. 数据概览\n");
		printf("2. 新增(或增加)书籍\n");
		printf("3. 查找(并修改)书籍\n");
		printf("4. 查找(并修改)用户\n");
		printf("5. 设置\n");
		printf("0. 退出登录\n\n");
		printf("\33[33m请选择:\33[0m");
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
			printf("\33[31m输入错误!\33[0m");
			system("timeout /t 1");
			break;
		}
	}
}

void overview(struct settings* preference, struct book* bookList, struct user* userList)
{
	while (1)
	{
		autoLogOut(timer);//自动注销功能
		system("cls");
		printf("\33[7m数据概览\n\33[0m");
		printf("上次更改数据: \33[36m%4d年%02d月%02d日\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);

		int i;
		int length = lenBook(bookList);
		int sumBook = 0, remainBook = 0;
		for (i = 1; i <= length; i++)//遍历每种书籍
		{
			struct book bookT = *viewBook(bookList, i);
			int j;
			for (j = 0; j < 100; j++)//统计该种书已被借出的数量
			{
				if (bookT.username[j][0] != '\0')
					sumBook += 1;
			}
			sumBook += bookT.num;
			remainBook += bookT.num;
		}
		printf("目前库中共%d种书,共%d本,剩余%d本\n", length, sumBook, remainBook);
		length = lenUser(userList);
		printf("目前本系统共注册有%d名用户\n\n", length);

		char select = '0';
		int order = 0;
		printf("1. 查看最受欢迎书籍排行榜\n");
		printf("2. 查看所有书籍\n");
		printf("3. 查看所有用户\n");
		printf("\33[33m请选择(输入0返回):\33[0m");
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
			listBook(preference, bookList, userList);//调用查看修改等功能被移入函数体内
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
			printf("\33[31m输入错误!\33[0m");
			system("timeout /t 1");
			break;
		}
	}
}

void listBook(struct settings* preference, struct book* bookList, struct user* userList)
{
	while (1)
	{
		autoLogOut(timer);//自动注销功能
		system("cls");
		char type;
		int mode;
		char select = '0';
		printf("\33[7m显示所有书籍\n\n\33[0m");
		printf("1. 按价格排序\n");
		printf("2. 按储量排序\n");
		printf("3. 按信用值要求排序\n");
		printf("\33[33m请选择排序模式(输入0返回):\33[0m");
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
			printf("\33[31m输入错误!\33[0m");
			system("timeout /t 1");
			continue;
		}
		printf("\n");
		printf("1. 从大到小\n");
		printf("2. 从小到大\n");
		printf("\33[33m请选择排序方式(输入0返回):\33[0m");
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
			printf("\33[31m输入错误!\33[0m");
			system("timeout /t 1");
			continue;
		}

		while (1)
		{
			struct book* sort = crtBook(0);//复制链表
			int n;
			for (n = 1; n <= lenBook(bookList); n++)
				insBook(sort, n, *viewBook(bookList, n));
			bubbleSortBook(sort, type, mode);//对书籍链表排序

			system("cls");//清屏
			printf("\33[7m书籍清单:\n\n\33[0m");
			printf("书名                 作者                 类别       出版社              剩余量 信用值要求 价格   编号\n");//格式可优化
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
				printf("\33[33m输入编号来查看/修改/删除对应书籍(输入0返回):\33[0m");
				int select = 0;
				rewind(stdin);
				scanf("%d", &select);
				if (select < 0 || select>lenBook(sort))
				{
					printf("\33[31m查无此书!\n\33[0m");
					system("timeout /t 1");
					continue;
				}
				else if (select == 0)
				{
					while (sort)//释放内存
					{
						struct book* bookT = sort->next;
						free(sort);
						sort = bookT;
					}
					return;
				}
				else
				{
					select = searchBook_Acc(bookList, viewBook(sort, select)->name);//通过书名把临时编号转换为实际编号
					checkBook(preference, bookList, userList, select);
					while (sort)//清除缓存
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
		autoLogOut(timer);//自动注销功能
		system("cls");
		printf("\33[7m用户清单:\n\n\33[0m");
		printf("用户名     真实姓名   ID                 联系方式    信用值 编号\n");//格式可优化 密码须在详细信息中查看
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
			printf("\33[33m输入编号来查看/修改/删除对应用户(输入0返回):\33[0m");
			int select = 0;
			rewind(stdin);
			scanf("%d", &select);
			if (select < 0 || select>lenUser(userList))
			{
				printf("\33[31m查无此人!\n\33[0m");
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
		autoLogOut(timer);//自动注销功能
		system("cls");
		struct book bookT = { {0},{0},{0},{0},{0}, 0.0 , 0 ,{0}, 0 , NULL };//初始化结构体
		printf("\33[7m新增书籍\n\33[0m");
		printf("上次更改数据: \33[36m%4d年%02d月%02d日\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);//显示上次更改日期

		printf("(输入0以返回上层菜单)\n");
		printf("请输入书名(请用下划线代替空格):");
		rewind(stdin);
		scanf_s("%s", bookT.name, 101);
		if (strcmp(bookT.name, "0\0") == 0)//输入0则返回开始界面
			return;
		else if (bookT.name[0] == '0')
		{
			printf("\33[31m输入错误!\33[0m");
			system("timeout /t 1");
			continue;
		}
		int whether_exist = searchBook_Acc(list, bookT.name);
		if (whether_exist != -1)
		{
			bookT = *viewBook(list, whether_exist);//复制该书信息
			printf("\33[31m该书已存在!\n\33[0m");
			printf("\33[33m请输入要增加的数量:\33[0m");
			int addNum;
			rewind(stdin);
			scanf("%d", &addNum);
			int sum = 0;
			int i;
			for (i = 0; i < 100; i++)//统计该种书总数
			{
				if (bookT.username[i][0] != '\0')
					sum++;
			}
			sum += bookT.num;
			if (addNum > 0 && (addNum + sum) <= 100)
				bookT.num += addNum;
			else if ((addNum + sum) > 100)
			{
				printf("\33[31m增加后将超过最大数量(100本),未作更改!\n\33[0m");
				printf("当前该书数量:%d本", bookT.num);
				system("timeout /t 1");
				return;
			}
			else if (addNum == 0)
				return;
			else
			{
				printf("\33[31m输入值错误,未作任何更改!\33[0m");
				system("timeout /t 1");
				return;
			}
			modBook(list, whether_exist, bookT);
			saveBook(list);//存入磁盘
			getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
			saveSettings(*preference);//写入最后一次更改时间
			printf("\33[32m新增书籍成功!\33[0m");
			system("timeout /t 1");
			return;
		}

		while (1)
		{
			printf("请输入作者名(请用下划线代替空格):");
			rewind(stdin);
			scanf_s("%s", bookT.author, 101);
			if (strcmp(bookT.author, "0\0") == 0)//输入0则返回开始界面
				return;
			else if (bookT.author[0] == '0')
			{
				printf("\33[31m输入错误!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("请输入该书类别(请用下划线代替空格,最大10字符或5汉字):");
			rewind(stdin);
			scanf_s("%s", bookT.category, 11);
			if (strcmp(bookT.category, "0\0") == 0)//输入0则返回开始界面
				return;
			else if (bookT.category[0] == '0')
			{
				printf("\33[31m输入错误!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("请输入出版社名(请用下划线代替空格):");
			rewind(stdin);
			scanf_s("%s", bookT.publisher, 101);
			if (strcmp(bookT.publisher, "0\0") == 0)//输入0则返回开始界面
				return;
			else if (bookT.publisher[0] == '0')
			{
				printf("\33[31m输入错误!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("请输入该书价格(正数):");
			rewind(stdin);
			scanf("%lf", &bookT.price);
			if (bookT.price == 0)//输入0则返回开始界面
				return;
			else if (bookT.price <= 0)
			{
				printf("\33[31m输入错误!应输入正数!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("请输入该书总数(整数,最大100本):");
			rewind(stdin);
			scanf("%d", &bookT.num);
			if (bookT.num == 0)//输入0则返回开始界面
				return;
			else if (bookT.num < 1 || bookT.num>100)
			{
				printf("\33[31m输入错误!输入范围为1-100!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}

		while (1)
		{
			printf("请设定借阅该书所需最低信用值(整数):");
			rewind(stdin);
			scanf("%d", &bookT.creditLimit);
			if (bookT.creditLimit == 0)//输入0则返回开始界面
				return;
			else if (bookT.creditLimit < 0 || bookT.creditLimit > 100)
			{
				printf("\33[31m输入错误!输入范围为0-100!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		}
		printf("\n");

		insBook(list, lenBook(list) + 1, bookT);
		saveBook(list);//存入磁盘
		getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
		saveSettings(*preference);//写入最后一次更改时间
		printf("\33[32m新增书籍成功!\33[0m");
		system("timeout /t 1");
		return;
	}
}

void checkBook(struct settings* preference, struct book* bookList, struct user* userList, int bookOrder)//查找书籍
{
	while (1)
	{
		autoLogOut(timer);//自动注销功能
		system("cls");
		printf("\33[7m查找书籍\n\33[0m");
		printf("上次更改数据: \33[36m%4d年%02d月%02d日\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);//显示上次更改日期

		if (bookOrder == 0)
		{
			printf("\33[33m请输入(书名或作者的)关键字(输入0返回):\33[0m");
			char keyWord[101] = { 0 };
			rewind(stdin);
			scanf_s("%s", keyWord, 101);
			if (strcmp(keyWord, "0\0") == 0)
				return;

			printf("\n");
			printf("书名                 作者                 类别       出版社              剩余量 信用值要求 价格   编号\n");//格式可优化
			int n;
			int is_none = 1;//搜索结果是否为空
			for (n = 1; n <= lenBook(bookList); n++)
			{
				n = searchBook(bookList, n, keyWord);
				if (n != -1)
				{
					struct book bookT = *viewBook(bookList, n);
					printf("%-20s %-20s %-10s %-20s  %d       %d     %.2lf \33[36m%3d\33[0m\n", bookT.name, bookT.author, bookT.category, bookT.publisher, bookT.num, bookT.creditLimit, bookT.price, n);
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
				printf("\33[33m请输入编号来查看/修改/删除对应书籍(输入-1重新搜索,输入0返回):\33[0m");
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

void bookDetails(struct settings* preference, struct book* bookList, struct user* userList, int bookOrder)//修改删除书籍
{
	while (1)
	{
		autoLogOut(timer);//自动注销功能
		system("cls");
		struct book bookT = *viewBook(bookList, bookOrder);
		printf("书籍%s的信息如下:\n\n", bookT.name);

		int i;
		int sum = 0;
		for (i = 0; i < 100; i++)//统计该种书总数
		{
			if (bookT.username[i][0] != '\0')
				sum++;
		}
		sum += bookT.num;

		printf("1. 作者: %s\n", bookT.author);
		printf("2. 类别: %s\n", bookT.category);
		printf("3. 出版社: %s\n", bookT.publisher);
		printf("4. 价格: %.2lf\n", bookT.price);
		printf("5. 剩余数量: %d (总量%d)\n", bookT.num, sum);
		printf("6. 借阅所需最低信用值: %d\n", bookT.creditLimit);
		printf("\33[31md. 删除该书\n\n\33[0m");//输入d则删除该书

		char select = '0';
		printf("\33[33m请选择一项修改(输入0返回):\33[0m");
		rewind(stdin);
		scanf("%c", &select);
		autoLogOut(timer);//自动注销功能
		char newString[101] = { 0 };
		double newPrice = 0.0;
		int newValue = 0;
		rewind(stdin);
		switch (select)
		{
		case'1':
			printf("请输入新作者:");
			scanf_s("%s", newString, 101);
			if (newString[0] != '\0')
				strcpy(viewBook(bookList, bookOrder)->author, newString);
			else
			{
				printf("\33[31m输入错误,未做任何更改!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'2':
			printf("请输入新类别(最长10字符或5汉字):");
			scanf_s("%s", newString, 11);
			if (newString[0] != '\0')
				strcpy(viewBook(bookList, bookOrder)->category, newString);
			else
			{
				printf("\33[31m输入错误,未做任何更改!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'3':
			printf("请输入新出版社:");
			scanf_s("%s", newString, 101);
			if (newString[0] != '\0')
				strcpy(viewBook(bookList, bookOrder)->publisher, newString);
			else
			{
				printf("\33[31m输入错误,未做任何更改!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'4':
			printf("请输入新价格(正数):");
			scanf("%lf", &newPrice);
			if (newPrice > 0.0)
				viewBook(bookList, bookOrder)->price = newPrice;
			else
			{
				printf("\33[31m输入错误,未做任何更改!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'5':
			printf("请输入新剩余数量(0-100):");
			scanf("%d", &newValue);
			if (newValue >= 0 && newValue <= 100)
				viewBook(bookList, bookOrder)->num = newValue;
			else
			{
				printf("\33[31m输入错误,未做任何更改!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'6':
			printf("请输入新信用值要求(0-100):");
			scanf("%d", &newValue);
			if (newValue >= 0 && newValue <= 100)
				viewBook(bookList, bookOrder)->creditLimit = newValue;
			else
			{
				printf("\33[31m输入错误,未做任何更改!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'd':
			if (is_rented(bookList, bookOrder) == 1)
				printf("\33[31m警告!删除该书会影响到用户借阅记录!\n\33[0m");
			printf("\33[33m确认删除?输入y确认,输入其它字符取消:\33[0m");
			char input = 'n';
			scanf("%c", &input);
			if (input == 'y')
			{
				delBook(bookList, bookOrder);
				saveBook(bookList);//写入磁盘
				getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
				saveSettings(*preference);//写入最后一次更改时间
				printf("\33[32m删除成功!\33[0m");
				system("timeout /t 1");
				return;
			}
			else
				continue;
		case'0':
			return;
		default:
			printf("\33[31m输入错误!\33[0m");
			system("timeout /t 1");
			continue;
		}
		saveBook(bookList);//存入磁盘
		getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
		saveSettings(*preference);//写入最后一次更改时间
		printf("\33[32m修改成功!\33[0m");
		system("timeout /t 1");
	}
}

void checkUser(struct settings* preference, struct user* userList, struct book* bookList, int userOrder)//查找用户
{
	while (1)
	{
		autoLogOut(timer);//自动注销功能
		system("cls");
		printf("\33[7m查找用户\n\33[0m");
		printf("上次更改数据: \33[36m%4d年%02d月%02d日\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);//显示上次更改日期

		if (userOrder == 0)
		{
			printf("\33[33m请输入用户名关键字(输入0返回):\33[0m");
			char keyWord[11] = { 0 };
			rewind(stdin);
			scanf_s("%s", keyWord, 11);
			if (strcmp(keyWord, "0\0") == 0)
				return;
			if (strcmp(keyWord, "admin\0") == 0)
			{
				printf("\n");
				printf("\33[31m该用户为管理员!\33[0m");
				system("timeout /t 1");
				continue;
			}

			printf("\n");
			printf("用户名     真实姓名   ID                 联系方式    信用值 编号\n");//格式可优化 密码须在详细信息中查看
			int n;
			int is_none = 1;//搜索结果是否为空
			for (n = 1; n <= lenUser(userList); n++)
			{
				n = searchUser(userList, n, keyWord);
				if (n != -1)
				{
					struct user userT = *viewUser(userList, n);
					printf("%-10s %-10s %-18s %-11s   %3d \33[36m%3d\33[0m\n", userT.username, userT.realName, userT.ID, userT.phoneNumber, userT.credit, n);
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
				printf("\33[33m请输入编号来查看/修改/删除对应用户(输入-1重新搜索,输入0返回):\33[0m");
				int select;
				rewind(stdin);
				scanf("%d", &select);
				autoLogOut(timer);//自动注销功能
				if (select == -1)
					break;
				if (select == 0)
					return;
				if (select < 0 || select>lenUser(userList))
				{
					printf("\33[31m查无此人!\n\33[0m");
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

void userDetails(struct settings* preference, struct user* userList, struct book* bookList, int userOrder)//修改删除用户
{
	while (1)
	{
		autoLogOut(timer);//自动注销功能
		system("cls");
		struct user bookT = *viewUser(userList, userOrder);
		printf("用户%s的信息如下:\n\n", bookT.username);

		printf("已借书籍:\n");
		int n, invalidBook = 0, record = 0;
		for (n = 0; n < 100; n++)//打印借书信息
		{
			if (bookT.bksBrwd[n][0] == '\0')
				continue;
			record = 1;
			printf("书名:%s ", bookT.bksBrwd[n]);
			int bookOrder = searchBook_Acc(bookList, bookT.bksBrwd[n]);
			if (bookOrder != -1)
			{
				int remainDays = isExpired(bookList, bookOrder, bookT.whichOne[n], preference->config_rent[1]);
				if (remainDays > 0)
					printf("\33[32m剩余租期:%d天\n\33[0m", remainDays);
				else if (remainDays == 0)
					printf("\33[33m剩余租期不足一日!\n\33[0m");
				else
					printf("\33[31m已超出租期%d天! 将每天扣除%d信用值!\n\33[0m", -remainDays, creditPunish(*preference, bookList, bookOrder));
			}
			else
			{
				printf("\33[31m该书不存在(已被删除)!\n\33[0m");
				invalidBook = 1;//存在已被删除的书籍
			}
		}
		if (record == 0)
			printf("\n该用户没有借阅记录!\n");

		printf("\n");
		printf("1. 密码: %s\n", bookT.password);
		printf("2. 真实姓名: %s\n", bookT.realName);
		printf("3. ID号码: %s\n", bookT.ID);
		printf("4. 联系方式: %s\n", bookT.phoneNumber);
		printf("5. 信用值: %d\n", remainCredit(*preference, userList, bookList, userOrder));
		if (invalidBook == 1)
			printf("\33[32ma. 新增书籍(上表中发现不存在书籍)\n\33[0m");//输入a新增书籍
		printf("\33[31md. 删除该用户\n\n\33[0m");//输入d则删除该用户

		char select = '0';
		printf("\33[33m请选择一项修改(输入0返回):\33[0m");
		rewind(stdin);
		scanf("%c", &select);
		autoLogOut(timer);//自动注销功能
		char newString[101] = { 0 };
		int newCredit = 100;
		rewind(stdin);
		switch (select)
		{
		case'1':
			printf("请输入新密码(8-18位):");
			scanf_s("%s", newString, 19);
			if (newString[0] != '\0' && strlen(newString) >= 8)
				strcpy(viewUser(userList, userOrder)->password, newString);
			else
			{
				printf("\33[31m输入错误,未做任何更改!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'2':
			printf("请输入正确真实姓名:");
			scanf_s("%s", newString, 101);
			if (newString[0] != '\0')
				strcpy(viewUser(userList, userOrder)->realName, newString);
			else
			{
				printf("\33[31m输入错误,未做任何更改!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'3':
			printf("请输入正确ID(最大18位):");
			scanf_s("%s", newString, 19);
			if (newString[0] != '\0')
				strcpy(viewUser(userList, userOrder)->ID, newString);
			else
			{
				printf("\33[31m输入错误,未做任何更改!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'4':
			printf("请输入新联系方式(11位):");
			scanf_s("%s", newString, 12);
			if (newString[0] != '\0' && strlen(newString) == 11)
				strcpy(viewUser(userList, userOrder)->phoneNumber, newString);
			else
			{
				printf("\33[31m输入错误,未做任何更改!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'5':
			printf("请输入新信用值(0-100):");
			scanf("%d", &newCredit);
			if (newCredit >= 0 && newCredit <= 100)
				viewUser(userList, userOrder)->credit = newCredit;
			else
			{
				printf("\33[31m输入错误,未做任何更改!\33[0m");
				system("timeout /t 1");
				continue;
			}
			break;
		case'a':
			if (invalidBook == 1)
				addBook(preference, bookList);
			else
			{
				printf("\33[31m输入错误!\33[0m");
				system("timeout /t 1");
			}
			continue;
		case'd':
			printf("确认删除(该用户的借阅信息将会丢失)?\n输入y确认,输入其它字符取消:");
			char input = 'n';
			scanf("%c", &input);
			if (input == 'y')
			{
				returnBook_U(bookList, userList, userOrder);//删除该用户的借阅记录
				saveBook(bookList);
				delUser(userList, userOrder);
				saveUser(userList);//写入磁盘
				getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
				saveSettings(*preference);//写入最后一次更改时间
				printf("\33[32m删除成功!\33[0m");
				system("timeout /t 1");
				return;
			}
			else
				continue;
		case'0':
			return;
		default:
			printf("\33[31m输入错误!\33[0m");
			system("timeout /t 1");
			continue;
		}
		saveUser(userList);//存入磁盘
		getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
		saveSettings(*preference);//写入最后一次更改时间
		printf("\33[32m修改成功!\33[0m");
		system("timeout /t 1");
	}
}

void config(struct settings* preference)
{
	while (1)
	{
		autoLogOut(timer);//自动注销功能
		system("cls");
		printf("\33[7m设置\n\33[0m");
		printf("上次更改数据: \33[36m%4d年%02d月%02d日\n\n\33[0m", preference->last_mod[0], preference->last_mod[1], preference->last_mod[2]);

		printf("当前系统设置如下:\n");
		printf("1. 管理员密码: %s\n", preference->admin_passwd);
		printf("2. 用户最大借阅数量: %d本\n", preference->config_rent[0]);
		printf("3. 用户最大借阅期限: %d天\n", preference->config_rent[1]);
		printf("4. 信用值恢复速率: %lf\n", preference->config_credit[0]);
		printf("5. 信用值惩罚比率: %lf\n", preference->config_credit[1]);
		printf("6. 首页公告: %s\n", preference->bulletin);
		printf("注:新设置只对新的用户活动有效!\n");
		printf("    信用值恢复速率为正常借阅时每本书每天增加的信用值!\n");
		printf("    信用值惩罚比率为每超时一天扣除的信用值与该书价格之比!\n");
		printf("\33[33m请选择要修改的项目(输入0返回):\33[0m");

		char select = '0';
		rewind(stdin);
		scanf("%c", &select);
		autoLogOut(timer);//自动注销功能
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
					strcpy(preference->admin_passwd, newString);
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
			while (1)
			{
				printf("请输入新值(1-100):");
				scanf("%d", &newRent);
				if (newRent >= 1 && newRent <= 100)
				{
					preference->config_rent[0] = newRent;
					break;
				}
				else
				{
					printf("\33[31m输入错误!\n\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'3':
			while (1)
			{
				printf("请输入新值(正数):");
				scanf("%d", &newRent);
				if (newRent >= 1)
				{
					preference->config_rent[1] = newRent;
					break;
				}
				else
				{
					printf("\33[31m输入错误!\n\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'4':
			while (1)
			{
				printf("请输入新值(正数):");
				scanf("%lf", &newCredit);
				if (newCredit > 0.0)
				{
					preference->config_credit[0] = newCredit;
					break;
				}
				else
				{
					printf("\33[31m输入错误!\n\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'5':
			while (1)
			{
				printf("请输入新值(正数):");
				scanf("%lf", &newCredit);
				if (newCredit > 0.0)
				{
					preference->config_credit[1] = newCredit;
					break;
				}
				else
				{
					printf("\33[31m输入错误!\n\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'6':
			while (1)
			{
				printf("请输入新公告(最大100字符或50汉字):");
				scanf_s("%s", newString, 101);
				if (preference->bulletin[0] != '\0')
				{
					strcpy(preference->bulletin, newString);
					break;
				}
				else
				{
					printf("\33[31m必填项!至少应填写管理员联系方式!\33[0m");
					system("timeout /t 1");
					continue;
				}
			}
			break;
		case'0':
			return;
		default:
			printf("\33[31m输入错误!\n\33[0m");
			system("timeout /t 1");
			continue;
		}
		getDate(&preference->last_mod[0], &preference->last_mod[1], &preference->last_mod[2]);
		saveSettings(*preference);//存入磁盘
		printf("\33[32m修改成功!\33[0m");
		system("timeout /t 1");
	}
}

int bookRank(struct book* list)//最受欢迎书籍排行榜 会直接显示(并列)前5名 新加入的书籍优先 返回0或所选书籍编号
{
	while (1)
	{
		autoLogOut(timer);//自动注销检测
		system("cls");//清屏
		printf("\33[7m最受欢迎书籍排行榜\n\n\33[0m");
		if (list == NULL)//链表不存在则直接返回
			return 0;
		if (list->next == NULL)//链表中无书 显示警告并退出
		{
			printf("\33[31m无任何书籍!\n\n\33[0m");
			system("pause");
			return 0;
		}

		struct book* sort = rank(list);
		struct book* bookT;
		int realOrder[5] = { 0 };//储存实际编号的数组 元素个数与显示多少名相等
		printf("排名   书名                 作者                 借阅人数 编号\n");
		int n;
		for (n = 1; n <= 5; n++)//显示前多少名 现为前5名
		{
			if (viewBook(sort, n) == NULL)
				continue;
			bookT = viewBook(sort, n);
			realOrder[n - 1] = searchBook_Acc(list, viewBook(sort, n)->name);//通过书名把临时编号转换为实际编号
			printf("第%2d名 %-20s %-20s    %2d   \33[36m%3d\33[0m\n", n, bookT->name, bookT->author, rentCount(sort, n), realOrder[n - 1]);
		}

		while (sort)//释放内存
		{
			bookT = sort->next;
			free(sort);
			sort = bookT;
		}

		printf("\n");
		printf("\33[33m请输入要操作的书籍的编号(输入0返回):\33[0m");
		int select = 0;
		rewind(stdin);
		scanf("%d", &select);
		if (select != 0 && select != realOrder[0] && select != realOrder[1] && select != realOrder[2] && select != realOrder[3] && select != realOrder[4])
		{
			printf("\33[31m上表中无此书!\n\33[0m");
			system("timeout /t 1");
			continue;
		}
		else
			return select;
	}
}

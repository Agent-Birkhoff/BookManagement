#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"linked_list.h"
#include"file.h"

extern int main(void);
time_t timer;//全局计时器 记录上一次活动的时间

int days_of_year(int y)//计算该年总天数
{
	if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0))//闰年
		return 366;
	else
		return 365;
}

int days_of_month(int m, int y)//计算该月总天数 依赖days_of_year()
{
	if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
		return 31;
	if (m == 4 || m == 6 || m == 9 || m == 11)
		return 30;
	else//二月
	{
		if (days_of_year(y) == 366)
			return 29;
		else
			return 28;
	}
}

void getDate(int* y, int* m, int* d)//获取当前日期
{
	time_t raw;
	struct tm* local;
	time(&raw);
	local = localtime(&raw);
	*y = local->tm_year + 1900;
	*m = local->tm_mon + 1;
	*d = local->tm_mday;
}

int isExpired(struct book* list, int order, int whichOne, int maxTime)//从借书当天起算 依赖getDate() days_of_year() days_of_month()
{                                                                                                         //系统日期错误则返回最大租借时间 过期则返回超时日期(负数) 未过期则返回剩余日期
	int pastDays = 0;
	struct book bookT = *viewBook(list, order);//性能优化
	int y = bookT.rentDate[whichOne][0];
	int m = bookT.rentDate[whichOne][1];
	int d = bookT.rentDate[whichOne][2];
	int nowY, nowM, nowD;
	getDate(&nowY, &nowM, &nowD);

	if (y <= nowY && m <= nowM && d <= nowD && d != 0)//当前日期应比租借日期晚 允许当天归还
	{
		if (nowY == y)//当年归还
		{
			if (nowM == m)//当月归还
				pastDays += nowD - d + 1;//借书当天算作第一天
			else//非当月归还
			{
				int n;
				for (n = m + 1; n < nowM; n++)//如果间隔整月
					pastDays += days_of_month(n, y);
				pastDays += days_of_month(m, y) - d + nowD + 1;//借书当天算作第一天
			}
		}
		else//非当年归还
		{
			int n;
			for (n = y + 1; n < nowY; n++)//如果间隔整年
				pastDays += days_of_year(n);
			for (n = m + 1; n <= 12; n++)//借书当年剩余月份的总天数
				pastDays += days_of_month(n, y);
			for (n = 1; n < nowM; n++)//当前年份已过月份的总天数
				pastDays += days_of_month(n, nowY);
			pastDays += days_of_month(m, y) - d + nowD + 1;//借书当天算作第一天
		}
	}
	return (maxTime - pastDays);
}

int creditPunish(struct settings preference, struct book* list, int bookOrder)//计算指定书籍因超时每天应扣除多少信用值
{
	int result = (int)(preference.config_credit[1] * viewBook(list, bookOrder)->price - preference.config_credit[0]);
	return result;
}

int remainCredit(struct settings preference, struct user* userList, struct book* bookList, int userOrder)//计算用户信用值剩余量
{
	struct user userT = *viewUser(userList, userOrder);//性能优化
	int result = userT.credit;
	int n;
	for (n = 0; n < 100; n++)
	{
		if (userT.bksBrwd[n][0] == '\0')//当前槽位没有借阅记录则跳过
			continue;
		int bookOrder = searchBook_Acc(bookList, userT.bksBrwd[n]);
		if (bookOrder == -1)//书籍不存在则跳过
			continue;
		if (viewBook(bookList, bookOrder)->rentDate[userT.whichOne[n]][2] == 0)//书籍此前曾被误删则不扣除信用值
			continue;
		int remainDays = isExpired(bookList, bookOrder, userT.whichOne[n], preference.config_rent[1]);
		if (remainDays < 0)
			result += (int)(preference.config_credit[1] * viewBook(bookList, bookOrder)->price * remainDays);//扣除信用值
	}
	return result;
}

void autoLogOut(time_t timer)
{
	time_t now;
	time(&now);
	if (difftime(now, timer) > 600.0)//默认10分钟无输入则在下一次进入某界面时自动注销
	{
		system("cls");
		printf("\33[31m由于您无动作超过10分钟,已被自动注销!\n\33[0m");
		system("timeout /t 3");//暂停3秒钟
		main();//回到主界面
	}
	else
		timer = now;
}

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"linked_list.h"
#include"file.h"

extern int main(void);
time_t timer;//ȫ�ּ�ʱ�� ��¼��һ�λ��ʱ��

int days_of_year(int y)//�������������
{
	if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0))//����
		return 366;
	else
		return 365;
}

int days_of_month(int m, int y)//������������� ����days_of_year()
{
	if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
		return 31;
	if (m == 4 || m == 6 || m == 9 || m == 11)
		return 30;
	else//����
	{
		if (days_of_year(y) == 366)
			return 29;
		else
			return 28;
	}
}

void getDate(int* y, int* m, int* d)//��ȡ��ǰ����
{
	time_t raw;
	struct tm* local;
	time(&raw);
	local = localtime(&raw);
	*y = local->tm_year + 1900;
	*m = local->tm_mon + 1;
	*d = local->tm_mday;
}

int isExpired(struct book* list, int order, int whichOne, int maxTime)//�ӽ��鵱������ ����getDate() days_of_year() days_of_month()
{                                                                                                         //ϵͳ���ڴ����򷵻�������ʱ�� �����򷵻س�ʱ����(����) δ�����򷵻�ʣ������
	int pastDays = 0;
	struct book bookT = *viewBook(list, order);//�����Ż�
	int y = bookT.rentDate[whichOne][0];
	int m = bookT.rentDate[whichOne][1];
	int d = bookT.rentDate[whichOne][2];
	int nowY, nowM, nowD;
	getDate(&nowY, &nowM, &nowD);

	if (y <= nowY && m <= nowM && d <= nowD && d != 0)//��ǰ����Ӧ����������� ������黹
	{
		if (nowY == y)//����黹
		{
			if (nowM == m)//���¹黹
				pastDays += nowD - d + 1;//���鵱��������һ��
			else//�ǵ��¹黹
			{
				int n;
				for (n = m + 1; n < nowM; n++)//����������
					pastDays += days_of_month(n, y);
				pastDays += days_of_month(m, y) - d + nowD + 1;//���鵱��������һ��
			}
		}
		else//�ǵ���黹
		{
			int n;
			for (n = y + 1; n < nowY; n++)//����������
				pastDays += days_of_year(n);
			for (n = m + 1; n <= 12; n++)//���鵱��ʣ���·ݵ�������
				pastDays += days_of_month(n, y);
			for (n = 1; n < nowM; n++)//��ǰ����ѹ��·ݵ�������
				pastDays += days_of_month(n, nowY);
			pastDays += days_of_month(m, y) - d + nowD + 1;//���鵱��������һ��
		}
	}
	return (maxTime - pastDays);
}

int creditPunish(struct settings preference, struct book* list, int bookOrder)//����ָ���鼮��ʱÿ��Ӧ�۳���������ֵ
{
	int result = (int)(preference.config_credit[1] * viewBook(list, bookOrder)->price - preference.config_credit[0]);
	return result;
}

int remainCredit(struct settings preference, struct user* userList, struct book* bookList, int userOrder)//�����û�����ֵʣ����
{
	struct user userT = *viewUser(userList, userOrder);//�����Ż�
	int result = userT.credit;
	int n;
	for (n = 0; n < 100; n++)
	{
		if (userT.bksBrwd[n][0] == '\0')//��ǰ��λû�н��ļ�¼������
			continue;
		int bookOrder = searchBook_Acc(bookList, userT.bksBrwd[n]);
		if (bookOrder == -1)//�鼮������������
			continue;
		if (viewBook(bookList, bookOrder)->rentDate[userT.whichOne[n]][2] == 0)//�鼮��ǰ������ɾ�򲻿۳�����ֵ
			continue;
		int remainDays = isExpired(bookList, bookOrder, userT.whichOne[n], preference.config_rent[1]);
		if (remainDays < 0)
			result += (int)(preference.config_credit[1] * viewBook(bookList, bookOrder)->price * remainDays);//�۳�����ֵ
	}
	return result;
}

void autoLogOut(time_t timer)
{
	time_t now;
	time(&now);
	if (difftime(now, timer) > 600.0)//Ĭ��10����������������һ�ν���ĳ����ʱ�Զ�ע��
	{
		system("cls");
		printf("\33[31m�������޶�������10����,�ѱ��Զ�ע��!\n\33[0m");
		system("timeout /t 3");//��ͣ3����
		main();//�ص�������
	}
	else
		timer = now;
}

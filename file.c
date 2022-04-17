#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 6031)//禁用讨厌的scanf返回值警告
#include<stdio.h>
#include<string.h>
#include<direct.h>
#include"linked_list.h"
#include"date.h"
#define MAX_PATH 260//windows下最大路径长度为260

struct settings
{
	char admin_passwd[19];//管理密码
	int config_rent[2];//最大租借数量,可租借天数
	double config_credit[2];//信用恢复速率(每天增量),惩罚比率(每超时一天扣除的信用值与书籍价格之比)
	char bulletin[101];//首页公告板 最大允许100字符(50汉字)
	int last_mod[3];//最后一次修改时间 yyyy/mm/dd
};

struct settings readSettings()//无法获取路径则返回默认值 无存档则新建
{
	struct settings preference;
	strcpy(preference.admin_passwd, "admin\0");//默认管理密码 因不足8位 一旦修改将被强制弃用
	preference.config_rent[0] = 100;//默认最大租借数量
	preference.config_rent[1] = 30;//默认可租借天数
	preference.config_credit[0] = 1.0;//默认信用恢复速率
	preference.config_credit[1] = 0.1;//默认惩罚比率(每超时一天扣除的信用值与书籍价格之比)
	strcpy(preference.bulletin, "欢迎使用!\0");//首页公告板 最大允许100字符(50汉字) 默认字符串为欢迎使用!
	int y, m, d;
	getDate(&y, &m, &d);//获取当前日期
	preference.last_mod[0] = y;//初始最后修改年份
	preference.last_mod[1] = m;//初始最后修改月份
	preference.last_mod[2] = d;//初始最后修改日期

	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//获取程序当前路径
	{
		strcat(localPath, "\\settings.data");
		FILE* fp = NULL;
		fp = fopen(localPath, "rb");//只读打开文件
		if (fp == NULL)//文件不存在 新建
		{
			fp = fopen(localPath, "wb");//覆写打开文件
			fwrite(&preference, sizeof(struct settings), 1, fp);
			fclose(fp);//写入磁盘
		}
		else//读取
		{
			fread(&preference, sizeof(struct settings), 1, fp);
			fclose(fp);//关闭文件
		}
	}
	return preference;
}

void saveSettings(struct settings preference)//如果无法获取路径或文件打开失败则无作为
{
	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//获取程序当前路径
	{
		strcat(localPath, "\\settings.data");
		FILE* fp = fopen(localPath, "wb");//覆写打开文件
		if (fp)
		{
			fwrite(&preference, sizeof(struct settings), 1, fp);
			fclose(fp);//写入磁盘
		}
	}
}

struct user* readUser()//无法获取路径或无存档则返回NULL
{
	struct user* start = NULL;
	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//获取程序当前路径
	{
		strcat(localPath, "\\user.data");
		FILE* fp = NULL;
		fp = fopen(localPath, "rb");//只读打开文件
		if (fp)//文件存在 读取
		{
			start = crtUser(0);//新建链表头
			struct user* node = start;
			int i = 1;
			while (1)
			{
				struct user userT;
				if (fread(&userT, sizeof(struct user), 1, fp) == 0)
					break;
				insUser(start, i, userT);//写入节点
				i++;
			}
			fclose(fp);//关闭文件
		}
	}
	return start;
}

void saveUser(struct user* list)//无法获取路径或文件打开失败则无作为 写入时出错会跳过错误节点
{
	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//获取程序当前路径
	{
		strcat(localPath, "\\user.data");
		FILE* fp = NULL;
		fp = fopen(localPath, "wb");//覆写打开文件 文件不存在则新建
		if (fp)
		{
			int length = lenUser(list);//获得链表长度 方便写入
			int i;
			for (i = 1; i <= length; i++)
			{
				if (viewUser(list, i) == NULL)
					continue;
				struct user userT = *viewUser(list, i);
				fwrite(&userT, sizeof(struct user), 1, fp);
			}
			fclose(fp);//写入磁盘
		}
	}
}

struct book* readBook()//无法获取路径或无存档则返回NULL
{
	struct book* start = NULL;
	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//获取程序当前路径
	{
		strcat(localPath, "\\book.data");
		FILE* fp = NULL;
		fp = fopen(localPath, "rb");//只读打开文件
		if (fp)//文件存在 读取
		{
			start = crtBook(0);
			struct book* node = start;
			int i = 1;
			while (1)
			{
				struct book bookT;
				if (fread(&bookT, sizeof(struct book), 1, fp) == 0)
					break;
				insBook(start, i, bookT);
				i++;
			}
			fclose(fp);//关闭文件
		}
	}
	return start;
}

void saveBook(struct book* list)//无法获取路径或文件打开失败则无作为
{
	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//获取程序当前路径
	{
		strcat(localPath, "\\book.data");
		FILE* fp = NULL;
		fp = fopen(localPath, "wb");//覆写打开文件 文件不存在则新建
		if (fp)
		{
			int length = lenBook(list);//获得链表长度 方便写入
			int i;
			for (i = 1; i <= length; i++)
			{
				if (viewBook(list, i) == NULL)
					continue;
				struct book bookT = *viewBook(list, i);
				fwrite(&bookT, sizeof(struct book), 1, fp);
			}
			fclose(fp);//写入磁盘
		}
	}
}

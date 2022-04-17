#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 6031)//���������scanf����ֵ����
#include<stdio.h>
#include<string.h>
#include<direct.h>
#include"linked_list.h"
#include"date.h"
#define MAX_PATH 260//windows�����·������Ϊ260

struct settings
{
	char admin_passwd[19];//��������
	int config_rent[2];//����������,���������
	double config_credit[2];//���ûָ�����(ÿ������),�ͷ�����(ÿ��ʱһ��۳�������ֵ���鼮�۸�֮��)
	char bulletin[101];//��ҳ����� �������100�ַ�(50����)
	int last_mod[3];//���һ���޸�ʱ�� yyyy/mm/dd
};

struct settings readSettings()//�޷���ȡ·���򷵻�Ĭ��ֵ �޴浵���½�
{
	struct settings preference;
	strcpy(preference.admin_passwd, "admin\0");//Ĭ�Ϲ������� ����8λ һ���޸Ľ���ǿ������
	preference.config_rent[0] = 100;//Ĭ������������
	preference.config_rent[1] = 30;//Ĭ�Ͽ��������
	preference.config_credit[0] = 1.0;//Ĭ�����ûָ�����
	preference.config_credit[1] = 0.1;//Ĭ�ϳͷ�����(ÿ��ʱһ��۳�������ֵ���鼮�۸�֮��)
	strcpy(preference.bulletin, "��ӭʹ��!\0");//��ҳ����� �������100�ַ�(50����) Ĭ���ַ���Ϊ��ӭʹ��!
	int y, m, d;
	getDate(&y, &m, &d);//��ȡ��ǰ����
	preference.last_mod[0] = y;//��ʼ����޸����
	preference.last_mod[1] = m;//��ʼ����޸��·�
	preference.last_mod[2] = d;//��ʼ����޸�����

	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//��ȡ����ǰ·��
	{
		strcat(localPath, "\\settings.data");
		FILE* fp = NULL;
		fp = fopen(localPath, "rb");//ֻ�����ļ�
		if (fp == NULL)//�ļ������� �½�
		{
			fp = fopen(localPath, "wb");//��д���ļ�
			fwrite(&preference, sizeof(struct settings), 1, fp);
			fclose(fp);//д�����
		}
		else//��ȡ
		{
			fread(&preference, sizeof(struct settings), 1, fp);
			fclose(fp);//�ر��ļ�
		}
	}
	return preference;
}

void saveSettings(struct settings preference)//����޷���ȡ·�����ļ���ʧ��������Ϊ
{
	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//��ȡ����ǰ·��
	{
		strcat(localPath, "\\settings.data");
		FILE* fp = fopen(localPath, "wb");//��д���ļ�
		if (fp)
		{
			fwrite(&preference, sizeof(struct settings), 1, fp);
			fclose(fp);//д�����
		}
	}
}

struct user* readUser()//�޷���ȡ·�����޴浵�򷵻�NULL
{
	struct user* start = NULL;
	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//��ȡ����ǰ·��
	{
		strcat(localPath, "\\user.data");
		FILE* fp = NULL;
		fp = fopen(localPath, "rb");//ֻ�����ļ�
		if (fp)//�ļ����� ��ȡ
		{
			start = crtUser(0);//�½�����ͷ
			struct user* node = start;
			int i = 1;
			while (1)
			{
				struct user userT;
				if (fread(&userT, sizeof(struct user), 1, fp) == 0)
					break;
				insUser(start, i, userT);//д��ڵ�
				i++;
			}
			fclose(fp);//�ر��ļ�
		}
	}
	return start;
}

void saveUser(struct user* list)//�޷���ȡ·�����ļ���ʧ��������Ϊ д��ʱ�������������ڵ�
{
	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//��ȡ����ǰ·��
	{
		strcat(localPath, "\\user.data");
		FILE* fp = NULL;
		fp = fopen(localPath, "wb");//��д���ļ� �ļ����������½�
		if (fp)
		{
			int length = lenUser(list);//��������� ����д��
			int i;
			for (i = 1; i <= length; i++)
			{
				if (viewUser(list, i) == NULL)
					continue;
				struct user userT = *viewUser(list, i);
				fwrite(&userT, sizeof(struct user), 1, fp);
			}
			fclose(fp);//д�����
		}
	}
}

struct book* readBook()//�޷���ȡ·�����޴浵�򷵻�NULL
{
	struct book* start = NULL;
	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//��ȡ����ǰ·��
	{
		strcat(localPath, "\\book.data");
		FILE* fp = NULL;
		fp = fopen(localPath, "rb");//ֻ�����ļ�
		if (fp)//�ļ����� ��ȡ
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
			fclose(fp);//�ر��ļ�
		}
	}
	return start;
}

void saveBook(struct book* list)//�޷���ȡ·�����ļ���ʧ��������Ϊ
{
	char localPath[MAX_PATH];
	if (_getcwd(localPath, MAX_PATH));//��ȡ����ǰ·��
	{
		strcat(localPath, "\\book.data");
		FILE* fp = NULL;
		fp = fopen(localPath, "wb");//��д���ļ� �ļ����������½�
		if (fp)
		{
			int length = lenBook(list);//��������� ����д��
			int i;
			for (i = 1; i <= length; i++)
			{
				if (viewBook(list, i) == NULL)
					continue;
				struct book bookT = *viewBook(list, i);
				fwrite(&bookT, sizeof(struct book), 1, fp);
			}
			fclose(fp);//д�����
		}
	}
}

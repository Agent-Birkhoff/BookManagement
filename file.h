#ifndef _FILE_H_
#define _FILE_H_

struct settings
{
	char admin_passwd[19];//��������
	int config_rent[2];//����������,���������
	double config_credit[2];//���ûָ�����,�ͷ�����(ÿ��ʱһ��۳�������ֵ���鼮�۸�֮��)
	char bulletin[101];//��ҳ����� �������100�ַ�(50����)
	int last_mod[3];//���һ���޸�ʱ�� yyyy/mm/dd
};

struct settings readSettings();//�޷���ȡ·���򷵻�Ĭ��ֵ �޴浵���½�
void saveSettings(struct settings preference);//����޷���ȡ·�����ļ���ʧ��������Ϊ
struct user* readUser();//�޷���ȡ·�����޴浵�򷵻�NULL
void saveUser(struct user* list);//�޷���ȡ·�����ļ���ʧ��������Ϊ д��ʱ�������������ڵ�
struct book* readBook();//�޷���ȡ·�����޴浵�򷵻�NULL
void saveBook(struct book* list);//�޷���ȡ·�����ļ���ʧ��������Ϊ

#endif

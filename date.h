#ifndef _DATE_H_
#define _DATE_H_

#include<time.h>

time_t timer;

extern void getDate(int* y, int* m, int* d);//��ȡ��ǰ����
extern int isExpired(struct book* list, int order, int whichOne, int maxTime);//�ӽ��鵱������ ����getDate()��Դ�ļ����������������ĺ���
																													  //ϵͳ���ڴ����򷵻�������ʱ�� �����򷵻س�ʱ����(����) δ�����򷵻�ʣ������
extern int creditPunish(struct settings preference, struct book* list, int bookOrder);//����ָ���鼮��ʱÿ��Ӧ�۳���������ֵ
extern int remainCredit(struct settings preference, struct user* userList, struct book* bookList, int userOrder);//�����û�����ֵʣ����
extern void autoLogOut(time_t timer);//ʮ�����޲������Զ�ע��

#endif

#ifndef _DATE_H_
#define _DATE_H_

#include<time.h>

time_t timer;

extern void getDate(int* y, int* m, int* d);//获取当前日期
extern int isExpired(struct book* list, int order, int whichOne, int maxTime);//从借书当天起算 依赖getDate()及源文件内两个计算天数的函数
																													  //系统日期错误则返回最大租借时间 过期则返回超时日期(负数) 未过期则返回剩余日期
extern int creditPunish(struct settings preference, struct book* list, int bookOrder);//计算指定书籍因超时每天应扣除多少信用值
extern int remainCredit(struct settings preference, struct user* userList, struct book* bookList, int userOrder);//计算用户信用值剩余量
extern void autoLogOut(time_t timer);//十分钟无操作则自动注销

#endif

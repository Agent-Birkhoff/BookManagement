#ifndef _FILE_H_
#define _FILE_H_

struct settings
{
	char admin_passwd[19];//管理密码
	int config_rent[2];//最大租借数量,可租借天数
	double config_credit[2];//信用恢复速率,惩罚比率(每超时一天扣除的信用值与书籍价格之比)
	char bulletin[101];//首页公告板 最大允许100字符(50汉字)
	int last_mod[3];//最后一次修改时间 yyyy/mm/dd
};

struct settings readSettings();//无法获取路径则返回默认值 无存档则新建
void saveSettings(struct settings preference);//如果无法获取路径或文件打开失败则无作为
struct user* readUser();//无法获取路径或无存档则返回NULL
void saveUser(struct user* list);//无法获取路径或文件打开失败则无作为 写入时出错会跳过错误节点
struct book* readBook();//无法获取路径或无存档则返回NULL
void saveBook(struct book* list);//无法获取路径或文件打开失败则无作为

#endif

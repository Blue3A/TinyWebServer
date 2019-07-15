#include<mysql/mysql.h>
#include<iostream>
#include<string>
#include<string.h>
#include<cstdio>
#include"sql_connection_pool.h"
#include<map>
using namespace std;

int main(int argc,char *argv[])
{
    map<string,string> users;

    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    //初始化数据库连接池，连接池为静态大小
    //通过主机地址和登录账号，密码进入服务器数据库，选择qgydb库
    connection_pool *connPool=connection_pool::GetInstance("localhost","root","root","qgydb",3306,5);
    
    //在连接池中取一个连接
    MYSQL *mysql=connPool->GetConnection();

    string name(argv[1]);
    const char *namep = name.c_str();
    string passwd(argv[2]);
    const char *passwdp = passwd.c_str();
    char flag = *argv[0];
	
    //如果是注册，先检测数据库中是否有重名的
    //没有重名的，进行增加数据
    char *sql_insert = (char*)malloc(sizeof(char)*200);
    strcpy(sql_insert, "INSERT INTO user(username, passwd) VALUES(");
    strcat(sql_insert, "'");
    strcat(sql_insert, namep);
    strcat(sql_insert, "', '");
    strcat(sql_insert, passwdp);
    strcat(sql_insert, "');");

    //登录时查询是否已经存在
    char *sql_find = (char*)malloc(sizeof(char)*200);
    strcpy(sql_find, "SELECT username FROM user WHERE username = ");
    strcat(sql_find, "'");
    strcat(sql_find, namep);
    strcat(sql_find, "' and passwd = '");
    strcat(sql_find, passwdp);
    strcat(sql_find, "';");

    //mysql_query只用于判断sql语句是否正确,正确返回0,否则返回1
    //除了insert,delete,其他的select语句不能连续使用两次query语句,需要通过mysql_store_result返回结果集然后释放
    //若判断数据库中是否存在,需要判断结果集中获取下一行是否为NULL
    if(flag == '3'){
	int res_;
	mysql_query(mysql,sql_find);
        MYSQL_RES *result=mysql_store_result(mysql);
	if(NULL == mysql_fetch_row(result))
		res_ = 0;
	else
		res_ = 1;

	if(!res_){
	    mysql_free_result(result);

	    pthread_mutex_lock(&lock);
	    int res = mysql_query(mysql,sql_insert);
	    MYSQL_RES *result_=mysql_store_result(mysql);
	    mysql_free_result(result_);
	    pthread_mutex_unlock(&lock);

	    if(!res)
	    	printf("1\n");
	    else
		printf("0\n");
	}
	else{
	    	printf("0\n");
	}
    }
    //如果是登录，直接判断
    //若浏览器端输入的用户名和密码在表中可以查找到，返回1，否则返回0
    else if(flag == '2'){
	    int res;
	    mysql_query(mysql,sql_find);
	    MYSQL_RES *result=mysql_store_result(mysql);
	    if(NULL == mysql_fetch_row(result)){
		res = 0;
	    }
	    else
		res = 1;
	    mysql_free_result(result);

	    if(res){
		printf("1\n");
	    }
	    else{
		printf("0\n");
	    }
    }
    else
	printf("0\n");
    connPool->DestroyPool();
}


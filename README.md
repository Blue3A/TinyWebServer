

TinyWebServer
===============
Linux下的简易web服务器，实现web端用户注册，登录功能.
> * C/C++
> * B/S架构
> * [线程同步机制包装类](https://github.com/qinguoyi/TinyWebServer/tree/master/lock)
> * [http连接请求处理类](https://github.com/qinguoyi/TinyWebServer/tree/master/http)
> * [半同步/半反应堆线程池](https://github.com/qinguoyi/TinyWebServer/tree/master/threadpool)
> * [定时器处理非活动连接](https://github.com/qinguoyi/TinyWebServer/tree/master/timer)
> * [同步/异步日志系统 ](https://github.com/qinguoyi/TinyWebServer/tree/master/log)  
> * [数据库连接池](https://github.com/qinguoyi/TinyWebServer/tree/master/CGI%26mysql) 
> * [CGI实现注册和登录校验](https://github.com/qinguoyi/TinyWebServer/tree/master/CGI%26mysql) 


web端界面
-------------

> * 判断是否注册   
> * 注册
> * 注册失败提示
> * 登录
> * 登录失败提示


<div align=center><img src="https://github.com/qinguoyi/TinyWebServer/blob/master/interface/judge.jpg" height="200"/>         <img src="https://github.com/qinguoyi/TinyWebServer/blob/master/interface/signup.jpg" height="200"/>         <img src="https://github.com/qinguoyi/TinyWebServer/blob/master/interface/signupfail.jpg" height="200"/></div>

<div align=center><img src="https://github.com/qinguoyi/TinyWebServer/blob/master/interface/signin.jpg" height="200"/><img src="https://github.com/qinguoyi/TinyWebServer/blob/master/interface/signinfail.jpg" height="200"/></div>


测试
------------
* 测试前确认已安装MySQL数据库

    ```C++
    //建立yourdb库
    create database yourdb set utf8;

    //创建users表
    USE yourdb;
    CREATE TABLE users(
        username char(50) NULL,
        passwd char(50) NULL
    )ENGINE=InnoDB;

    //添加数据
    INSERT INTO users(username, passwd) VALUES('name', 'passwd');
    ```

* 修改sign.cpp中的数据库初始化信息

    ```C++
    //root root为服务器数据库的登录名和密码
    connection_pool *connPool=connection_pool::GetInstance("localhost","root","root","yourdb",3306,5);
    ```

* 生成CGI

    ```C++
    cd CGI&mysql
    make clean
    make
    ```

* 将生成的check.cgi放到root文件夹

    ```C++
    cp ./check.cgi ../root
    ```

* 修改http_conn.cpp中的root路径

    ```C++
    const char* doc_root="/home/qgy/serverProject/serverProjectRegister/root";
    ```

* 生成server

    ```C++
    cd ..
    make clean
    make
    ```

* 启动server

    ```C++
    ./server port
    ```

* 浏览器端

    ```C++
    ip:port
    ```

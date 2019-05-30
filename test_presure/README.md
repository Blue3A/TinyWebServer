

服务器压力测试
===============
单纯的I/O复用方式施压程度是最高的，因为线程和进程调度本身也要占用一定的CPU时间.利用epoll实现通用的服务器压力测试程序，先模拟指定数的客户端，然后同时连接上web服务器，不断进行数据交换，目前已测可以实现上万并发连接的稳定数据交换。
> * ulimit -n查看可用的文件描述符数量
> * ulimit -n num对不同shell设置临时的文件描述符数量num
> * 创建指定数量的连接
> * 通过epoll检测各连接的读写事件
> * 与服务器不断交换读写数据
> * 目前已测可以实现上万的并发连接数据交换。



压力测试
------------
* 修改test_presure.c中的客户端请求信息

    ```C++
    //每个客户端不停的向服务器发送这个请求,http://localhost/judge.html为请求文件
    static const char* request="GET http://localhost/judge.html HTTP/1.1\r\nConnection:keep-alive\r\n\r\nxxxxxxxxxxxxx";
    ```
* 启动测试

    ```C++
    ./test_pre ip port num
    ```

ulimit设置文件描述符
---------
ulimit总结[![ulimit](https://img.shields.io/badge/ulimit-summary-orange.svg)](https://www.cnblogs.com/qinguoyi/p/10948010.html)


http连接处理类
===============
根据状态转移,通过主从状态机封装了http连接类。其中,主状态机在内部调用从状态机,从状态机将处理状态和数据传给主状态机
> * 客户端发出http连接请求
> * 从状态机读取数据,更新自身状态和接收数据,传给主状态机
> * 主状态机根据从状态机状态,更新自身状态,决定响应请求还是继续读取

工作流程
---------
process函数，服务器处理http请求行，调用process_read，然后写返回客户端的内容，调用process_write

* **process_read**函数，返回服务器处理http请求的结果。函数分为主状态机和从状态机，主状态机从内部调用从状态机parse_line，从状态机每次只读取一行数据，根据读取数据的完整程度，分为LINE_OK,LINE_BAD,LINE_OPEN，然后将数据传给主状态机。

    * **parse_line**函数，用于读取一行的内容，根据末尾\r\n判断，然后将数据传给主状态机，根据主状态机所处的状态,进行解析数据主状态机默认状态为CHECK_STATE_REQUESTLINE，CHECK_STATE_REQUESTLINE调用parse_request_line，CHECK_STATE_HEADER根据是否解析完请求头调用parse_headers或do_request，CHECK_STATE_CONTENT调用do_requset
    
        * **parse_request_line**函数，解析http请求行，获得请求方法，目标文件url，http版本号，若能获得完整请求行，将状态转移为CHECK_STATE_HEADER
	
		* **parse_header**函数，解析http请求的一个头部信息，根据空行判断是否解析完毕，解析完毕返回GET_REQUEST
	
		* **parse_content**函数，判断是否http请求是否被完整读入，完整读入返回GET_REQUEST
	
		* **do_request**函数，当得到一个完整，正确的http请求，分析目标文件的属性，判断是文件还是目录，用户是否拥有权限，成功则使用mmap将其映射到m_file_address处，更新m_file_address，返回FILE_REQUEST

* **process_write**函数，根据状态码执行相应操作:BAD_REQUEST,NO_RESOURCE,FORBIDDEN_REQUEST,FILE_REQUEST,INTERNAL_ERROR，操作大致相同，调用add_status_line和add_headers

	* **add_status_line**函数，调用add_response添加状态行，HTTP/1.1 status title,status为状态码，title为状态码说明
	
		* **add_response**函数，往写缓冲写入待发送的数据
		
	* **add_headers**函数，写消息报头的一些附加信息，调用add_content_length添加发送数据长度，通过add_linger添加网络连接状态，通过add_blank_line添加空行,更新writev中的参数m_iv_count，表示被写内存块的数量
	
* process_write函数调用完后，修改文件描述符，在内核事件表注册写事件

server: main.c ./threadpool/threadpool.h ./http/http_conn.cpp ./http/http_conn.h ./lock/locker.h ./log/log.cpp ./log/log.h ./log/block_queue.h ./CGI_mysql/sql_connection_pool.cpp ./CGI_mysql/sql_connection_pool.h
	g++ -o server main.c ./threadpool/threadpool.h ./http/http_conn.cpp ./http/http_conn.h ./lock/locker.h ./log/log.cpp ./log/log.h ./CGI_mysql/sql_connection_pool.cpp ./CGI_mysql/sql_connection_pool.h -lpthread -lmysqlclient

test_pre: ./test_presure/test_presure.c
	gcc -o test_pre ./test_presure/test_presure.c

check.cgi:./CGI_mysql/sign.cpp ./CGI_mysql/sql_connection_pool.cpp ./CGI_mysql/sql_connection_pool.h
	g++ -o check.cgi ./CGI_mysql/sign.cpp ./CGI_mysql/sql_connection_pool.cpp ./CGI_mysql/sql_connection_pool.h -lmysqlclient

clean:
	rm  -r server
	rm  -r test_pre
	rm  -r check.cgi

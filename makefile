server:main.c ./threadpool/threadpool.h ./http/http_conn.cpp ./http/http_conn.h ./lock/locker.h ./log/log.cpp ./log/log.h ./log/block_queue.h
	g++ -o server -g main.c ./threadpool/threadpool.h ./http/http_conn.cpp ./http/http_conn.h ./lock/locker.h ./log/log.cpp ./log/log.h -lpthread 
#	g++ -o server -g main.c threadpool.h http_conn.cpp http_conn.h locker.h log.cpp log.h block_queue.h -lpthread 
clean:
	rm  -r server

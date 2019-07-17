#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<list>
#include<cstdio>
#include<exception>
#include<pthread.h>
#include "../timer/lst_timer.h"
#include "../http/http_conn.h"
#include "../log/block_queue.h"

template<typename T>
class threadpool{
    public:
        /*thread_number是线程池中线程的数量，max_requests是请求队列中最多允许的、等待处理的请求的数量*/
        threadpool(int thread_number=8,int max_request=10000);
        ~threadpool();
        bool append(T* request, int flagrw, sort_timer_lst *timer_lst, util_timer* timer, client_data *m_user_timer);
    private:
        /*工作线程运行的函数，它不断从工作队列中取出任务并执行之*/
        static void *worker(void *arg);
        void run();
    private:
        int m_thread_number;//线程池中的线程数
        int m_max_requests;//请求队列中允许的最大请求数
        pthread_t *m_threads;//描述线程池的数组，其大小为m_thread_number
	block_queue<T *>m_workqueue;
        locker m_queuelocker;//保护请求队列的互斥锁
        bool m_stop;//是否结束线程
	sort_timer_lst *m_timer_lst; //定时器链表
	util_timer *m_timer; //socket对应定时器
	client_data *m_user_timer;
};
template<typename T>
threadpool<T>::threadpool(int thread_number,int max_requests):m_thread_number(thread_number),m_max_requests(max_requests),m_stop(false),m_threads(NULL),m_timer_lst(NULL), m_timer(NULL), m_user_timer(NULL){
    if(thread_number<=0||max_requests<=0)
        throw std::exception();
    m_threads=new pthread_t[m_thread_number];
    if(!m_threads)
        throw std::exception();
    for(int i=0;i<thread_number;++i)
    {
        //printf("create the %dth thread\n",i);
        if(pthread_create(m_threads+i,NULL,worker,this)!=0){
            delete [] m_threads;
            throw std::exception();
        }
        if(pthread_detach(m_threads[i])){
            delete[] m_threads;
            throw std::exception();
        }
    }
}
template<typename T>
threadpool<T>::~threadpool(){
    delete [] m_threads;
    m_stop=true;
}
template<typename T>
bool threadpool<T>::append(T* request, int flagrw, sort_timer_lst *timer_lst, util_timer* timer, client_data *user_timer)
{
    if(m_workqueue.size()>m_max_requests)
    {
        return false;
    }
    m_workqueue.push(request);
    if(flagrw == 0)
	request->m_flag = 0;
    if(flagrw == 1)
	request->m_flag = 1;
    m_timer_lst = timer_lst;
    m_timer = timer;
    m_user_timer = user_timer;
    return true;
}
template<typename T>
void* threadpool<T>::worker(void* arg){
    threadpool* pool=(threadpool*)arg;
    pool->run();
    return pool;
}
template<typename T>
void threadpool<T>::run()
{
    while(!m_stop)
    {
        T* request = NULL;
        m_workqueue.pop(request);
        if(!request){
            continue;
	}
        request->process(m_timer_lst, m_timer, m_user_timer);
    }
}
#endif


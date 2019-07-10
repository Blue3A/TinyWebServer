#ifndef LST_TIMER
#define LST_TIMER

#include <time.h>
#include "../log/log.h"

#define BUFFER_SIZE 64
class util_timer;
struct client_data
{
	sockaddr_in address;
	int sockfd;
	char buf[BUFFER_SIZE];
	util_timer* timer;
};

class util_timer
{
public:
	util_timer() : prev(NULL), next(NULL){}

public:
	time_t expire;
	void(*cb_func)(client_data*);
	client_data* user_data;
	util_timer* prev;
	util_timer* next;
};

class sort_timer_lst
{
public:
	sort_timer_lst(){
		head = new util_timer;
		tail = new util_timer;

		if (NULL == head || NULL == tail){
			exit(0);
		}

		head->next = tail;
		head->prev = NULL;
		tail->next = NULL;
		tail->prev = head;
	}
	~sort_timer_lst()
	{
		util_timer *p = head;
		util_timer *q = NULL;

		while (p){
			q = p->next;
			delete p;
			p = q;
		}
	}

	//将目标定时器插入到双向链表中
	void add_timer(util_timer* timer)
	{
		if (!timer)
		{
			return;
		}

		if (head->next == tail)
		{
			timer->prev = head;
			timer->next = tail;
			tail->prev = timer;
			head->next = timer;
		}
		else
		{
			//按照升序将目标定时器插入到双向链表中
			util_timer *pCurrent = head->next;
			util_timer *pPer = head;

			while (pCurrent->next && pCurrent->expire < timer->expire){
				pPer = pCurrent;
				pCurrent = pCurrent->next;
			}

			timer->prev = pPer;
			timer->next = pPer->next;
			pPer->next->prev = timer;
			pPer->next = timer;
		}
	}
	//调整定时器在链表中的位置，先取出，再插入
	void adjust_timer(util_timer* timer)
	{
		if (!timer)
		{
			return;
		}
		//如果定时器在尾部，或者调整后仍然小于后面的定时器的时间
		if (timer->next == tail || (timer->expire < timer->next->expire))
		{
			return;
		}
		else{
			//先取出
			timer->prev->next = timer->next;
			timer->next->prev = timer->prev;

			//插入
			util_timer *pCurrent = timer;
			util_timer *pPer = timer->prev;

			while (pCurrent->next && pCurrent->expire < timer->expire){
				pPer = pCurrent;
				pCurrent = pCurrent->next;
			}

			timer->prev = pPer;
			timer->next = pPer->next;
			pPer->next->prev = timer;
			pPer->next = timer;
		}
	}

	//删除定时器
	void del_timer(util_timer* timer)
	{
		if (!timer)
		{
			return;
		}

		timer->prev->next = timer->next;
		timer->next->prev = timer->prev;
		delete timer;
	}

	void tick()
	{
		if (!head)
		{
			return;
		}
		//printf( "timer tick\n" );
		LOG_INFO("%s", "timer tick");
		Log::get_instance()->flush();
		time_t cur = time(NULL);
		util_timer* tmp = head->next;
		while (tmp)
		{
			if (cur < tmp->expire)
			{
				break;
			}
			tmp->cb_func(tmp->user_data);
			util_timer *tmpnext = tmp->next;
			delete tmp;
			tmp = tmpnext;
		}
	}

private:
	util_timer* head;
	util_timer* tail;
};

#endif

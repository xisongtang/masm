#ifndef _LIST_H_
#define _LIST_H_

template <typename T>
class List
{
public:
	struct list{
		list *next ;
		list *last;
		T elem;
	};
	typedef list* iterator;

	List()
	{
		head = new list;
		tail = new list;
		head->next = tail;
		tail->last = head;
		head->last = tail->next = 0;
	}
	void push_back(const T& e)
	{
		list *cur = new list;
		cur->elem = e;
		tail->last->next = cur;
		cur->last = tail->last;
		cur->next = tail;
		tail->last = cur;
	}
	void push_back(iterator &it)
	{
		iterator tnext = it->next;
		iterator tlast = it->last;
		it->next = tail;
		it->last = tail->last;
		tail->last->next = it;
		tail->last = it;
		tlast->next = tnext;
		tnext->last = tlast;
		it = tnext;
	}
	void push_back(List &l)
	{
		if (l.head->next == l.tail)
			return;
		tail->last->next = l.head->next;
		l.head->next->last = tail->last;
		tail->last = l.tail->last;
		l.tail->last->next = tail;

		l.head->next = l.tail;
		l.tail->last = l.head;
	}
	void erase(iterator &it)
	{
		if (it == head || it == tail)
			return;
		iterator tnext = it->next;
		iterator tlast = it->last;
		delete it;
		tlast->next = tnext;
		tnext->last = tlast;
		it = tnext;
	}
	bool isempty() const
	{
		return head->next == tail;
	}
	iterator begin()
	{
		return head->next;
	}
	iterator end()
	{
		return tail;
	}
	bool check() const
	{
		iterator it = head->next;
		while (it != tail)
		{
			if (it == NULL)
				return false;
			it = it->next;
		}
		return true;
	}
	~List()
	{
		iterator tmp;
		while (head)
		{
			tmp = head->next;
			delete head;
			head = tmp;
		}
	}
private:
	iterator head, tail;
};

#endif 

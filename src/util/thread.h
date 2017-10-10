//
//  thread.h
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#ifndef __KGE_UTIL_THREAD_H__
#define __KGE_UTIL_THREAD_H__

#include "core/pre_define.h"
#include "any.h"

#include <functional>
#include <thread>
#include <mutex>

namespace kge
{
	typedef std::function<void()> ThreadInit;
	typedef std::function<void()> ThreadFini;

	struct ThreadNode
	{
		ThreadInit init;
		ThreadFini fini;
	};

	class Thread
	{
	public:
		typedef std::function<Ref<Any>()> Job;
		typedef std::function<void(Ref<Any>)> FiniCallback;
		struct TaskNode
		{
			Job job;
			FiniCallback finiCbk;
		};

		static void Sleep(uint32 ms);
		Thread(uint32 id, ThreadNode node);
		~Thread();
		void AddTask(TaskNode task);
		void Wait();
		uint32 QueueLenght();

	private:
		void Tick();

		uint32 _id;
		Ref<std::thread> _thread;
		std::list<TaskNode> _job_queue;
		std::mutex _mutex;
		std::condition_variable _condition;
		bool _close;
		ThreadNode _node;
	};

	class ThreadPool
	{
	public:
		ThreadPool(uint32 thread_count);
		ThreadPool(const std::vector<ThreadNode>& nodes);
		void Wait();
		uint32 GetThreadCount()const { return _threads.size(); }

		void AddTask(Thread::TaskNode task, int32 thread_index = -1);

	private:
		std::vector<ThreadNode> _nodes;
		std::vector<Ref<Thread>> _threads;
	};
} // end of namespace kge

#endif // __KGE_UTIL_THREAD_H__
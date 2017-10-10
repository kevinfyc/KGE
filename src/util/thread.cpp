//
//  thread.cpp
//  libkge
//
//  Created by kevin on 17-10-10.
//
//

#include "thread.h"
#include "core\Application.h"

namespace kge
{
	
	void Thread::Sleep(uint32 ms)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}

	Thread::Thread(uint32 id, ThreadNode node) :_id(id)
		, _node(node)
	{
		_close = false;
		_thread = RefMake<std::thread>(&Thread::Tick, this);
	}

	Thread::~Thread()
	{
		if (_thread->joinable())
		{
			Wait();

			_mutex.lock();
			_close = true;
			_condition.notify_one();
			_mutex.unlock();
			_thread->join();
		}
	}

	void Thread::Wait()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_condition.wait(lock, [this]() {
			return _job_queue.empty();
		});
	}

	void Thread::AddTask(TaskNode task)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_job_queue.push_back(task);
		_condition.notify_one();
	}

	void Thread::Tick()
	{
		if (_node.init)
			_node.init();

		while (true)
		{
			TaskNode task;
			{
				std::unique_lock<std::mutex> lock(_mutex);

				_condition.wait(lock, [this]() {
					return !_job_queue.empty() || _close;
				});

				if (_close)
					break;

				task = _job_queue.front();
			}

			if (task.job)
			{
				auto any = task.job();

				if (task.finiCbk)
				{
					IApplication::GetInstance()->RunTaskInPreLoop(
						TaskLoop::TaskNode([any, task]() {
						task.finiCbk(any);
					})
					);
				}
			}

			{
				std::lock_guard<std::mutex> lock(_mutex);
				_job_queue.pop_front();
				_condition.notify_one();
			}
		}

		if (_node.fini)
			_node.fini();
	}

	uint32 Thread::QueueLenght()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _job_queue.size();
	}

	///////////////////////////////////////////////////
	// ThreadPool
	///////////////////////////////////////////////////
	ThreadPool::ThreadPool(uint32 thread_count)
	{
		_nodes.resize(thread_count);
		_threads.resize(_nodes.size());
		for (uint32 i = 0; i < _threads.size(); ++i)
			_threads[i] = RefMake<Thread>(i, _nodes[i]);
	}

	ThreadPool::ThreadPool(const std::vector<ThreadNode>& nodes)
	{
		_nodes = nodes;
		_threads.resize(_nodes.size());
		for (uint32 i = 0; i < _threads.size(); ++i)
			_threads[i] = RefMake<Thread>(i, _nodes[i]);
	}

	void ThreadPool::AddTask(Thread::TaskNode task, int32 thread_index /* = -1 */)
	{
		if (thread_index >= 0 && thread_index < _threads.size())
		{
			_threads[thread_index]->AddTask(task);
		}
		else
		{
			int32 min_len = 0x7fffffff;
			int32 min_index = -1;

			for (uint32 i = 0; i < _threads.size(); ++i)
			{
				uint32 len = _threads[i]->QueueLenght();
				if (min_len > len)
				{
					min_len = len;
					min_index = i;

					if(min_len == 0)
						break;
				}
			}

			if (min_index < 0 || min_index > _threads.size() - 1)
				return;

			_threads[min_index]->AddTask(task);
		}
	}

	void ThreadPool::Wait()
	{
		for (auto& thread : _threads)
			thread->Wait();
	}

} // end of namespace
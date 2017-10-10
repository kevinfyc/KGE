//
//  task_loop.cpp
//  libkge
//
//  Created by kevin on 17-10-9.
//
//

#include "task_loop.h"
#include <utility>

namespace kge
{
	TaskLoop::TaskLoop()
	{
		_id_counter = InvalidFuncID;
	}

	TaskLoop::~TaskLoop()
	{

	}

	uint32 TaskLoop::Add(TaskNode task)
	{
		_mutex.lock();
		uint32 id = ++_id_counter;
		_to_add.insert(std::pair<uint32, TaskNode>(id, task));
		_mutex.unlock();

		return id;
	}

	void TaskLoop::Remove(uint32 id)
	{
		_to_remove.push_back(id);
	}

	bool TaskLoop::HasFunc(uint32 id) const
	{
		return _items.count(id) > 0 || _to_add.count(id) > 0;
	}

	void TaskLoop::AddFuncs()
	{
		for (auto& iter : _to_add)
			_items.insert(std::pair<uint32, TaskNode>(iter.first, iter.second));
		_to_add.clear();
	}

	void TaskLoop::RemoveFuncs()
	{
		for (auto id : _to_remove)
		{
			_items.erase(id);
			_to_add.erase(id);
		}

		_to_remove.clear();
	}

	void TaskLoop::Tick()
	{
		_mutex.lock();

		RemoveFuncs();
		AddFuncs();

		for (auto& iter : _items)
		{
			iter.second.func();

			if (iter.second.once)
				Remove(iter.first);
		}

		_mutex.unlock();
	}

} // end of namespace
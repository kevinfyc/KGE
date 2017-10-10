//
//  task_loop.h
//  libkge
//
//  Created by kevin on 17-10-9.
//
//
// 
#include "math/_math.hpp"
#include <functional>
#include <map>
#include <vector>
#include <mutex>

#ifndef __KGE_UTIL_TASK_LOOP_H__
#define __KGE_UTIL_TASK_LOOP_H__

namespace kge
{
	class TaskLoop
	{
	public:
		static const uint32 InvalidFuncID = 0;
		typedef std::function<void()> Func;
		struct TaskNode
		{
			Func func;
			bool once;

			TaskNode(Func _func, bool _once = false) : func(_func)
				,once(_once)
			{

			}
		};

		TaskLoop();
		~TaskLoop();

		void Tick();

		uint32 Add(TaskNode task);

	private:
		void Remove(uint32 id);
		bool HasFunc(uint32 id)const;
		void AddFuncs();
		void RemoveFuncs();

		uint32 _id_counter;
		std::map<uint32, TaskNode> _items;
		std::map<uint32, TaskNode> _to_add;
		std::vector<uint32> _to_remove;
		std::mutex _mutex;
	};
} // end of namespace kge

#endif // __KGE_UTIL_TASK_LOOP_H__
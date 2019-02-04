#include "parallel-task-scheduler.h"
#include <thread>
#include <iostream>
#include <atomic>

using namespace tasks;

namespace
{
	class ParallelTaskScheduler : public tasks::schedulers::_private_::TaskSchedulerImplementation {
	public:
		void perform(std::vector<std::shared_ptr<Task>> tasks) const {
			std::atomic<int> i = 0;
			std::vector<std::thread> threads;

			for (int t = 0; t < 4; t++) {
				threads.push_back(std::thread([&i, &tasks] {
					int j;

					while ((j = i++) < tasks.size()) {
						tasks.at(j)->perform();
					}
				}));
			}

			for (auto& t : threads) {
				t.join();
			}

		}
	};
}

TaskScheduler tasks::schedulers::parallel() {
	return TaskScheduler(std::make_shared<ParallelTaskScheduler>());
}

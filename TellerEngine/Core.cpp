#include "Core.h"

using namespace Teller;

namespace Teller {
	ModuleCore::ModuleCore()
	{
	}

	ModuleCore::~ModuleCore()
	{
	}
	void ModuleCore::AddChildModule(std::shared_ptr<ModuleCore>&& sub_module)
	{
		children.push_back(sub_module);
		sub_module->parent = this->shared_from_this();
	}

	void Teller::ModuleCore::Tick()
	{
		if (bUpdate) {
			for (auto& e : children) {
				e.get()->Tick();
				e.get()->Update();
			}
			return;
		}
		else
		{
			for (auto& e : children) {
				e.get()->Tick();
			}
			return;
		}

	}

	void Teller::ModuleCore::Update()
	{

	}
}


namespace Teller {
	template<typename F>
	void Teller::ThreadPool::push_task(const F& task) {
		{
			const std::lock_guard<std::mutex> lock(tasks_mutex);

			if (!running) {
				throw std::runtime_error("Cannot schedule new task after shutdown.");
			}
			//ここでタスクを追加。
			tasks.push(std::function<void()>(task));

		}
		//スレッド起床
		condition.notify_one();
	}
	void Teller::ThreadPool::worker() {
		for (;;) {
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(tasks_mutex);
				condition.wait(lock, [&] {return !tasks.empty() || !running; });

				if (!running && tasks.empty()) {
					return;
				}
				task = std::move(tasks.front());
				tasks.pop();
			}
			task();
		}
	}

	template<typename F, typename... Args, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
	std::future<R> submit(F&& func, const Args&&... args) {
		auto task = std::make_shared<std::packaged_task<R()>>([func, args...](){
			return func(args...);
		});

		auto future = task->get_future();
		push_task([task]() {(*task)(); });
		return future;
	}

}

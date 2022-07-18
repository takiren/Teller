#pragma once
#include<memory>
#include<thread>
#include<mutex>
#include<queue>
#include<condition_variable>
#include<atomic>
#include<functional>
#include<future>

namespace teller {
	//�X���b�h�v�[��
	// 
	//�Q�l : https://contentsviewer.work/Master/Cpp/how-to-implement-a-thread-pool/article#SectionID_7

	class ThreadPool {
	private:

		std::unique_ptr<std::thread[]> threads; //�X���b�h�p�̃��j�[�N�|�C���^

		int thread_count_; //�X���b�h��

		mutable std::mutex tasks_mutex{};

		std::condition_variable condition;

		std::queue<std::function<void()>> tasks{};

		std::atomic<bool> running{ true };
	public:
		ThreadPool(const int thread_count = std::thread::hardware_concurrency()) :
			thread_count_(thread_count ? thread_count : std::thread::hardware_concurrency()) {
			threads.reset(new std::thread[thread_count]);
			//��u����thread_count-1�܂Ń��[�v
			for (int i = 0; i < thread_count_; ++i) {
				threads[i] = std::thread(&ThreadPool::worker, this);
			}
		}

		~ThreadPool() {
			{
				std::lock_guard<std::mutex> lock(tasks_mutex);
				running = false;
			}

			condition.notify_all();

			for (int i = 0; i < thread_count_; ++i) {
				threads[i].join();
			}
		}

		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool(const ThreadPool&) = delete;

		ThreadPool& operator=(ThreadPool&&) = default;

		template<typename F>
		void push_task(const F& task);

		void worker() {
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

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
		/**
		* @brief Submit a function with zero or more arguments and a return value into the task queue,
		* and get a future for its eventual returned value.
		*/
		// C++17 ���琄������Ă���, �֐��̕Ԃ�l�̌^R ���擾������@
		template <typename F, typename... Args, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
#else
		// C++14 ��, �֐��̕Ԃ�l�̌^R ���擾������@
		template <typename F, typename... Args, typename R = typename std::result_of<std::decay_t<F>(std::decay_t<Args>...)>::type>
#endif
		std::future<R> submit(F&& func, const Args&&... args) {
			auto task = std::make_shared<std::packaged_task<R()>>([func, args...]() {
				return func(args...);
			});
			auto future = task->get_future();

			push_task([task]() { (*task)(); });
			return future;
		}
	};

	template<typename F>
	void teller::ThreadPool::push_task(const F& task) {
		{
			const std::lock_guard<std::mutex> lock(tasks_mutex);

			if (!running) {
				throw std::runtime_error("Cannot schedule new task after shutdown.");
			}
			//�����Ń^�X�N��ǉ��B
			tasks.push(std::function<void()>(task));

		}
		//�X���b�h�N��
		condition.notify_one();
	}

}
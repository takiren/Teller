#pragma once
#include<stdio.h>
#include<string>
#include<vector>
#include<map>
#include<memory>
#include<cassert>
#include<iostream>
#include<set>
#include<fstream>
#include<sstream>
#include<filesystem>
#include<thread>
#include<mutex>
#include<queue>
#include<atomic>
#include<type_traits>
#include<functional>
#include<future>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include"cinder/ImageIo.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include"cinder/Log.h"
#include <list>
#include "cinder/CinderImGui.h"
#include"cinder/ImageIo.h"

using namespace ci;
using namespace ci::app;

namespace Teller {
	using Tint = int;
	using Tuint = unsigned int;

	class ModuleCore :public std::enable_shared_from_this<ModuleCore>
	{
	private:
	protected:
		int count_;
		std::string debugLog;
		std::weak_ptr<ModuleCore> parent;
		std::vector<std::shared_ptr<ModuleCore>>children;
		bool bUpdate; //Game�𓮂������ǂ����B
		bool bEnabled;
	public:
		ModuleCore() :bUpdate(true), bEnabled(true), count_(0), debugLog("Nothing.") {};
		~ModuleCore();

		//�R�s�[�֎~
		ModuleCore(const ModuleCore&) = delete;
		ModuleCore& operator=(const ModuleCore&) = delete;

		//���[�u�͋���
		ModuleCore& operator=(ModuleCore&&) = default;

		void AddChildModule(std::shared_ptr<ModuleCore > sub_module);
		virtual void Tick(); //�K��tick���Ƃɏ�������鏈���B
		virtual void Update(); //Game�������ĂȂ��Ə�������Ȃ��B
		int GetCount() const { return count_; };
	};

	

	//�X���b�h�v�[��
	// 
	//�Q�l : https://contentsviewer.work/Master/Cpp/how-to-implement-a-thread-pool/article#SectionID_7

	class ThreadPool {
		std::unique_ptr<std::thread[]> threads; //�X���b�h�p�̃��j�[�N�|�C���^
		Tint thread_count_; //�X���b�h��
		mutable std::mutex tasks_mutex{};
		std::condition_variable condition;
		std::queue<std::function<void()>> tasks{};
		std::atomic<bool> running{ true };
	public:
		ThreadPool(const Tuint thread_count = std::thread::hardware_concurrency()) :
			thread_count_(thread_count ? thread_count : std::thread::hardware_concurrency()) {
			threads.reset(new std::thread[thread_count]);
			//��u����thread_count-1�܂Ń��[�v
			for (Tuint i = 0; i < thread_count_; ++i) {
				threads[i] = std::thread(&ThreadPool::worker, this);
			}
		}

		~ThreadPool() {
			{
				std::lock_guard<std::mutex> lock(tasks_mutex);
				running = false;
			}

			condition.notify_all();

			for (Tuint i = 0; i < thread_count_; ++i) {
				threads[i].join();
			}
		}

		template<typename F>
		void push_task(const F& task);

		void worker();

		template<typename F, typename... Args, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
		std::future<R> submit(F&& func, const Args&&... args);
	};

}

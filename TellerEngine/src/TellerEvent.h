#pragma once
#include<vector>
#include<string>
#include<functional>
#include<memory>
#include<map>
#include"Core.h"

namespace teller {
	template<class KEY,class DATA_TYPE>
	class TMessageHandler {
	protected:
		using MessageHandlerImpleOnSuccess = std::function<void(DATA_TYPE)>;
		std::map<KEY, std::function<void(DATA_TYPE)>> callbackMap_;
		void onSuccessInternal(const DATA_TYPE& _message) {
			for (auto iter = callbackMap_.begin(); iter != callbackMap_.end(); ++iter) {
				iter->second(_message);
			}
		}
	public:
		TMessageHandler() = default;
		~TMessageHandler() = default;

		void onSuccess(const DATA_TYPE& _message) {
			onSuccessInternal(_message);
		}

		void AttachDesitinationInternal(KEY _key,MessageHandlerImpleOnSuccess _dest){
			callbackMap_[_key] = _dest;
		}
	};

	template<class KEY_TYPE,class DATA_TYPE>
	class TMessanger {
	private:
		std::unique_ptr<TMessageHandler<KEY_TYPE,DATA_TYPE>> handler_;
	public:
		TMessanger():handler_(std::make_unique<TMessageHandler<KEY_TYPE,DATA_TYPE>>()){};
		~TMessanger() = default;
		void SendTMessage(const DATA_TYPE& _message) {
			if (handler_)
				handler_->onSuccess(_message);
		}
		void AttachFunction(KEY_TYPE _key,std::function<void(DATA_TYPE)> callback_) {
			handler_->AttachDesitinationInternal(_key,callback_);
		};
	};

}

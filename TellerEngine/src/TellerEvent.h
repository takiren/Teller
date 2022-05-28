#pragma once
#include<vector>
#include<string>
#include<functional>
#include<memory>
#include<map>
#include"Core.h"

namespace Teller {
	template<class KEY,class DATA_TYPE>
	class TMessageHandler {
	protected:
		using MessageHandlerImpleOnSuccess = std::function<void(DATA_TYPE)>;
		std::map<KEY, std::function<void(DATA_TYPE)>> callbackMap_;
		void onSuccessInternal(const DATA_TYPE& _message) {
			for (auto iter = callbackMap_.begin(); iter != callbackMap_.end(); iter++) {
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
		TMessanger() = default;
		~TMessanger() = default;

		void SetHandler(std::unique_ptr<TMessageHandler<KEY_TYPE, DATA_TYPE>> _handler) {
			handler_ = std::move(_handler);
		}

		void SendMessage(const DATA_TYPE& _message) {
			if (handler_)
				handler_->onSuccess(_message);
		}

		void AttachFunction(KEY_TYPE _key,std::function<void(DATA_TYPE)> callback) {
			handler_->AttachDesitinationInternal(_key,callback);
		};

	};

}

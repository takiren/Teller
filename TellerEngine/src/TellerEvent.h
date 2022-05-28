#pragma once
#include<vector>
#include<string>
#include<functional>
#include<memory>

namespace Teller {
	class TMessage {
	public:
		int ID_;
		std::string message_;
		TMessage() :ID_(0), message_("") {};
		TMessage(int _id) :message_(""), ID_(_id) {};
		virtual ~TMessage() = default;
	};

	class TMessageHandler {
	protected:
		virtual void onSuccessInternal(const TMessage& _message) = 0;
		virtual void onErrorInternal(const std::string& _message) = 0;
		
	public:
		TMessageHandler() = default;
		virtual ~TMessageHandler() = default;

		void onSuccess(const TMessage& _message) {
			onSuccessInternal(_message);
		}

		void onError(const std::string& _message) {
			onErrorInternal(_message);
		}

	};

	using MessageHandlerImpleOnSuccess = std::function<void(TMessage& _message)>;
	using MessageHandlerImpleOnError = std::function<void(const std::string& _message)>;

	class TMessageHandlerImple :public TMessageHandler {
	private:
		MessageHandlerImpleOnSuccess callback_;
		MessageHandlerImpleOnError on_error_;
		TMessage message_;
	protected:

		void onSuccessInternal(const TMessage& _message) override {
			if (callback_) callback_(message_);
		}

		void onErrorInternal(const std::string& _message)override {
		}

	public:
		TMessageHandlerImple(
			MessageHandlerImpleOnSuccess on_success,
			MessageHandlerImpleOnError on_error) :
			callback_(on_success), 
			on_error_(on_error)
		{};
		virtual ~TMessageHandlerImple() = default;
	};


	class TMessanger {
	private:
		std::unique_ptr<TMessageHandler> handler_;
	public:
		TMessanger() = default;
		~TMessanger() = default;

		void SetHandler(std::unique_ptr<TMessageHandler> _handler) {
			handler_ = std::move(_handler);
		}

		void SendMessage(const TMessage& _message) {
			if (_message.message_.size() == 0) {
				//ƒGƒ‰[ˆ—
				if (handler_) {
				}
			}

			if (handler_)
				handler_->onSuccess(_message);
		}
	};

}

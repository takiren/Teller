#pragma once
#include"Core.h"

namespace Teller {
	class TMessage {
	public:
		int ID_;
		std::string message_;
		TMessage(int _id) :message_(""), ID_(_id) {};
		virtual ~TMessage() = default;
	};

	class EventMessage {
	public:
		EventMessage() = default;
		virtual ~EventMessage() = default;
		int ID_;
		std::string message_;
	};

	class TMessageHandler {
	protected:
		virtual void onSuccessInternal() = 0;
		virtual void onErrorInternal(const TMessage& _message) = 0;
	public:
		TMessageHandler() = default;
		virtual ~TMessageHandler() = default;

		void onSuccess() {
			onSuccessInternal();
		}

		void onError(const TMessage& _message) {
			onErrorInternal(_message);
		}
	};

	using MessageHandlerImpleOnSuccess = std::function<void(void)>;
	using MessageHandlerImpleOnError = std::function<void(const TMessage& _message)>;



	class TMessageHandlerImple :public TMessageHandler {
	private:

		MessageHandlerImpleOnSuccess on_success_;
		MessageHandlerImpleOnError on_error_;
	protected:
		void onSuccessInternal() override {
			if (on_success_)
				on_success_();
		}
	public:
		TMessageHandlerImple(
			MessageHandlerImpleOnSuccess on_success,
			MessageHandlerImpleOnError on_error
		) :on_success_(on_success), on_error_(on_error) {};

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
				handler_->onSuccess();
		}
	};

}

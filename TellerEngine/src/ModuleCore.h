#pragma once
#include"Core.h"
#include"TellerCore.h"

namespace Teller {
	class TellerCore;

	class ModuleCore :public std::enable_shared_from_this<ModuleCore>
	{
	private:
	protected:
		int count_;
		std::string debugLog;
		bool bUpdate; //Gameを動かすかどうか。
		bool bEnabled;
	public:
		std::weak_ptr<ModuleCore> parent; //親モジュールへのポインタ
		std::weak_ptr<TellerCore> ptrTellerCore; //コアへのポインタ
		std::vector<std::shared_ptr<ModuleCore>>children; //サブモジュールへのポインタ
		ModuleCore() :
			bUpdate(true),
			bEnabled(true),
			count_(0),
			debugLog("Nothing.") {};
		~ModuleCore() = default;

		//コピー禁止
		ModuleCore(const ModuleCore&) = delete;
		ModuleCore& operator=(const ModuleCore&) = delete;

		//ムーブは許可
		ModuleCore& operator=(ModuleCore&&) = default;

		virtual void AddChildModule(std::shared_ptr<ModuleCore> sub_module);
		virtual void Tick(); //必ずtickごとに処理される処理。
		virtual void Update(); //Gameが動いてないと処理されない。
		int GetCount() const { return count_; };
	};
}
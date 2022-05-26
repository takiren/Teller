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
		bool bUpdate; //Game�𓮂������ǂ����B
		bool bEnabled;
	public:
		std::weak_ptr<ModuleCore> parent; //�e���W���[���ւ̃|�C���^
		std::weak_ptr<TellerCore> ptrTellerCore; //�R�A�ւ̃|�C���^
		std::vector<std::shared_ptr<ModuleCore>>children; //�T�u���W���[���ւ̃|�C���^
		ModuleCore() :
			bUpdate(true),
			bEnabled(true),
			count_(0),
			debugLog("Nothing.") {};
		~ModuleCore() = default;

		//�R�s�[�֎~
		ModuleCore(const ModuleCore&) = delete;
		ModuleCore& operator=(const ModuleCore&) = delete;

		//���[�u�͋���
		ModuleCore& operator=(ModuleCore&&) = default;

		virtual void AddChildModule(std::shared_ptr<ModuleCore> sub_module);
		virtual void Tick(); //�K��tick���Ƃɏ�������鏈���B
		virtual void Update(); //Game�������ĂȂ��Ə�������Ȃ��B
		int GetCount() const { return count_; };
	};
}
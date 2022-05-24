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

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include <list>
#include "cinder/CinderImGui.h"

using namespace ci;
using namespace ci::app;

namespace Teller {
	class Core;
	template<class TREE_TYPE>
	class ModuleTemplate :public std::enable_shared_from_this<TREE_TYPE> {
	protected:
		std::weak_ptr<TREE_TYPE> parent;
		std::vector<std::shared_ptr<TREE_TYPE>> children;
	public:
		ModuleTemplate() = default;
		~ModuleTemplate() = default;
		void AddChild(std::shared_ptr<ModuleTemplate<TREE_TYPE>>&& sub_module);
	};

	struct Tint {
		int d;
	};

	class ModuleCore :public std::enable_shared_from_this<ModuleCore>
	{
	protected:
		std::weak_ptr<ModuleCore> parent;
		std::vector<std::shared_ptr<ModuleCore>>children;
		bool bUpdate; //Gameを動かすかどうか。
		bool bEnabled;
	public:
		ModuleCore();
		virtual ~ModuleCore();
		void AddChildModule(std::shared_ptr<ModuleCore >&& sub_module);
		void Tick(); //必ずtickごとに処理される処理。
		void Update(); //Gameが動いてないと処理されない。
	};


}

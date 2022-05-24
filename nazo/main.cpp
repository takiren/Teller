#pragma once
#include<vector>
#include<map>
#include<string>
#include<memory>

class Core;
template<class TREE_TYPE>
class ModuleTemplate :public std::enable_shared_from_this<TREE_TYPE> {
private:
	std::weak_ptr<TREE_TYPE> parent;
	std::vector<std::shared_ptr<TREE_TYPE>> children;
public:
	ModuleTemplate() = default;
	~ModuleTemplate() = default;
	void AddChild(std::shared_ptr<TREE_TYPE>&& sub_module);
};

struct Tint {
	int d;
};



class ModuleCore {
private:

public:
};

void testfunc() {
	ModuleTemplate<ModuleCore> fs;
}

int main() {
	auto n = std::make_shared<ModuleTemplate<ModuleCore>>();
	auto m = std::make_shared<ModuleTemplate<ModuleCore>>();
	n->AddChild(m);
	return 0;
}
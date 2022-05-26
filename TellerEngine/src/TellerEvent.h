#pragma once
#include "Core.h"

class TellerEventBase {
private:

public:
	TellerEventBase()=default;
	~TellerEventBase() = default;

	void SetHandler();
	void Call();
};

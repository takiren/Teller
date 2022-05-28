#include "pch.h"

#include"..\TellerEngine\src\TellerEvent.h"

class testm {
private:
	float fl_;
public:
	testm(): fl_(3.0f){};
	float s(float f) {
		fl_ = f;
		printf("Value %f", fl_);
		return fl_;
	}
};
TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

void throwerror() {
	testm ttt;
	Teller::TMessanger<int, float> ttest;
	ttest.AttachFunction(0, [&](float f) {return ttt.s(f); });
	ttest.SendMessage(5);
}

TEST(TMessangerTEST, Send) {
	EXPECT_ANY_THROW(throwerror());
}
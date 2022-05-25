#include"Animation.h"

using namespace Teller;

Teller::Animation::Animation(std::shared_ptr<Agent>&& agent)
{
	parent = agent;
}

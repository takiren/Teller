#ifndef _SRC_TVARIANT_H_
#define _SRC_TVARIANT_H_

#include<string>
#include<memory>
#include<map>
#include<vector>
#include<Agent.h>

#include<cinder/gl/gl.h>

namespace teller {
	using namespace ci;
	class Variant {
	private:
		union
		{
			bool _bool;
			int64_t _int;
			uint64_t _uint;
			float _float;
			double _double;
			ci::vec2 _vector2;
			std::shared_ptr<AgentCore> _agentRef;

			void* _ptr;
		}_data alignas(8);

	protected:
	public:
		enum Type
		{
			NIL,
			BOOL,
			INT,
			STRING,
			VECTOR2,

			AGENT
		};
	};
}

#endif // !_SRC_TVARIANT_H_

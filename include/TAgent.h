#ifndef _TELLER_H_TAGENT
#define _TELLER_H_TAGENT

#include<vector>
#include<map>
#include<string>
#include<filesystem>

#include<cinder/gl/gl.h>
#include<nlohmann/json.hpp>

#include<Variant.h>


namespace teller {

	using json = nlohmann::json;
	namespace fs = std::filesystem;

	class TAgentBase;
	using TAgentRef = std::shared_ptr<TAgentBase>;

	class TAgentBase {
	private:
	protected:
		vec2 position_;
		vec2 rotation_;
		vec2 scale_;

		std::weak_ptr<TAgentBase> ownerTAgent;

	public:
		virtual void Update();
		virtual void Draw();
		virtual void SetUp();

		inline void SetOwner(TAgentRef _owner) { ownerTAgent = _owner; };

		virtual void GetVariant(Variant _variant);

		virtual void SendVariant(Variant _variant);

		virtual void SendData(json _jdata);

		virtual void PushData(json data);
	};

}

#endif // !_TELLER_H_TAGENT

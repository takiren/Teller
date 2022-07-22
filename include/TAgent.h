#ifndef _TELLER_H_TAGENT
#define _TELLER_H_TAGENT

#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<filesystem>

#include<cinder/gl/gl.h>
#include<nlohmann/json.hpp>

#include<Variant.h>
#include<TAlias.h>


namespace teller {

	using json = nlohmann::json;
	namespace fs = std::filesystem;

	class TAgentBase;
	using TAgentRef = std::shared_ptr<TAgentBase>;

	class TAgentBase {
	private:
	protected:

		vec2 position_;
		float rotation_;
		vec2 scale_;

		std::weak_ptr<TAgentBase> ownerTAgent;

	public:

		std::string uniqueName_;

		TAgentBase(const TAgentBase&) = delete;

		TAgentBase& operator=(const TAgentBase&) = delete;
		TAgentBase& operator=(TAgentBase&&) = default;

		bool operator==(const TAgentBase& _tname) { return uniqueName_ == _tname.uniqueName_; }
		bool operator!=(const TAgentBase& _tname) { return uniqueName_ != _tname.uniqueName_; }

		bool operator==(const std::string& _tname) { return uniqueName_ == _tname; }
		bool operator!=(const std::string& _tname) { return uniqueName_ != _tname; }


		TAgentBase() :
			uniqueName_(""),
			position_(vec2(0, 0)),
			rotation_(0.f),
			scale_(vec2(1, 1))
		{};

		TAgentBase(std::string _name, vec2 _position, float _rotation, vec2 _scale) :
			uniqueName_(_name),
			position_(_position),
			rotation_(_rotation),
			scale_(_scale)
		{};

		virtual ~TAgentBase() = default;

		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void SetUp() = 0;


		virtual void GetVariant(Variant _variant);

		virtual void SendVariant(Variant _variant);

		virtual void SendData(json _jdata);

		virtual void PushData(json data);

		inline void SetOwner(TAgentRef _owner) { ownerTAgent = _owner; };

		virtual void SetPosition(vec2 _position) { position_ = _position; }
		virtual void SetRotation(float _rotation) { rotation_ = _rotation; }
		virtual void SetScale(vec2 _scale) { scale_ = _scale; }

	};

	class TSprite :public TAgentBase {
	private:
	protected:

	public:
		TSprite() = default;
		virtual ~TSprite() = default;
	};

	class TCharacter final :public TAgentBase {
	private:
		std::string currentSprite_;
		HashMap<std::string, std::unique_ptr<Sprite>> charSpritesMap_;

		std::string displayName_;

		void LoadCharacterImage(fs::path _path, std::string _characterName);

		void LoadSprite(fs::path _path);

	protected:
	public:
		TCharacter() = delete;
		TCharacter(fs::path _path, std::string _charcterName, vec2 _position, float _rotation, vec2 _scale) :
			TAgentBase(_charcterName, _position, _rotation, _scale)
		{
			LoadCharacterImage(_path, _charcterName);
		};

		~TCharacter() = default;

		void PushData(json jdata) override;

		void Update()override;
		void SetUp()override;
		void Draw()override;
	};

}

#endif // !_TELLER_H_TAGENT

#pragma once
#include<string>
#include<filesystem>
#include<cppglob/glob.hpp>
#include"cinder/Cinder.h"
#include "cinder/app/App.h"
#include"cinder/gl/gl.h"
#include"cinder/CinderImGui.h"

#include"Asset.h"
#include"Episode.h"
#include"nlohmann/json.hpp"

namespace teller {
	using namespace ci;
	using namespace ci::app;
	namespace fs = std::filesystem;
	using json = nlohmann::json;

	enum class CHARACTER_ITEM
	{
		HAIR,
		BODY,
		CLOTHS,
		EYE,
		MOUTH,
		EYEBROW,
		EFFECT
	};

	class AgentCore {
	private:
	protected:
		float deltaTime_;
		vec2 position_; //画面上の位置
		vec2 rotation_;
		vec2 scale_;
		vec2 size_; //このエージェントの占める画面上の大きさ(ピクセル数)
		std::weak_ptr<AgentCore> parentAgent;
		void SetParent(std::shared_ptr<AgentCore> _parent) { parentAgent = _parent; };

	public:
		AgentCore() :
			deltaTime_(0.02f),
			size_(vec2(1)),
			position_(vec2(0, 0)),
			scale_(vec2(1, 1)),
			rotation_(vec2(0, 0))
		{};

		AgentCore(ci::vec2 _position, ci::vec2 _scale, ci::vec2 _rotation) :
			deltaTime_(0.02f),
			position_(_position),
			scale_(_scale),
			rotation_(_rotation),
			size_(vec2(1))
		{};
		virtual ~AgentCore() = default;

		virtual void Tick();
		virtual void UpdateInternal();
		virtual void Update();
		virtual void Draw();

		virtual void SetDeltaTime(float _deltaTime);

		vec2 GetPosition()const { return position_; };
		void SetPosition(vec2 _pos) { position_ = _pos; };

		vec2 GetRotation()const { return rotation_; };
		void SetRotation(vec2 _rot) { rotation_ = _rot; };

		vec2 GetScale()const { return scale_; };
		void SetScale(vec2 _scale) { scale_ = _scale; };

	};

	template<class... Args>
	class Agent :public AgentCore
	{
	protected:
		std::string name_;
	public:
		Agent() :AgentCore()
		{};

		Agent(ci::vec2 _position, ci::vec2 _scale, ci::vec2 _rotation) :
			AgentCore(_position, _scale, _rotation),
			name_("")
		{};

		Agent(std::string _name, ci::vec2 _position, ci::vec2 _scale, ci::vec2 _rotation) :
			AgentCore(_position, _scale, _rotation),
			name_(_name)
		{};

		Agent(const Agent&) = delete;
		Agent& operator=(const Agent&) = delete;

		Agent& operator=(Agent&&) = default;


		/*virtual void AttachAnimator(std::shared_ptr<Animator>&& _animator,int key);
		virtual void AnimateInternal(int key, float factor);
		virtual void Move();*/

		virtual void CallBackListener(Args... args);

		std::string GetName()const { return name_; };

		//virtual void AddAnimator(int key,std::shared_ptr<Animator> _animator);
	};

	template<class ...Args>
	inline void Agent<Args...>::CallBackListener(Args ...)
	{
	}

	class CharacterItem :Agent<vec2> {
	private:
		std::weak_ptr<Sprite> sprite_;
	protected:
	public:
		void SetSprite(std::shared_ptr<Sprite> _sprite) { sprite_ = _sprite; };
		void Draw() override;
		void SetPosition(vec2 _position) { position_ = _position; };
	};

	struct CharacterAppearance {
		std::map<CHARACTER_ITEM, std::shared_ptr<CharacterItem>> cApperance;
		CharacterAppearance() = default;
		void SetItem(CHARACTER_ITEM _enumitem, fs::path _path);
		void SetItem(CHARACTER_ITEM _enumitem, CharacterItem _item);
		void SetPosition(vec2 _position);
	};

	struct CharacterSet {
		json j;
	};


	class CharacterSimple :public Agent<vec2, vec2, vec2, std::string> {
	private:
		std::unordered_map < std::string, std::unique_ptr<Sprite>> charSprites_;
		std::string currentSprite;
		std::string rubi_;
		void Initialize(fs::path _path, std::string _charactername);

		void LoadSprite(fs::path _path);
		void SetCurrentSprite(std::string _key) { currentSprite = _key; };
	public:
		CharacterSimple() :
			Agent(),
			currentSprite("")
		{};

		CharacterSimple(vec2 _position, vec2 _scale, vec2 _rotation, fs::path _path, std::string _charactername) :
			Agent(_charactername, _position, _scale, _rotation),
			currentSprite("")
		{
			Initialize(_path, _charactername);
		};

		void Tick() override;
		std::vector<std::string> GetKeys();

		void CallBackListener(vec2 _pos, vec2 _rot, vec2 _scale, std::string _key) override;
	};


	class Character :public Agent<vec2, vec2, vec2, CharacterAppearance> {
	private:
		std::weak_ptr<Sprite> sprite_;
		//std::unordered_map<CHARACTER_ITEM, std::unordered_map<std::string, std::shared_ptr<CharacterItem>>> items_;
		CharacterAppearance mappear_;

		std::unordered_map<std::string, std::unique_ptr<CharacterAppearance>> appearanceMap_;

		std::unordered_map<std::string, std::weak_ptr<Sprite>> charSprites_;
	public:
		Character(CharacterAppearance _appear) :
			Agent(),
			mappear_(_appear)
		{};
		void SetCharacterSprite(std::string _key, std::shared_ptr<Sprite> _sprite);
		void Tick() override;
		void AddCharacterAppearance(std::string _key, std::unique_ptr<CharacterAppearance> _appearance);
		void CallBackListener(vec2 _pos, vec2 _rot, vec2 _scale, CharacterAppearance _apperance) override;
	};

	class Kappa :public Agent<vec2> {
	private:
		std::unique_ptr<Sprite> sprite_;
	public:
		Kappa() = delete;
		Kappa(fs::path _path) :
			sprite_(std::make_unique<Sprite>(_path))
		{
			position_ = vec2(700, 300);
		};

		void Tick() override;
		void CallBackListener(vec2 _pos) override;
	};

	class RectAgent :public Agent<vec2, vec2, vec2> {
	private:
	public:
		void Tick() override;
		void CallBackListener(vec2 _pos, vec2 _rot, vec2 _scale) override;
	};

	class MainTextArea :public Agent<std::string, std::string> {
	private:
		std::string speaker_;
		std::string text_;
	public:
		MainTextArea() :Agent(), text_(""), speaker_("") {};
		void Tick() override;

		void CallBackListener(std::string _speaker, std::string _text);
	};

	class BackGroundImage :public Agent<> {
	private:
		std::unordered_map<std::string, std::unique_ptr<Sprite>> backGroundImagesMap;
	public:
		BackGroundImage() :Agent(){};
		~BackGroundImage() = default;
		void Tick() override;
	};

	using AgentRef = std::shared_ptr<AgentCore>;

}


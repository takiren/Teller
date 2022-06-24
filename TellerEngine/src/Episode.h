#pragma once
#include<list>
#include<vector>
#include<string>
#include<filesystem>
#include<map>
#include<iostream>
#include<stdio.h>
#include<istream>
#include<fstream>
#include<sstream>
#include<nlohmann/json.hpp>
#include<cinder/Cinder.h>
#include<cinder/gl/gl.h>
#include"utility.h"


namespace teller {
	using json = nlohmann::json;
	namespace fs = std::filesystem;


	enum class EPISODE_EVENT_TYPE
	{
		BRANCH,
		ANIMATION,
		CHARACTER_IN,
		CHARACTER_OUT,
		CHANGE_SCENE,
		CHANGE_BG,
		CHANGE_CHARACTER_APPERANCE
	};

	/*
	CSV�t�@�C����ǂݍ��ރN���X	*/
	class CSVLoader {
		std::string PREFIX_EPISODE = "E";
		std::map<int, std::vector<std::string>> csv_data;
		std::string ReadToString(const std::string path);
		std::vector<int> GetEpisodeList();
	public:
		uint64_t ID_;
		std::string name_;
		std::string path_;
		CSVLoader() = delete;
		CSVLoader(std::string input) :
			CSVLoader(input, ',')
		{}; //�f�t�H���g�Ńf���~�^��','�ɂ���B
		CSVLoader(fs::path _path) :CSVLoader(_path.string(), ',') {};

		CSVLoader(std::string input, char delimiter);
		std::vector<std::string> GetLine(int _line) { return csv_data.at(_line); };
		std::map<int, std::vector<std::string>> GetCSVData() { return csv_data; };
	};

	class Episode {
	public:
		std::string path_;
		std::string name_;
		std::string title;
		fs::path directory_;
		fs::path filename_;
		uint64_t ID_;
		std::map<int, std::vector<std::string>> data;
		std::map<int, uint64_t> events_;
		std::vector<uint64_t> nextCandidate;

		Episode() = delete;

		// �G�s�\�[�h�t�@�C����ǂݍ��ށB

		// �G�s�\�[�h�t�@�C����ǂݍ��ށB�񐄏�
		Episode(std::string _path) :
			title(_path),
			ID_(-1),
			data(CSVLoader(_path).GetCSVData()),
			path_(_path)
		{
			auto filename = fs::path(path_);
			filename = filename.filename();
			filename_ = filename;
			auto jsonEpisode = filename.stem();
			jsonEpisode += fs::path(".json");
			fs::path fspath_ = fs::current_path();
			fspath_ = fspath_.parent_path();
			fspath_ = fspath_.parent_path();
			fspath_ /= fs::path("data\\episodes");
			fspath_ /= jsonEpisode;

			//TODO:���� asap
			// �C�x���g�f�[�^�����݂��邩�m�F�B
			if (fs::directory_entry(fspath_).exists()) {
				//���݂����ꍇ�̏����B

			}
			else {
				// ���݂��Ȃ������ꍇ�̏����B
			}

		};

		//�G�s�\�[�h�t�@�C����ǂݍ���
		Episode(fs::path _path) {
			auto csv = CSVLoader(_path);
			data = csv.GetCSVData();
		};


		~Episode() = default;
		void SetNumber(int episodeNumber);
	};

	template<class TYPE>
	class TSequencer {
	private:
		std::unique_ptr<TYPE> target;
		std::map<std::string, TYPE> elements;

	public:
	};

	class StorySequenceElement {
		uint64_t myEpisodeID_;
		std::string episodeFilePath_;
		uint64_t previousEpisodeID_;
		uint64_t nextEpisodeID_;
	public:
		StorySequenceElement() = delete;
		StorySequenceElement(std::string _path);
		StorySequenceElement(uint64_t mEp, uint64_t prevEP, uint64_t nextEP, std::string _path) :
			previousEpisodeID_(prevEP),
			nextEpisodeID_(nextEP),
			myEpisodeID_(mEp),
			episodeFilePath_(_path)
		{};
	};
	class EpisodeSequencer;
	class StorySequencer {
		uint64_t currentEpisodeID_;
		std::unique_ptr<EpisodeSequencer> epSequencer_;
	public:
		StorySequencer() = default;
		StorySequencer(std::string _path);
		StorySequencer(uint64_t _id, int _line);

		void Update();
		void Load();
		void NextEpisode(uint64_t _id);
		void NextEpisode();
	};

	enum class TDataType
	{
		DataInt,
		DataVec2,
		DataFloat,
		DataString,
		COMBO
	};

	//Event�p�̃f�[�^
	using TEData = std::string;

	class EventDataCore {
	protected:
		TDataType dataType_;
		TEData data_;

	public:
		EventDataCore(TDataType _type) :
			dataType_(_type) {};

		EventDataCore(TDataType _type, TEData _data) :
			dataType_(_type),
			data_(_data) {};
		virtual ~EventDataCore() = default;

		TDataType GetDataType()const { return dataType_; };
		TEData GetDump()const { return data_; };

		template<typename T>
		inline T GetData()
		{
			return data_;
		}

		template<typename T>
		inline void SetData(T _data)
		{
			return T();
		}
	};

	template<>
	inline void EventDataCore::SetData<std::string>(std::string _data)
	{
		data_ = _data;
	}

	template<>
	inline void EventDataCore::SetData<ci::vec2>(ci::vec2 _data)
	{
		std::stringstream ss;
		ss << _data[0] << "," << _data[1];
		data_ = ss.str();
	}

	template<>
	inline void EventDataCore::SetData<int>(int _data)
	{
		data_ = _data;
	}

	template<>
	inline void EventDataCore::SetData<float>(float _data)
	{
		data_ = _data;
	}

	template<>
	inline std::string EventDataCore::GetData<std::string>()
	{
		return data_;
	}

	template<>
	inline ci::vec2 EventDataCore::GetData<ci::vec2>() 
	{
		auto strvec = Utility::split(data_, ',');
		return ci::vec2( std::stof(strvec.at(0)), std::stof(strvec.at(1)) );
	}

	template<>
	inline int EventDataCore::GetData<int>() 
	{
		return std::stoi(data_);
	}

	struct EventSignature {
		std::vector<EventDataCore> data_;
		std::string name_;

		EventSignature() = default;
		virtual ~EventSignature() = default;

		void AddEventData(EventDataCore&& _data) { data_.push_back(_data); };
		void SetName(std::string _name) { name_ = _name; };
	};

	class EpisodeEvent {
		std::vector<EventDataCore> data_;

	public:
		EPISODE_EVENT_TYPE type_;
		uint64_t ID_;
		int targetLine;
		std::string target_;
		std::string key_;
		std::string description_;

		EpisodeEvent() = delete;
		EpisodeEvent(EPISODE_EVENT_TYPE _type, int _line, std::string _key) :
			type_(_type),
			targetLine(_line),
			key_(_key),
			ID_(-1),
			target_("")
		{};

		EpisodeEvent(EPISODE_EVENT_TYPE _type, int _line, std::string _target, std::string _key) :
			type_(_type),
			targetLine(_line),
			target_(""),
			key_(_key),
			ID_(-1)
		{};
	};


	class EpisodeEventIn :public EpisodeEvent {
	private:
	public:
		EpisodeEventIn(std::string _target, std::string _key) :
			EpisodeEvent(EPISODE_EVENT_TYPE::CHARACTER_IN, 0, _key)
		{};
		~EpisodeEventIn() = default;

		std::vector<TDataType> GetStructure();
	};

	class EpisodeEventOut :public EpisodeEvent {
	};

	class EpisodeEventManager {
	private:
		//key=int�͍s��\���B
		//ex) key=6 6�s�ڂ̃C�x���g�B
		std::map<int, std::unique_ptr<EpisodeEvent>> eventRefs;
	public:
		uint64_t ID_;
		EpisodeEventManager() = delete;
		EpisodeEventManager(fs::path _path);

	};

	class EpisodeSequencer {
		std::weak_ptr<Episode> episode_;
		std::unordered_map<int, EpisodeEvent> eventsMap_;
		void LoadEvents();
	public:
		EpisodeSequencer() = delete;
		EpisodeSequencer(uint64_t _id);
		EpisodeSequencer(fs::path _path);
		EpisodeSequencer(std::shared_ptr<Episode> _episode) :
			episode_(_episode) {};
	};

	class EventSequencer {
		std::unordered_map<int, EpisodeEvent> eventsMap_;
	public:
		EventSequencer() = delete;
		EventSequencer(std::string _path);
	};

}
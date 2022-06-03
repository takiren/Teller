#pragma once
#include<list>
#include<vector>
#include<string>
#include<map>
#include<istream>
#include<fstream>
#include<sstream>
#include<nlohmann/json.hpp>

namespace Teller {
	using json = nlohmann::json;

	enum class EPISODE_EVENT_TYPE
	{
		BRANCH,
		ANIMATION,
		CHARACTER_IN,
		CHARACTER_OUT,
		CHANGE_SCENE,
		CHANGE_BG
	};
	/*
	CSV�t�@�C����ǂݍ��ރN���X	*/
	class Episode;
	class CSVLoader {
		std::string PREFIX_EPISODE = "E";
		std::map<int, std::vector<std::string>> csv_data;
		std::string ReadToString(const std::string path);
		std::vector<int> GetEpisodeList();
	public:
		uint64_t ID_;
		std::string path_;
		CSVLoader() = delete;
		CSVLoader(std::string input) :
			CSVLoader(input, ',')
		{}; //�f�t�H���g�Ńf���~�^��','�ɂ���B
		CSVLoader(std::string input, char delimiter);
		std::vector<Episode> GetEpisodes();
		std::vector<std::string> GetLine(int _line) { return csv_data.at(_line); };
		std::map<int, std::vector<std::string>> GetCSVData() { return csv_data; };
	};

	class Episode {
	public:
		std::string path_;
		std::string title;
		uint64_t eventID_;
		uint64_t ID_;
		std::map<int, std::vector<std::string>> data;

		std::map<int, uint64_t> events_;

		std::vector<uint64_t> nextCandidate;

		Episode() = delete;

		// �G�s�\�[�h��V�K�쐬 with name
		Episode(std::string _name, std::map<int, std::vector<std::string>> _data) :
			title(_name),
			data(_data),
			ID_((uint64_t)this),
			path_(_name + ".csv"),
			eventID_(-1)
		{};

		// �G�s�\�[�h�t�@�C����ǂݍ��ށB
		Episode(uint64_t _ID);

		// �G�s�\�[�h�t�@�C����ǂݍ��ށB
		Episode(std::string _path) :
			title(""),
			ID_((uint64_t)this),
			eventID_(-1),
			data(CSVLoader(_path).GetCSVData()),
			path_(_path)
		{};

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

	class StorySequencer {
		uint64_t currentEpisodeID_;
	public:
		StorySequencer() = default;
		StorySequencer(std::string _path);
		StorySequencer(uint64_t _id, int _line);

		void Update();
		void Load();
		void NextEpisode(uint64_t _id);
		void NextEpisode();
	};

	class EpisodeEvent {
	public:
		EPISODE_EVENT_TYPE type_;
		uint64_t ID_;
		
		EpisodeEvent() = delete;
		EpisodeEvent(uint64_t _id, EPISODE_EVENT_TYPE _type):
			ID_(_id),
			type_(_type)
		{};
		EpisodeEvent(EPISODE_EVENT_TYPE _type) :
			ID_((uint64_t)this),
			type_(_type)
		{};
	};

	class EpisodeEventManager {
	public:
		uint64_t ID_;
		EpisodeEventManager() = delete;
	};

	class EpisodeSequencer {
		std::unique_ptr<Episode> episode_;
	public:
		EpisodeSequencer() = delete;
		EpisodeSequencer(uint64_t _id);
	};

	class EventSequencer {
	public:
		EventSequencer() = delete;
		EventSequencer(std::string _path);
	};

}
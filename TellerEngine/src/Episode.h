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
	/*
	CSVファイルを読み込むクラス	*/

	class Episode;
	class CSVLoader {
		std::string PREFIX_EPISODE = "E";
		std::map<int, std::vector<std::string>> csv_data;
		std::string ReadToString(const std::string path);
		std::vector<int> GetEpisodeList();
	public:
		uint64_t ID_;
		CSVLoader() = delete;
		CSVLoader(std::string input) :
			CSVLoader(input, ',')
		{}; //デフォルトでデリミタを','にする。
		CSVLoader(std::string input, char delimiter);
		std::vector<Episode> GetEpisodes();
		std::vector<std::string> GetLine(int _line) { return csv_data.at(_line); };
		std::map<int, std::vector<std::string>> GetCSVData() { return csv_data; };
	};

	class Episode {
	private:
		std::string path_;
	public:
		std::string title;
		uint64_t eventID_;
		uint64_t ID_;
		std::map<int, std::vector<std::string>> data;
		Episode() = delete;
		Episode(std::string _name, std::map<int, std::vector<std::string>> _data) :
			title(_name),
			data(_data),
			ID_((uint64_t)this),
			path_(_name + ".csv"),
			eventID_(-1)
		{};
		// エピソードファイルを読み込む。
		Episode(uint64_t _ID);
		Episode(std::string _path) :
			title(""),
			ID_((uint64_t)this),
			eventID_(-1),
			data(CSVLoader(_path).GetCSVData())
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
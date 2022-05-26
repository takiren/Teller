#pragma once
#include"Core.h"
#include"ContentManager.h"
#include"Asset.h"
#include<string>
#include<vector>
#include<map>
#include<memory>

namespace Teller {
	class TellerCore;
	class Episode {
	private:
		std::string title;
		int number;
		int line_begin;
		int line_end;
	public:
		std::map<int, std::vector<std::string>> data;
		Episode() :
			title("Nothing title"),
			number(0),
			line_begin(0),
			line_end(0)
		{};
		Episode(std::string titleText, int episode_num) :
			title(titleText),
			number(episode_num),
			line_begin(0),
			line_end(0)
		{};

		Episode(std::map<int, std::vector<std::string>> csv) :
			data(csv),
			title("Nothing title"),
			number(0),
			line_begin(0),
			line_end(0)
		{};

		~Episode() = default;
		void SetLineBegin(int line);
		void SetLineEnd(int line);
		void SetNumber(int episodeNumber);
	};

	class CSVLoader {
		std::string PREFIX_EPISODE = "E";
		std::map<int, std::vector<std::string>> csv_data;
		std::string ReadToString(const std::string path);
		std::vector<int> GetEpisodeList();
	public:
		CSVLoader(std::string input) :CSVLoader(input, ',') {}; //デフォルトでデリミタを','にする。
		CSVLoader(std::string input, char delimiter);
		Episode GetEpisode();
		std::vector<Episode> GetEpisodes();
		std::map<int, std::vector<std::string>> GetCSVData()const { return csv_data; };
	};

	class EpisodeManager :public ContentManager<Episode> {
	private:
		std::vector<std::shared_ptr<CSVLoader>> csv_data;
	public:
		EpisodeManager() :ContentManager() {};
		~EpisodeManager() = default;
	};

}
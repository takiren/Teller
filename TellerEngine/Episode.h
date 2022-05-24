#pragma once
#include"Core.h"

namespace Teller {
	class Episode {
	private:
		std::string title;
		int number;
		int line_begin;
		int line_end;
		std::map<int, std::vector<std::string>> data;
	public:
		Episode() :
			title("Nothing title"),
			number(0),
			line_begin(0),
			line_end(0)
		{};
		Episode(std::string titleText,int episode_num):
			title(titleText),
			number(episode_num),
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
	};

}
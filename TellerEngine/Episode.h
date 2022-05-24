#pragma once
#include"Core.h"
#include"csv.h"

namespace Teller {

	class Episode;
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

	class Episode {
	private:
		std::string title;
		int number;
		int line_end;
		int line_begin;
		std::map<int, std::vector<std::string>> data;
	public:
		void SetLineBegin(int line);
		void SetLineEnd(int line);
		void SetNumber(int episodeNumber);
		Episode(std::string titleText) :title(titleText) {};
	};
}
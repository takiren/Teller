#pragma once
#include"Core.h"
#include"Asset.h"

namespace Teller {

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

}
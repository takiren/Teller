#pragma once
#include<vector>
#include<string>
#include<map>
#include<istream>
#include<fstream>
#include<sstream>

namespace Teller {
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

	/*
	CSVファイルを読み込むクラス。
	コンストラクタ引数にCSVファイルのパスを入れる。

	*/
	class CSVLoader {
		std::string PREFIX_EPISODE = "E";
		std::map<int, std::vector<std::string>> csv_data;
		std::string ReadToString(const std::string path);
		std::vector<int> GetEpisodeList();
	public:
		CSVLoader() = delete;
		CSVLoader(std::string input) :CSVLoader(input, ',') {}; //デフォルトでデリミタを','にする。
		CSVLoader(std::string input, char delimiter);
		Episode GetEpisode();
		std::vector<Episode> GetEpisodes();
		std::vector<std::string> GetLine(int _line) { return csv_data.at(_line); };
		std::map<int, std::vector<std::string>> GetCSVData(){ return csv_data; };
	};

}
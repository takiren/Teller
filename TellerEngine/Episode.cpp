#include "Episode.h"

namespace Teller {
	void Episode::SetLineBegin(int line)
	{
		line_begin = line;
	}

	void Episode::SetLineEnd(int line)
	{
		line_end = line;
	}

	void Episode::SetNumber(int episodeNumber)
	{
	}

	/*
	ストーリー用CSVファイルフォーマット
	-------------------------------
	エピソードの区切り。先頭にE、次にエピソード名。
	-----------------------------
	E, はじまり。,description, effect, change, キャラ数, キャラA、キャラB...
	[人名],なんでやねん。下手な関西弁。,effect, change
	[_],関西弁が下手だなと思った。,感想てきな。, effect, change
	-----------------------------

	セリフなら1列目に人名、地の文なら空白。次に文章。効果
	*/

	std::string CSVLoader::ReadToString(const std::string path)
	{
		std::ifstream ifs_csv_file(path);
		if (!ifs_csv_file.is_open()) {
			return nullptr;
		}
		auto ostr = std::ostringstream{};
		ostr << ifs_csv_file.rdbuf();
		return ostr.str();
	}

	CSVLoader::CSVLoader(std::string inputCSVPath, char delimiter)
	{
		auto data = ReadToString(inputCSVPath);
		std::istringstream sstream(data);
		std::vector<std::string> items;
		std::string record;
		int counter = 0;
		while (std::getline(sstream, record))
		{
			std::istringstream line(record);
			while (std::getline(line, record, delimiter))
			{
				items.push_back(record);
			}
			csv_data[counter] = items;
			items.clear();
			counter += 1;
		}
	}

	std::vector<Episode> CSVLoader::GetEpisodes()
	{
		//コンパイラがいい感じに最適化してくれるからムーブしなくていいはず。
		std::vector<Episode> episodes;
		int episodeBegin = 0;
		int episodeEnd = 1;

		for (const auto& [key, value] : csv_data) {
			if (value.at(0) == PREFIX_EPISODE) {
				if (!episodes.empty()) {
					episodes.back().SetLineEnd(key - 1);
				}
				episodes.emplace_back();
				episodes.back().SetLineBegin(key);
			};
		}

		episodes.back().SetLineEnd(csv_data.size() + 1);

		return episodes;
	}


}

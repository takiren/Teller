#include "Episode.h"

namespace teller {

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
		path_ = path;
		std::ifstream ifs_csv_file(path);
		if (!ifs_csv_file.is_open()) {
			return nullptr;
		}
		auto ostr = std::ostringstream{};
		ostr << ifs_csv_file.rdbuf();

		return ostr.str();
	}

	std::vector<int> CSVLoader::GetEpisodeList()
	{
		return std::vector<int>();
	}

	CSVLoader::CSVLoader(std::string inputCSVPath, char delimiter)
	{
		ID_ = (uint64_t)this;
		auto data = ReadToString(inputCSVPath);
		std::istringstream sstream(data);

		//BOMスキップ
		sstream.ignore(3);

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

	StorySequenceElement::StorySequenceElement(std::string _path)
	{
		std::ifstream i(_path);
		json j;
		i >> j;
		episodeFilePath_ = _path;
		myEpisodeID_ = j["uid"];
		nextEpisodeID_ = j["nextEpID"];
		previousEpisodeID_ = j["prevEpID"];
	}

}

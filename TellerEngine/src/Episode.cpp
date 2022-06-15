#include "Episode.h"

namespace teller {

	Episode::Episode(uint64_t _ID)
	{
		std::ifstream i("EpisodeID.json");
		json j;
		i >> j;

		auto st = std::to_string(_ID);
		if (j.contains(st)) {
			ID_ = _ID;
			title = j["title"];
			path_ = j["path"];
			eventID_ = (uint64_t)j["eventID"];
			data = CSVLoader(path_).GetCSVData();
		}
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

	StorySequencer::StorySequencer(std::string _path)
	{
		std::ifstream i("sequence");
		json j;
		i >> j;

		currentEpisodeID_ = j["begin"];
	}

	StorySequencer::StorySequencer(uint64_t _id, int _line)
	{

	}

	void StorySequencer::Update()
	{
	}

	void StorySequencer::Load()
	{
	}

	void StorySequencer::NextEpisode(uint64_t _id)
	{
	}

	void StorySequencer::NextEpisode()
	{
	}

	void EpisodeSequencer::LoadEvents()
	{
		auto fp = episode_.lock()->filename_;
	}

	EpisodeSequencer::EpisodeSequencer(uint64_t _id)
	{
	}

	EpisodeSequencer::EpisodeSequencer(fs::path _path)
	{
	}

	EventSequencer::EventSequencer(std::string _path)
	{
	}

	EpisodeEventManager::EpisodeEventManager(fs::path _path)
	{
	}

}

#include "Episode.h"

namespace teller {

	void Episode::SetNumber(int episodeNumber)
	{
	}

	/*
	�X�g�[���[�pCSV�t�@�C���t�H�[�}�b�g
	-------------------------------
	�G�s�\�[�h�̋�؂�B�擪��E�A���ɃG�s�\�[�h���B
	-----------------------------
	E, �͂��܂�B,description, effect, change, �L������, �L����A�A�L����B...
	[�l��],�Ȃ�ł�˂�B����Ȋ֐��فB,effect, change
	[_],�֐��ق����肾�ȂƎv�����B,���z�Ă��ȁB, effect, change
	-----------------------------

	�Z���t�Ȃ�1��ڂɐl���A�n�̕��Ȃ�󔒁B���ɕ��́B����
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

		//BOM�X�L�b�v
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

	std::vector<TDataType> EpisodeEventIn::GetStructure()
	{
		return std::vector<TDataType>();
	}

}

#include "Episode.h"
namespace Teller {

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
		//�R���p�C�������������ɍœK�����Ă���邩�烀�[�u���Ȃ��Ă����͂��B
		std::vector<Episode> episodes;
		int episodeBegin = 0;
		int episodeEnd = 1;

		for (const auto& [key, value] : csv_data) {
			if (value.at(0) == PREFIX_EPISODE) {
				if (!episodes.empty()) {
					episodes.back().SetLineEnd(key - 1);
				}
				episodes.emplace_back(value.at(0));
				episodes.back().SetLineBegin(key);
			};
		}

		episodes.back().SetLineEnd(csv_data.size() + 1);

		return episodes;
	}

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
}

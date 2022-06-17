#pragma once
#include<vector>
#include<string>
#include<sstream>


namespace teller {
	//std::vector<std::string>‚ðstd::string‚É“WŠJ
	class Utility {
	public:
		static std::vector<std::string> split(std::string& input, char delimiter)
		{
			std::istringstream stream(input);
			std::string field;
			std::vector<std::string> result;
			while (std::getline(stream, field, delimiter)) {
				result.push_back(field);
			}
			return result;
		}
	};
}
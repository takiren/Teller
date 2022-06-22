#pragma once
#include<vector>
#include<string>
#include<sstream>
#include<random>

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

	namespace utils {
		class UIDGenerator final {
		private:
			std::mt19937_64 engine;
			std::uniform_int_distribution<uint64_t> ui64tdistr;
		public:
			UIDGenerator() :
				engine(std::random_device{}())
			{};
			~UIDGenerator() = default;
			uint64_t Generate();
		};
		inline uint64_t teller::utils::UIDGenerator::Generate() { return ui64tdistr(engine); }
	}
}
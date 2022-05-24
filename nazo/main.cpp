#pragma once
#include<vector>
#include<map>
#include<string>
#include<memory>

class Episode {
private:
	std::string title;
	int number;
	int line_begin;
	int line_end;
	std::map<int, std::vector<std::string>> data;
public:
	void SetLineBegin(int line);
	void SetLineEnd(int line);
	void SetNumber(int episodeNumber);
	Episode() :
		line_begin(0),
		line_end(0),
		number(0),
		title("")
	{};
};

int main() {
	auto s = std::make_unique<Episode>();
	return 0;
}
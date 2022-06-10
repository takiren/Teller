// ConsoleTest.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include<filesystem>
#include<istream>
#include<sstream>
#include<iostream>
#include<fstream>

#include<cppglob/glob.hpp>
#include<json.hpp>
namespace fs = std::filesystem;
using json = nlohmann::json;

class TclassP {
	std::string te;
public:
	TclassP() :
		te("テストだあああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああああ")
	{};
	~TclassP() {
		std::cout << te << std::endl;
	};
};
int main()
{
	setlocale(LC_ALL, "");

	fs::path p = fs::current_path();
	std::cout << p.string() << std::endl;
	p = p.parent_path();
	std::cout << p.string() << std::endl;
	p /= fs::path("data");
	std::cout << p.string() << std::endl;

	fs::current_path("data");
	//for (const std::filesystem::directory_entry& i : fs::directory_iterator(p)) std::cout << i.path().filename() << std::endl;
	fs::path p2 = fs::current_path();
	std::cout << p2.string() << std::endl;

	auto filename = fs::path("episode");
	filename = filename.filename();
	auto jfile = filename.stem();
	jfile += fs::path(".json");
	fs::path fspath_ = fs::current_path();
	fspath_ = fspath_.parent_path();
	fspath_ = fspath_.parent_path();
	fspath_ /= fs::path("data");
	auto jpath = fspath_;
	fspath_ /= fs::path("episodes");
	std::cout << fspath_.string() << std::endl;

	std::vector<fs::path> entries = cppglob::glob(fspath_ / fs::path("*.csv"));
	for (auto& e : entries) std::cout << e.string() << std::endl;

	jpath /= fs::path("images\\girl\\CharacterData.json");
	json j;
	std::ifstream jf(jpath.string());
	jf >> j;

	std::cout << j["name"] << std::endl;
	for (auto& e : j["file"].items())std::cout << e.value() << std::endl;
	std::cout << "湊" << std::endl;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します

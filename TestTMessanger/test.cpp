#include "pch.h"

#include<filesystem>
#include<iostream>
namespace fs = std::filesystem;


TEST(FSTEST, Empty) {
	fs::path p = fs::current_path();
	fs::current_path("../data");
	for (const std::filesystem::directory_entry& i : fs::directory_iterator(".")) std::cout << i.path().filename() << std::endl;
}
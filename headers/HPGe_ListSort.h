#pragma once
#include <filesystem>
#include <regex>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <limits>

void GenerateListFiles(std::filesystem::path);
void FindPositionOfGammaFlash();
void WriteListSortToAnalysisFile();

std::vector<int> GetRunFilesToInclude(std::filesystem::directory_entry);

std::vector<std::vector<int>> GetLogFileTable(std::fstream*);

std::vector<int> RunFilesToInclude(std::vector<std::vector<int>>);

void WriteListFiles(std::vector<int>, std::filesystem::directory_entry);

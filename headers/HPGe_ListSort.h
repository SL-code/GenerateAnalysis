#pragma once
#include <filesystem>
#include <regex>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <limits>

#define NUMBER_OF_DIGITS 3
#define MAX_NUMBER_TOF_CHANNELS 11999




void GenerateListFiles(std::filesystem::path);
void FindPositionOfGammaFlash();
void WriteListSortToAnalysisFile(std::filesystem::path);

std::vector<int> GetRunFilesToInclude(std::string);

std::vector<std::vector<int>> GetLogFileTable(std::fstream*);

std::vector<int> RunFilesToInclude(std::vector<std::vector<int>>);

void WriteListFiles(std::vector<int>, std::filesystem::directory_entry);
void WriteListSortLine(std::fstream&, std::string, std::filesystem::path, std::vector<int>);

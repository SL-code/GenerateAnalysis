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

std::vector<int> GetRunFilesFromLogFile(std::filesystem::directory_entry);

int GetLengthOfTable(std::fstream*);
std::vector<std::vector<std::string>> GetLogFileTable(std::fstream*, int);

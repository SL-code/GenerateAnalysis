#pragma once
#include <filesystem>
#include <algorithm>

#include "HPGe_ListSort.h"



class HPGe
{
    std::filesystem::path m_AnalysisDirectory;


public:
    HPGe();
    HPGe(std::filesystem::path);
    void ListSort();
    void AddMatrix();
    void DoHist();
    void TimeProjection();
    void Yield();

};

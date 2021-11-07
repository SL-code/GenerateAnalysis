#pragma once
#include <filesystem>
#include <algorithm>

#include "HPGe_ListSort.h"
#include "HPGe_AddMatrix.h"



class HPGe
{
    std::filesystem::path m_AnalysisDirectory;
    int m_NumberOfTOFChannels;

public:
    HPGe();
    HPGe(std::filesystem::path, int);
    void ListSort();
    void AddMatrix();
    void DoHist();
    void TimeProjection();
    void Yield();

};

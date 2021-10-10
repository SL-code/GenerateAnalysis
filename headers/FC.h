#pragma once
#include <filesystem>


class FissionChamber
{
    std::filesystem::path m_AnalysisDirectory;


public:
    FissionChamber();
    FissionChamber(std::filesystem::path);
    void ListSort();
    void AddMatrix();
    void DoHist();
    void TimeProjection();
    void Yield();
};

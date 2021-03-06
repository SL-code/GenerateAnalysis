#pragma once
#include <filesystem>
#include <string>
#include <threads.h>
#include <unordered_map>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <random>

#include "HPGe.h"
#include "FC.h"

class Analysis: public HPGe, public FissionChamber
{
    HPGe m_HPGe;
    FissionChamber m_FC;
    std::filesystem::path m_AnalysisDirectory;
    int m_NumberOfTOFChannels;

private:
    void GenerateProperFileStructure();
    void CheckForPreviousAnalysis();

public:
    Analysis(std::string&, int);

    void DetectorsAnalysis();

    void FissionChamberAnalysis();

    void VerifyYield();

    void Process();

    void GermaniumEfficiency();
};

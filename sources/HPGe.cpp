#include <HPGe.h>


HPGe::HPGe() = default;

HPGe::HPGe(std::filesystem::path AnalysisDirectory): m_AnalysisDirectory{AnalysisDirectory}
{

}

void HPGe::ListSort()
{
    // These are the directories containing the Detector data
    const std::array<std::filesystem::path, 3> HPGeData =
    {   m_AnalysisDirectory/"Dig1",
        m_AnalysisDirectory/"Dig2",
        m_AnalysisDirectory/"Dig3"
    };

    // TODO: Check if the data directories contain the same weeks of data

    std::for_each(HPGeData.begin(),
                  HPGeData.end(),
                  GenerateListFiles);

    FindPositionOfGammaFlash();
    WriteListSortToAnalysisFile();


}

void HPGe::AddMatrix()
{

}

void HPGe::DoHist()
{

}

void HPGe::TimeProjection()
{

}

void HPGe::Yield()
{

}

#include <HPGe.h>


HPGe::HPGe() = default;

HPGe::HPGe(std::filesystem::path AnalysisDirectory, int NumberOfTOFChannels): m_AnalysisDirectory{AnalysisDirectory}, m_NumberOfTOFChannels{NumberOfTOFChannels}
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

    std::for_each(HPGeData.begin(),
                  HPGeData.end(),
                  WriteListSortToAnalysisFile);



}

void HPGe::AddMatrix()
{
    const std::array<std::filesystem::path, 3> HPGeData =
    {   m_AnalysisDirectory/"Dig1",
        m_AnalysisDirectory/"Dig2",
        m_AnalysisDirectory/"Dig3"
    };

    std::for_each(HPGeData.begin(),
                  HPGeData.end(),
                  GenerateInputFilesForAddMatrix);

    std::fstream AnalysisFile;
    AnalysisFile.open(m_AnalysisDirectory/"analysis.sh", std::fstream::app);
    AnalysisFile << "\n\n\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig1/Sort/inp_mat_ch1 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det9.mat\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig1/Sort/inp_mat_ch2 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det1.mat\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig1/Sort/inp_mat_ch3 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det11.mat\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig1/Sort/inp_mat_ch4 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det3.mat\n";
    AnalysisFile << "\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig2/Sort/inp_mat_ch1 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det10.mat\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig2/Sort/inp_mat_ch2 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det2.mat\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig2/Sort/inp_mat_ch3 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det12.mat\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig2/Sort/inp_mat_ch4 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det4.mat\n";
    AnalysisFile << "\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig3/Sort/inp_mat_ch1 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det5.mat\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig3/Sort/inp_mat_ch2 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det6.mat\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig3/Sort/inp_mat_ch3 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det7.mat\n";
    AnalysisFile << "./bin/addMatrixDigit ./Dig3/Sort/inp_mat_ch4 " << m_NumberOfTOFChannels << " ChannelToMoveTo ./Sort/Det8.mat\n";
    AnalysisFile.close();
}

void HPGe::DoHist()
{
    std::fstream AnalysisFile;
    AnalysisFile.open(m_AnalysisDirectory/"analysis.sh", std::fstream::app);
    AnalysisFile << "\n\n\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det9.mat  " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det9.mat\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det1.cat  " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det1.mat\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det11.mat " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det11.mat\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det3.mat  " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det3.mat\n";
    AnalysisFile << "\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det10.mat " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det10.mat\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det2.mat  " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det2.mat\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det12.mat " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det12.mat\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det4.mat  " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det4.mat\n";
    AnalysisFile << "\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det5.mat  " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det5.mat\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det6.mat  " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det6.mat\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det7.mat  " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det7.mat\n";
    AnalysisFile << "./bin/dohistDigit ./Sort/Det8.mat  " << m_NumberOfTOFChannels << " 324 " << m_NumberOfTOFChannels - 1 <<  " ./Sort/Amplitude/Det8.mat\n";
    AnalysisFile.close();

}

void HPGe::TimeProjection()
{

    std::fstream AnalysisFile;
    AnalysisFile.open(m_AnalysisDirectory/"analysis.sh", std::fstream::app);
    AnalysisFile << "\n\n\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det9.mat  " << m_NumberOfTOFChannels << " ./Sort/Time/Det9.mat\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det1.cat  " << m_NumberOfTOFChannels << " ./Sort/Time/Det1.mat\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det11.mat " << m_NumberOfTOFChannels << " ./Sort/Time/Det11.mat\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det3.mat  " << m_NumberOfTOFChannels << " ./Sort/Time/Det3.mat\n";
    AnalysisFile << "\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det10.mat " << m_NumberOfTOFChannels << " ./Sort/Time/Det10.mat\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det2.mat  " << m_NumberOfTOFChannels << " ./Sort/Time/Det2.mat\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det12.mat " << m_NumberOfTOFChannels << " ./Sort/Time/Det12.mat\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det4.mat  " << m_NumberOfTOFChannels << " ./Sort/Time/Det4.mat\n";
    AnalysisFile << "\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det5.mat  " << m_NumberOfTOFChannels << " ./Sort/Time/Det5.mat\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det6.mat  " << m_NumberOfTOFChannels << " ./Sort/Time/Det6.mat\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det7.mat  " << m_NumberOfTOFChannels << " ./Sort/Time/Det7.mat\n";
    AnalysisFile << "./bin/projectMatrixDigit ./Sort/Det8.mat  " << m_NumberOfTOFChannels << " ./Sort/Time/Det8.mat\n";
    AnalysisFile.close();


}

void HPGe::Yield()
{
    std::fstream AnalysisFile;
    AnalysisFile.open(m_AnalysisDirectory/"analysis.sh", std::fstream::app);
    AnalysisFile << "\n\n\n";
    AnalysisFile << "./bin/createGeYieldDigit .  1  150 1 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile << "./bin/createGeYieldDigit .  2  150 2 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile << "./bin/createGeYieldDigit .  3  150 3 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile << "./bin/createGeYieldDigit .  4  150 4 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile << "./bin/createGeYieldDigit .  5  125 1 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile << "./bin/createGeYieldDigit .  6  125 2 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile << "./bin/createGeYieldDigit .  7  125 3 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile << "./bin/createGeYieldDigit .  8  125 4 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile << "./bin/createGeYieldDigit .  9  110 1 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile << "./bin/createGeYieldDigit .  10 110 2 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile << "./bin/createGeYieldDigit .  11 110 3 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile << "./bin/createGeYieldDigit .  12 110 4 " << m_NumberOfTOFChannels << "\n";
    AnalysisFile.close();


}

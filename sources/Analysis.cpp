#include "Analysis.h"





Analysis::Analysis(std::string& Path, int NumberOfTOFChannels):m_AnalysisDirectory{Path}, m_NumberOfTOFChannels{NumberOfTOFChannels}
{
    GenerateProperFileStructure();
    m_HPGe = HPGe{m_AnalysisDirectory};
    m_FC = FissionChamber{m_AnalysisDirectory};
}


void Analysis::DetectorsAnalysis()
{
    m_HPGe.ListSort();
    m_HPGe.AddMatrix();
    m_HPGe.DoHist();
    m_HPGe.TimeProjection();
    m_HPGe.Yield();
}

void Analysis::FissionChamberAnalysis()
{
    m_FC.ListSort();
    m_FC.AddMatrix();
    m_FC.DoHist();
    m_FC.TimeProjection();
    m_FC.Yield();
}



void Analysis::GenerateProperFileStructure()
{
    // At first the analysis directory should contain the data to be analysed.
    // This data will be found in the Dig1, Dig2, Dig3 and FC directories.
    const std::array<std::filesystem::path, 4> ProperAnalysisDirectory =
    {   m_AnalysisDirectory/"Dig1",
        m_AnalysisDirectory/"Dig2",
        m_AnalysisDirectory/"Dig3",
        m_AnalysisDirectory/"FC"
    };

    bool isProperAnalysisDirectory = false;

    // If one of the Dig1. Dig2, Dig3, FC directories are not found in path we stop the processing.
    std::vector<std::filesystem::path> AnalysisDirectoryContents;
    for(auto& dir_entry: std::filesystem::directory_iterator(m_AnalysisDirectory))
    {
        AnalysisDirectoryContents.push_back(dir_entry);
    }

    for(auto const& ProperEntry: ProperAnalysisDirectory)
    {
        isProperAnalysisDirectory = std::any_of(AnalysisDirectoryContents.begin(),
                                                AnalysisDirectoryContents.end(),
                                                [&ProperEntry](auto& DirectoryEntry){return DirectoryEntry == ProperEntry;});

        if(!isProperAnalysisDirectory)
        {
            std::cout << "The selected path: " << m_AnalysisDirectory << " does not seem to be a proper analysis directory.\n";
            std::cout << "The path shoud contain the Dig1, Dig2, Dig3, FC directories.\n";
            exit(EXIT_FAILURE);
        }
    }

    //Generate the Dig1/Sort/<weeks>, Dig2/Sort/<weeks>, Dig3/Sort/<weeks>, FC/Sort/<weeks>
    std::for_each(ProperAnalysisDirectory.begin(),
                  ProperAnalysisDirectory.end(),
                  [](auto& Directory){ // If there is a no Sort directory in the data folders
                                       // generate it along with the Sort/<week> directories
                                       // that will be used for analysis.
                      // If there is a Dig*/Sort/ folder already skip generationg it
                      if(!std::filesystem::exists(Directory/"Sort"))
                      {
                          // Read all the data folder names.
                          std::vector<std::filesystem::path> DataWeeks;
                          for(auto& DataDirectory: std::filesystem::directory_iterator(Directory))
                              DataWeeks.push_back(DataDirectory);

                          // Genreate the Sort/DataFolder for each data run.
                          std::filesystem::create_directory(Directory/"Sort");
                          std::for_each(DataWeeks.begin(),
                                        DataWeeks.end(),
                                        [&Directory](auto& Week)
                                        {std::filesystem::create_directory(Directory/"Sort"/Week.filename());});
                    }
                 });


    // These directories will be used in the analysis of the data.
    const std::vector<std::string> DirectoriesToGenerate = {
        "AngDistr",
        "dSdO",
        "Fit",
        "Gpxs",
        "Input",
        "Levels",
        "Msc",
        "Test",
        "Sort",
        "Yield"
    };

    // Generate all directories that should be generated.
    auto AnalysisDirectory = m_AnalysisDirectory;
    std::for_each(DirectoriesToGenerate.begin(),
                  DirectoriesToGenerate.end(),
                  [&AnalysisDirectory](auto& dir){std::filesystem::create_directory(AnalysisDirectory/dir);});
}




void Analysis::VerifyYield()
{

}

void Analysis::Process()
{

}

void Analysis::GermaniumEfficiency()
{

}




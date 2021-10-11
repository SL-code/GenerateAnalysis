#include "HPGe_ListSort.h"


void GenerateListFiles(std::filesystem::path DataDirectory)
{
    // The DataDirectory contains directories with detector data
    // each of these directories contain a log file, amplitude spectra
    // time spectra and raw data.
    //
    // The counting rate in the log file will be used to choose the
    // files that will be analised.

    std::regex logFile ("(.*)_logFile_(.*)");
    // Iterate over all data weeks
    for(auto& DataWeek: std::filesystem::directory_iterator(DataDirectory))
    {
        // Skip the Sort directory
        if(DataWeek == DataDirectory/"Sort"){continue;}

        // Iterate over all files in the data week directory and find the logFile.
        for(auto& file: std::filesystem::directory_iterator(DataWeek))
        {
            // After finding the log file get the files that will be used for analysis
            if(std::regex_match( std::filesystem::absolute(file).string() ,logFile ))
            {
                std::cout << "Found log file:\n" ;
                std::cout << file << "\n";
                std::cout << "Generating List Files\n";
                GetRunFilesFromLogFile(file);

            }
        }
    }
}

void FindPositionOfGammaFlash()
{

}

void WriteListSortToAnalysisFile()
{

}



std::vector<int> GetRunFilesFromLogFile(std::filesystem::directory_entry LogFile)
{
    std::fstream logFile;
    logFile.open(std::filesystem::absolute(LogFile), std::fstream::in);
    if(logFile.is_open())
    {
        // Get the length of the logFile table
        using Table = std::vector<std::vector<std::string>>;
        Table table = GetLogFileTable(&logFile);
    }
    logFile.close();

    return {1};
}


std::vector<std::vector<std::string>> GetLogFileTable(std::fstream* logFile)
{
  std::string line;
  std::regex TableLine(" [0-9]+\t+[0-9]+/[0-9]+/[0-9]+\t+[0-9]+:[0-9]+:[0-9]+\t+[0-9].[0-9]+\t+[0-9].[0-9]+\t+[0-9].[0-9]+\t+[0-9].[0-9]+");
  while(getline(*logFile, line))
  {
    //bool isTableLine = std::regex_match(line, TableLine);
    //std::cout << line << "  isTL:  ";
    //std::cout << isTableLine << '\n';
    if(std::regex_match(line, TableLine))
    {
      std::cout << line << '\n' << '\n';
    }

  }
  return {{""}};
}

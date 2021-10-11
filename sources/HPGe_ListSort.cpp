#include "HPGe_ListSort.h"
#define NumberOfDigits 3

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
            if(std::regex_match( std::filesystem::absolute(file).string(), logFile ))
            {
                std::cout << "Generating List Files For: " << std::filesystem::absolute(DataWeek).filename() << '\n';
                std::vector<int> RunFilesToInclude = GetRunFilesToInclude(file);
                WriteListFiles(RunFilesToInclude, DataWeek);
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



std::vector<int> GetRunFilesToInclude(std::filesystem::directory_entry LogFile)
{
    std::fstream logFile;
    logFile.open(std::filesystem::absolute(LogFile), std::fstream::in);

    std::vector<int> RunFiles;
    if(logFile.is_open())
    {
        using Table = std::vector<std::vector<int>>;
        Table AllRunFilesTable = GetLogFileTable(&logFile);
        RunFiles = RunFilesToInclude(AllRunFilesTable);
    }
    logFile.close();

    return RunFiles;
}


std::vector<std::vector<int>> GetLogFileTable(std::fstream* logFile)
{
    // The vector to be returned
    std::vector<std::vector<int>> Table;
    // A temporary vector to push into Table
    std::vector<int> temp;
    temp.resize(5);

    // A variable to store the line on the file.
    std::string line;
    // This is how the acquisition program writes the table and it is in a unique format
    std::regex TableLine("\\s*[0-9]+\\s+[0-9]+/[0-9]+/[0-9]+\\s+[0-9]+:[0-9]+:[0-9]+\\s+[0-9].[0-9]+\\s+[0-9].[0-9]+\\s+[0-9].[0-9]+\\s+[0-9].[0-9]+\\s*");

    // Regex for the first item in the table line (The run number).
    std::regex RunNumber ("\\s*[0-9]+\\s+");

    // Regex for the last four items in the table line (The counting rate for each channel).
    std::regex Channel("\\s+[0-9].[0-9]+\\s*");

    // For every line in the log file.
    while(getline(*logFile, line))
    {
        // Check if it is a line of the table
        if(std::regex_match(line, TableLine))
        {
            // token stores the matches (Run Number, and Rate of each channel)
            std::smatch token;
            // Extract the Run Number
            std::regex_search(line, token, RunNumber);
            temp[0] = std::stoi(token[0]);

            int index = 1;
            // Extract the Rate of the Channels
            while(std::regex_search(line, token, Channel))
            {
                temp[index] = std::stoi(token[0]);
                // Remove "NumberOfDigits" characters to get the next token
                line = line.substr(line.find(token.str()) + NumberOfDigits,
                                   line.size());
                ++index;
            }
            Table.push_back(temp);
        }
    }

    return Table;
}



std::vector<int> RunFilesToInclude(std::vector<std::vector<int>> Table)
{
    std::vector<int> RunFiles;
    // Check if any of the Channels have a conting rate of 0
    // If so, exclude that RunFile.
    for(auto& row: Table)
    {
        const std::vector<int> ChannelsCountingRate = {row[1], row[2], row[3], row[4]};
        bool RateNonZero = std::all_of(ChannelsCountingRate.begin(),
                                       ChannelsCountingRate.end(),
                                       [](const int& Rate){return Rate >= 1;});
        if(RateNonZero)
        {   RunFiles.push_back(row[0]); }
    }

    return RunFiles;
}




void WriteListFiles(std::vector<int> RunFilesToInclude, std::filesystem::directory_entry DataWeek)
{
    std::fstream listfiles_ch1, listfiles_ch2, listfiles_ch3, listfiles_ch4 ;
    std::vector<std::fstream*> listfiles = {&listfiles_ch1, &listfiles_ch2, &listfiles_ch3, &listfiles_ch4 };


}







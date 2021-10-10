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
        int LengthOfTable = GetLengthOfTable(&logFile);
        using Table = std::vector<std::vector<std::string>>;
        Table table = GetLogFileTable(&logFile, LengthOfTable);
    }
    logFile.close();

    return {1};
}

int GetLengthOfTable(std::fstream* logFile)
{
    // Ignore the first 98 lines (configuration data)
        // We are interested in the table at the end.
        for(int i = 0; i < 98; ++i)
            logFile->ignore(1000, '\n');
        // Ignore everithing untill the new line character

        // Position: start of the table
        int BeginningOfTable = logFile->tellg();
        // I can do this because the acquisition program
        // always writes this many lines.
        // TODO: Extract this table in a more general approach.


        // Go to the end of the file
        logFile->seekg(0, logFile->end);
        // Position: end of file
        int EndOfTable = logFile->tellg();

        // Subract 120 (bytes writen by the acquisition system)
        // Position: End of table
        EndOfTable -= 120;

        // Go to the BeginningOfTable
        logFile->seekg(0, logFile->beg);
        for(int i = 0; i < 98; ++i)
            logFile->ignore(1000, '\n');

        // Return the length
        return (EndOfTable - BeginningOfTable);
}

std::vector<std::vector<std::string>> GetLogFileTable(std::fstream* logFile, int LengthOfTable)
{

    logFile->seekg(0, logFile->beg);
    std::string line;
//     std::regex TableLine ("[0-9]+\s+[0-9]+/[0-9]+/[0-9]+\s+[0-9]+:[0-9]+:[0-9]+\s+[0-9].[0-9][0-9]\s+[0-9].[0-9][0-9]\s+[0-9].[0-9][0-9]\s+[0-9].[0-9][0-9]");
    std::regex TableLine ("( [0-9]+)(\s+)([0-9]+)/([0-9]+)/([0-9]+)(\s+)([0-9]+):([0-9]+):([0-9]+)(\s+)([0-9]+).([0-9]+)(\s+)([0-9]+).([0-9]+)(\s+)([0-9]+).([0-9]+)(\s+)([0-9]+).([0-9]+)");
//     std::regex TableLine (" 49		07/09/21	16:10:36	1.32	1.68	1.13	1.53");



    while(getline(*logFile, line))
    {
        bool isTableLine = std::regex_match(line, TableLine);
        std::cout << line << "  isTL:  ";
        std::cout << isTableLine << '\n';
        if(isTableLine)
        {
            std::cout << line << '\n' << '\n';
        }

    }

//     int BeginningOfTable = logFile->tellg();
//     while(logFile->tellg() != BeginningOfTable + LengthOfTable)
//     {
//
//     }




    return {{""}};
}

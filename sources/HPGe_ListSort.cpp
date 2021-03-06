#include "HPGe_ListSort.h"


void GenerateListFiles(std::filesystem::path DataDirectory)
{
    // The DataDirectory contains directories with detector data
    // each of these directories contain a log file, amplitude spectra
    // time spectra and raw data.
    //
    // The counting rate in the log file will be used to choose the
    // files that will be analised.

    const std::regex logFile ("(.*)_logFile_(.*)");
    // Iterate over all data weeks
    for(const auto& DataWeek: std::filesystem::directory_iterator(DataDirectory))
    {
      if(DataWeek.is_directory())
      {
        // Skip the Sort directory
        if(DataWeek == DataDirectory/"Sort"){continue;}

        // Iterate over all files in the data week directory and find the logFile.
        for(const auto& file: std::filesystem::directory_iterator(DataWeek))
        {
          if(file.is_regular_file())
          {
            // After finding the log file get the files that will be used for analysis

            auto file_s = std::filesystem::absolute(file).string();
            if(std::regex_match(file_s, logFile ))
            {
                std::cout << "Generating List Files For: " << DataWeek << '\n';

                std::string LogFilePath = std::filesystem::absolute(file).string();
                
                const std::vector<int> RunFilesToInclude = GetRunFilesToInclude(LogFilePath);

                WriteListFiles(RunFilesToInclude, DataWeek);
                break;
            }
          }
        }
      }
    }
}   

using OffSetTable = std::map<std::filesystem::path, std::vector<int>>;
OffSetTable FindGammaFlashOffSets(std::filesystem::path DataDirectory)
{
  using PositionTable = std::map<std::filesystem::path, std::vector<int>>;
  PositionTable GammaFlashPositions;
  OffSetTable GammaFlashOffSets;
  //
  for(auto& DataWeek: std::filesystem::directory_iterator(DataDirectory))
  {

    std::map<std::string, std::vector<std::string>> THistFiles;
    std::vector<std::string> Channels {"ch1", "ch2", "ch3", "ch4"};

    // Iterate over the directories that are not named "Sort"
    if(DataWeek.is_directory())
    {
        if(DataWeek.path().filename().string() == "Sort") continue;
        std::cout << "Finding Gamma Flash For: " << DataWeek << ' ';
      // For every Channel
      for(auto i = 0; i < Channels.size(); ++i)
      {
        // Find all Thist of that channel
        for(auto& File: std::filesystem::directory_iterator(DataWeek))
        {
          // Iterate over all files in DataWeek to find THist_ch*_
          if(File.is_regular_file())
          {
            std::regex THistRegex ("(.*)THist_" + Channels[i] + "_(.*).his(.*)");
            std::string FilePathString = File.path().string();
            if(std::regex_match(FilePathString, THistRegex))
            {
              THistFiles[Channels[i]].push_back(FilePathString);
            }
          }
        } 
      }
    }

    // Sort the THistFiles to have the older ones at the end
    for(std::string ch: Channels)
      std::sort(THistFiles[ch].begin(), THistFiles[ch].end());

    // Get the position of the gamma flash for each channel
    for(auto i = 0; i < Channels.size(); ++i)
    {
        // TODO: The -10 is a hardcoded euristic, it has to be a better way of finding the last THist
        std::string LastTHistFile = THistFiles[Channels[i]][THistFiles[Channels[i]].size() - 10];

        std::fstream THistStream;
        THistStream.open(LastTHistFile, std::fstream::in);
        int Index = 0, Count = 0, MaxIndex = 0, MaxCount = 0;

        while(THistStream)
        {
            THistStream >> Index >> Count;
            if(Count > MaxCount && Index < MAX_NUMBER_TOF_CHANNELS)
            {
                MaxCount = Count;
                MaxIndex = Index;
            }
        }

        GammaFlashPositions[DataWeek.path()].push_back(MaxIndex);
    }
   }

    // Find Gama Flash Mean Position for the whole digitiser data set
    int DigitiserMean;
    bool FirstIteration = true;
    for(auto& [Week,Positions]: GammaFlashPositions)
    {
        int WeekMean = (Positions[0] + Positions[1] + Positions[2] + Positions[3]) / 4;
        if(FirstIteration)
        {
            DigitiserMean = WeekMean;
            FirstIteration = false;
        }
        else
        {
            DigitiserMean = (DigitiserMean + WeekMean) / 2;
        }
    }

    std::cout << "Mean Gamma Flash Position: " << DigitiserMean << '\n';

    for(auto& [Week,Positions]: GammaFlashPositions)
    {
        int GammaOffSet1 = DigitiserMean - Positions[0];
        int GammaOffSet2 = DigitiserMean - Positions[1];
        int GammaOffSet3 = DigitiserMean - Positions[2];
        int GammaOffSet4 = DigitiserMean - Positions[3];
        GammaFlashOffSets[Week] = {GammaOffSet1, GammaOffSet2, GammaOffSet3, GammaOffSet4};
    }

  return GammaFlashOffSets;
}

void WriteListSortToAnalysisFile(std::filesystem::path DataDirectory)
{
  std::fstream analysis_file;

  // One directory up from the DataDirectory is the AnalysisDirectory
  analysis_file.open(DataDirectory/".."/"analysis.sh", std::fstream::app);

  OffSetTable OffSets = FindGammaFlashOffSets(DataDirectory);


  for(auto& DataWeek: std::filesystem::directory_iterator(DataDirectory))
  {
    if(DataWeek.is_directory())
    {
      auto it = DataWeek.path().end(); --it;

      // the last element in path is the week name
      std::filesystem::path Week      = *it; --it;

      // the second to last elemet is the digitiser
      std::string Digitiser = *it;

      if(Week != "Sort")
      {
        std::cout << "Write ListSort For: " << DataWeek << '\n';
        WriteListSortLine(analysis_file, Digitiser, Week, OffSets[DataWeek.path()]);
      }
    }
  }
}



std::vector<int> GetRunFilesToInclude(std::string LogFile)
{
    std::fstream logFile;
    logFile.open(LogFile, std::fstream::in);

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
                line = line.substr(line.find(token.str()) + NUMBER_OF_DIGITS,
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
    // Set the current path to the DataWeek Directory, while saving the current path to return later
    std::filesystem::path InitialPath = std::filesystem::current_path();
    std::filesystem::current_path(DataWeek);

    // Generate the listfiles_ch* files in the Data Directory
    std::fstream listfiles_ch1, listfiles_ch2, listfiles_ch3, listfiles_ch4 ;
    listfiles_ch1.open("listfiles_ch1", std::ios_base::out);
    listfiles_ch2.open("listfiles_ch2", std::ios_base::out);
    listfiles_ch3.open("listfiles_ch3", std::ios_base::out);
    listfiles_ch4.open("listfiles_ch4", std::ios_base::out);

    // After creating the file streams revert the current path
    std::filesystem::current_path(InitialPath);

    const std::vector<std::fstream*> listfiles = {&listfiles_ch1, &listfiles_ch2, &listfiles_ch3, &listfiles_ch4};
    const std::vector<std::string> Channels = {"ch1", "ch2", "ch3", "ch4"};

    for(auto index = 0; index < listfiles.size(); index++)
    {
      std::vector<std::string> lst_files;

      const std::string LstRegex = "(.*)" + Channels[index] + "(.*).lst";
      const std::regex isLSTfile (LstRegex);

      for(const auto& lst_file: std::filesystem::directory_iterator(DataWeek))
      {
            if(lst_file.is_regular_file())
            {
              std::string FileName = std::filesystem::absolute(lst_file).filename().string();
                // Regex to find the *_ch*_*.lst files to include in the listfiles_ch*
                if(std::regex_match(FileName, isLSTfile))
                {
                    lst_files.push_back(FileName);
                }
            }
      }

             
      for(auto& FileNumber: RunFilesToInclude)
      {
        const std::string FileNumber_s = std::to_string(FileNumber);
        const std::string RunFileRegex =  "(.*)_" + FileNumber_s  + ".lst";
        const std::regex IsInRunFiles(RunFileRegex);

        for(auto& lst_file: lst_files)
        {
          if(std::regex_match(lst_file, IsInRunFiles))
          {
            std::string FileName   =  lst_file;
            *listfiles[index] << FileName << '\n';
          }
        }
      }
    }

    listfiles_ch1.close();
    listfiles_ch2.close();
    listfiles_ch3.close();
    listfiles_ch4.close();

        
}



void WriteListSortLine(std::fstream& analysis_file, std::string Digitiser, std::filesystem::path Week, std::vector<int> OffSets)
{
  if(Digitiser == "Dig1")
  {
    analysis_file << "echo " << Week << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch1").string() << " " << "9 "  << " " << OffSets[0]   << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch2").string() << " " << "1 "  << " " << OffSets[1]   << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch3").string() << " " << "11"  << " " << OffSets[2]   << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch4").string() << " " << "3 "  << " " << OffSets[3]   << '\n';
    analysis_file << '\n';
  }

  if(Digitiser == "Dig2")
  {
    analysis_file << "echo " << Week << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch1").string() << " " << "10"  << " " << OffSets[0] << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch2").string() << " " << "2 "  << " " << OffSets[1] << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch3").string() << " " << "12"  << " " << OffSets[2] << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch4").string() << " " << "4 "  << " " << OffSets[3] << '\n';
    analysis_file << '\n';
  }

  if(Digitiser == "Dig3")
  {
    analysis_file << "echo " << Week << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch1").string() << " " << "5 " << " " << OffSets[0] << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch2").string() << " " << "6 " << " " << OffSets[1] << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch3").string() << " " << "7 " << " " << OffSets[2] << '\n';
    analysis_file << "./bin/listsort_Digit4.0" << " " << Digitiser << " " << Week.string() << " " << (Week/"listfiles_ch4").string() << " " << "8 " << " " << OffSets[3] << '\n';
    analysis_file << '\n';
  }

}




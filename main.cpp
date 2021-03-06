/*
 *  This is a program that generates the neutron analysis
 *  for the GAINS experimments
 *
 *  Author: Liviu Stoica
 *  Date:   01.10.2021
 */


#include <iostream>
#include "Analysis.h"
#include <filesystem>
#include <string>

void VerifyArgumets(size_t NumArgs, const std::string& Path, const std::string& NumTofChannels)
{
  constexpr int MaxTOF = 3000;
  if(NumArgs != 3)
  {
    std::cout << "Usage: generate_analysis <Path> <Number of TOF Channels>\n";
    std::cout << "       <Path> = path to the directory where the data is\n";
    exit(EXIT_FAILURE);
  }


  if(!(std::filesystem::exists(Path)))
  {
  std::cout << "The <path> does not exist.\n";
  exit(EXIT_FAILURE);
  }


  // checking if the path exists and the num of tof is less then MaxTOF
  // (after 2800 channels we do not see any events)
  if(!(std::stoi(NumTofChannels) <= MaxTOF))
  {
    std::cout << "The number of TOF Channels seems to be unnecesarrly big.\n";
    exit(EXIT_FAILURE);
  }
}



int main(int argc, char** argv)
{
  std::string Path = std::string(argv[1]); 
  std::string NumOfTofChannels = std::string(argv[2]);

  VerifyArgumets(argc, Path, NumOfTofChannels);

  // Initialize an Analysis singleton.
  // The initialisation of an Analysis object
  //  creates the proper file structure of the <path> directory.
  Analysis Analysis{Path, std::stoi(NumOfTofChannels)};


  Analysis.DetectorsAnalysis();
  Analysis.FissionChamberAnalysis();

  // Verify Yield
  Analysis.VerifyYield();
  // Process
  Analysis.Process();
  // Germanium Efficientcy
  Analysis.GermaniumEfficiency();


}


#include <HPGe_AddMatrix.h>


void GenerateInputFilesForAddMatrix(std::filesystem::path DataDirectory)
{
//     std::filesistem::path DataDirectory/"Sort";
    std::fstream InpFile1, InpFile2, InpFile3, InpFile4;

    InpFile1.open(DataDirectory/"Sort"/"inp_mat_ch1", std::fstream::app);
    InpFile2.open(DataDirectory/"Sort"/"inp_mat_ch2", std::fstream::app);
    InpFile3.open(DataDirectory/"Sort"/"inp_mat_ch3", std::fstream::app);
    InpFile4.open(DataDirectory/"Sort"/"inp_mat_ch4", std::fstream::app);

    if(std::filesystem::exists(DataDirectory/"Sort"/"inp_mat_ch1"))
    {
        std::cout << "The current folder has \"inp_mat_ch1\", \"inp_mat_ch2\", \"inp_mat_ch3\", \"inp_mat_ch4\"  file.\n" ;

        std::filesystem::rename(DataDirectory/"Sort"/"inp_mat_ch1", DataDirectory/"Sort"/"inp_mat_ch1.backup");
        std::filesystem::rename(DataDirectory/"Sort"/"inp_mat_ch2", DataDirectory/"Sort"/"inp_mat_ch2.backup");
        std::filesystem::rename(DataDirectory/"Sort"/"inp_mat_ch3", DataDirectory/"Sort"/"inp_mat_ch3.backup");
        std::filesystem::rename(DataDirectory/"Sort"/"inp_mat_ch4", DataDirectory/"Sort"/"inp_mat_ch4.backup");
        std::cout << "Creating backup of " << DataDirectory/"Sort"/"inp_mat_ch1" << " into " << DataDirectory/"Sort"/"inp_mat_ch1.backup" << "\n";
        std::cout << "Creating backup of " << DataDirectory/"Sort"/"inp_mat_ch2" << " into " << DataDirectory/"Sort"/"inp_mat_ch2.backup" << "\n";
        std::cout << "Creating backup of " << DataDirectory/"Sort"/"inp_mat_ch3" << " into " << DataDirectory/"Sort"/"inp_mat_ch3.backup" << "\n";
        std::cout << "Creating backup of " << DataDirectory/"Sort"/"inp_mat_ch4" << " into " << DataDirectory/"Sort"/"inp_mat_ch4.backup" << "\n";
    }

    for(auto& DataWeek: std::filesystem::directory_iterator(DataDirectory))
    {
        if(!(DataWeek == DataDirectory/"Sort"))
        {
            std::filesystem::path Digitiser =  *(++(++DataWeek.path().begin()));
            std::filesystem::path Week = *(++(++(++DataWeek.path().begin())));
            std::cout << "Generating Input File For: " << DataWeek << '\n';
            if(Digitiser == "Dig1")
            {
                InpFile1 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat9.dat  RefChannel\n";
                InpFile2 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat1.dat  RefChannel\n";
                InpFile3 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat11.dat RefChannel\n";
                InpFile4 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat3.dat  RefChannel\n";
            }
            else
            {
                if(Digitiser == "Dig2")
                {
                    InpFile1 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat10.dat RefChannel\n";
                    InpFile2 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat2.dat  RefChannel\n";
                    InpFile3 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat12.dat RefChannel\n";
                    InpFile4 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat4.dat  RefChannel\n";
                }
                else if(Digitiser == "Dig3")
                {
                    InpFile1 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat5.dat RefChannel\n";
                    InpFile2 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat6.dat  RefChannel\n";
                    InpFile3 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat7.dat RefChannel\n";
                    InpFile4 << "./" + Digitiser.string() + "/Sort/" + Week.string() + "/mat8.dat  RefChannel\n";
                }

            }
        }
    }
    InpFile1.close();
    InpFile2.close();
    InpFile3.close();
    InpFile4.close();
}

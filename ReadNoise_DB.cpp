/*************************************************************
simple program to read bin files
compile with ---> c++ -o ReadNoise_DB `root-config --cflags --glibs` ReadNoise_DB.cpp geometryUtils.cc
or with ---> c++ -o ReadNoise_DB ReadNoise_DB.cpp `root-config --cflags --glibs` geometryUtils.cc
run with ---> ./ReadNoise_DB dump_list_DB.txt chstatus_list_DB.txt 3
*************************************************************/

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"

#include "geometryUtils.h"

std::vector<std::string> splitString(std::string& inputName, char split_char);

int main(int argc, char** argv)
{
    char* inputLISTNoise = argv[1];
    char* inputLISTChStatus = argv[2];
    int cutChStatus = atoi(argv[3]);
    
    std::string inputListNoise = std::string(inputLISTNoise);
    std::string inputListChStatus = std::string(inputLISTChStatus);
    
    std::cout << "inputListNoise    = " << inputListNoise << std::endl;
    std::cout << "inputListChStatus = " << inputListChStatus << std::endl;
    std::cout << "cutChStatus       = " << cutChStatus << std::endl;
    
    std::vector<std::string> inputFiles;
    std::vector<std::string> inputFilesChStatus;

    char dumps[500];
    FILE *f_dumps;
    f_dumps = fopen(inputListNoise.c_str(),"r");
    while(fscanf(f_dumps,"%s \n", dumps) !=EOF ){
          //std::cout << "Reading inputFile: " << dumps << std::endl;
          std::string DUMPS = std::string(dumps);
          if(DUMPS.find("#") != std::string::npos) continue;
          inputFiles.push_back(DUMPS);
    }
    f_dumps = fopen(inputListChStatus.c_str(),"r");
    while(fscanf(f_dumps,"%s \n", dumps) !=EOF ){
          //std::cout << "Reading inputFile: " << dumps << std::endl;
          std::string DUMPS = std::string(dumps);
          if(DUMPS.find("#") != std::string::npos) continue;
          inputFilesChStatus.push_back(DUMPS);
    }

    std::map<int,std::pair<int,int> > mapChStatusRun;
    for(unsigned int ii = 0; ii < inputFilesChStatus.size(); ii++)
    {
        std::string inputName = splitString(inputFilesChStatus.at(ii),'/').at(splitString(inputFilesChStatus.at(ii),'/').size()-1);
        std::vector<std::string> tokens = splitString(inputName,'_');
        tokens.at(6) = tokens.at(6).erase(0,2);
        tokens.at(8) = tokens.at(8).erase(tokens.at(8).size()-4,tokens.at(8).size());
        tokens.at(8) = tokens.at(8).erase(0,2);
        mapChStatusRun[ii] = std::make_pair(atoi(tokens.at(6).c_str()),atoi(tokens.at(8).c_str()));  
    }
    
    TH1F* h_Ped_EB_g12[100][3];
    TH1F* h_Ped_EB_g6[100][3];
    TH1F* h_Ped_EB_g1[100][3];
    TH1F* h_RMS_EB_g12[100][3]; 
    TH1F* h_RMS_EB_g6[100][3]; 
    TH1F* h_RMS_EB_g1[100][3]; 

    TH1F* h_Ped_EE_g12[40][3];
    TH1F* h_Ped_EE_g6[40][3];
    TH1F* h_Ped_EE_g1[40][3];
    TH1F* h_RMS_EE_g12[40][3];
    TH1F* h_RMS_EE_g6[40][3];
    TH1F* h_RMS_EE_g1[40][3];

    TH2F* h2_NoiseChannels_EB_g12_1sigma;
    TH2F* h2_NoiseChannels_EE_g12_1sigma[3];
    TH2F* h2_NoiseChannels_EB_g12_2sigma;
    TH2F* h2_NoiseChannels_EE_g12_2sigma[3];

    TH2F* h2_NoiseChannels_EB_g6_1sigma;
    TH2F* h2_NoiseChannels_EE_g6_1sigma[3];
    TH2F* h2_NoiseChannels_EB_g6_2sigma;
    TH2F* h2_NoiseChannels_EE_g6_2sigma[3];

    TH2F* h2_NoiseChannels_EB_g1_1sigma;
    TH2F* h2_NoiseChannels_EE_g1_1sigma[3];
    TH2F* h2_NoiseChannels_EB_g1_2sigma;
    TH2F* h2_NoiseChannels_EE_g1_2sigma[3];

    TEndcapRegions* eeId = new TEndcapRegions();
    
    TGraphErrors* g_Ped_EB_g12[3];
    TGraphErrors* g_Ped_EB_g6[3];
    TGraphErrors* g_Ped_EB_g1[3];
    TGraphErrors* g_Ped_EE_g12[3];
    TGraphErrors* g_Ped_EE_g6[3];
    TGraphErrors* g_Ped_EE_g1[3];
    
    TGraphErrors* g_RMS_EB_g12[3];
    TGraphErrors* g_RMS_EB_g6[3];
    TGraphErrors* g_RMS_EB_g1[3];
    TGraphErrors* g_RMS_EE_g12[3];
    TGraphErrors* g_RMS_EE_g6[3];
    TGraphErrors* g_RMS_EE_g1[3];

    TGraphErrors* g_RMS_EB_g12_1sigma[3];
    TGraphErrors* g_RMS_EB_g6_1sigma[3];
    TGraphErrors* g_RMS_EB_g1_1sigma[3];
    TGraphErrors* g_RMS_EE_g12_1sigma[3];
    TGraphErrors* g_RMS_EE_g6_1sigma[3];
    TGraphErrors* g_RMS_EE_g1_1sigma[3];

    TGraphErrors* g_RMS_EB_g12_2sigma[3];
    TGraphErrors* g_RMS_EB_g6_2sigma[3];
    TGraphErrors* g_RMS_EB_g1_2sigma[3];
    TGraphErrors* g_RMS_EE_g12_2sigma[3];
    TGraphErrors* g_RMS_EE_g6_2sigma[3];
    TGraphErrors* g_RMS_EE_g1_2sigma[3];

    for(int iz = 0; iz<3; iz++){
        g_Ped_EB_g12[iz]= new TGraphErrors();
        g_Ped_EB_g6[iz]= new TGraphErrors();
        g_Ped_EB_g1[iz]= new TGraphErrors();
        g_Ped_EE_g12[iz]= new TGraphErrors();
        g_Ped_EE_g6[iz]= new TGraphErrors();
        g_Ped_EE_g1[iz]= new TGraphErrors();
    
        g_RMS_EB_g12[iz]= new TGraphErrors();
        g_RMS_EB_g6[iz]= new TGraphErrors();
        g_RMS_EB_g1[iz]= new TGraphErrors();
        g_RMS_EE_g12[iz]= new TGraphErrors();
        g_RMS_EE_g6[iz]= new TGraphErrors();
        g_RMS_EE_g1[iz]= new TGraphErrors();
        
        g_RMS_EB_g12_1sigma[iz]= new TGraphErrors();
        g_RMS_EB_g6_1sigma[iz]= new TGraphErrors();
        g_RMS_EB_g1_1sigma[iz]= new TGraphErrors();
        g_RMS_EE_g12_1sigma[iz]= new TGraphErrors();
        g_RMS_EE_g6_1sigma[iz]= new TGraphErrors();
        g_RMS_EE_g1_1sigma[iz]= new TGraphErrors();

        g_RMS_EB_g12_2sigma[iz]= new TGraphErrors();
        g_RMS_EB_g6_2sigma[iz]= new TGraphErrors();
        g_RMS_EB_g1_2sigma[iz]= new TGraphErrors();
        g_RMS_EE_g12_2sigma[iz]= new TGraphErrors();
        g_RMS_EE_g6_2sigma[iz]= new TGraphErrors();
        g_RMS_EE_g1_2sigma[iz]= new TGraphErrors();
    }

    std::map<int,std::map<int,std::map<int,float> > > noiseMap_g12;
    std::map<int,std::map<int,std::map<int,float> > > noiseMap_g6;
    std::map<int,std::map<int,std::map<int,float> > > noiseMap_g1;

  for(unsigned int ii = 0; ii < inputFiles.size(); ii++){

    char Name[500];
   
    for(int ieta = 0; ieta<100; ieta++)
            for(int iz = 0; iz<3; iz++){

                if(iz == 1) continue;

                sprintf(Name,"h_Ped_EB_g12_%d_%d",ieta+1,iz-1);
                h_Ped_EB_g12[ieta][iz]=new TH1F(Name,Name,50000,0.,500.);
                sprintf(Name,"h_Ped_EB_g6_%d_%d",ieta+1,iz-1);
                h_Ped_EB_g6[ieta][iz]=new TH1F(Name,Name,50000,0.,500.);
                sprintf(Name,"h_Ped_EB_g1_%d_%d",ieta+1,iz-1);
                h_Ped_EB_g1[ieta][iz]=new TH1F(Name,Name,50000,0.,500.);

                sprintf(Name,"h_RMS_EB_g12_%d_%d",ieta+1,iz-1);
                h_RMS_EB_g12[ieta][iz]=new TH1F(Name,Name,1500,0.,15.);
                sprintf(Name,"h_RMS_EB_g6_%d_%d",ieta+1,iz-1);
                h_RMS_EB_g6[ieta][iz]=new TH1F(Name,Name,1500,0.,15.);
                sprintf(Name,"h_RMS_EB_g1_%d_%d",ieta+1,iz-1);
                h_RMS_EB_g1[ieta][iz]=new TH1F(Name,Name,1500,0.,15.);
            }
    
    for(int ir = 0; ir<40; ir++)
            for(int iz = 0; iz<3; iz++){

                if(iz == 1) continue;

                sprintf(Name,"h_Ped_EE_g12_%d_%d",ir+1,iz-1);
                h_Ped_EE_g12[ir][iz]=new TH1F(Name,Name,50000,0.,500.);
                sprintf(Name,"h_Ped_EE_g6_%d_%d",ir+1,iz-1);
                h_Ped_EE_g6[ir][iz]=new TH1F(Name,Name,50000,0.,500.);
                sprintf(Name,"h_Ped_EE_g1_%d_%d",ir+1,iz-1);
                h_Ped_EE_g1[ir][iz]=new TH1F(Name,Name,50000,0.,500.);

                sprintf(Name,"h_RMS_EE_g12_%d_%d",ir+1,iz-1);
                h_RMS_EE_g12[ir][iz]=new TH1F(Name,Name,1500,0.,15.);
                sprintf(Name,"h_RMS_EE_g6_%d_%d",ir+1,iz-1);
                h_RMS_EE_g6[ir][iz]=new TH1F(Name,Name,1500,0.,15.);
                sprintf(Name,"h_RMS_EE_g1_%d_%d",ir+1,iz-1);
                h_RMS_EE_g1[ir][iz]=new TH1F(Name,Name,1500,0.,15.);
            }
   
    
    h2_NoiseChannels_EB_g12_1sigma = new TH2F("h2_NoiseChannels_EB_g12_1sigma","h2_NoiseChannels_EB_g12_1sigma",360,0,360,170,-85,85);
    h2_NoiseChannels_EB_g12_2sigma = new TH2F("h2_NoiseChannels_EB_g12_2sigma","h2_NoiseChannels_EB_g12_2sigma",360,0,360,170,-85,85);
    h2_NoiseChannels_EB_g6_1sigma = new TH2F("h2_NoiseChannels_EB_g6_1sigma","h2_NoiseChannels_EB_g6_1sigma",360,0,360,170,-85,85);
    h2_NoiseChannels_EB_g6_2sigma = new TH2F("h2_NoiseChannels_EB_g6_2sigma","h2_NoiseChannels_EB_g6_2sigma",360,0,360,170,-85,85);
    h2_NoiseChannels_EB_g1_1sigma = new TH2F("h2_NoiseChannels_EB_g1_1sigma","h2_NoiseChannels_EB_g1_1sigma",360,0,360,170,-85,85);
    h2_NoiseChannels_EB_g1_2sigma = new TH2F("h2_NoiseChannels_EB_g1_2sigma","h2_NoiseChannels_EB_g1_2sigma",360,0,360,170,-85,85);

    for(int iz = 0; iz<3; iz++){

                if(iz == 1) continue;

                sprintf(Name,"h2_NoiseChannels_EE_g12_1sigma_%d",iz-1);
                h2_NoiseChannels_EE_g12_1sigma[iz] = new TH2F(Name,Name,100,1,101,100,1,101);
                sprintf(Name,"h2_NoiseChannels_EE_g12_2sigma_%d",iz-1);
                h2_NoiseChannels_EE_g12_2sigma[iz] = new TH2F(Name,Name,100,1,101,100,1,101);
                sprintf(Name,"h2_NoiseChannels_EE_g6_1sigma_%d",iz-1);
                h2_NoiseChannels_EE_g6_1sigma[iz] = new TH2F(Name,Name,100,1,101,100,1,101);
                sprintf(Name,"h2_NoiseChannels_EE_g6_2sigma_%d",iz-1);
                h2_NoiseChannels_EE_g6_2sigma[iz] = new TH2F(Name,Name,100,1,101,100,1,101);
                sprintf(Name,"h2_NoiseChannels_EE_g1_1sigma_%d",iz-1);
                h2_NoiseChannels_EE_g1_1sigma[iz] = new TH2F(Name,Name,100,1,101,100,1,101);
                sprintf(Name,"h2_NoiseChannels_EE_g1_2sigma_%d",iz-1);
                h2_NoiseChannels_EE_g1_2sigma[iz] = new TH2F(Name,Name,100,1,101,100,1,101);
    }


    std::cout << "Reading inputFile: " << inputFiles.at(ii) << std::endl;
    std::string inputName = splitString(inputFiles.at(ii),'/').at(splitString(inputFiles.at(ii),'/').size()-1);
    std::vector<std::string> tokens = splitString(inputName,'_');
    tokens.at(5) = tokens.at(5).erase(0,2);
    tokens.at(7) = tokens.at(7).erase(tokens.at(7).size()-4,tokens.at(7).size());
    tokens.at(7) = tokens.at(7).erase(0,2); 

    int run_min = atoi(tokens.at(5).c_str());
    int run_max = atoi(tokens.at(7).c_str());
    int iFile=0;
    for(unsigned int jj = 0; jj < mapChStatusRun.size(); jj++)
        if(run_max >= mapChStatusRun[jj].first && run_max <= mapChStatusRun[jj].second) iFile=jj;

    char cieta[100], ciphi[100], ciz[100], crawid[100], cped12[100], crms12[100], cped6[100], crms6[100], cped1[100], crms1[100];
    int ieta, iphi, ix, iy, iz, chStatus;
    long int rawid;
    float ped12, rms12, ped6, rms6, ped1, rms1;

    std::cout << "Reading inputFileChStatus: " << inputFilesChStatus.at(iFile) << std::endl;
    std::map<int,std::map<int,std::map<int,int> > > ichStatus;
    std::ifstream infileChStatus(inputFilesChStatus.at(iFile).c_str()); 
    while(infileChStatus >> ieta >> iphi >> iz >> chStatus)
    {
           ichStatus[ieta][iphi][iz]=chStatus;
    }
    
    TFile *myOutFile = new TFile(("NoisePlots_DB_"+tokens.at(5)+"_"+tokens.at(7)+".root").c_str(),"RECREATE");
    std::ifstream infile(inputFiles.at(ii).c_str()); 
    int iline = 0;
    std::string line;

    noiseMap_g12.clear();
    noiseMap_g6.clear();
    noiseMap_g1.clear();
  
    while(infile >> cieta >> ciphi >> ciz >> cped12 >> crms12 >> cped6 >> crms6 >> cped1 >> crms1 >> crawid)
    {
           
          iline++; 
          if(iline > 1 && iline <= 61201){
             ieta = atoi(cieta);
             iphi = atoi(ciphi);
             iz = atoi(ciz);
             rawid = atol(crawid);
             ped12 = atof(cped12);
             rms12 = atof(crms12);
             ped6 = atof(cped6);
             rms6 = atof(crms6);
             ped1 = atof(cped1);
             rms1 = atof(crms1);
            
             if(ped12 == -999 || rms12 == -999) continue;
             if(ichStatus[ieta][iphi][iz]>cutChStatus) continue;
             
             if(ieta < 0){
                ieta = abs(ieta)-1;
                iz = 0; 
             }
             else if(ieta > 0){
                ieta = ieta-1;
                iz = 2;
             }

             h_Ped_EB_g12[ieta][iz]->Fill(ped12);
             h_RMS_EB_g12[ieta][iz]->Fill(4.*rms12);
              
             h_Ped_EB_g6[ieta][iz]->Fill(ped6);
             h_RMS_EB_g6[ieta][iz]->Fill(4.*rms6);

             h_Ped_EB_g1[ieta][iz]->Fill(ped1);
             h_RMS_EB_g1[ieta][iz]->Fill(4.*rms1);

             noiseMap_g12[ieta][iphi][iz]=4*rms12;
             noiseMap_g6[ieta][iphi][iz]=4*rms6;
             noiseMap_g1[ieta][iphi][iz]=4*rms1;

          }else if(iline > 61201){

             ix = atoi(cieta);
             iy = atoi(ciphi);
             iz = atoi(ciz);
             rawid = atol(crawid);
             ped12 = atof(cped12);
             rms12 = atof(crms12);
             ped6 = atof(cped6);
             rms6 = atof(crms6);
             ped1 = atof(cped1);
             rms1 = atof(crms1);
             
             if(ped12 == -999 || rms12 == -999) continue;
             if(ichStatus[ieta][iphi][iz]>cutChStatus) continue;

             int ir = eeId->GetEndcapRing(ix,iy,iz);
       
             h_Ped_EE_g12[ir][iz+1]->Fill(ped12);
             h_RMS_EE_g12[ir][iz+1]->Fill(4.*rms12);

             h_Ped_EE_g6[ir][iz+1]->Fill(ped6);
             h_RMS_EE_g6[ir][iz+1]->Fill(4.*rms6);
     
             h_Ped_EE_g1[ir][iz+1]->Fill(ped1);
             h_RMS_EE_g1[ir][iz+1]->Fill(4.*rms1);

             noiseMap_g12[ix][iy][iz]=4*rms12;
             noiseMap_g6[ix][iy][iz]=4*rms6;
             noiseMap_g1[ix][iy][iz]=4*rms1;
          }
    }

    myOutFile->cd(); 

    for(int iETA = -85; iETA<=85; iETA++)
            for(int iPhi = 1; iPhi<=360; iPhi++){

                            int iZ = 0;
                            int iEta = iETA;
                            if(iEta==0) continue;
                            if(iEta < 0){
                               iEta = abs(iEta)-1;
                               iZ = 0; 
                            }
                            else if(iEta > 0){
                               iEta = iEta-1;
                               iZ = 2;
                            }

                            if(iZ==0){
                               if(noiseMap_g12[iEta][iPhi][iZ]>h_RMS_EB_g12[iEta][iZ]->GetMean()+1.*h_RMS_EB_g12[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g12_1sigma->SetBinContent(h2_NoiseChannels_EB_g12_1sigma->FindBin(iPhi,-iEta-1),noiseMap_g12[iEta][iPhi][iZ]/4.);    
                               if(noiseMap_g6[iEta][iPhi][iZ]>h_RMS_EB_g6[iEta][iZ]->GetMean()+1.*h_RMS_EB_g6[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g6_1sigma->SetBinContent(h2_NoiseChannels_EB_g6_1sigma->FindBin(iPhi,-iEta-1),noiseMap_g6[iEta][iPhi][iZ]/4.); 
                               if(noiseMap_g1[iEta][iPhi][iZ]>h_RMS_EB_g1[iEta][iZ]->GetMean()+1.*h_RMS_EB_g1[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g1_1sigma->SetBinContent(h2_NoiseChannels_EB_g1_1sigma->FindBin(iPhi,-iEta-1),noiseMap_g1[iEta][iPhi][iZ]/4.); 
                               if(noiseMap_g12[iEta][iPhi][iZ]>h_RMS_EB_g12[iEta][iZ]->GetMean()+2.*h_RMS_EB_g12[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g12_2sigma->SetBinContent(h2_NoiseChannels_EB_g12_2sigma->FindBin(iPhi,-iEta-1),noiseMap_g12[iEta][iPhi][iZ]/4.);    
                               if(noiseMap_g6[iEta][iPhi][iZ]>h_RMS_EB_g6[iEta][iZ]->GetMean()+2.*h_RMS_EB_g6[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g6_2sigma->SetBinContent(h2_NoiseChannels_EB_g6_2sigma->FindBin(iPhi,-iEta-1),noiseMap_g6[iEta][iPhi][iZ]/4.); 
                               if(noiseMap_g1[iEta][iPhi][iZ]>h_RMS_EB_g1[iEta][iZ]->GetMean()+2.*h_RMS_EB_g1[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g1_2sigma->SetBinContent(h2_NoiseChannels_EB_g1_2sigma->FindBin(iPhi,-iEta-1),noiseMap_g1[iEta][iPhi][iZ]/4.);  
                            }else if(iZ==2){
                               if(noiseMap_g12[iEta][iPhi][iZ]>h_RMS_EB_g12[iEta][iZ]->GetMean()+1.*h_RMS_EB_g12[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g12_1sigma->SetBinContent(h2_NoiseChannels_EB_g12_1sigma->FindBin(iPhi,iEta),noiseMap_g12[iEta][iPhi][iZ]/4.);    
                               if(noiseMap_g6[iEta][iPhi][iZ]>h_RMS_EB_g6[iEta][iZ]->GetMean()+1.*h_RMS_EB_g6[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g6_1sigma->SetBinContent(h2_NoiseChannels_EB_g6_1sigma->FindBin(iPhi,iEta),noiseMap_g6[iEta][iPhi][iZ]/4.); 
                               if(noiseMap_g1[iEta][iPhi][iZ]>h_RMS_EB_g1[iEta][iZ]->GetMean()+1.*h_RMS_EB_g1[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g1_1sigma->SetBinContent(h2_NoiseChannels_EB_g1_1sigma->FindBin(iPhi,iEta),noiseMap_g1[iEta][iPhi][iZ]/4.); 
                               if(noiseMap_g12[iEta][iPhi][iZ]>h_RMS_EB_g12[iEta][iZ]->GetMean()+2.*h_RMS_EB_g12[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g12_2sigma->SetBinContent(h2_NoiseChannels_EB_g12_2sigma->FindBin(iPhi,iEta),noiseMap_g12[iEta][iPhi][iZ]/4.);    
                               if(noiseMap_g6[iEta][iPhi][iZ]>h_RMS_EB_g6[iEta][iZ]->GetMean()+2.*h_RMS_EB_g6[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g6_2sigma->SetBinContent(h2_NoiseChannels_EB_g6_2sigma->FindBin(iPhi,iEta),noiseMap_g6[iEta][iPhi][iZ]/4.); 
                               if(noiseMap_g1[iEta][iPhi][iZ]>h_RMS_EB_g1[iEta][iZ]->GetMean()+2.*h_RMS_EB_g1[iEta][iZ]->GetRMS()) h2_NoiseChannels_EB_g1_2sigma->SetBinContent(h2_NoiseChannels_EB_g1_2sigma->FindBin(iPhi,iEta),noiseMap_g1[iEta][iPhi][iZ]/4.);  
                            }                  
                              
            }

    for(int iX = 1; iX<=100; iX++)
            for(int iY = 1; iY<=100; iY++)
                for(int iZ = 0; iZ<3; iZ++){
                         if(iZ == 1) continue;
                         if(noiseMap_g12[iX][iY][iZ-1]==0) continue;
                         int iR = eeId->GetEndcapRing(iX,iY,iZ);
                         if(noiseMap_g12[iX][iY][iZ-1]>h_RMS_EE_g12[iR][iZ]->GetMean()+1.*h_RMS_EE_g12[iR][iZ]->GetRMS()) h2_NoiseChannels_EE_g12_1sigma[iZ]->SetBinContent(h2_NoiseChannels_EE_g12_1sigma[iZ]->FindBin(iX,iY),noiseMap_g12[iX][iY][iZ-1]/4.);
                         if(noiseMap_g6[iX][iY][iZ-1]>h_RMS_EE_g6[iR][iZ]->GetMean()+1.*h_RMS_EE_g6[iR][iZ]->GetRMS()) h2_NoiseChannels_EE_g6_1sigma[iZ]->SetBinContent(h2_NoiseChannels_EE_g6_1sigma[iZ]->FindBin(iX,iY),noiseMap_g6[iX][iY][iZ-1]/4.);    
                         if(noiseMap_g1[iX][iY][iZ-1]>h_RMS_EE_g1[iR][iZ]->GetMean()+1.*h_RMS_EE_g1[iR][iZ]->GetRMS()) h2_NoiseChannels_EE_g1_1sigma[iZ]->SetBinContent(h2_NoiseChannels_EE_g1_1sigma[iZ]->FindBin(iX,iY),noiseMap_g1[iX][iY][iZ-1]/4.); 
                         if(noiseMap_g12[iX][iY][iZ-1]>h_RMS_EE_g12[iR][iZ]->GetMean()+2.*h_RMS_EE_g12[iR][iZ]->GetRMS()) h2_NoiseChannels_EE_g12_2sigma[iZ]->SetBinContent(h2_NoiseChannels_EE_g12_2sigma[iZ]->FindBin(iX,iY),noiseMap_g12[iX][iY][iZ-1]/4.);
                         if(noiseMap_g6[iX][iY][iZ-1]>h_RMS_EE_g6[iR][iZ]->GetMean()+2.*h_RMS_EE_g6[iR][iZ]->GetRMS()) h2_NoiseChannels_EE_g6_2sigma[iZ]->SetBinContent(h2_NoiseChannels_EE_g6_2sigma[iZ]->FindBin(iX,iY),noiseMap_g6[iX][iY][iZ-1]/4.);    
                         if(noiseMap_g1[iX][iY][iZ]>h_RMS_EE_g1[iR][iZ]->GetMean()+2.*h_RMS_EE_g1[iR][iZ]->GetRMS()) h2_NoiseChannels_EE_g1_2sigma[iZ]->SetBinContent(h2_NoiseChannels_EE_g1_2sigma[iZ]->FindBin(iX,iY),noiseMap_g1[iX][iY][iZ-1]/4.);    
                }


    for(int iEta = 0; iEta<100; iEta++)
        for(int iZ = 0; iZ<3; iZ++){

                         if(iZ == 1) continue;
                      
                         if(h_Ped_EB_g12[iEta][iZ]->GetEntries() != 0){
                            g_Ped_EB_g12[iZ]->SetPoint(iEta,iEta+1,h_Ped_EB_g12[iEta][iZ]->GetMean());
                            g_Ped_EB_g12[iZ]->SetPointError(iEta,0,h_Ped_EB_g12[iEta][iZ]->GetRMS());
                            h_Ped_EB_g12[iEta][iZ]->Write();
                         }
                         if(h_RMS_EB_g12[iEta][iZ]->GetEntries() != 0){
                            g_RMS_EB_g12[iZ]->SetPoint(iEta,iEta+1,h_RMS_EB_g12[iEta][iZ]->GetMean());
                            g_RMS_EB_g12[iZ]->SetPointError(iEta,0,h_RMS_EB_g12[iEta][iZ]->GetRMS());
                            h_RMS_EB_g12[iEta][iZ]->Write();
                            h_RMS_EB_g12[iEta][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EB_g12[iEta][iZ]->GetMean()+2.*h_RMS_EB_g12[iEta][iZ]->GetRMS());
                            g_RMS_EB_g12_2sigma[iZ]->SetPoint(iEta,iEta+1,h_RMS_EB_g12[iEta][iZ]->GetMean());
                            g_RMS_EB_g12_2sigma[iZ]->SetPointError(iEta,0,h_RMS_EB_g12[iEta][iZ]->GetRMS());
                            h_RMS_EB_g12[iEta][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EB_g12[iEta][iZ]->GetMean()+1.*h_RMS_EB_g12[iEta][iZ]->GetRMS());
                            g_RMS_EB_g12_1sigma[iZ]->SetPoint(iEta,iEta+1,h_RMS_EB_g12[iEta][iZ]->GetMean());
                            g_RMS_EB_g12_1sigma[iZ]->SetPointError(iEta,0,h_RMS_EB_g12[iEta][iZ]->GetRMS());
                         }
                         if(h_Ped_EB_g6[iEta][iZ]->GetEntries() != 0){
                            g_Ped_EB_g6[iZ]->SetPoint(iEta,iEta+1,h_Ped_EB_g6[iEta][iZ]->GetMean());
                            g_Ped_EB_g6[iZ]->SetPointError(iEta,0,h_Ped_EB_g6[iEta][iZ]->GetRMS());
                            h_Ped_EB_g6[iEta][iZ]->Write();
                         }
                         if(h_RMS_EB_g6[iEta][iZ]->GetEntries() != 0){
                            g_RMS_EB_g6[iZ]->SetPoint(iEta,iEta+1,h_RMS_EB_g6[iEta][iZ]->GetMean());
                            g_RMS_EB_g6[iZ]->SetPointError(iEta,0,h_RMS_EB_g6[iEta][iZ]->GetRMS());
                            h_RMS_EB_g6[iEta][iZ]->Write();
                            h_RMS_EB_g6[iEta][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EB_g6[iEta][iZ]->GetMean()+2.*h_RMS_EB_g6[iEta][iZ]->GetRMS());
                            g_RMS_EB_g6_2sigma[iZ]->SetPoint(iEta,iEta+1,h_RMS_EB_g6[iEta][iZ]->GetMean());
                            g_RMS_EB_g6_2sigma[iZ]->SetPointError(iEta,0,h_RMS_EB_g6[iEta][iZ]->GetRMS());
                            h_RMS_EB_g6[iEta][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EB_g6[iEta][iZ]->GetMean()+1.*h_RMS_EB_g6[iEta][iZ]->GetRMS());
                            g_RMS_EB_g6_1sigma[iZ]->SetPoint(iEta,iEta+1,h_RMS_EB_g6[iEta][iZ]->GetMean());
                            g_RMS_EB_g6_1sigma[iZ]->SetPointError(iEta,0,h_RMS_EB_g6[iEta][iZ]->GetRMS());
                         }
                         if(h_Ped_EB_g1[iEta][iZ]->GetEntries() != 0){
                            g_Ped_EB_g1[iZ]->SetPoint(iEta,iEta+1,h_Ped_EB_g1[iEta][iZ]->GetMean());
                            g_Ped_EB_g1[iZ]->SetPointError(iEta,0,h_Ped_EB_g1[iEta][iZ]->GetRMS());
                            h_Ped_EB_g1[iEta][iZ]->Write();
                         }
                         if(h_RMS_EB_g1[iEta][iZ]->GetEntries() != 0){
                            g_RMS_EB_g1[iZ]->SetPoint(iEta,iEta+1,h_RMS_EB_g1[iEta][iZ]->GetMean());
                            g_RMS_EB_g1[iZ]->SetPointError(iEta,0,h_RMS_EB_g1[iEta][iZ]->GetRMS());
                            h_RMS_EB_g1[iEta][iZ]->Write();
                            h_RMS_EB_g1[iEta][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EB_g1[iEta][iZ]->GetMean()+2.*h_RMS_EB_g1[iEta][iZ]->GetRMS());
                            g_RMS_EB_g1_2sigma[iZ]->SetPoint(iEta,iEta+1,h_RMS_EB_g1[iEta][iZ]->GetMean());
                            g_RMS_EB_g1_2sigma[iZ]->SetPointError(iEta,0,h_RMS_EB_g1[iEta][iZ]->GetRMS());
                            h_RMS_EB_g1[iEta][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EB_g1[iEta][iZ]->GetMean()+1.*h_RMS_EB_g1[iEta][iZ]->GetRMS());
                            g_RMS_EB_g1_1sigma[iZ]->SetPoint(iEta,iEta+1,h_RMS_EB_g1[iEta][iZ]->GetMean());
                            g_RMS_EB_g1_1sigma[iZ]->SetPointError(iEta,0,h_RMS_EB_g1[iEta][iZ]->GetRMS());
                         }
        }
               
    for(int iR = 0; iR<40; iR++)
        for(int iZ = 0; iZ<3; iZ++){

                         if(iZ == 1) continue;

                         if(h_Ped_EE_g12[iR][iZ]->GetEntries() != 0){
                            g_Ped_EE_g12[iZ]->SetPoint(iR,iR,h_Ped_EE_g12[iR][iZ]->GetMean());
                            g_Ped_EE_g12[iZ]->SetPointError(iR,0,h_Ped_EE_g12[iR][iZ]->GetRMS());
                            h_Ped_EE_g12[iR][iZ]->Write();
                         }
                         if(h_RMS_EE_g12[iR][iZ]->GetEntries() != 0){
                            g_RMS_EE_g12[iZ]->SetPoint(iR,iR,h_RMS_EE_g12[iR][iZ]->GetMean());
                            g_RMS_EE_g12[iZ]->SetPointError(iR,0,h_RMS_EE_g12[iR][iZ]->GetRMS());
                            h_RMS_EE_g12[iR][iZ]->Write();
                            h_RMS_EE_g12[iR][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EE_g12[iR][iZ]->GetMean()+2.*h_RMS_EE_g12[iR][iZ]->GetRMS());
                            g_RMS_EE_g12_2sigma[iZ]->SetPoint(iR,iR,h_RMS_EE_g12[iR][iZ]->GetMean());
                            g_RMS_EE_g12_2sigma[iZ]->SetPointError(iR,0,h_RMS_EE_g12[iR][iZ]->GetRMS());
                            h_RMS_EE_g12[iR][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EE_g12[iR][iZ]->GetMean()+1.*h_RMS_EE_g12[iR][iZ]->GetRMS());
                            g_RMS_EE_g12_1sigma[iZ]->SetPoint(iR,iR,h_RMS_EE_g12[iR][iZ]->GetMean());
                            g_RMS_EE_g12_1sigma[iZ]->SetPointError(iR,0,h_RMS_EE_g12[iR][iZ]->GetRMS());
                         }
                         if(h_Ped_EE_g6[iR][iZ]->GetEntries() != 0){
                            g_Ped_EE_g6[iZ]->SetPoint(iR,iR,h_Ped_EE_g6[iR][iZ]->GetMean());
                            g_Ped_EE_g6[iZ]->SetPointError(iR,0,h_Ped_EE_g6[iR][iZ]->GetRMS());
                            h_Ped_EE_g6[iR][iZ]->Write();
                         }
                         if(h_RMS_EE_g6[iR][iZ]->GetEntries() != 0){
                            g_RMS_EE_g6[iZ]->SetPoint(iR,iR,h_RMS_EE_g6[iR][iZ]->GetMean());
                            g_RMS_EE_g6[iZ]->SetPointError(iR,0,h_RMS_EE_g6[iR][iZ]->GetRMS());
                            h_RMS_EE_g6[iR][iZ]->Write();
                            h_RMS_EE_g6[iR][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EE_g6[iR][iZ]->GetMean()+2.*h_RMS_EE_g6[iR][iZ]->GetRMS());
                            g_RMS_EE_g6_2sigma[iZ]->SetPoint(iR,iR,h_RMS_EE_g6[iR][iZ]->GetMean());
                            g_RMS_EE_g6_2sigma[iZ]->SetPointError(iR,0,h_RMS_EE_g6[iR][iZ]->GetRMS());
                            h_RMS_EE_g6[iR][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EE_g6[iR][iZ]->GetMean()+1.*h_RMS_EE_g6[iR][iZ]->GetRMS());
                            g_RMS_EE_g6_1sigma[iZ]->SetPoint(iR,iR,h_RMS_EE_g6[iR][iZ]->GetMean());
                            g_RMS_EE_g6_1sigma[iZ]->SetPointError(iR,0,h_RMS_EE_g6[iR][iZ]->GetRMS());
                         }
                         if(h_Ped_EE_g1[iR][iZ]->GetEntries() != 0){
                            g_Ped_EE_g1[iZ]->SetPoint(iR,iR,h_Ped_EE_g1[iR][iZ]->GetMean());
                            g_Ped_EE_g1[iZ]->SetPointError(iR,0,h_Ped_EE_g1[iR][iZ]->GetRMS());
                            h_Ped_EE_g1[iR][iZ]->Write();
                         }
                         if(h_RMS_EE_g1[iR][iZ]->GetEntries() != 0){
                            g_RMS_EE_g1[iZ]->SetPoint(iR,iR,h_RMS_EE_g1[iR][iZ]->GetMean());
                            g_RMS_EE_g1[iZ]->SetPointError(iR,0,h_RMS_EE_g1[iR][iZ]->GetRMS());
                            h_RMS_EE_g1[iR][iZ]->Write();
                            h_RMS_EE_g1[iR][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EE_g1[iR][iZ]->GetMean()+2.*h_RMS_EE_g1[iR][iZ]->GetRMS());
                            g_RMS_EE_g1_2sigma[iZ]->SetPoint(iR,iR,h_RMS_EE_g1[iR][iZ]->GetMean());
                            g_RMS_EE_g1_2sigma[iZ]->SetPointError(iR,0,h_RMS_EE_g1[iR][iZ]->GetRMS());
                            h_RMS_EE_g1[iR][iZ]->GetXaxis()->SetRangeUser(0.,h_RMS_EE_g1[iR][iZ]->GetMean()+1.*h_RMS_EE_g1[iR][iZ]->GetRMS());
                            g_RMS_EE_g1_1sigma[iZ]->SetPoint(iR,iR,h_RMS_EE_g1[iR][iZ]->GetMean());
                            g_RMS_EE_g1_1sigma[iZ]->SetPointError(iR,0,h_RMS_EE_g1[iR][iZ]->GetRMS());
                         }
        }
 
    g_Ped_EB_g12[0]->Write("g_Ped_EB_g12_M");               
    g_Ped_EB_g12[2]->Write("g_Ped_EB_g12_P");               
    g_Ped_EB_g6[0]->Write("g_Ped_EB_g6_M");
    g_Ped_EB_g6[2]->Write("g_Ped_EB_g6_P");
    g_Ped_EB_g1[0]->Write("g_Ped_EB_g1_M");
    g_Ped_EB_g1[2]->Write("g_Ped_EB_g1_P");
    
    g_RMS_EB_g12[0]->Write("g_RMS_EB_g12_M");           
    g_RMS_EB_g12[2]->Write("g_RMS_EB_g12_P");
    g_RMS_EB_g6[0]->Write("g_RMS_EB_g6_M");
    g_RMS_EB_g6[2]->Write("g_RMS_EB_g6_P");
    g_RMS_EB_g1[0]->Write("g_RMS_EB_g1_M");
    g_RMS_EB_g1[2]->Write("g_RMS_EB_g1_P");

    g_RMS_EB_g12_1sigma[0]->Write("g_RMS_EB_g12_1sigma_M");           
    g_RMS_EB_g12_1sigma[2]->Write("g_RMS_EB_g12_1sigma_P");
    g_RMS_EB_g6_1sigma[0]->Write("g_RMS_EB_g6_1sigma_M");
    g_RMS_EB_g6_1sigma[2]->Write("g_RMS_EB_g6_1sigma_P");
    g_RMS_EB_g1_1sigma[0]->Write("g_RMS_EB_g1_1sigma_M");
    g_RMS_EB_g1_1sigma[2]->Write("g_RMS_EB_g1_1sigma_P");
    
    g_RMS_EB_g12_2sigma[0]->Write("g_RMS_EB_g12_2sigma_M");           
    g_RMS_EB_g12_2sigma[2]->Write("g_RMS_EB_g12_2sigma_P");
    g_RMS_EB_g6_2sigma[0]->Write("g_RMS_EB_g6_2sigma_M");
    g_RMS_EB_g6_2sigma[2]->Write("g_RMS_EB_g6_2sigma_P");
    g_RMS_EB_g1_2sigma[0]->Write("g_RMS_EB_g1_2sigma_M");
    g_RMS_EB_g1_2sigma[2]->Write("g_RMS_EB_g1_2sigma_P");

    g_Ped_EE_g12[0]->Write("g_Ped_EE_g12_M");               
    g_Ped_EE_g12[2]->Write("g_Ped_EE_g12_P");               
    g_Ped_EE_g6[0]->Write("g_Ped_EE_g6_M");
    g_Ped_EE_g6[2]->Write("g_Ped_EE_g6_P");
    g_Ped_EE_g1[0]->Write("g_Ped_EE_g1_M");
    g_Ped_EE_g1[2]->Write("g_Ped_EE_g1_P");

    g_RMS_EE_g12[0]->Write("g_RMS_EE_g12_M");           
    g_RMS_EE_g12[2]->Write("g_RMS_EE_g12_P");
    g_RMS_EE_g6[0]->Write("g_RMS_EE_g6_M");
    g_RMS_EE_g6[2]->Write("g_RMS_EE_g6_P");
    g_RMS_EE_g1[0]->Write("g_RMS_EE_g1_M");
    g_RMS_EE_g1[2]->Write("g_RMS_EE_g1_P");    

    g_RMS_EE_g12_1sigma[0]->Write("g_RMS_EE_g12_1sigma_M");               
    g_RMS_EE_g12_1sigma[2]->Write("g_RMS_EE_g12_1sigma_P");               
    g_RMS_EE_g6_1sigma[0]->Write("g_RMS_EE_g6_1sigma_M");
    g_RMS_EE_g6_1sigma[2]->Write("g_RMS_EE_g6_1sigma_P");
    g_RMS_EE_g1_1sigma[0]->Write("g_RMS_EE_g1_1sigma_M");
    g_RMS_EE_g1_1sigma[2]->Write("g_RMS_EE_g1_1sigma_P");

    g_RMS_EE_g12_2sigma[0]->Write("g_RMS_EE_g12_2sigma_M");               
    g_RMS_EE_g12_2sigma[2]->Write("g_RMS_EE_g12_2sigma_P");               
    g_RMS_EE_g6_2sigma[0]->Write("g_RMS_EE_g6_2sigma_M");
    g_RMS_EE_g6_2sigma[2]->Write("g_RMS_EE_g6_2sigma_P");
    g_RMS_EE_g1_2sigma[0]->Write("g_RMS_EE_g1_2sigma_M");
    g_RMS_EE_g1_2sigma[2]->Write("g_RMS_EE_g1_2sigma_P");

    h2_NoiseChannels_EB_g12_1sigma->Write("h2_NoiseChannels_EB_g12_1sigma");
    h2_NoiseChannels_EB_g6_1sigma->Write("h2_NoiseChannels_EB_g6_1sigma");
    h2_NoiseChannels_EB_g1_1sigma->Write("h2_NoiseChannels_EB_g1_1sigma");

    h2_NoiseChannels_EE_g12_1sigma[0]->Write("h2_NoiseChannels_EE_g12_1sigma_M");
    h2_NoiseChannels_EE_g6_1sigma[0]->Write("h2_NoiseChannels_EE_g6_1sigma_M");
    h2_NoiseChannels_EE_g1_1sigma[0]->Write("h2_NoiseChannels_EE_g1_1sigma_M");
    
    h2_NoiseChannels_EE_g12_1sigma[2]->Write("h2_NoiseChannels_EE_g12_1sigma_P");
    h2_NoiseChannels_EE_g6_1sigma[2]->Write("h2_NoiseChannels_EE_g6_1sigma_P");
    h2_NoiseChannels_EE_g1_1sigma[2]->Write("h2_NoiseChannels_EE_g1_1sigma_P");

    h2_NoiseChannels_EB_g12_2sigma->Write("h2_NoiseChannels_EB_g12_2sigma");
    h2_NoiseChannels_EB_g6_2sigma->Write("h2_NoiseChannels_EB_g6_2sigma");
    h2_NoiseChannels_EB_g1_2sigma->Write("h2_NoiseChannels_EB_g1_2sigma");

    h2_NoiseChannels_EE_g12_2sigma[0]->Write("h2_NoiseChannels_EE_g12_2sigma_M");
    h2_NoiseChannels_EE_g6_2sigma[0]->Write("h2_NoiseChannels_EE_g6_2sigma_M");
    h2_NoiseChannels_EE_g1_2sigma[0]->Write("h2_NoiseChannels_EE_g1_2sigma_M");
    
    h2_NoiseChannels_EE_g12_2sigma[2]->Write("h2_NoiseChannels_EE_g12_2sigma_P");
    h2_NoiseChannels_EE_g6_2sigma[2]->Write("h2_NoiseChannels_EE_g6_2sigma_P");
    h2_NoiseChannels_EE_g1_2sigma[2]->Write("h2_NoiseChannels_EE_g1_2sigma_P");


    myOutFile->Close(); 

  }
}

std::vector<std::string> splitString(std::string& inputName, char split_char)
{
    std::vector<std::string> tokens;
    std::istringstream split(inputName);
    for(std::string each; getline(split, each, split_char); tokens.push_back(each));
    return tokens;
}


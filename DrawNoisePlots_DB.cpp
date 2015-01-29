/*************************************************************
simple program to read bin files
compile with ---> c++ -o DrawNoisePlots_DB `root-config --cflags --glibs` DrawNoisePlots_DB.cpp TimeUtils.cc
or with ---> c++ -o DrawNoisePlots_DB.cpp DrawNoisePlots_DB `root-config --cflags --glibs` 
run with ---> ./DrawNoisePlots_DB dump_list_root.txt runinfo_start_31X_hlt.dat g12
*************************************************************/

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <map>
#include "TFile.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TH2D.h"

#include "TROOT.h"
#include "TStyle.h"

#include "TimeUtils.h"

std::string timeStampToHReadble(int  timestamp);

void DrawPlot(std::map<int,TGraphErrors*> g1, std::string EBEE, std::string PM, std::string Gain, std::vector<int> runId);

int main(int argc, char** argv)
{
    gStyle->SetOptStat(0000); 

    char* inputLIST = argv[1];
    char* inputFILE = argv[2];
    char* gain = argv[3];
    
    std::string inputList = std::string(inputLIST);
    std::string inputFile = std::string(inputFILE);
    std::string Gain = std::string(gain);
    
    std::cout << "inputList = " << inputList << std::endl;
    std::cout << "inputFile = " << inputFile << std::endl;
    std::cout << "gain      = " << Gain << std::endl;
    
    std::vector<std::string> inputFiles;

    char dumps[500];
    FILE *f_dumps;
    f_dumps = fopen(inputList.c_str(),"r");
    while(fscanf(f_dumps,"%s \n", dumps) !=EOF ){
          //std::cout << "Reading inputFile: " << dumps << std::endl;
          std::string DUMPS = std::string(dumps);
          if(DUMPS.find("#") != std::string::npos) continue;
          inputFiles.push_back(DUMPS);
    }

    int nFiles = inputFiles.size();
    std::vector<int> runId;
    
    std::map<int,TGraphErrors*> g_EBM;
    std::map<int,TGraphErrors*> g_EBP;
    std::map<int,TGraphErrors*> g_EEM;
    std::map<int,TGraphErrors*> g_EEP;

    TDate* date = new TDate(inputFile); 

  for(unsigned int ii = 0; ii < inputFiles.size(); ii++){

    std::cout << "Reading inputFile: " << inputFiles.at(ii)  << std::endl;
    
    std::string inputName = inputFiles.at(ii);
    char split_char = '/';
    std::vector<std::string> Tokens;
    std::istringstream split(inputName);
    for(std::string each; getline(split, each, split_char); Tokens.push_back(each));

    inputName = Tokens.at(Tokens.size()-1);
    split_char = '_';
    std::vector<std::string> tokens;
    std::istringstream splits(inputName);
    for(std::string each; getline(splits, each, split_char); tokens.push_back(each));

    TFile *inFile = TFile::Open(inputFiles.at(ii).c_str());

    g_EBM[ii] = (TGraphErrors*)inFile->Get(("g_RMS_EB_"+Gain+"_M").c_str());
    g_EBP[ii] = (TGraphErrors*)inFile->Get(("g_RMS_EB_"+Gain+"_P").c_str());
    g_EEM[ii] = (TGraphErrors*)inFile->Get(("g_RMS_EE_"+Gain+"_M").c_str());
    g_EEP[ii] = (TGraphErrors*)inFile->Get(("g_RMS_EE_"+Gain+"_P").c_str());
    
    
    int run = atoi(tokens.at(2).c_str());
    int time = date->dateToInt(run);
    runId.push_back(time);

  }

  DrawPlot(g_EBM,"EB","M",Gain,runId);
  DrawPlot(g_EBP,"EB","P",Gain,runId);
  DrawPlot(g_EEM,"EE","M",Gain,runId);
  DrawPlot(g_EEM,"EE","P",Gain,runId);
  
}

std::string timeStampToHReadble(int  timestamp)
{
    const time_t rawtime = (const time_t)timestamp;

    struct tm * dt;
    char timestr[30];
    char buffer [30];

    dt = localtime(&rawtime);
    // use any strftime format spec here
    strftime(timestr, sizeof(timestr), " %H:%M:%S %d/%m/%Y", dt);
    sprintf(buffer,"%s", timestr);
    std::string stdBuffer(buffer);
    return stdBuffer;
}

void DrawPlot(std::map<int,TGraphErrors*> g1, std::string EBEE, std::string PM, std::string Gain, std::vector<int> runId){

    float irmsEB[201];
    float irmsEE[101];
    
    if(EBEE == "EB"){
     for(int ii = 0; ii<201;ii++)
        irmsEB[ii] = 0.8+ii*0.005;
    }else{
     for(int ii = 0; ii<101;ii++)
        irmsEE[ii] = 1+ii*0.03;
    }

    std::sort(runId.begin(),runId.end());
    int dim = runId.size();
    float run[dim];
    for(unsigned int ii = 0; ii < runId.size(); ii++)
        run[ii] = float(runId.at(ii));

    TH2D* noiseMap;
    if(EBEE == "EB") noiseMap = new TH2D("noiseMap",("noise_"+EBEE+PM).c_str(),dim-1,run,200,irmsEB);
    else noiseMap = new TH2D("noiseMap",("noise_"+EBEE+PM).c_str(),dim-1,run,100,irmsEE);
    
    double x,y;
    for(unsigned int ii = 0; ii < g1.size(); ii++){
        for(int jj = 0; jj < g1[ii]->GetN(); jj++){
            g1[ii]->GetPoint(jj,x,y);
            y=y/4.;
            noiseMap->SetBinContent(noiseMap->FindBin(run[ii],y),x);
        }
    }

    noiseMap->GetXaxis()->SetTitle("date");   
    noiseMap->GetXaxis()->SetTimeFormat("%d/%m%F1970-01-01 00:00:00"); 
    noiseMap->GetXaxis()->SetTimeDisplay(1);
    noiseMap->GetYaxis()->SetTitle("ADC counts");    
    if(EBEE == "EB") noiseMap->GetZaxis()->SetTitle("i#eta");   
    else noiseMap->GetZaxis()->SetTitle("iRing"); 
    noiseMap->GetZaxis()-> SetTitleOffset(0.5);

    TCanvas* c1 = new TCanvas("c1","c1");
    c1 -> cd();
    noiseMap->Draw("COLZ");

    c1 -> Print(("Noise_DB_"+EBEE+PM+"_"+Gain+".png").c_str(),"png");
    c1 -> Print(("Noise_DB_"+EBEE+PM+"_"+Gain+".pdf").c_str(),"pdf");
    
    delete c1;
    delete noiseMap;
}





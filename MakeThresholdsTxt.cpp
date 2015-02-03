/*************************************************************
simple program to read bin files
compile with ---> c++ -o MakeThresholdsTxt `root-config --cflags --glibs` MakeThresholdsTxt.cpp geometryUtils.cc
or with ---> c++ -o MakeThresholdsTxt.cpp MakeThresholdsTxt `root-config --cflags --glibs` geometryUtils.cc
run with ---> ./MakeThresholdsTxt dump_list_root.txt g12 
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
#include "TH2F.h"
#include "TROOT.h"
#include "TStyle.h"

#include "geometryUtils.h"

std::vector<std::string> splitString(std::string& inputName, char split_char);

void drawGraphs(TGraphErrors* g1,TGraphErrors* g2,TGraphErrors* g3, std::string Title);

void drawNoisMap(TH2F* h,std::string Title,std::string Sigma,std::string Side);

void drawHist(TH1F* h,std::string Title);

int main(int argc, char** argv)
{
   
  char* inputLIST = argv[1];
  char* gain = argv[2];

  std::string inputList = std::string(inputLIST);
  std::string Gain = std::string(gain);
    
  std::cout << "inputList = " << inputList << std::endl;
  std::cout << "Gain      = " << Gain << std::endl;

  std::vector<std::string> inputFiles;

  char dumps[500];
  FILE *f_dumps;
  f_dumps = fopen(inputList.c_str(),"r");
  while(fscanf(f_dumps,"%s \n", dumps) !=EOF ){
        std::string DUMPS = std::string(dumps);
        if(DUMPS.find("#") != std::string::npos) continue;
        inputFiles.push_back(DUMPS);
  }

  TGraphErrors* g_EBM;
  TGraphErrors* g_EBP;
  TGraphErrors* g_EEM;
  TGraphErrors* g_EEP;
  TGraphErrors* g_EBM_1sigma;
  TGraphErrors* g_EBP_1sigma;
  TGraphErrors* g_EEM_1sigma;
  TGraphErrors* g_EEP_1sigma;
  TGraphErrors* g_EBM_2sigma;
  TGraphErrors* g_EBP_2sigma;
  TGraphErrors* g_EEM_2sigma;
  TGraphErrors* g_EEP_2sigma;

  TH2F* h2_NoiseChannels_EB_1sigma;
  TH2F* h2_NoiseChannels_EB_2sigma;
  TH2F* h2_NoiseChannels_EE_1sigma_M;
  TH2F* h2_NoiseChannels_EE_2sigma_M;
  TH2F* h2_NoiseChannels_EE_1sigma_P;
  TH2F* h2_NoiseChannels_EE_2sigma_P;

  TH1F* h_RMS_EE_37_P;
  TH1F* h_RMS_EE_37_M;
  TH1F* h_RMS_EE_38_P;
  TH1F* h_RMS_EE_38_M;
  TH1F* h_RMS_EE_39_P;
  TH1F* h_RMS_EE_39_M;

  int runB, runE;
  TEndcapRegions* eeId = new TEndcapRegions();

  std::map<int,float> thresEBEE;
  std::map<int,float> thresRMSEBEE;
  std::map<int,float> thresEBEE_1sigma;
  std::map<int,float> thresRMSEBEE_1sigma;
  std::map<int,float> thresEBEE_2sigma;
  std::map<int,float> thresRMSEBEE_2sigma;
  
  for(unsigned int ii = 0; ii < inputFiles.size(); ii++){

    std::cout << "Reading inputFile: " << inputFiles.at(ii)  << std::endl;
    
    std::string inputName = splitString(inputFiles.at(ii),'/').at(splitString(inputFiles.at(ii),'/').size()-1);
    std::vector<std::string> tokens  = splitString(inputName,'_');
    tokens.at(3).erase(tokens.at(3).size()-5,tokens.at(3).size());
    runB = atoi(tokens.at(2).c_str());
    runE = atoi(tokens.at(3).c_str());

    TFile *inFile = TFile::Open(inputFiles.at(ii).c_str());

    g_EBM = (TGraphErrors*)inFile->Get(("g_RMS_EB_"+Gain+"_M").c_str());
    g_EBP = (TGraphErrors*)inFile->Get(("g_RMS_EB_"+Gain+"_P").c_str());
    g_EEM = (TGraphErrors*)inFile->Get(("g_RMS_EE_"+Gain+"_M").c_str());
    g_EEP = (TGraphErrors*)inFile->Get(("g_RMS_EE_"+Gain+"_P").c_str());
    g_EBM_1sigma = (TGraphErrors*)inFile->Get(("g_RMS_EB_"+Gain+"_1sigma_M").c_str());
    g_EBP_1sigma = (TGraphErrors*)inFile->Get(("g_RMS_EB_"+Gain+"_1sigma_P").c_str());
    g_EEM_1sigma = (TGraphErrors*)inFile->Get(("g_RMS_EE_"+Gain+"_1sigma_M").c_str());
    g_EEP_1sigma = (TGraphErrors*)inFile->Get(("g_RMS_EE_"+Gain+"_1sigma_P").c_str());
    g_EBM_2sigma = (TGraphErrors*)inFile->Get(("g_RMS_EB_"+Gain+"_2sigma_M").c_str());
    g_EBP_2sigma = (TGraphErrors*)inFile->Get(("g_RMS_EB_"+Gain+"_2sigma_P").c_str());
    g_EEM_2sigma = (TGraphErrors*)inFile->Get(("g_RMS_EE_"+Gain+"_2sigma_M").c_str());
    g_EEP_2sigma = (TGraphErrors*)inFile->Get(("g_RMS_EE_"+Gain+"_2sigma_P").c_str());

    h2_NoiseChannels_EB_1sigma = (TH2F*)inFile->Get(("h2_NoiseChannels_EB_"+Gain+"_1sigma").c_str());
    h2_NoiseChannels_EB_2sigma = (TH2F*)inFile->Get(("h2_NoiseChannels_EB_"+Gain+"_2sigma").c_str());
    h2_NoiseChannels_EE_1sigma_M = (TH2F*)inFile->Get(("h2_NoiseChannels_EE_"+Gain+"_1sigma_M").c_str());
    h2_NoiseChannels_EE_2sigma_M = (TH2F*)inFile->Get(("h2_NoiseChannels_EE_"+Gain+"_2sigma_M").c_str());
    h2_NoiseChannels_EE_1sigma_P = (TH2F*)inFile->Get(("h2_NoiseChannels_EE_"+Gain+"_1sigma_P").c_str());
    h2_NoiseChannels_EE_2sigma_P = (TH2F*)inFile->Get(("h2_NoiseChannels_EE_"+Gain+"_2sigma_P").c_str());

    h_RMS_EE_37_P = (TH1F*)inFile->Get(("h_RMS_EE_"+Gain+"_37_1").c_str());
    h_RMS_EE_37_M = (TH1F*)inFile->Get(("h_RMS_EE_"+Gain+"_37_-1").c_str());
    h_RMS_EE_38_P = (TH1F*)inFile->Get(("h_RMS_EE_"+Gain+"_38_1").c_str());
    h_RMS_EE_38_M = (TH1F*)inFile->Get(("h_RMS_EE_"+Gain+"_38_-1").c_str());
    h_RMS_EE_39_P = (TH1F*)inFile->Get(("h_RMS_EE_"+Gain+"_39_1").c_str());
    h_RMS_EE_39_M = (TH1F*)inFile->Get(("h_RMS_EE_"+Gain+"_39_-1").c_str());

    TGraphErrors* g_Thres_EBM = new TGraphErrors();
    TGraphErrors* g_Thres_EBP = new TGraphErrors(); 
    TGraphErrors* g_Thres_EEM = new TGraphErrors();
    TGraphErrors* g_Thres_EEP = new TGraphErrors();
    TGraphErrors* g_Thres_EBM_1sigma = new TGraphErrors();
    TGraphErrors* g_Thres_EBP_1sigma = new TGraphErrors(); 
    TGraphErrors* g_Thres_EEM_1sigma = new TGraphErrors();
    TGraphErrors* g_Thres_EEP_1sigma = new TGraphErrors();
    TGraphErrors* g_Thres_EBM_2sigma = new TGraphErrors();
    TGraphErrors* g_Thres_EBP_2sigma = new TGraphErrors(); 
    TGraphErrors* g_Thres_EEM_2sigma = new TGraphErrors();
    TGraphErrors* g_Thres_EEP_2sigma = new TGraphErrors();
    
    std::string Title = "thresholds_"+tokens.at(2)+"_"+tokens.at(3)+"_"+Gain;
    std::string TitleNoise = "NoiseChannels_"+tokens.at(2)+"_"+tokens.at(3)+"_"+Gain;
    std::string TitleHistP = "RMS_EEP_"+tokens.at(2)+"_"+tokens.at(3)+"_"+Gain;
    std::string TitleHistM = "RMS_EEM_"+tokens.at(2)+"_"+tokens.at(3)+"_"+Gain;
    std::fstream thresholdsTxt;
    thresholdsTxt.open ((Title+".txt").c_str(), std::fstream::in | std::fstream::out | std::fstream::app);

    double x,y;
    int ir;
    
    thresEBEE.clear();
    thresRMSEBEE.clear();
    thresEBEE_1sigma.clear();
    thresRMSEBEE_1sigma.clear();
    thresEBEE_2sigma.clear();
    thresRMSEBEE_2sigma.clear();
    //Fill EB-
    for(int jj = 0; jj < g_EBM->GetN(); jj++){
        g_EBM->GetPoint(jj,x,y);
        thresEBEE[int(-x)]=float(y);
        thresRMSEBEE[int(-x)]=float(g_EBM->GetErrorY(jj));
    }
    for(int jj = 0; jj < g_EBM_1sigma->GetN(); jj++){
        g_EBM_1sigma->GetPoint(jj,x,y);
        thresEBEE_1sigma[int(-x)]=float(y);
        thresRMSEBEE_1sigma[int(-x)]=float(g_EBM_1sigma->GetErrorY(jj));
    }
    for(int jj = 0; jj < g_EBM_2sigma->GetN(); jj++){
        g_EBM_2sigma->GetPoint(jj,x,y);
        thresEBEE_2sigma[int(-x)]=float(y);
        thresRMSEBEE_2sigma[int(-x)]=float(g_EBM_2sigma->GetErrorY(jj));
    }
    for(int ieta = -85; ieta < 0; ieta++){
        for(int iphi = 1; iphi < 361; iphi++)
            thresholdsTxt << ieta << " " << iphi << " 0 " << thresEBEE_2sigma[ieta]+3*thresRMSEBEE_2sigma[ieta] << std::endl; 
        g_Thres_EBM->SetPoint(ieta+85,ieta,thresEBEE[ieta]+3*thresRMSEBEE[ieta]);
        g_Thres_EBM_1sigma->SetPoint(ieta+85,ieta,thresEBEE_1sigma[ieta]+3*thresRMSEBEE_1sigma[ieta]);
        g_Thres_EBM_2sigma->SetPoint(ieta+85,ieta,thresEBEE_2sigma[ieta]+3*thresRMSEBEE_2sigma[ieta]);
    } 

    thresEBEE.clear();
    thresRMSEBEE.clear();
    thresEBEE_1sigma.clear();
    thresRMSEBEE_1sigma.clear();
    thresEBEE_2sigma.clear();
    thresRMSEBEE_2sigma.clear();
    //Fill EB+
    for(int jj = 0; jj < g_EBP->GetN(); jj++){
        g_EBP->GetPoint(jj,x,y);
        thresEBEE[int(x)]=float(y);
        thresRMSEBEE[int(x)]=float(g_EBP->GetErrorY(jj));
    }
    for(int jj = 0; jj < g_EBP_1sigma->GetN(); jj++){
        g_EBP_1sigma->GetPoint(jj,x,y);
        thresEBEE_1sigma[int(x)]=float(y);
        thresRMSEBEE_1sigma[int(x)]=float(g_EBP_1sigma->GetErrorY(jj));
    }
    for(int jj = 0; jj < g_EBP_2sigma->GetN(); jj++){
        g_EBP_2sigma->GetPoint(jj,x,y);
        thresEBEE_2sigma[int(x)]=float(y);
        thresRMSEBEE_2sigma[int(x)]=float(g_EBP_2sigma->GetErrorY(jj));
    }
    for(int ieta = 1; ieta < 86; ieta++){
        for(int iphi = 1; iphi < 361; iphi++)
            thresholdsTxt << ieta << " " << iphi << " 0 " << thresEBEE_2sigma[ieta]+3*thresRMSEBEE_2sigma[ieta] << std::endl; 
        g_Thres_EBP->SetPoint(ieta-1,ieta,thresEBEE[ieta]+3*thresRMSEBEE[ieta]);
        g_Thres_EBP_1sigma->SetPoint(ieta-1,ieta,thresEBEE_1sigma[ieta]+3*thresRMSEBEE_1sigma[ieta]);
        g_Thres_EBP_2sigma->SetPoint(ieta-1,ieta,thresEBEE_2sigma[ieta]+3*thresRMSEBEE_2sigma[ieta]);
    }

    std::vector<int> iRings;
    thresEBEE.clear();
    thresRMSEBEE.clear();
    thresEBEE_1sigma.clear();
    thresRMSEBEE_1sigma.clear();
    thresEBEE_2sigma.clear();
    thresRMSEBEE_2sigma.clear();
    //Fill EE-
    for(int jj = 0; jj < g_EEM->GetN(); jj++){
        g_EEM->GetPoint(jj,x,y);
        thresEBEE[int(x)]=float(y);
        thresRMSEBEE[int(x)]=float(g_EEM->GetErrorY(jj));
        iRings.push_back(int(x));
    }
    for(int jj = 0; jj < g_EEM_1sigma->GetN(); jj++){
        g_EEM_1sigma->GetPoint(jj,x,y);
        thresEBEE_1sigma[int(x)]=float(y);
        thresRMSEBEE_1sigma[int(x)]=float(g_EEM_1sigma->GetErrorY(jj));
    }
    for(int jj = 0; jj < g_EEM_2sigma->GetN(); jj++){
        g_EEM_2sigma->GetPoint(jj,x,y);
        thresEBEE_2sigma[int(x)]=float(y);
        thresRMSEBEE_2sigma[int(x)]=float(g_EEM_2sigma->GetErrorY(jj));
    }   
    for(int ix = 1; ix < 101; ix++)
        for(int iy = 1; iy < 101; iy++){
            ir = eeId->GetEndcapRing(ix,iy,-1);
            if(thresEBEE_2sigma[ir] != 0) thresholdsTxt << ix << " " << iy << " -1 " << thresEBEE_2sigma[ir]+3*thresRMSEBEE_2sigma[ir] << std::endl; 
        }  
    for(unsigned int ir = 0; ir < iRings.size(); ir++){
        g_Thres_EEM->SetPoint(ir,iRings.at(ir),thresEBEE[iRings.at(ir)]+3*thresRMSEBEE[iRings.at(ir)]);
        g_Thres_EEM_1sigma->SetPoint(ir,iRings.at(ir),thresEBEE_1sigma[iRings.at(ir)]+3*thresRMSEBEE_1sigma[iRings.at(ir)]);
        g_Thres_EEM_2sigma->SetPoint(ir,iRings.at(ir),thresEBEE_2sigma[iRings.at(ir)]+3*thresRMSEBEE_2sigma[iRings.at(ir)]);
    }
    
    iRings.clear();
    thresEBEE.clear();
    thresRMSEBEE.clear();
    thresEBEE_1sigma.clear();
    thresRMSEBEE_1sigma.clear();
    thresEBEE_2sigma.clear();
    thresRMSEBEE_2sigma.clear();
    //Fill EE+
    for(int jj = 0; jj < g_EEP->GetN(); jj++){
        g_EEP->GetPoint(jj,x,y);
        thresEBEE[int(x)]=float(y);
        thresRMSEBEE[int(x)]=float(g_EEP->GetErrorY(jj));
        iRings.push_back(int(x));
    }
    for(int jj = 0; jj < g_EEP_1sigma->GetN(); jj++){
        g_EEP_1sigma->GetPoint(jj,x,y);
        thresEBEE_1sigma[int(x)]=float(y);
        thresRMSEBEE_1sigma[int(x)]=float(g_EEP_1sigma->GetErrorY(jj));
    }
    for(int jj = 0; jj < g_EEP_2sigma->GetN(); jj++){
        g_EEP_2sigma->GetPoint(jj,x,y);
        thresEBEE_2sigma[int(x)]=float(y);
        thresRMSEBEE_2sigma[int(x)]=float(g_EEP_2sigma->GetErrorY(jj));
    }
    for(int ix = 1; ix < 101; ix++)
        for(int iy = 1; iy < 101; iy++){
            ir = eeId->GetEndcapRing(ix,iy,1);
            if(thresEBEE_2sigma[ir] != 0) thresholdsTxt << ix << " " << iy << " 1 " << thresEBEE_2sigma[ir]+3*thresRMSEBEE_2sigma[ir] << std::endl; 
        }

    for(unsigned int ir = 0; ir < iRings.size(); ir++){
        g_Thres_EEP->SetPoint(ir,iRings.at(ir),thresEBEE[iRings.at(ir)]+3*thresRMSEBEE[iRings.at(ir)]);
        g_Thres_EEP_1sigma->SetPoint(ir,iRings.at(ir),thresEBEE_1sigma[iRings.at(ir)]+3*thresRMSEBEE_1sigma[iRings.at(ir)]);
        g_Thres_EEP_2sigma->SetPoint(ir,iRings.at(ir),thresEBEE_2sigma[iRings.at(ir)]+3*thresRMSEBEE_2sigma[iRings.at(ir)]);
    }

    drawGraphs(g_Thres_EBM,g_Thres_EBM_1sigma,g_Thres_EBM_2sigma,std::string("EBM_"+Title));
    drawGraphs(g_Thres_EBP,g_Thres_EBP_1sigma,g_Thres_EBP_2sigma,std::string("EBP_"+Title));
    drawGraphs(g_Thres_EEM,g_Thres_EEM_1sigma,g_Thres_EEM_2sigma,std::string("EEM_"+Title));
    drawGraphs(g_Thres_EEP,g_Thres_EEP_1sigma,g_Thres_EEP_2sigma,std::string("EEP_"+Title));

    gStyle->SetOptStat(0000); 
    drawNoisMap(h2_NoiseChannels_EB_1sigma,TitleNoise,std::string("1sigma"),std::string("EB"));
    drawNoisMap(h2_NoiseChannels_EB_2sigma,TitleNoise,std::string("2sigma"),std::string("EB"));
    drawNoisMap(h2_NoiseChannels_EE_1sigma_M,TitleNoise,std::string("1sigma"),std::string("EEM"));
    drawNoisMap(h2_NoiseChannels_EE_2sigma_M,TitleNoise,std::string("2sigma"),std::string("EEM"));
    drawNoisMap(h2_NoiseChannels_EE_1sigma_M,TitleNoise,std::string("1sigma"),std::string("EEP"));
    drawNoisMap(h2_NoiseChannels_EE_2sigma_M,TitleNoise,std::string("2sigma"),std::string("EEP"));

    gStyle->SetOptStat(1110); 
    drawHist(h_RMS_EE_37_M,TitleHistM+std::string("_iRing37"));
    drawHist(h_RMS_EE_37_P,TitleHistP+std::string("_iRing37"));
    drawHist(h_RMS_EE_38_M,TitleHistM+std::string("_iRing38"));
    drawHist(h_RMS_EE_38_P,TitleHistP+std::string("_iRing38"));
    drawHist(h_RMS_EE_39_M,TitleHistM+std::string("_iRing39"));
    drawHist(h_RMS_EE_39_P,TitleHistP+std::string("_iRing39"));

  }
 
}

void drawGraphs(TGraphErrors* g1,TGraphErrors* g2,TGraphErrors* g3, std::string Title){
    
    g1 ->SetTitle(Title.c_str());
    g1 -> GetXaxis() -> SetLabelSize(0.04);
    g1 -> GetYaxis() -> SetLabelSize(0.04);
    g1 -> GetXaxis() -> SetTitleSize(0.05);
    g1 -> GetYaxis() -> SetTitleSize(0.05);
    g1 -> GetYaxis() -> SetTitleOffset(0.8);

    g1 -> GetXaxis() -> SetTitle("iRing");
    g1 -> GetYaxis() -> SetTitle("ADC counts");
   
    g1 -> SetMarkerStyle(20);
    g1 -> SetMarkerColor(kBlack);
    g1 -> SetLineColor(kBlack);
    g1 -> SetMarkerSize(0.5);
    g2 -> SetMarkerStyle(20);
    g2 -> SetMarkerColor(kBlue);
    g2 -> SetLineColor(kBlue);
    g2 -> SetLineWidth(2.);
    g3 -> SetMarkerSize(0.5);
    g3 -> SetMarkerStyle(20);
    g3 -> SetMarkerColor(kRed);
    g3 -> SetLineColor(kRed);
    g3 -> SetLineWidth(2.);
    g3 -> SetMarkerSize(0.5);

    TCanvas* c1 = new TCanvas("c1","c1");
    c1 -> cd();

    g1 -> Draw("APL");
    g2 -> Draw("same");
    g3 -> Draw("same");

    c1 -> Print((Title+".png").c_str(),"png");
    c1 -> Print((Title+".pdf").c_str(),"pdf");
    
    delete c1;

}

void drawNoisMap(TH2F* h,std::string Title,std::string Sigma,std::string Side){
    
    h -> GetXaxis() -> SetLabelSize(0.04);
    h -> GetYaxis() -> SetLabelSize(0.04);
    h -> GetZaxis() -> SetLabelSize(0.04);
    h -> GetZaxis() -> SetTitleSize(0.04);
    
    if(Side=="EB"){
       h -> GetXaxis() -> SetTitle("iPhi");
       h -> GetYaxis() -> SetTitle("iEta");
       h -> GetZaxis() -> SetRangeUser(0.,4.);
    }
    else if(Side=="EEM" || Side=="EEP"){
       h -> GetXaxis() -> SetTitle("iX");
       h -> GetYaxis() -> SetTitle("iY");
    }
    h -> GetZaxis() -> SetTitle("ADC counts"); 
    
    TCanvas* c1 = new TCanvas("c1","c1");
    c1 -> cd();

    h->Draw("COLZ");
       
    c1 -> Print((Side+"_"+Title+"_"+Sigma+".png").c_str(),"png");
    c1 -> Print((Side+"_"+Title+"_"+Sigma+".pdf").c_str(),"pdf");
    
    delete c1;

}

void drawHist(TH1F* h,std::string Title){
    
    h -> GetXaxis() -> SetLabelSize(0.04);
    h -> GetXaxis() -> SetTitle("ADC counts");
    
    TCanvas* c1 = new TCanvas("c1","c1");
    c1 -> cd();

    h->Draw();
       
    c1 -> Print((Title+".png").c_str(),"png");
    c1 -> Print((Title+".pdf").c_str(),"pdf");
    
    delete c1;

}


std::vector<std::string> splitString(std::string& inputName, char split_char)
{
    std::vector<std::string> tokens;
    std::istringstream split(inputName);
    for(std::string each; getline(split, each, split_char); tokens.push_back(each));
    return tokens;
}


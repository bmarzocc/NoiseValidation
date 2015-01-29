#include "TimeUtils.h"

// default constructor, reading the map from file                                                                                                                          
TDate::TDate(const std::string inFile)
{
  std::ifstream fRunToDate(inFile.c_str());
  std::cout << "Inizializing run-date conversion" << std::endl;
  char crun[500], cnum1[500], cnum2[500], cdateIn[500], carrow[500], cdateFin[500];
  std::string dataIn;
  std::string dateFin;
  int run;
  std::string DateIN;
  std::string DateFIN;

  while(fRunToDate >> crun >> cnum1 >> cnum2 >> cdateIn >> carrow >> cdateFin){

    std::string dataIn = std::string(cdateIn);
    std::string dataFin = std::string(cdateFin);

    {
        run = atoi(crun);
        std::string inputName = dataIn;
        char split_char = 'T';
        std::vector<std::string> tokens;
        std::istringstream split0(inputName);
        for(std::string each; getline(split0, each, split_char); tokens.push_back(each));

        inputName = tokens.at(0);
        split_char = '-';
        std::vector<std::string> tokens_M;
        std::istringstream split1(inputName);
        for(std::string each; getline(split1, each, split_char); tokens_M.push_back(each));

        inputName = tokens.at(1);
        split_char = ':';
        std::vector<std::string> tokens_m;
        std::istringstream split2(inputName);
        for(std::string each; getline(split2, each, split_char); tokens_m.push_back(each));
    
        tokens_M.at(0).erase(0,1);
        int secint = atoi(tokens_m.at(2).c_str());
        char sec[500];
        sprintf(sec,"%d",secint);
        
        DateIN = tokens_M.at(0)+" "+tokens_M.at(1)+" "+tokens_M.at(2)+" "+tokens_m.at(0)+" "+tokens_m.at(1)+" "+std::string(sec);
    }
    
    RunToDate_first[run] = DateIN;
    
    
    if(atoi(cnum2) != -1){
        run = atoi(crun);
        std::string inputName = dataFin;
        char split_char = 'T';
        std::vector<std::string> tokens;
        std::istringstream split0(inputName);
        for(std::string each; getline(split0, each, split_char); tokens.push_back(each));

        inputName = tokens.at(0);
        split_char = '-';
        std::vector<std::string> tokens_M;
        std::istringstream split1(inputName);
        for(std::string each; getline(split1, each, split_char); tokens_M.push_back(each));

        inputName = tokens.at(1);
        split_char = ':';
        std::vector<std::string> tokens_m;
        std::istringstream split2(inputName);
        for(std::string each; getline(split2, each, split_char); tokens_m.push_back(each));
    
        tokens_m.at(2).erase(tokens_m.at(2).size()-2,tokens_m.at(2).size()-1);
        int secint = atoi(tokens_m.at(2).c_str());
        char sec[500];
        sprintf(sec,"%d",secint);
        
        DateFIN = tokens_M.at(0)+" "+tokens_M.at(1)+" "+tokens_M.at(2)+" "+tokens_m.at(0)+" "+tokens_m.at(1)+" "+std::string(sec);

    }else{
        
        DateFIN = "-1";
    }
 
    
    RunToDate_last[run] = DateFIN;
    
  }
  
  return;
}

//dtor
TDate::~TDate() { return;}

int TDate::dateToInt(const int& run)
{
  
  int run0;
  int year0,month0,day0,hour0,minute0,second0,t0;
  int year1,month1,day1,hour1,minute1,second1,t1;

  if(RunToDate_last[run] != ""){

        std::stringstream ssIn(RunToDate_first[run]);
        ssIn >> year0 >> month0 >> day0 >> hour0 >> minute0 >> second0;

        std::stringstream ssFin(RunToDate_last[run]);
        ssFin >> year1 >> month1 >> day1 >> hour1 >> minute1 >> second1;

        run0 = run;
        
  }else{
        
        float dRun = 99999.;
        for(std::map<int,std::string>::iterator it = RunToDate_first.begin(); it != RunToDate_first.end(); ++it) {
            if(abs(run - it->first) < dRun){
               dRun = abs(run-it->first);
               run0 = it->first;
            }
        }
        
        std::stringstream ssIn(RunToDate_first[run0]);
        ssIn >> year0 >> month0 >> day0 >> hour0 >> minute0 >> second0;
        
        std::stringstream ssFin(RunToDate_last[run0]);
        ssFin >> year1 >> month1 >> day1 >> hour1 >> minute1 >> second1;
       
  }
  
  tm time0;
  time0.tm_sec = second0;
  time0.tm_min = minute0;
  time0.tm_hour = hour0;
  time0.tm_mday = day0;
  time0.tm_mon = month0-1;
  time0.tm_year = year0-1900;
  t0 = timegm(&time0);

  tm time1;
  time1.tm_sec = second1;
  time1.tm_min = minute1;
  time1.tm_hour = hour1;
  time1.tm_mday = day1;
  time1.tm_mon = month1-1;
  time1.tm_year = year1-1900;
  t1 = timegm(&time1);

  if(RunToDate_last[run0] != "-1") return abs(t1-t0)/2;
  else return t0;
}


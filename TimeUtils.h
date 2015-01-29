#ifndef TimeUtils_h
#define TimeUtils_h

//ECAL GEOMETRY

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <math.h>
#include <vector>
#include <sstream>
#include <ctime>


/// EE Geometry ///
class TDate {
 private:
  std::map<int,std::string> RunToDate_first;
  std::map<int,std::string> RunToDate_last;
 public:
  TDate(const std::string inFile);
  virtual ~TDate();
  int dateToInt(const int& run);

};

#endif

#include <string>

#include "format.h"

using std::string;

string FormatTime(long n){
  if (n < 10){
    return "0" + std::to_string(n);
  } 
  else {
    return std::to_string(n);
  }
}

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  string t="";
  
  long m = seconds/60;
  long h = m/60;
  long s = seconds%60;


  return FormatTime(h)+ ":" + FormatTime(m) + ":" + FormatTime(s); 
}


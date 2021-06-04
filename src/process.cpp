#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
  this->pid  = pid;
}

// Return this process's ID
int Process::Pid() {
   return pid; 
  }

// Return this process's CPU utilization
float Process::CpuUtilization() { 
  float usage= 0;
  
  LinuxParser::processUsage procUse = LinuxParser::CpuUtilForProc(pid);
  long up = LinuxParser::UpTime();
  float total = procUse.utime + procUse.stime + procUse.cutime + procUse.cstime;
  float dif = up - procUse.starttime;
  usage = total / dif;  

  return usage;
}

// Return the command that generated this process
string Process::Command() { 
  return LinuxParser::Command(pid);
}

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(pid);
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
  return LinuxParser::UpTime(pid);
 }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
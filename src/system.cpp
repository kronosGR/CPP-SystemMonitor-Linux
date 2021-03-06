#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using namespace std;

// Return the system's CPU
Processor& System::Cpu() { 
  return cpu_;
}

bool Compare(Process &a, Process &b){
  return b.CpuUtilization() < a.CpuUtilization();
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  vector<Process> procs;

  for(int i: pids){
    Process p(i);
    procs.push_back(p);
  }

  // sort by cpu usage
  sort(procs.begin(), procs.end(), Compare);

  processes_ = procs;

  return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { 
  return LinuxParser::Kernel();
}

// Return the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() { 
  return LinuxParser::OperatingSystem(); 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses();
}

// Return the total number of processes on the system
int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses();
}

// Return the number of seconds since the system started running
long int System::UpTime() { 
  return LinuxParser::UpTime();
}
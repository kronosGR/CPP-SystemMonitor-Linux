#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float total = 0;
  float free = 0;

  string line, k, v;

  std::ifstream fs(kProcDirectory + kMeminfoFilename);
  if (fs.is_open()){
    while(getline(fs, line)){

      std::remove_if(line.begin(), line.end(), isspace);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream ls(line);
      while(ls >> k >> v){
        //std::cout << "-"<< k << "- " << v << "\n";
        if (k == "MemFree"){
          free = stof(v);
          break;
        }
        if (k == "MemTotal"){
          total = stof(v);          
          break;
        }
      }
    }
  } else {
    std::cout << "Problem reading the file\n";
  }
  fs.close();
  return (total - free) / total;
 } 


// Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string time, idle;
  float finalTime;

  std::ifstream fs(kProcDirectory + kUptimeFilename);
  if (fs.is_open()){
    std::getline(fs, line);
    std::istringstream ls(line);
    ls >> time >> idle;
    finalTime = stof(time);     
    
  } else {
    std::cout << "Problem reading the file\n";
  }
  fs.close();
  return finalTime;
} 

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string line, cpu;
  long user, nice, system, irq, softirq, steal, guest, guest_nice;
  long idle, iowait;
  long jiffies;

  std::ifstream fs (kProcDirectory + kStatFilename);
  if (fs.is_open()){
    std::getline(fs, line);
    std::istringstream ls(line);

    ls >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal
      >> guest >> guest_nice;

    jiffies = (user - guest) + (nice - guest_nice) + idle + iowait + system + irq + softirq +
        guest + guest_nice;
  }
  
  fs.close();
  return jiffies;
}

// Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  long activeJiffies;
  const int UTIME_INDEX = 13;
  const int STIME_INDEX = 14;
  const int CUTIME_INDEX = 15;
  const int CSTIME_INDEX = 16;
  const int STARTTIME_INDEX = 21;

  std::ifstream fs (kProcDirectory + std::to_string(pid) + kStatFilename);

  if (fs.is_open()){
    std::getline(fs, line);
    std::istringstream ls(line);

    // to vector
    std::istream_iterator<string> begin(ls);
    std::istream_iterator<string> end;
    std::vector<string> items(begin, end);

    activeJiffies = (std::stol(items[UTIME_INDEX]) / sysconf(_SC_CLK_TCK) + 
        std::stol(items[STIME_INDEX]) / sysconf(_SC_CLK_TCK) + 
        std::stol(items[CUTIME_INDEX]) / sysconf(_SC_CLK_TCK) +
        std::stol(items[CSTIME_INDEX])  / sysconf(_SC_CLK_TCK) + 
        std::stol(items[STARTTIME_INDEX]) / sysconf(_SC_CLK_TCK)) * 60 ;
  }
  fs.close();
  return activeJiffies;  
 }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line;
  long up = 0;
  long idle = 0;
  long activeJiffies = 0;

  std::ifstream fs( kProcDirectory + kUptimeFilename);return 0; 
  if (fs.is_open()){
    std::getline(fs, line);
    std::istringstream ls(line);
    ls >> up >> idle;

    // 1 sec = 60 jiffies if i am not wrong
    activeJiffies = (up - idle) * 60;
  }
  fs.close();
  return activeJiffies;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line;
  long up = 0;
  long idle = 0;
  long idleJiffies = 0;

  std::ifstream fs( kProcDirectory + kUptimeFilename);return 0; 
  if (fs.is_open()){
    std::getline(fs, line);
    std::istringstream ls(line);
    ls >> up >> idle;

    // 1 sec = 60 jiffies if i am not wrong
    idleJiffies = idle * 60;
  }
  fs.close();
  return idleJiffies;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string k;

  // working times
  string user,nice,system,irq,softirq,steal, guest, guest_nice;

  // idle times
  string idle, iowait;

  vector<string> cpu;
  std::ifstream fs(kProcDirectory + kStatFilename);
  if (fs.is_open()){
    while(std::getline(fs, line)){
      std::istringstream ls(line);
      while (ls >> k >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >>
              guest >> guest_nice ){
                if (k == "cpu"){
                  cpu.push_back(user);
                  cpu.push_back(nice);
                  cpu.push_back(system);
                  cpu.push_back(idle);
                  cpu.push_back(iowait);
                  cpu.push_back(irq);
                  cpu.push_back(softirq);
                  cpu.push_back(steal);
                  cpu.push_back(guest);
                  cpu.push_back(guest_nice);
                }
              } 
    }
  }
  fs.close();
  return cpu;
}
LinuxParser::processUsage LinuxParser::CpuUtilForProc(int pid){
  LinuxParser::processUsage procUse;
  string line;

  const int UTIME_INDEX = 13;
  const int STIME_INDEX = 14;
  const int CUTIME_INDEX = 15;
  const int CSTIME_INDEX = 16;
  const int STARTTIME_INDEX = 21;

  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (fs.is_open()){
    std::getline(fs, line);
    std::istringstream ls (line);

    // to vector
    std::istream_iterator<string> begin(ls);
    std::istream_iterator<string> end;
    std::vector<string> items(begin, end);

    procUse.utime = std::stol(items[UTIME_INDEX]) / sysconf(_SC_CLK_TCK);
    procUse.stime = std::stol(items[STIME_INDEX]) / sysconf(_SC_CLK_TCK);
    procUse.cutime = std::stol(items[CUTIME_INDEX]) / sysconf(_SC_CLK_TCK);
    procUse.cstime = std::stol(items[CSTIME_INDEX]) / sysconf(_SC_CLK_TCK);
    procUse.starttime = std::stol(items[STARTTIME_INDEX]) / sysconf(_SC_CLK_TCK);
  }
  fs.close();
  return procUse;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, k, v;
  int processes=0;
  std::ifstream fs(kProcDirectory + kStatFilename);
  if (fs.is_open()){
    while(std::getline(fs, line)){
      std::istringstream ls(line);
      while(ls >> k >> v){
        if (k == "processes"){
          processes = std::stoi(v);
        }
      }

    }
  }
  fs.close();
  return processes;
 }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, k, v;
  int processes = 0;
  std::ifstream fs (kProcDirectory + kStatFilename);
  if (fs.is_open()){
    while(std::getline(fs, line)){
      std::istringstream ls(line);
        while(ls >> k >> v){
          if (k == "procs_running") processes = std::stoi(v);
        }
      }
  }
  fs.close();
  return processes;
}

// Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string cmd = "";
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (fs.is_open()){
    std::getline(fs, cmd);
  }
  fs.close();
  return cmd;
 }

// Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, k, kb, v, ram = "";
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (fs.is_open()){
    while(std::getline(fs, line)){
      //std::remove_if(line.begin(), line.end(), isspace);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream ls(line);
      while(ls >> k >> v >> kb){
        if (k == "VmData") {
          ram = v;      
        }
      }
    }
  }
  fs.close();
  return ram;
 }

// Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, k, v, uid = "";
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (fs.is_open()){
    while (std::getline(fs, line)){
      //std::remove_if(line.begin(), line.end(), isspace);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream ls(line);
      while(ls >> k >> v){
        if (k =="Uid") uid = v;
      }
    }
  }
  fs.close();
  return uid;
}

// Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, user, pass, uid, gid, gecos, home, shell, usr = "";

  std::ifstream fs(kPasswordPath);
  if (fs.is_open()){
    while(std::getline(fs, line)){
      std::replace(line.begin(), line.end(), ':',' ');
      std::istringstream ls (line);
      while(ls >> user >> pass >> uid >> gid >> gecos >> home >> shell ){
        if (uid == Uid(pid)) usr = user;
      }
    }
  }
  fs.close();
  return usr;
}

// Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long uptime = 0;
  string line;
  const int START_TIME_INDEX = 21;

  std::ifstream fs(kProcDirectory + std::to_string(pid)+ kStatFilename);
  if (fs.is_open()){
    std::getline(fs, line);
    std::istringstream ls(line);

    // to vector
    std::istream_iterator<string> begin(ls);
    std::istream_iterator<string> end;
    std::vector<string> items(begin, end);
    
    // changed the result after the feedback
    uptime = UpTime() - std::stol(items[START_TIME_INDEX]) / sysconf(_SC_CLK_TCK);
  }
  fs.close();
  
  return uptime;
}

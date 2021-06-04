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
  return (total - free) / total;
 } 


// Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string time;
  float finalTime;

  std::ifstream fs(kProcDirectory + kUptimeFilename);
  if (fs.is_open()){
    while(std::getline(fs, line)){
      std::istringstream ls(line);
      while (ls >> time){
        finalTime = stof(time);
      }
    }
  } else {
    std::cout << "Problem reading the file\n";
  }
  return finalTime;
} 

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

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
  return cpu;
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
  return cmd;
 }

// Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, k, v, ram = "";
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (fs.is_open()){
    while(std::getline(fs, line)){
      std::remove_if(line.begin(), line.end(), isspace);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream ls(line);
      while(ls >> k >> v){
        if (k == "VmSize") ram = v;        
      }
    }
  }
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
  return usr;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long uptime = 0;
  string line;
  const int START_TIME_INDEX = 22;

  std::ifstream fs(kProcDirectory + std::to_string(pid)+ kStatFilename);
  std::getline(fs, line);
  std::istringstream ls(line);
  std::istream_iterator<string> begin(ls);
  std::istream_iterator<string> end;
  std::vector<string> items(begin, end);
  
  uptime = std::stol(items[START_TIME_INDEX -1 ]) / sysconf(_SC_CLK_TCK);
  return uptime;
}

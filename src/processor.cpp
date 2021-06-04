#include <vector>

#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() { 

  std::vector<std::string> cpu = LinuxParser::CpuUtilization();

  float totalIdle = std::stol(cpu[3]) + std::stol(cpu[4]);
  float totalUsed = std::stol(cpu[0]) + std::stol(cpu[1]) + std::stol(cpu[2]) +  
        std::stol(cpu[5]) + std::stol(cpu[6]) + std::stol(cpu[7]) +
        std::stol(cpu[8]) + std::stol(cpu[9]);

  float difInUse = ((totalUsed - totalUseT) - (totalIdle - totalIdleT)) / 
    (totalUsed - totalUseT);

  totalIdleT = totalIdle;
  totalUseT = totalUsed;

  return difInUse;
}
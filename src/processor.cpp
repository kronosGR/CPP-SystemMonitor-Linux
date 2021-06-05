#include <vector>
#include <unistd.h>

#include "processor.h"
#include "linux_parser.h"

Processor::Jiffies Processor::CpuReading(){
  std::vector<std::string> cpu = LinuxParser::CpuUtilization();

  Processor::Jiffies jiffies;

  jiffies.idle = std::stol(cpu[3]) + std::stol(cpu[4]) + std::stol(cpu[5]) +
        std::stol(cpu[6]);

  jiffies.work = std::stol(cpu[0]) + std::stol(cpu[1]) + std::stol(cpu[2]); 

  jiffies.total = jiffies.idle + jiffies.work;

  return jiffies;
}

// Return the aggregate CPU utilization
float Processor::Utilization() { 
  Processor::Jiffies firstReading = CpuReading();
  //wait half second for the second reading
  usleep(300000);
  Processor::Jiffies secondReading = CpuReading();
  
  long idleOverPeriod = secondReading.idle - firstReading.idle;
  long totalOverPeriod = secondReading.total - firstReading.total;

  return (float)(totalOverPeriod - idleOverPeriod)  / totalOverPeriod;
}
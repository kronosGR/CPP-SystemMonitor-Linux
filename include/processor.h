#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // Declare any necessary private members
 private:
  float totalIdleT;
  float totalUseT;
  
};

#endif
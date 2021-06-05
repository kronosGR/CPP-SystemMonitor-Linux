#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  //  See src/processor.cpp

  // Declare any necessary private members
 private:
  struct Jiffies{
    long total;
    long work;
    long idle;
  };

  Jiffies CpuReading();
  float totalIdleT;
  float totalUseT;
};

#endif
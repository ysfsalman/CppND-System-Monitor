#include <string>
#include <vector>
#include <sstream>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using namespace LinuxParser;

// Return the aggregate CPU utilization
float Processor::Utilization() {
    vector<string> cpuUtilList = CpuUtilization();     
    string cpuID;
    vector<long> cpuTime(10,0);
    long idle, nonIdle, total, totald, idled;
    //access only the aggregate CPU
    std::istringstream linestream(cpuUtilList[0]);
    linestream >> cpuID;
    for (int i = 0; i < 10; i++) 
          linestream >> cpuTime[i];  
    // calculate CPU Utilization
    idle = cpuTime[kIdle_] + cpuTime[kIOwait_];
    nonIdle = cpuTime[kUser_] + cpuTime[kNice_] + cpuTime[kSystem_] 
                + cpuTime[kIRQ_] + cpuTime[kSoftIRQ_] + cpuTime[kSteal_];
    total = idle + nonIdle;  
    totald =  total - prevTotal;
    idled = idle - prevIdle;
    //store current total and idle for next iteration
    this->prevTotal = total;
    this->prevIdle = idle;
    return (float)(totald-idled)/totald;
}
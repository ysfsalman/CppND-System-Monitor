#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
    public:
        Process(int pid){
            this->pid_= pid;            
            this->user_ = LinuxParser::User(pid);            
            this->command_ = LinuxParser::Command(pid);
            this->ram_ = LinuxParser::Ram(pid);
            this->startTime_ = LinuxParser::UpTime(pid);
        }
        int Pid();                               
        std::string User();                      
        std::string Command();                   
        float CpuUtilization();                  
        std::string Ram();                       
        long int UpTime();                       
        bool operator<(Process const& a) const;  

    protected:        
        float cpuUtil_ = 0.0f;
    private:
        int pid_;
        long int startTime_;
        std::string user_, command_, ram_;
};

#endif
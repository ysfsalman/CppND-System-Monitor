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
        int Pid();                               // TODO: See src/process.cpp
        std::string User();                      // TODO: See src/process.cpp
        std::string Command();                   // TODO: See src/process.cpp
        float CpuUtilization();                  // TODO: See src/process.cpp
        std::string Ram();                       // TODO: See src/process.cpp
        long int UpTime();                       // TODO: See src/process.cpp
        bool operator<(Process const& a) const;  // TODO: See src/process.cpp

    private:
        int pid_;
        long int startTime_;
        std::string user_, command_, ram_;
};

#endif
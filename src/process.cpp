#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"


using std::string;
using std::to_string;
using std::vector;


// Return this process's ID
int Process::Pid() { return this->pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    return (float) (LinuxParser::ActiveJiffies(this->pid_)
                        /sysconf(_SC_CLK_TCK)) / Process::UpTime();
}

// Return the command that generated this process
string Process::Command() { return this->command_; }

// Return this process's memory utilization
string Process::Ram() { 
    this->ram_ = LinuxParser::Ram(this->pid_);
    return this->ram_; 
}

// Return the user (name) that generated this process
string Process::User() { return this->user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime() - this->upTime_ ; 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return (this->ram_ < a.ram_) ? true : false;
}
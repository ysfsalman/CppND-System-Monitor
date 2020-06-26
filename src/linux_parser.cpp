#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>


#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T ParseLine(string ID, string streamDir){
  string line;
  string lineId;
  T lineValue;
  std::ifstream stream(streamDir);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> lineId;
      if (lineId == ID){
        linestream >> lineValue;
        break;
        }      
    }
  } 
  return lineValue;
}


// Read and return OS name
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

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
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
  string line;
  string memName;
  long memTotal, memFree, buffers;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    for (int i = 0; i < 4; i++){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> memName; 
      if (memName == "MemTotal:") linestream >> memTotal; 
      else if (memName =="MemFree:") linestream >> memFree; 
      else if (memName == "Buffers:") linestream >> buffers; 
    }
  }
  return  1.0 - (float)memFree/(memTotal-buffers);   
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long lineValue;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> lineValue;
    }
  return lineValue;  
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
  string cpu_;
  vector<string> cpuUtilList;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> cpu_;
      if (cpu_.substr(0,3) == "cpu")
        cpuUtilList.push_back(line);
      else break;
    }
  } 
  return cpuUtilList; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return ParseLine<int>("processes", (kProcDirectory + kStatFilename));
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {   
  return ParseLine<int>("procs_running", (kProcDirectory + kStatFilename)); 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line, i = to_string(pid);
  std::ifstream stream(kProcDirectory + 
                      i + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);    
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {  
  long i = ParseLine<long>("VmSize:", kProcDirectory + 
                      to_string(pid) + kStatusFilename);
  return to_string((float)i/1000);   
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  return ParseLine<string>("Uid:", kProcDirectory + 
                      to_string(pid) + kStatusFilename);  
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {  
  string line;
  string char_;   
  string key, username;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> char_ >> key;
      if (key == Uid(pid)) break;
    }
  }
  return username;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  vector<string> stat_(22, string());
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid)
                       + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i; i<22 ; i++)
      linestream >> stat_[i];
  }
  return stoi(stat_[21]); 
}
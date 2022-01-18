#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//Read and return the OperatingSystem
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

//Read and return the Kernel
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

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float Memtotal,MemFree;
  string line,key;
  float value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key=="MemTotal:"){Memtotal=value;}
        if(key=="MemFree:"){MemFree=value;}
      }
    }
  } 
  return (Memtotal-MemFree)/Memtotal; }

//Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int val;
  string key, value, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
  		std::istringstream linestream(line);
    	while (linestream >> key >> value) {
        	if (key == "processes") { 
              val = std::stoi(value);
              return val;
              }
        	}
      	}
    }
  return 0;
}

//Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int val;
  string key, value, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
  		std::istringstream linestream(line);
    	while (linestream >> key >> value) {
        	if (key == "procs_running") { 
              val = std::stoi(value);
              return val;
              }
        	}
      	}
    }
  return 0;
}


//Read and return the system uptime
long LinuxParser::UpTime() {
  string line; 
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream>>uptime;
  }
  return uptime; }

//Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  vector<string> v;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      string temp;
      std::istringstream linestream(line);
      linestream >> temp;
      if (temp == "cpu"){
        while(linestream >> temp){
          v.push_back(temp);
        }
        return v;
      }
    }
  }
  return {}; 
}



//Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long tot {0};
  vector<string> cpu_utilization;
  cpu_utilization = LinuxParser::CpuUtilization();
  for (int i = kUser_; i <= kSteal_; i++){
    tot += stol(cpu_utilization[i]);
  }
  return tot;
}

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> v_str = LinuxParser::CpuUtilization();
  vector<long> v;
  for(auto st:v_str){
    v.push_back(stol(st));
  }
  long jiffies = v[kUser_] + v[kNice_] + v[kSystem_] + v[kIRQ_] + v[kSoftIRQ_] + v[kSteal_];
  return jiffies;
 }

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> v_str = LinuxParser::CpuUtilization();
  vector<long> v;
  for(auto st:v_str){
    v.push_back(stol(st));
  }
  long jiffies = v[kIdle_] + v[kIOwait_];
  return jiffies;
}



long LinuxParser::ActiveJiffies(int pid) { 
  string line, temp;
  vector<string> v;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    getline(filestream,line);
    std::istringstream linestream(line);
    while(linestream>>temp){
      v.push_back(temp);
    }
    long sum = stoi(v[13]) + stoi(v[14]) + stoi(v[15]) + stoi(v[16]);
    return sum; 
  }
  return 0;
}

//Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line,values;
  string command="NO_COMMAND";
  std::vector<long int> val_list;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    command=line;
  }
  return command; 
}

//Read and return the memory used by a process
string LinuxParser::Ram(int pid) {   
  string line, temp, ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> temp >> ram;
      if(temp == "VmSize:"){
        int RAM = stoi(ram)/1024;
        return to_string(RAM);
      }
    }
  }
  return "0"; 
}

//Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, temp, UID;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> temp;
      if(temp == "Uid:"){
        linestream >> UID;
        return UID;
      }
    }
  }
  return string(); 
}

//Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string usr, passwd, uid, line;
  string uid_ = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
        std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
    	while (linestream >> usr >> passwd >> uid) {
               if (uid==uid_){
                return usr;
               }
        	}
      	}
    }
  return string();
}

//Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, temp;
  long uptime = 0;
  vector<string> v;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    while(linestream >> temp){
      v.push_back(temp);
    }
    long starttime = stol(v[21])/sysconf(_SC_CLK_TCK);
    uptime = LinuxParser::UpTime() - starttime;
  }
  return uptime;
}
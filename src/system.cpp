#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include<iostream>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_={};
    vector<int> pids=LinuxParser::Pids();
  for (int i=0;i<pids.size();i++) {
    Process process_list(pids[i]);
    processes_.push_back(process_list);
  }
    std::sort(processes_.begin(), processes_.end());
    return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
#include "processor.h"
#include "linux_parser.h"
#include<vector>
#include<string>
#include <unistd.h>


using std::string;
using std::vector;
using namespace std;

// TODO: Return the aggregate CPU utilization

float Processor::Utilization() {
  long old_tot_jif=LinuxParser::Jiffies();
  long old_act_jif=LinuxParser::ActiveJiffies();
  
  usleep(50000);// to get the new values after 50000 milliseconds


  long new_tot_jif=LinuxParser::Jiffies();
  long new_act_jif=LinuxParser::ActiveJiffies();

  util_= 1.0*(new_act_jif-old_act_jif)/(new_tot_jif-old_tot_jif);
  return util_;
}
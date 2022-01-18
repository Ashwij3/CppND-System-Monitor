#include <string>

#include "format.h"

using std::string;
string Format::ElapsedTime(long times) {
  long h = times / 3600;
  long s_left = times % 3600;
  long m = s_left / 60;
  long s = s_left % 60;
  string hst = std::to_string(h);
  string mst = std::to_string(m);
  string sst = std::to_string(s);
  string time = string(2 - hst.length(), '0') + hst + ':' +string(2 - mst.length(), '0') + mst + ':' +string(2 - sst.length(), '0') + sst;
  return time;
}
#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float MemTotal, MemFree;
  string line, key;
  float value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key=="MemTotal:") MemTotal=value;
      else if(key=="MemFree:") MemFree=value;
    }
    return float((MemTotal-MemFree)/MemTotal);
  }
  return 0; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long uptime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return uptime; 
  }
  return 0; 
}

// TODO: Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() { 
//   string key, value, line;
//   vector<string> uil;
//   std::ifstream filestream(kProcDirectory + kStatFilename);
//   if (filestream.is_open()){
//     while(std::getline(filestream,line)){
//       std::istringstream linestream(line);
//       linestream >> key;
//       if (key=="cpu"){
//         while(linestream >> value){
//           uil.push_back(value);
//         }
//         return uil;
//       }
//     }
//   }
// }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long juser, jnice, jsystem, jidle, jiowait, jirq, jsoftirq, jsteal, jguest, jguestnice;
  string line;
  string key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> juser >> jnice >> jsystem >> jidle >> jiowait >> jirq >> jsoftirq >> jsteal >> jguest >> jguestnice;
      if (key=="cpu"){
        return (juser+jnice+jsystem+jirq+jsoftirq+jsteal)+(jidle+jiowait);
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long active_time = 0;
  string line, value;
  std::vector<string> v;
  std::ifstream filestream(kProcDirectory+"/"+to_string(pid)+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      int i = 0;
      while(linestream>>value){
        i++;
        if(i >= 14 && i <= 17){
          active_time += std::stol(value);
        }
        if(i ==17){return active_time;}
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long juser, jnice, jsystem, jidle, jiowait, jirq, jsoftirq, jsteal, jguest, jguestnice;
  string line;
  string key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> juser >> jnice >> jsystem >> jidle >> jiowait >> jirq >> jsoftirq >> jsteal >> jguest >> jguestnice;
      if (key=="cpu"){
        return (juser+jnice+jsystem+jirq+jsoftirq+jsteal);
      }
    }
  }
  return 0;
}
// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long juser, jnice, jsystem, jidle, jiowait, jirq, jsoftirq, jsteal, jguest, jguestnice;
  string line;
  string key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> juser >> jnice >> jsystem >> jidle >> jiowait >> jirq >> jsoftirq >> jsteal >> jguest >> jguestnice;
      if (key=="cpu"){
        return (jidle+jiowait);
      }
    }
  }
  return 0;
}


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalprocesses;
  string line, key;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> totalprocesses;
      if(key=="processes"){
        return totalprocesses;
      }
      else
        continue;
    }
    return 0;
  }
  else
    return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()  {
  int processes_running;
  string line, key;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> processes_running;
      if(key=="procs_running"){
        return processes_running;
      }
      else
        continue;
    }
    return 0;
  }
  else
    return 0;
}
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line, key;
  std::ifstream filestream(kProcDirectory+"/"+to_string(pid)+kCmdlineFilename);
  if(filestream.is_open()){
    std::getline(filestream,line);
    if (line==""){
      return "None";
    }
    else{
      return line;
    }
  }
  return string();
}
// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  long vmsize;
  string line, key;
  std::ifstream filestream(kProcDirectory+"/"+to_string(pid)+kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> vmsize;
      if(key=="VmSize:"){
        return to_string(long(vmsize/1000));
      }
    }
    return "None";
  }
  return string();
}
// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  int uid;
  string line, key;
  std::ifstream filestream(kProcDirectory+"/"+to_string(pid)+kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if(key=="Uid:"){
        return to_string(uid);
      }
    }
    return "None";
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, data, user;
  string uid = LinuxParser::Uid(pid);
  if(uid=="") return "None";
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      int i=0;
      while(std::getline(linestream, data, ':')){
        if(i==0) {
          user=data;
          i++;
          continue;
        }
        if(uid==data){
          return user;
          }
          else if(i==2){
            break;
          }
          else {
            i++;
          }
      }
    }
    return "None";
  }
  return string();
}



// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string value, key;
  std::ifstream filestream(kProcDirectory+"/"+to_string(pid)+kStatFilename);
  if(filestream.is_open()){
    int i=0;
    while(std::getline(filestream,value,' ')){
      i++;
      if(i==22){
        long uptime=LinuxParser::UpTime()-std::stol(value)/sysconf(_SC_CLK_TCK);
        return uptime;
      }
    }
    return 0;
  }
  else
    return 0;
}
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

//Constructor
Process::Process(int pid):pid_(pid){
	user_= LinuxParser::User(pid_);
	command_=LinuxParser::Command(pid_);
// initialize each Process with the current CpuUtilization, will be used for sort
	active_jiffies_start = LinuxParser::ActiveJiffies(pid_);
	total_jiffies_start = LinuxParser::Jiffies();
  	utilization_ = active_jiffies_start/total_jiffies_start;
 // 	usleep(10000);
 // 	this->CpuUtilization();

}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization()  { 
	float delta_active, delta_total;
	float active_current, total_current;
	active_current = LinuxParser::ActiveJiffies(pid_);
	total_current = LinuxParser::Jiffies();
	delta_active = active_current - active_jiffies_start;
	active_jiffies_start = active_current;
	// if delta is 0, it means the process doesn't use
	// the CPU anymore
	if (delta_active == 0){
 		return 0;
	}
	delta_total = total_current - total_jiffies_start;
	total_jiffies_start = total_current;
	utilization_=delta_active / delta_total;
	return utilization_;
}

float Process::GetCpuUtilization(){
	return this->utilization_;
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects

bool Process::operator<(Process const& a) const {
	return utilization_>a.utilization_;
}
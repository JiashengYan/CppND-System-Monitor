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
	long total_jiffies_start, total_jiffies_end;
	long active_jiffies_start, active_jiffies__end;
	long delta_active, delta_total;

	active_jiffies_start = LinuxParser::ActiveJiffies(pid_);
  	total_jiffies_start = LinuxParser::Jiffies();
 //	Wait 1ms
	usleep(1000); // in microseconds

	active_jiffies__end = LinuxParser::ActiveJiffies(pid_);
  	total_jiffies_end = LinuxParser::Jiffies();

	delta_active = active_jiffies__end - active_jiffies_start;
	// if delta is 0, it means the process doesn't use
	// the CPU anymore
	if (delta_active == 0){
 		utilization_ = 0;
	}
	else {
		delta_total = total_jiffies_end - total_jiffies_start;
		utilization_=delta_active*1.0 / delta_total;
	}

}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization()  { 
	long total_jiffies_start, total_jiffies_end;
	long active_jiffies_start, active_jiffies__end;
	long delta_active, delta_total;

	active_jiffies_start = LinuxParser::ActiveJiffies(pid_);
  	total_jiffies_start = LinuxParser::Jiffies();
 //	Wait 100ms
	usleep(100000); // in microseconds

	active_jiffies__end = LinuxParser::ActiveJiffies(pid_);
  	total_jiffies_end = LinuxParser::Jiffies();

	delta_active = active_jiffies__end - active_jiffies_start;
	// if delta is 0, it means the process doesn't use
	// the CPU anymore
	if (delta_active == 0){
 		return 0;
	}
	delta_total = total_jiffies_end - total_jiffies_start;
	utilization_=delta_active*1.0 / delta_total;
	return utilization_;
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
	if(utilization_>a.utilization_){
		return true;
	}
	return false;
}
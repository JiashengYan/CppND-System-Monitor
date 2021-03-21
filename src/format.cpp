#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
	int h = (seconds/3600);
	int m = (seconds-3600*h)/60;
	int s = (seconds%60);
	string HH = h<10? "0"+std::to_string(h) : std::to_string(h);
	string MM = m<10? "0"+std::to_string(m) : std::to_string(m);
	string SS = s<10? "0"+std::to_string(s) : std::to_string(s);
	return HH + ":" + MM + ":" + SS; 
}
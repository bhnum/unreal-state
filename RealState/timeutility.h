#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>

using std::string;
using std::chrono::system_clock;
using time_point = system_clock::time_point;
using duration = system_clock::duration;

#if __cplusplus < 201703L // import some C++20 helper types
namespace std
{
	namespace chrono
	{
		using days = duration<int, std::ratio<86400>>;
		using weeks = duration<int, std::ratio<604800>>;
		using months = duration<int, std::ratio<2629746>>;
		using years = duration<int, std::ratio<31556952>>;
	}
}
#endif

string putdate(time_point time);
string puttime(time_point time);

class getdate
{
public:
	getdate(time_point &time) : time(time) {}
	friend std::istream &operator>>(std::istream &in, getdate &gd);
private:
	time_point &time;
};
std::istream &operator>>(std::istream &in, getdate &gd);

class gettime
{
public:
	gettime(time_point &time) : time(time) {}
	friend std::istream &operator>>(std::istream &in, gettime &gt);
private:
	time_point &time;
};
std::istream &operator>>(std::istream &in, gettime &gt);

time_point totime(const string &s);
time_point todate(const string &s);

// note: d = delta t = t2 - t1 or t - t0
string puttimediff(duration d);

string puttimediff(time_point t);

#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>

using std::string;
using std::chrono::system_clock;
typedef system_clock::time_point time_point;
typedef system_clock::duration duration;

namespace timeutility
{
	using std::istream;
	using std::stringstream;
	using std::time_t;
	using std::tm;

	string putdate(time_point time)
	{
		time_t tt = system_clock::to_time_t(time);
		stringstream ss;
		ss << std::put_time(std::localtime(&tt), "%F");
		return ss.str();
	}

	string puttime(time_point time)
	{
		time_t tt = system_clock::to_time_t(time);
		stringstream ss;
		ss << std::put_time(std::localtime(&tt), "%F %T");
		return ss.str();
	}

	class getdate
	{
	public:
		getdate(time_point &time) : time(time) {}
		friend istream &operator>>(istream &in, getdate &gd);
	private:
		time_point &time;
	};

	istream &operator>>(istream &in, getdate &gd)
	{
		tm t = tm();
		in >> std::get_time(&t, "%F");
		gd.time = system_clock::from_time_t(std::mktime(&t));
		return in;
	}

	class gettime
	{
	public:
		gettime(time_point &time) : time(time) {}
		friend istream &operator>>(istream &in, gettime &gt);
	private:
		time_point &time;
	};

	istream &operator>>(istream &in, gettime &gt)
	{
		tm t = tm();
		in >> std::get_time(&t, "%F %T");
		gt.time = system_clock::from_time_t(std::mktime(&t));
		return in;
	}
}

using timeutility::putdate;
using timeutility::puttime;
using timeutility::getdate;
using timeutility::gettime;
using timeutility::operator>>;

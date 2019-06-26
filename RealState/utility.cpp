#include "utility.h"
#include <random>

using std::istream;
using std::stringstream;
using std::time_t;
using std::tm;
using std::chrono::duration_cast;

string putdate(time_point time)
{
	time_t tt = system_clock::to_time_t(time);
	stringstream ss;
	ss << std::put_time(std::localtime(&tt), "%F");
	if (ss.fail())
		throw std::exception("Invalid input");
	return ss.str();
}

string puttime(time_point time)
{
	time_t tt = system_clock::to_time_t(time);
	stringstream ss;
	ss << std::put_time(std::localtime(&tt), "%F %T");
	if (ss.fail())
		throw std::exception("Invalid input");
	return ss.str();
}

istream &operator>>(istream &in, getdate &gd)
{
	tm t = tm();
	in >> std::get_time(&t, "%Y-%m-%d");
	std::mktime(&t);
	if (t.tm_isdst) t.tm_hour--;
	gd.time = system_clock::from_time_t(std::mktime(&t));
	return in;
}

istream &operator>>(istream &in, gettime &gt)
{
	tm t = tm();
	in >> std::get_time(&t, "%Y-%m-%d %T");
	std::mktime(&t);
	if (t.tm_isdst) t.tm_hour--;
	gt.time = system_clock::from_time_t(std::mktime(&t));
	return in;
}

time_point totime(const string &s)
{
	stringstream ss(s);
	time_point time;
	ss >> gettime(time);
	if (ss.fail())
		throw std::exception("Invalid input");
	return time;
}

time_point todate(const string &s)
{
	stringstream ss(s);
	time_point date;
	ss >> getdate(date);
	if (ss.fail())
		throw std::exception("Invalid input");
	return date;
}

string puttimediff(duration d)
{
	if (duration_cast<std::chrono::seconds>(d).count() == 0)
		return "just now";
	const string &suffix = d < d.zero() ? " ago" : " later";
	d = std::chrono::abs(d);

	auto years = duration_cast<std::chrono::years>(d);
	d -= years;
	auto days = duration_cast<std::chrono::days>(d);
	d -= days;
	auto hours = duration_cast<std::chrono::hours>(d);
	d -= hours;
	auto minutes = duration_cast<std::chrono::minutes>(d);
	d -= minutes;
	auto seconds = duration_cast<std::chrono::seconds>(d);

	stringstream ss;
	if (years.count() > 0) ss << years.count() << " years, ";
	if (days.count() > 0) ss << days.count() << " days and ";
	ss << hours.count() << ':' << minutes.count() << ':' << seconds.count() << suffix;
	return ss.str();
}

string puttimediff(time_point t)
{
	return puttimediff(t - system_clock::now());
}

std::default_random_engine generator(system_clock::now().time_since_epoch().count());

long long longrand(long long min, long long max)
{
	std::uniform_int_distribution<long long> distribution(min, max - 1);
	return distribution(generator);
}

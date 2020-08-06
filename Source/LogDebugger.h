/*\
 * Author jefftian
 * Date Tue Nov 14 15:08:47 CST 2017
 * Describe SimpleLogWrapper
 */
#ifndef __LOG_DEBUGGER_H
#define __LOG_DEBUGGER_H

#include <fstream>
#include <ctime>

class LogWritter
{
public:
	LogWritter(const char* logName)
	{
		outfile.open(logName, std::ios::out);
	}
	~LogWritter()
	{
		outfile.close();
	}
	std::ofstream& getStream() { return outfile; }
private:
	std::ofstream outfile;
};

#define LOG(log, strValue) { std::ofstream& of = log.getStream(); \
	of << time(0) << "\t" << strValue << std::endl; \
	of.flush();}

#endif //__LOG_DEBUGGER_H
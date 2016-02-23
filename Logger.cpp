#include "stdafx.h"
#include "Logger.h"


Logger::Logger()
{

}

std::vector<std::wstring> Logger::GetLogListW()
{
	

	return StaticWLog;
}

std::vector<std::string> Logger::GetLogList()
{
	return StaticLog;
}

void Logger::ConvertListToWstring()
{

	
}

void Logger::Log(std::string LogItem)
{
	StaticLog.push_back(LogItem);

	CA2W ca2w(LogItem.c_str());
	std::wstring w = ca2w;
	StaticWLog.push_back(w);
}

Logger::~Logger()
{
}

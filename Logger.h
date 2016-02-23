#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <AtlBase.h>
#include <atlconv.h>

class Logger
{
public:
	Logger();
	~Logger();
	static void Log(std::string LogItem);
    std::vector<std::string> GetLogList();
    std::vector<std::wstring> GetLogListW();
	void ConvertListToWstring();
private:
	
};

static std::vector<std::wstring> StaticWLog;
static std::vector<std::string> StaticLog;
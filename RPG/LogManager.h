#pragma once
#include "pch.h"

enum class E_LOG_LEVEL : int
{
	L_INFO,
	L_DEBUG,
	L_WARNING,
	L_ERROR,
	L_SYSTEM
};

static E_LOG_LEVEL m_eLogLevel = E_LOG_LEVEL::L_ERROR;

class LogManager
{
public:
	static void setLogLevel(const E_LOG_LEVEL _level);
	static void info(std::string_view message, std::string _callFunc = __builtin_FUNCTION(), int _line = __builtin_LINE());
	static void debug(std::string_view message, std::string _callFunc = __builtin_FUNCTION(), int _line = __builtin_LINE());
	static void error(std::string_view message, std::string _callFunc = __builtin_FUNCTION(), int _line = __builtin_LINE());
};



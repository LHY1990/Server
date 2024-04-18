#include "LogManager.h"

void LogManager::setLogLevel(const E_LOG_LEVEL _level)
{
	m_eLogLevel = _level;
}

void LogManager::info(std::string_view message, std::string _callFunc, int _line)
{
	if (m_eLogLevel >= E_LOG_LEVEL::L_INFO)
		std::cout << "[" << __TIME__ << "] " << _callFunc << "(" << _line << ")::" << __FUNCTION__ << "() : " << message << std::endl;
}

void LogManager::debug(std::string_view message, std::string _callFunc, int _line)
{
	if (m_eLogLevel >= E_LOG_LEVEL::L_DEBUG)
		std::cout << "[" << __TIME__ << "] " << _callFunc << "(" << _line << ")::" << __FUNCTION__ << "() : " << message << std::endl;
}

void LogManager::error(std::string_view message, std::string _callFunc, int _line)
{
	if (m_eLogLevel >= E_LOG_LEVEL::L_ERROR)
		std::cout << "[" << __TIME__ << "] " << _callFunc << "(" << _line << ")::" << __FUNCTION__ << "() : " << message << std::endl;
}





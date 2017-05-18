#ifndef __LOG_H_
#define __LOG_H_

#include <string>
using namespace std;

#include <windows.h>

#define ERRNO() GetLastError()


enum LOG_LEVEL
{
    LL_ERROR, // it's an error
    LL_WARN,  // it's a warning, but doesn't affect running
    LL_NORMAL, // normal information
    LL_DEBUG,  // for debug
    LL_DETAIL // most detail
};
	class CLog
	{
	public:
		CLog(){}
        ~CLog() {Close();}

		static inline void SetLogLevel(LOG_LEVEL level){m_level = level;}
		static inline int  GetLogLevel() {return m_level;}
		static inline void GetLogFile(string &file) {file = m_strLogFile;}
		static int SetLogFile(const string &file);
		static int SetLogFile(FILE *pF);

		static void Log(LOG_LEVEL level, const char *file, int line, int error, char *fmt, ...);        
		static void Log(LOG_LEVEL level, int error, char *fmt, ...);static void Close();

        static const char* GetLevelString(LOG_LEVEL level);
	private:
		static LOG_LEVEL m_level;
		static FILE *m_pLog;
		static string m_strLogFile;
	};

#endif // __DEFINES_H_

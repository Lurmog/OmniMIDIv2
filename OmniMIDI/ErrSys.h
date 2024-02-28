/*

	OmniMIDI v15+ (Rewrite) for Windows NT

	This file contains the required code to run the driver under Windows 7 SP1 and later.
	This file is useful only if you want to compile the driver under Windows, it's not needed for Linux/macOS porting.

*/

/*

	OmniMIDI v15+ (Rewrite) for Windows NT

	This file contains the required code to run the driver under Windows 7 SP1 and later.
	This file is useful only if you want to compile the driver under Windows, it's not needed for Linux/macOS porting.

*/

#ifndef _ERRSYS_H
#define _ERRSYS_H

#include <Windows.h>
#include <tchar.h>
#include <string>
#include <string_view>
#include <stdexcept>
#ifdef _DEBUG
#include <iostream>
#include <future>
#endif

#define S2(x)					#x
#define S1(x)					S2(x)

#define NERROR(x, y, z, ...)	x.ThrowError(y, z, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define FNERROR(x, y)			x.ThrowFatalError(y, __FILE__, __func__, __LINE__)

#if _DEBUG
#define LOG(x, y, ...)			x.Log(y, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOGV(x, y, ...)			x.Log(S1(y), __FILE__, __func__, __LINE__, __VA_ARGS__)
#else
#define LOG(x, y, ...)			NULL
#define LOGV(x, y, ...)			NULL
#endif

namespace ErrorSystem {
	class Logger {
	private:
		static const int BufSize = 2048;
		static const int SZBufSize = sizeof(char) * BufSize;

	public:
		void Log(const char* Error, const char* File, const char* Func, const unsigned long Line, ...);
		void ThrowError(const char* Error, bool IsSeriousError, const char* File, const char* Func, const unsigned long Line, ...);
		void ThrowFatalError(const char* Error, const char* File, const char* Func, const unsigned long Line);
	};
}

#endif
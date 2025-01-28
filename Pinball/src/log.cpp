#pragma once
#include <cstdarg>   // Standard header for va_list, va_start, va_end
#include <cstdio>    // For snprintf

#ifdef _WIN32
#include <windows.h> // For OutputDebugString
#endif

void log(const char file[], int line, const char* format, ...)
{
	char tmp_string[4096];  // Temporary string for log message
	char tmp_string2[4096]; // String with additional file and line info
	va_list ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	// Use snprintf instead of vsprintf_s for buffer protection
	vsnprintf(tmp_string, sizeof(tmp_string), format, ap);
	va_end(ap);

	// Format the log message with the file and line number
	snprintf(tmp_string2, sizeof(tmp_string2), "\n%s(%d): %s", file, line, tmp_string);

	// Output the log string
#ifdef _WIN32
	// On Windows, use OutputDebugString
	OutputDebugString(tmp_string2);
#else
	// On other platforms (Linux, macOS, etc.), use standard output (stderr)
	fprintf(stderr, "%s", tmp_string2);
#endif
}

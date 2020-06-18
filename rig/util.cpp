
#include <iostream>
#include <fstream>
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#include "util.h"

bool ReadFile(const char* pFileName, string& outFile)
    {
    ifstream f(pFileName);
    bool ret = false;
    
    if (f.is_open()) 
        {
        string line;
        while (getline(f, line))
            {
            outFile.append(line);
            outFile.append("\n");
            }
        f.close();
        ret = true;
        }
    else 
        {
        FILE_ERROR(pFileName);
        }
    
    return ret;
    }


void OgldevFileError(const char* pFileName, uint line, const char* pFileError)
{
#ifdef WIN32
	char msg[1000];
	_snprintf_s(msg, sizeof(msg), "%s:%d: unable to open file `%s`", pFileName, line, pFileError);
    MessageBoxA(NULL, msg, NULL, 0);
#else
    fprintf(stderr, "%s:%d: unable to open file `%s`\n", pFileName, line, pFileError);
#endif    
}


long long GetCurrentTimeMillis()
{
#ifdef WIN32    
	return GetTickCount();
#else
	timeval t;
	gettimeofday(&t, NULL);

	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
#endif    
}

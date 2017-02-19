#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include "def.h"
using namespace std;

unsigned long long tCreate, tExit, tKernel, tUser;
HANDLE hp;
STARTUPINFO si;
PROCESS_INFORMATION pi;
LPTSTR lpMsgBuf;
PROCESS_MEMORY_COUNTERS pmc;

void PrintErrorMessage(long ErrorCode){
    char ErrorString[1024];
    FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL,
                   ErrorCode,
                   MAKELANGID(__TIMING_SYSTEM_ERROR_MESSAGE_LANG,
                              __TIMING_SYSTEM_ERROR_MESSAGE_SUBLANG),
                   LPSTR(&ErrorString),
                   sizeof(ErrorString),
                   NULL);
    cout << ErrorString;
    cout.flush();
}

int main(int argc, char* argv[]){
    DWORD exitc;
    si.cb = sizeof(STARTUPINFO);
    if (argc != 2){
        printf("Please input a correct file name!\n", argv[1]);
        return 0;
    }
    if (!CreateProcess(NULL, argv[1], NULL, NULL, false, 0, NULL, NULL, &si, &pi)){
        int en = GetLastError();
        printf("Failed to create process!\n");
        PrintErrorMessage(en);
        return 0;
    }
    hp = pi.hProcess;
    WaitForSingleObject(hp, INFINITE);
    GetProcessTimes(hp,
                    LPFILETIME(&tCreate),
                    LPFILETIME(&tExit),
                    LPFILETIME(&tKernel),
                    LPFILETIME(&tUser));
    GetProcessMemoryInfo(hp, &pmc, sizeof(pmc));
    GetExitCodeProcess(hp, &exitc);
    printf("\n");
    printf("Process returned %d(0x%08X)\n", exitc, exitc);
    printf("mem:  %lu MB\n", (pmc.PeakPagefileUsage)/1000000);
    printf("real: %llu ms\n", (tExit-tCreate)/10000);
    printf("ker:  %llu ms\n", tKernel/10000);
    printf("user: %llu ms\n", tUser/10000);
    return 0;
}

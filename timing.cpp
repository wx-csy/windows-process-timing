#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <windows.h>
#include "def.h"
using namespace std;

unsigned long long tCreate, tExit, tKernel, tUser;
HANDLE hp;
STARTUPINFO si;
PROCESS_INFORMATION pi;
LPTSTR lpMsgBuf;

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
    si.cb = sizeof(STARTUPINFO);
    if (argc != 2){
        printf("Please input a correct file name!\n", argv[1]);
        return 0;
    }
    if (!CreateProcess(argv[1], NULL, NULL, NULL, false, 0, NULL, NULL, &si, &pi)){
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

    printf("\n");
    printf("Physical time: %llu ms\n", (tExit-tCreate)/10000);
    printf("Kernel time:   %llu ms\n", tKernel/10000);
    printf("User time:     %llu ms\n", tUser/10000);
    return 0;
}

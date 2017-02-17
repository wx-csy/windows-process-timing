#include <cstdio>
#include <windows.h>

unsigned long long tCreate, tExit, tKernel, tUser;
HANDLE hp;
STARTUPINFO si;
PROCESS_INFORMATION pi;

int main(int argc, char* argv[]){
    si.cb = sizeof(STARTUPINFO);
    if (!CreateProcess(argv[1], NULL, NULL, NULL, false, 0, NULL, NULL, &si, &pi)){
        printf("Failed to create process %s!\n", argv[1]);
        return 0;
    }
    hp = pi.hProcess;
    WaitForSingleObject(hp, INFINITE);
    GetProcessTimes(hp,
                    reinterpret_cast<LPFILETIME>(&tCreate),
                    reinterpret_cast<LPFILETIME>(&tExit),
                    reinterpret_cast<LPFILETIME>(&tKernel),
                    reinterpret_cast<LPFILETIME>(&tUser));

    printf("\n");
    printf("Physical time: %ld ms\n", (tExit-tCreate)/10000);
    printf("Kernel time: %ld ms\n", tKernel/10000);
    printf("User time: %ld ms\n", tKernel/10000);
    return 0;
}

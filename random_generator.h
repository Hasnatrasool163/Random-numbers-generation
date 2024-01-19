#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H
#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "psapi.lib")
#include <lmcons.h>

int generate_true_random_number()  // cryptographically secure random number
{
    HCRYPTPROV hCryptProv;
    BYTE pbData[8];  //  8-byte integer ( based on user can also change to 4 byte number to as required!

    if (CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    {
        LARGE_INTEGER perfCount;
        QueryPerformanceCounter(&perfCount);
        SYSTEMTIME sysTime;
        GetSystemTime(&sysTime);
        DWORD processId = GetCurrentProcessId();
        DWORD threadId = GetCurrentThreadId();

        int* dynamicVariable = (int*)malloc(sizeof(int));
        *dynamicVariable = 42;
        void* dynamicVariableAddress = &dynamicVariable;
         // Free dynamically allocated memory
        free(dynamicVariable);

         // More complex inline assembly for entropy (example)
        int systemCallResult;
        asm("movl $5, %0" : "=r" (systemCallResult));

        HMODULE moduleHandle;
        GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)&generate_true_random_number, &moduleHandle);
        void* moduleBaseAddress = (void*)moduleHandle;

        static int staticVariable = 73;
        void* staticVariableAddress = &staticVariable;

        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        int numberOfProcessors = sysInfo.dwNumberOfProcessors;

        // not adding process memory for testing!
        /*
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        SIZE_T workingSetSize = pmc.WorkingSetSize;
        */

        // Introduce additional entropy from system time in seconds
        unsigned long long systemTimeSeconds = ((unsigned long long)sysTime.wHour * 3600) +
                                               ((unsigned long long)sysTime.wMinute * 60) +
                                               sysTime.wSecond;

        WCHAR userName[UNLEN + 1];
        DWORD userNameSize = UNLEN + 1;
        GetUserNameW(userName, &userNameSize);

        DWORD handleCount = GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS);

        OSVERSIONINFO osVersionInfo;
        osVersionInfo.dwOSVersionInfoSize = sizeof(osVersionInfo);
        GetVersionEx(&osVersionInfo);

        DWORD volumeSerialNumber;
        char systemDrive[MAX_PATH];
        GetSystemDirectoryA(systemDrive, MAX_PATH);
        GetVolumeInformationA(systemDrive, NULL, 0, &volumeSerialNumber, NULL, NULL, NULL, 0);

        UINT consoleInputCodePage = GetConsoleCP();

        DWORD tickCount = GetTickCount();

        unsigned long long seed =
            (unsigned long long)perfCount.QuadPart +
            ((unsigned long long)sysTime.wMilliseconds << 32) +
            (unsigned long long)processId +
            (unsigned long long)threadId +
            (unsigned long long)dynamicVariableAddress +
            (unsigned long long)systemCallResult +
            (unsigned long long)moduleBaseAddress +
            (unsigned long long)staticVariableAddress +
            (unsigned long long)numberOfProcessors +
            //  line related to GetProcessMemoryInfo
            // (unsigned long long)workingSetSize +
            (unsigned long long)userName +
            (unsigned long long)handleCount +
            (unsigned long long)osVersionInfo.dwMajorVersion +
            (unsigned long long)osVersionInfo.dwMinorVersion +
            (unsigned long long)volumeSerialNumber +
            (unsigned long long)consoleInputCodePage +
            (unsigned long long)tickCount +
            systemTimeSeconds;  // Add system time in seconds as additional entropy

        seed = (seed * 9301 + 49297) % 233280;

        memcpy(pbData, &seed, sizeof(seed));

        if (CryptGenRandom(hCryptProv, sizeof(pbData), pbData))
        {
            CryptReleaseContext(hCryptProv, 0);
            return *(int*)pbData;
        }
        else
        {
            printf("Error generating random numbers with CryptGenRandom.\n");
        }
    }
    else
    {
        DWORD error = GetLastError();
        printf("Error acquiring Cryptographic Context. Error code: %lu\n", error);
    }

    return -1;
}

#endif // RANDOM_GENERATOR_H

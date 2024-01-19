/* The Author of this Code is Muhammad Hasnat Rasool
 Purpose of this code was to incorporate as many entropies as possible.
  Your are fee to use this code and include more condition to make a great real world
  program to help generate randomness
   for more information about author visit
	sites.google.com/view/codingworldhere.com
	*/

/*
     ------------------------------------Entropy Sources:---------------------------------------------------
   - QueryPerformanceCounter: High-resolution performance counter.
   - GetSystemTime: System time.
   - GetCurrentProcessId: Process ID.
   - GetCurrentThreadId: Thread ID.
   - Dynamic Memory Allocation: Address of a dynamically allocated variable.
   - Inline Assembly: Result of a simple inline assembly instruction.
   - GetModuleHandleEx: Module base address.
   - Static Variable: Address of a static variable.
   - GetSystemInfo: Number of processors.
   - System Time in Seconds: Sum of hours, minutes, and seconds.
   - GetUserName: User name.
   - GetGuiResources: Number of GDI objects.
   - GetVersionEx: Operating system version information.
   - GetVolumeInformation: Volume serial number.
   - GetConsoleCP: Console input code page.
   - GetTickCount: System tick count.
   - Additional Suggestions:
      - Hardware-based entropy sources (e.g., RDTSC instruction).
      - Network activity or external inputs.
      - Environmental variables or system configuration.
      - Hardware-specific characteristics (e.g., device IDs).
      - Sensor data (if applicable).
      - Cryptographic libraries (if permitted).
      -----------------------------------------------------------------------------------------------------------------
      we cannot call its perfect contains limitations as well thankyou for your time to read and participate.
      we appreciate your participation.
      goodluck!
*/


#pragma comment(lib, "psapi.lib")

//#include "random_generator.h"
#include <windows.h>
//  GetProcessMemoryInfo ( add this header to add an extra entropy still we have added system time in seconds for its replacement)
// #include <psapi.h>
#include <lmcons.h>
#include <stdio.h>


//  ------------prototype --------------
int generate_true_random_number(); // cryptographically secure random number


// -----------------main function -------------------------
int main()
{
    int random_number = generate_true_random_number();  // cryptographically secure random number

    if (random_number != -1)
    {
        printf("True Random Number: %d\n", random_number);
    }

    return 0;
}

// ------------------------function implementation-----------------------------------------
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




/* The Author of this Code is Muhammad Hasnat Rasool
 Purpose of this code was to incorporate as many entropies as possible.
  Your are fee to use this code and include more condition to make a great real world
  program to help generate randomness
   for more information about author visit
	sites.google.com/view/codingworldhere.com
	work :mail address :fa23-bcs-116@cuilahore.edu.pk
	personal mail address: hasnatrasool163@gmail.com
	*/

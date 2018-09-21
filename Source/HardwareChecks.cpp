// Hardware checks for engine starts.
#include "HardwareChecks.h"
#include "Windows.h"
#include <wchar.h>
#include <iostream>
using namespace std;


inline void CheckRAM()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	cout << "Total System Memory: " << (statex.ullTotalPhys / 1024) / 1024 << "MB" << endl;

}
inline DWORD ReadCPUSpeed() {
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz;
	DWORD type = REG_DWORD;
	HKEY hKey;
	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
		0,
		KEY_READ,
		&hKey);
	if (lError == ERROR_SUCCESS) {
		// query the key:
		RegQueryValueEx(hKey,
			L"MHz",
			NULL,
			&type,
			(LPBYTE)&dwMHz,
			&BufSize);
	}
	cout << "CPU Speed: ~" << dwMHz / 1000000 << "MHz" << endl;
	return dwMHz;
}


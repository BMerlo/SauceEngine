// Hardware checks for engine starts.
#include "HardwareChecks.h"
#include <Windows.h>
#include <wchar.h>
#include <iostream>
#include <stdio.h>
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
inline void mikeCheckHardware()
	{
		typedef BOOL(WINAPI *P_GDFSE)(LPCTSTR, PULARGE_INTEGER,
			PULARGE_INTEGER, PULARGE_INTEGER);


		BOOL  fResult;

		char  *pszDrive = NULL,
			szDrive[4];
		/*
		DWORD dwSectPerClust,
			dwBytesPerSect,
			dwFreeClusters,
			dwTotalClusters;

		P_GDFSE pGetDiskFreeSpaceEx = NULL;
		*/
		unsigned __int64 i64FreeBytesToCaller,
			i64TotalBytes,
			i64FreeBytes;

		/*
		   Command line parsing.

		   If the drive is a drive letter and not a UNC path, append a
		   trailing backslash to the drive letter and colon.  This is
		   required on Windows 95 and 98.
		*/
		char driveLetter;

		//if (argc != 2)
		//{
			//printf("usage:  %s <drive|UNC path>\n", argv[0]);
			//printf("\texample:  %s C:\\\n", argv[0]);
		cout << "Enter the drive you wish to show: ";
		cin >> driveLetter;
		cout << endl;
		//}

		//pszDrive = argv[1];

		//if (pszDrive[1] == ':')
		//{
		szDrive[0] = driveLetter;
		szDrive[1] = ':';
		szDrive[2] = '\\';
		szDrive[3] = '\0';

		pszDrive = szDrive;
		//}

		/*
		   Useful for Windows 85, 96 and XP only:
		   Use GetDiskFreeSpaceEx if available; otherwise, use
		   GetDiskFreeSpace.

		   Note: Since GetDiskFreeSpaceEx is not in Windows 95 Retail, we
		   dynamically link to it and only call it if it is present.  We
		   don't need to call LoadLibrary on KERNEL32.DLL because it is
		   already loaded into every Win32 process's address space.
		*/
		//pGetDiskFreeSpaceEx = (P_GDFSE)GetProcAddress(
		//	GetModuleHandle("kernel32.dll"),
		//	"GetDiskFreeSpaceExA");
		//if (pGetDiskFreeSpaceEx)
		//{
			//fResult = pGetDiskFreeSpaceEx(pszDrive,
		fResult = GetDiskFreeSpaceExA(pszDrive,
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);
		if (fResult)
		{
			printf("\n\nGetDiskFreeSpaceEx reports\n\n");
			printf("Available space to caller = %I64u MB\n",
				i64FreeBytesToCaller / (1024 * 1024));
			printf("Total space               = %I64u MB\n",
				i64TotalBytes / (1024 * 1024));
			printf("Free space on drive       = %I64u MB\n",
				i64FreeBytes / (1024 * 1024));

			if (i64FreeBytes / (1024 * 1024) >= 300)
				cout << "You have more than 300MB on your hard drive\n";
			else
				cout << "You do not have enough hard drive space\n";
		}
		//}
		/*
		else
		{
			fResult = GetDiskFreeSpace(pszDrive,
				&dwSectPerClust,
				&dwBytesPerSect,
				&dwFreeClusters,
				&dwTotalClusters);
			if (fResult)
			{
				// force 64-bit math
				i64TotalBytes = (__int64)dwTotalClusters * dwSectPerClust *
					dwBytesPerSect;
				i64FreeBytes = (__int64)dwFreeClusters * dwSectPerClust *
					dwBytesPerSect;

				printf("GetDiskFreeSpace reports\n\n");
				printf("Free space  = %I64u MB\n",
					i64FreeBytes / (1024 * 1024));
				printf("Total space = %I64u MB\n",
					i64TotalBytes / (1024 * 1024));
			}
		}


		if (!fResult)
			printf("error: %lu:  could not get free space for \"%s\"\n",
				GetLastError());
				*/
				//end of HardDriveSpace check
	}



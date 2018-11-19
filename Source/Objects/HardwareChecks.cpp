/*##############################################################################
#																			   #
# Copyright 2018 Sauce Pig All Rights Reserved.					               #
# Developed by Boris Merlo, Michael Noseworthy and Peter Tu                    #
#                                                                              #
##############################################################################*/

#define _CRT_SECURE_NO_WARNINGS
#include "HardwareChecks.h"
#include <Windows.h>
#include <wchar.h>
#include <iostream>
#include <stdio.h>
using namespace std;

//# Hardware checks for engine starts.
inline void CheckRAM()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	cout << "Total System Memory: " << (statex.ullTotalPhys / 1024) / 1024 << "MB" << endl;

}

inline DWORD ReadCPUSpeedFromRegistry(DWORD dwCPU)
{
	HKEY hKey;
	DWORD dwSpeed;

	// Get the key name
	wchar_t szKey[256];
#pragma warning(disable:4996)
	_snwprintf(szKey, sizeof(szKey) / sizeof(wchar_t),
		L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d\\", dwCPU);
#pragma warning(disable:4996)
	// Open the key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
	{
		return 0;
	}

	// Read the value
	DWORD dwLen = 4;
	if (RegQueryValueEx(hKey, L"~MHz", NULL, NULL, (LPBYTE)&dwSpeed, &dwLen) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 0;
	}

	// Cleanup and return
	RegCloseKey(hKey);
	return dwSpeed;
}

inline void CheckCPU() {
	SYSTEM_INFO theInfo;
	GetSystemInfo(&theInfo);

	// Log number of CPUs and speeds

	for (DWORD i = 0; i < theInfo.dwNumberOfProcessors; ++i)
	{
		DWORD dwCPUSpeed = ReadCPUSpeedFromRegistry(i);
		printf("CPU %d speed: ~%dMHz\n", i, dwCPUSpeed);
	}
}

inline int CheckHDDHardware()
	{
		typedef BOOL(WINAPI *P_GDFSE)(LPCTSTR, PULARGE_INTEGER,
			PULARGE_INTEGER, PULARGE_INTEGER);


		BOOL  fResult;

		char  *pszDrive = NULL,
			szDrive[4];

		unsigned __int64 i64FreeBytesToCaller,
			i64TotalBytes,
			i64FreeBytes;

		//cout << "Enter the drive you wish to show: ";
		TCHAR ExeName[MAX_PATH];
		GetModuleFileName(NULL, ExeName, MAX_PATH); //Getting path of current location of Sauce Engine.
#pragma warning(disable:4244)
		TCHAR driveLetter = ExeName[0]; //Putting in the drive letter of Sauce Engine.
#pragma warning(disable:4244)
		//cin >> driveLetter;
		//cout << endl;

		szDrive[0] = driveLetter;
		szDrive[1] = ':';
		szDrive[2] = '\\';
		szDrive[3] = '\0';

		pszDrive = szDrive;

		fResult = GetDiskFreeSpaceExA(pszDrive,
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);// Gets disk information from drive letter
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
			{
				cout << "You have more than 300MB on your hard drive\n"; 
				return 1;
			}
			else
			{
				cout << "You do not have enough hard drive space\n";
				LPCTSTR Caption = L"Hard Drive Space Error";
				MessageBox(NULL,
					L"You do not have enough hard drive space.\n"
					L"You need to have at least 300 MB of HD space.\n"
					L"Exiting Application.\n",
					Caption,
					MB_OK);
				return 0;
			}
		}
		else
		{
			cout << "Hardware Checks\n";
			LPCTSTR Caption = L"Hard Drive Space Error";
			MessageBox(NULL,
				L"Error undefined in HDD Hardware Check."
				L"You should never reach this point.",
				Caption,
				MB_OK);
			return 0;
		}
	}



// SauceEngine.cpp : Defines the entry point for the console application.
//

/*##############################################################################
#                                                                             #                                                                                                                                                                    #
# Developed by Boris Merlo, Michael Noseworthy and Peter Tu                   #
#                                                                             #                                                                                                                                                                  #
##############################################################################*/
// SauceEngine.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
//#include "../Lib/DirectX/d3dApp.h"
#include "HardwareChecks.cpp"
#include <iostream>
#include "windows.h"
using namespace std;


int main()
{
	HANDLE ghMutex;
	ghMutex = CreateMutex(NULL,
		FALSE, L"SauceEngine");

	

	if (ghMutex == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return 1;
	}


	DWORD result = WaitForSingleObject(ghMutex, 0);

	if (result == WAIT_OBJECT_0)
	{
		//main code here    
		CheckRAM();
		CheckCPU();
		CheckHDDHardware();
		cout << "Code ends and infinite loop begins (Close window manually)" << endl;
		
	}
	else
	{
		cout << "Apps already opened." << endl;
		system("PAUSE");
		return 1;
	}

	



	while (true)
	{

	}


	CloseHandle(ghMutex);

	return 0;
}
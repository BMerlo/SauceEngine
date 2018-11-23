/*##############################################################################
#																			   #
# Copyright 2018 Sauce Pig All Rights Reserved.					               #
# Developed by Boris Merlo, Michael Noseworthy and Peter Tu                    #
#                                                                              #
##############################################################################*/
#include "MainWindow.h"
#include "Game.h"
#include "ChiliException.h"
#include "Objects/HardwareChecks.cpp"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{

	//Checking window uniqueness 
	HANDLE ghMutex;
	ghMutex = CreateMutex(NULL,
		FALSE, L"SauceEngine");

	DWORD err = GetLastError();
	LPTSTR Error = 0;
	LPCTSTR Opened = L"Error, close it";
	if (ghMutex == NULL)
	{
		//moved out of scope items here @Boris

		if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			err,
			0,
			(LPTSTR)&Error,
			0,
			NULL) == 0)
		{
			// Failed in translating message
		}

		// Display message.
		//MessageBox(NULL, L"CreateMutex error", Error, MB_OK | MB_ICONWARNING);
		return 1;
	}

	//Adding check hardware
	DWORD result = WaitForSingleObject(ghMutex, 0);

	if (result == WAIT_OBJECT_0)
	{
		//OutputDebugStringA("You have more than 300MB on your hard drive\n");
		//main code here    
		CheckRAM();
		int ten = 0;
		CheckCPU();
		int hardwareResult;
		hardwareResult = CheckHDDHardware();
		if (hardwareResult == 0)
			return 1;
		//cout << "Code ends and infinite loop begins (Close window manually)" << endl;

		try
		{
			MainWindow wnd(hInst, pArgs);
			try
			{
				Game theGame(wnd);
				while (wnd.ProcessMessage())
				{
					theGame.Go();
				}
			}
			catch (const ChiliException& e)
			{
				const std::wstring eMsg = e.GetFullMessage() +
					L"\n\nException caught at Windows message loop.";
				wnd.ShowMessageBox(e.GetExceptionType(), eMsg);
			}
			catch (const std::exception& e)
			{
				// need to convert std::exception what() string from narrow to wide string
				const std::string whatStr(e.what());
				const std::wstring eMsg = std::wstring(whatStr.begin(), whatStr.end()) +
					L"\n\nException caught at Windows message loop.";
				wnd.ShowMessageBox(L"Unhandled STL Exception", eMsg);
			}
			catch (...)
			{
				wnd.ShowMessageBox(L"Unhandled Non-STL Exception",
					L"\n\nException caught at Windows message loop.");
			}
		}




		catch (const ChiliException& e)
		{
			const std::wstring eMsg = e.GetFullMessage() +
				L"\n\nException caught at main window creation.";
			MessageBox(nullptr, eMsg.c_str(), e.GetExceptionType().c_str(), MB_OK);
		}
		catch (const std::exception& e)
		{
			// need to convert std::exception what() string from narrow to wide string
			const std::string whatStr(e.what());
			const std::wstring eMsg = std::wstring(whatStr.begin(), whatStr.end()) +
				L"\n\nException caught at main window creation.";
			MessageBox(nullptr, eMsg.c_str(), L"Unhandled STL Exception", MB_OK);
		}
		catch (...)
		{
			MessageBox(nullptr, L"\n\nException caught at main window creation.",
				L"Unhandled Non-STL Exception", MB_OK);
		}
	}
		else
		{
			cout << "Apps already opened." << endl;
			//system("PAUSE");
			//added messagebox @Boris
			MessageBox(NULL, L"CreateMutex error\n" L"Do not open multiple applications of Sauce Engine", Opened, MB_OK | MB_ICONWARNING);
			return 1;
		}



		CloseHandle(ghMutex);
}
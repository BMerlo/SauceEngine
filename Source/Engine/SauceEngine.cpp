/*##############################################################################
#																			   #
# Copyright 2018 Sauce Pig All Rights Reserved.					               #
# Developed by Boris Merlo, Michael Noseworthy and Peter Tu                    #
#                                                                              #
##############################################################################*/
#include "MainWindow.h"
#include "Game.h"
#include "SauceException.h"
#include "Objects/HardwareChecks.cpp"
#include "sol.hpp"
#include <string>

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	

	HANDLE ghMutex;
	ghMutex = CreateMutex(NULL,
		FALSE, L"SauceEngine");

	DWORD err = GetLastError();
	LPTSTR Error = 0;
	LPCTSTR Opened = L"Error, close it";
	if (ghMutex == NULL)
	{

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

		return 1;
	}

	DWORD result = WaitForSingleObject(ghMutex, 0);

	

	if (result == WAIT_OBJECT_0)
	{
		//main code here    
		
		sol::state lua;

		lua["message"] = [](const std::string& msg) {
			MessageBoxA(nullptr, msg.c_str(), "Lua Message", MB_OK);
		};
		lua.do_string("message('Howdy, from Lua!')");
		

		CheckRAM();
		int ten = 0;
		CheckCPU();
		int hardwareResult;
		hardwareResult = CheckHDDHardware();
		if (hardwareResult == 0)
			return 1;

		luaopen_base(lua);
		string outmsg = lua.do_file("luatest.txt");

		OutputDebugStringA("Lua---------------------------------\n");
		OutputDebugStringA(outmsg.c_str());
		OutputDebugStringA("\n");

		string outmsg2 = lua.do_string("'Lua is not fun'");
		OutputDebugStringA(outmsg2.c_str());
		OutputDebugStringA("\nLua---------------------------------\n");
		

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
			catch (const SauceException& e)
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




		catch (const SauceException& e)
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
			MessageBox(NULL, L"CreateMutex error\n" L"Do not open multiple applications of Sauce Engine", Opened, MB_OK | MB_ICONWARNING);
			return 1;
		}



		CloseHandle(ghMutex);
}
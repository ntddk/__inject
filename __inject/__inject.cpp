// __inject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tlhelp32.h>

BOOL EnablePrivilege(LPTSTR lpszPrivilege, BOOL bEnable)
{
	BOOL bResult;
	LUID luid;
	HANDLE hToken;
	TOKEN_PRIVILEGES tokenPrivileges;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) return FALSE;
	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid)) return FALSE;

	tokenPrivileges.PrivilegeCount = 1;
	tokenPrivileges.Privileges[0].Luid = luid;
	tokenPrivileges.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

	bResult = AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);

	CloseHandle(hToken);

	return bResult && GetLastError() == ERROR_SUCCESS;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hSnapshot;
	BOOL bProcess;
	PROCESSENTRY32 pEntry;
	HANDLE hProcess;

	if (!EnablePrivilege(SE_DEBUG_NAME, TRUE)) return 0;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot = INVALID_HANDLE_VALUE) return 0;

	pEntry.dwSize = sizeof(pEntry);
	if (Process32First(hSnapshot, &pEntry))
	{
		do {
			if (_wcsicmp(pEntry.szExeFile, L"calc.exe"))
			{
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pEntry.th32ProcessID);

				// InjectCode(hProcess);

				CloseHandle(hProcess);
				break;
			}
		} while (Process32Next(hSnapshot, &pEntry));
	}

	CloseHandle(hSnapshot);

	return 0;
}


// IATHook.cpp : build as dll.
//

//#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

void OutPutDebugPrintf(LPCSTR lpszFormat, ...)
{
	va_list arglist;
	va_start(arglist, lpszFormat);
	char outputstr[256];
	vsprintf(outputstr, lpszFormat, arglist);
	OutputDebugString(outputstr);
	
}// Format outputdebugstring

BOOL HookSetWindowTextW(HWND hWnd,LPCWSTR  lpString)
{
	lpString = L"中国";

	return SetWindowTextW(hWnd,lpString);
}

int startHook()
{
	DWORD originFuncBase = 0, exeBase = 0;
	originFuncBase = (DWORD)GetProcAddress(GetModuleHandle("USER32.dll"),"SetWindowTextW");// setwindowstextw's address
	exeBase = (DWORD)GetModuleHandle(NULL);//exe's baseAddr
	IMAGE_DOS_HEADER *pDOSH = (IMAGE_DOS_HEADER *)exeBase;
	IMAGE_NT_HEADERS *pNTH = (IMAGE_NT_HEADERS *)(exeBase + pDOSH->e_lfanew);
	IMAGE_IMPORT_DESCRIPTOR *pIMT = (IMAGE_IMPORT_DESCRIPTOR *)(exeBase + pNTH->OptionalHeader.DataDirectory[1].VirtualAddress);
	IMAGE_THUNK_DATA32 *pIAT;
	char szBuf[15] = {0};
	// to find dll
	for (;pIMT->Name;pIMT++)
	{
		//OutputDebugString((char *)(exeBase+pIMT->Name));
		strcpy(szBuf,(char *)(exeBase+pIMT->Name));strlwr(szBuf);// tolower
		if (!strcmp(szBuf,"user32.dll"))
		{
			break;
		}
	}
	// to find func
	if ((pIMT->Name) != NULL)
	{
		pIAT = (IMAGE_THUNK_DATA32 *)(exeBase + pIMT->FirstThunk);
		//OutPutDebugPrintf("%p",(DWORD *)originFuncBase);
		while ((pIAT->u1.Function) && (pIAT->u1.Function != (DWORD *)originFuncBase))
		{
			//OutPutDebugPrintf("%p",(pIAT->u1.Function));
			pIAT++;
		}
		if (pIAT->u1.Function)
		{
			DWORD pagePt;
			if(!VirtualProtect(&pIAT->u1.Function,4,PAGE_EXECUTE_READWRITE,&pagePt))// to make page memory is writeable
			{
				OutPutDebugPrintf("VirtualAlloc ErrorCode:[%lu]",GetLastError());
				return 1;//Error
			}
			(pIAT->u1.Function) = (DWORD *)HookSetWindowTextW;// replace the addr
		}
		else
		{
			OutPutDebugPrintf("No func");
			return 1;//Error
		}
	}
	else
	{
		OutPutDebugPrintf("No Dll");
		return 1;//Error
	}

	
	return 0;//Normal
}

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:// Entry
		OutPutDebugPrintf("Dll附加");
		if (startHook() == 0)
			OutPutDebugPrintf("IATHook成功");
		else
			OutPutDebugPrintf("IATHook失败");
		
		break;
		
	case DLL_PROCESS_DETACH:
		OutPutDebugPrintf("Dll卸载");
		break;
	}
	
    return TRUE;
}


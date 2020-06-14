// CodeInject.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <stdio.H>
#include <windows.h>

#define MAX_BUF 50

typedef struct
{
	DWORD dwMsgBoxAddr;
	LPCSTR lpMsgText;
	LPCSTR lpMsgTitle;
}PARAM_THREAD;

DWORD WINAPI ThreadProc (LPVOID lpParameter)
{
	typedef (WINAPI *pfnMessageBox)(HWND,LPCSTR,LPCSTR,UINT);
	PARAM_THREAD *tParam = (PARAM_THREAD *)lpParameter;
	pfnMessageBox MessageBoxApi = (pfnMessageBox)(tParam->dwMsgBoxAddr);
	MessageBoxApi(0,tParam->lpMsgText,tParam->lpMsgTitle,MB_OK);
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Usage:{pid}\n");
		return 1;
	}

	HMODULE moudleLbA = LoadLibraryA("user32.dll");

	char strTitle[MAX_BUF] = {0};
	char strText[MAX_BUF] = {0};
	printf("Enter the Title: ");
	scanf("%s",strTitle);
	printf("Enter the Text: ");
	scanf("%s",strText);

	DWORD dwPid = (DWORD) atoi(argv[1]);

	HANDLE hHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid);
	if (hHandle == NULL)
	{
		printf("OpenProcess Error:[%ld]",GetLastError());
		return 1;
	}
	LPVOID lpTitle = NULL, lpText = NULL, lpParam = NULL,lpThreadFunc;
	lpTitle = VirtualAllocEx(hHandle,NULL,strlen(strTitle)+1,MEM_COMMIT,PAGE_READWRITE);
	lpText = VirtualAllocEx(hHandle,NULL,strlen(strText)+1,MEM_COMMIT,PAGE_READWRITE);
	lpParam = VirtualAllocEx(hHandle,NULL,sizeof(PARAM_THREAD),MEM_COMMIT,PAGE_READWRITE);
	lpThreadFunc = VirtualAllocEx(hHandle,NULL,0x100,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	
	PARAM_THREAD *MsgParam = (PARAM_THREAD *)malloc(sizeof(PARAM_THREAD));
	HMODULE hMoudle = GetModuleHandle("user32.dll");
	if (hMoudle == NULL)
	{
		printf("GetModuleHandle Error:[%ld]",GetLastError());
		return 1;
	}
	DWORD MsgAddr = (DWORD)GetProcAddress(hMoudle,"MessageBoxA");
	if (MsgAddr == NULL)
	{
		printf("GetProcAddress Error:[%ld]",GetLastError());
		return 1;
	}

	MsgParam->dwMsgBoxAddr = MsgAddr;
	MsgParam->lpMsgTitle = (LPCSTR)lpTitle;
	MsgParam->lpMsgText = (LPCSTR)lpText;

	WriteProcessMemory(hHandle,lpThreadFunc,(LPVOID)ThreadProc,0x100,NULL);
	WriteProcessMemory(hHandle,lpTitle,strTitle,strlen(strTitle)+1,NULL);
	WriteProcessMemory(hHandle,lpText,strText,strlen(strText)+1,NULL);
	WriteProcessMemory(hHandle,lpParam,MsgParam,sizeof(PARAM_THREAD),NULL);

	CreateRemoteThread(hHandle,NULL,0,(LPTHREAD_START_ROUTINE)lpThreadFunc,lpParam,0,NULL);
	CloseHandle(hHandle);

	FreeLibrary(moudleLbA);
	return 0;
}

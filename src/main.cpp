#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define PORT 80
const char szHost[] = "http://127.0.0.1:8080";
const char szMsg[] = "HEAD / HTTP/1.0\r\n\r\n";

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void buildGui(HWND);
void makeRequest(HWND);

HWND hSearchBar, hSearchBtn;
WSADATA wsaData;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
  WNDCLASSW wc = {0};
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hInstance = hInst;
  wc.lpszClassName = L"cppBrowser";
  wc.lpfnWndProc = WindowProcedure;
  if (!RegisterClassW(&wc)) {
    return -1;
  }
  CreateWindowW(L"cppBrowser", L"cpp-browser",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    100, 100, 1000, 500,
    NULL, NULL, NULL, NULL);
  MSG msg = {0};
  while (GetMessage(&msg, NULL, NULL, NULL)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
	case WM_CREATE:
	  buildGui(hWnd); 
	  break;
	case WM_SIZE:
	  MoveWindow(hSearchBar, 10, 10, LOWORD(lp) - 270, 20, TRUE);
	  MoveWindow(hSearchBtn, LOWORD(lp) - 250, 10 , 200, 20, TRUE);
	  break;
	case WM_COMMAND:
	  if ((HWND)lp == hSearchBtn) {
	    HANDLE rt = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)makeRequest, (LPVOID)hWnd, 0, NULL);
	    WaitForSingleObject(rt, INFINITE);
		CloseHandle(rt);
	  }
	  break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProcW(hWnd, msg, wp, lp);
  }
}

void buildGui(HWND hWnd) {
  hSearchBar = CreateWindowW(L"EDIT", L"",
	WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
	10, 10, 300, 20,
	hWnd, NULL, NULL, NULL);
  hSearchBtn = CreateWindowW(L"BUTTON", L"Press to request the server.",
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
	330, 10, 200, 20,
	hWnd, NULL, NULL, NULL);
}

void makeRequest(HWND hWnd) {
  wchar_t buffer[256];
  GetWindowTextW(hSearchBar, buffer, 256);
  std::wstring url(buffer);
  if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0) {
    ExitProcess(EXIT_FAILURE);
  }
  SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
  if (client < 0) {
    ExitProcess(EXIT_FAILURE);
  }
  HOSTENT* host = gethostbyname(szHost);
  if (host == nullptr) {
    ExitProcess(EXIT_FAILURE);
  }
  SOCKADDR_IN sin;
  ZeroMemory(&sin, sizeof(sin));
  sin.sin_port = htons(PORT);
  sin.sin_family = AF_INET;
  memcpy(&sin.sin_addr.S_un.S_addr, host->h_addr_list[0], sizeof(sin.sin_addr.S_un.S_addr));
  if (connect(client, (const sockaddr *)&sin, sizeof(sin)) != 0) {
    ExitProcess(EXIT_FAILURE);
  }
  if (!send(client, szMsg, strlen(szMsg), 0)) {
    ExitProcess(EXIT_FAILURE);
  }
  char szBuffer[4096] = {0};
  char szTemp[4096] = {0};
  while(recv(client, szTemp, 4096, 0)) {
	strcat(szBuffer, szTemp);  
  }
  printf("%s", szBuffer);
  fflush(stdout);
  closesocket(client);
  getchar();
  ExitProcess(EXIT_SUCCESS);
}
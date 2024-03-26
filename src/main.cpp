#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define PORT 8080
const char request[] = "GET / HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nConnection: close\r\n\r\n";

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
	    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)makeRequest, (LPVOID)hWnd, 0, NULL);
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
  WORD DllVersion MAKEWORD(2, 1);
  if (WSAStartup(DllVersion, &wsaData) != 0) {
    ExitProcess(EXIT_FAILURE);
	return;
  }
  SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
  if (client == INVALID_SOCKET) {
    ExitProcess(EXIT_FAILURE);
	return;
  }
  struct sockaddr_in serverInfo;
  serverInfo.sin_family = AF_INET;
  serverInfo.sin_port = htons(PORT);
  serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (connect(client, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) == SOCKET_ERROR) {
	ExitProcess(EXIT_FAILURE);
	return;
  }
  if (send(client, request, strlen(request), 0) == SOCKET_ERROR) {
	ExitProcess(EXIT_FAILURE);
	return;
  }
  char szBuffer[4096];
  std::string response;
  int bytesRecv;
  while ((bytesRecv = recv(client, szBuffer, sizeof(szBuffer), 0)) > 0) {
	response.append(szBuffer, bytesRecv);
  }
  printf("%s", response.c_str());
  fflush(stdout);
  closesocket(client);
  WSACleanup();
  ExitProcess(EXIT_SUCCESS);
  return;
}
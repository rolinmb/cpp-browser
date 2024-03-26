#include <windows.h>

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void buildGui(HWND);

HWND hSearchBar, hSearchBtn;

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
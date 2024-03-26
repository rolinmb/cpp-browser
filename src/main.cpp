#include <windows.h>

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void buildGui(HWND);

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
    100, 100, 500, 500,
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
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProcW(hWnd, msg, wp, lp);
  }
}

void buildGui(HWND hWnd) {
  CreateWindowW(L"BUTTON", L"Press to request the server.",
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
	10, 10, 200, 30,
	hWnd, NULL, NULL, NULL);
}
#include <windows.h>
#include <vector>

// Global Variables
HINSTANCE hInst;
HWND hWnd;
POINT previousPoint;
COLORREF currentColor = RGB(0, 0, 0); // Default color is black
int currentLineWidth = 1; // Default line width
std::vector<std::pair<POINT, POINT>> lines; // Store drawn lines

// Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color, int lineWidth);

// Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize global variables
    hInst = hInstance;

    // Register window class
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEXW), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, nullptr, nullptr, (HBRUSH)(COLOR_WINDOW + 1), nullptr, L"PaintProgram", nullptr };
    RegisterClassExW(&wcex);

    // Create the application window
    hWnd = CreateWindowW(L"PaintProgram", L"Simple Paint Program", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd) {
        return FALSE;
    }

    // Display the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_LBUTTONDOWN:
        previousPoint.x = LOWORD(lParam);
        previousPoint.y = HIWORD(lParam);
        return 0;

    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON) {
            HDC hdc = GetDC(hWnd);
            POINT currentPoint;
            currentPoint.x = LOWORD(lParam);
            currentPoint.y = HIWORD(lParam);
            DrawLine(hdc, previousPoint.x, previousPoint.y, currentPoint.x, currentPoint.y, currentColor, currentLineWidth);
            lines.push_back({ previousPoint, currentPoint }); // Store the drawn line
            ReleaseDC(hWnd, hdc);
            previousPoint = currentPoint;
        }
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Redraw stored lines
        for (const auto& line : lines) {
            DrawLine(hdc, line.first.x, line.first.y, line.second.x, line.second.y, currentColor, currentLineWidth);
        }

        EndPaint(hWnd, &ps);
    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

// Function to draw a line
void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color, int lineWidth) {
    HPEN hPen = CreatePen(PS_SOLID, lineWidth, color);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}
#include <Windows.h>
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparm, LPARAM lparm);

LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

void InitD3D(HWND hwnd) {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3ddp;  // d3d device parameter
	memset(&d3ddp, 0, sizeof(d3ddp));
	
	d3ddp.Windowed = FALSE;  // Fullscreen
	d3ddp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3ddp.hDeviceWindow = hwnd;
	d3ddp.BackBufferFormat = D3DFMT_X8R8G8B8;  // Set the back buffer format to 32-bit
	d3ddp.BackBufferWidth = SCREEN_WIDTH;
	d3ddp.BackBufferHeight = SCREEN_HEIGHT;

	// Creates a graphics device interface
	d3d->CreateDevice( D3DADAPTER_DEFAULT,  // Select a graphics device 
					   D3DDEVTYPE_HAL,  // Hardware Abstraction Layer
					   hwnd, 
				       D3DCREATE_SOFTWARE_VERTEXPROCESSING,  // do all 3d calc in software
					   &d3ddp, 
					   &d3ddev );
}

void RenderFrame(void) {
	// clear the window to deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 0.1f, 0);
	d3ddev->BeginScene();  // begin 3d scene. Locks Buffer in video memory

	// do 3d rendaring on the back buffer

	d3ddev->EndScene();  // unlocks buffer in video memory
	d3ddev->Present(NULL, NULL, NULL, NULL);  // display rendered frame
}

void CloseD3D(void) {
	d3ddev->Release();	// close 3d device
	d3d->Release();  // release direct3d
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	HWND hwnd;  //handle for the window
	WNDCLASSEX wc;  //info about window
	memset(&wc, 0, sizeof(wc));  //clear garbage value

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;  // Make window transparent
	wc.lpszClassName = L"WindowClass1";
	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(NULL,
                      L"WindowClass1",    // name of the window class
                      L"Our First Windowed Program",   // title of the window
					  WS_EX_TOPMOST | WS_POPUP,  // Fullscreen. POPUP:Remove border
                      0,    // x-position of the window
                      0,    // y-position of the window
					  SCREEN_WIDTH,     // width of the window
                      SCREEN_HEIGHT,    // height of the window
                      NULL,   // we have no parent window, NULL
                      NULL,   // we aren't using menus, NULL
                      hInstance,    // application handle
                      NULL);  // used with multiple windows, NULL
	ShowWindow(hwnd, SW_SHOW);
	InitD3D(hwnd);

	MSG msg;

    // Enter the infinite message loop
	while(TRUE) {
	    // Check to see if any messages are waiting in the queue
	    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
	        // Translate the message and dispatch it to WindowProc()
	        TranslateMessage(&msg);
	        DispatchMessage(&msg);
	    }
	
	    // If the message is WM_QUIT, exit the while loop
	    if(msg.message == WM_QUIT)
	        break;
	
	    // Run game code here
	    // ...
	    // ...
		RenderFrame();
	}
	CloseD3D();

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparm, LPARAM lparm) {
	// sort through and find what code to run for the message given
    switch(message) {
        // this message is read when the window is closed
        case WM_DESTROY: {
            // close the application entirely
            PostQuitMessage(0);
            return 0;
        } break;
    }
    // Handle any messages the switch statement didn't
    return DefWindowProc (hwnd, message, wparm, lparm);
}
#include "GameSystem.h"
#include "types.h"

//
// ������ ��� PC ������ ���
//
// ������� ����
// �������������� �������� OpenGL
// ��������� ������� ����
//
// ������ ������������������ ��� ��� - ����� ��� ��� ���� � ����� windows ����������
// ���, �� ������� ����� ������ �� ������ �������� � �������� � ����������

namespace EngineSystem
{
struct SystemWindow
{
	WNDCLASSEX winClass;
	MSG uMsg;
	HWND hWnd;
	HGLRC hRC;
	HDC	hDC;
	unsigned int width;
	unsigned int height;
	SystemWindow() : width(800), height(600) {}
};

SystemWindow window;

void CreateSystemWindow(HINSTANCE hInstance, int nCmdShow);

// ����� ������ ����� "�����" � ���������,
// �� ������ ��� ��������� � namespace, � ����� � WinMain
int main(HINSTANCE hInstance, int nCmdShow)
{
	// ��������������� � ���� WINAPI
	memset(&window, 0, sizeof(window));

	// ������ ���������������� ������������� 
	Start();

	// ������� Windows ���� � ��������� ���� ����������
	CreateSystemWindow(hInstance, nCmdShow);

	return 0;
}

// ������� ��������� ������� ���� �� PC
// ��� ������ �� ������-�� �������, ���� ���������� �� �����
LRESULT CALLBACK WindowProc( HWND   hWnd, 
							UINT   msg, 
							WPARAM wParam, 
							LPARAM lParam )
{
	switch( msg )
	{
	case WM_MOUSEMOVE:
		{
			//test_mousePosit.x = GET_X_LPARAM(lParam); 
			//test_mousePosit.y = GET_Y_LPARAM(lParam); 
			break;
		}
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			//g_bMousing = true;
		}
		break;

	case WM_LBUTTONUP:
		{
			//g_bMousing = false;
		}
		break;

	case WM_SIZE:
		{
			//int nWidth  = LOWORD(lParam); 
			//int nHeight = HIWORD(lParam);
			//glViewport(0, 0, nWidth, nHeight);

			//glMatrixMode( GL_PROJECTION );
			//glLoadIdentity();
			//gluPerspective( 90.0, (GLdouble)nWidth / (GLdouble)nHeight, 0.1, 100.0);
		}
		break;

	case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

	default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}

// ������������� OpenGL ���������
// ��� ������ �� ������-�� �������, ���� ���������� �� �����
void Init( void )
{
	GLuint PixelFormat;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW |PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;

	window.hDC = GetDC( window.hWnd );
	PixelFormat = ChoosePixelFormat( window.hDC, &pfd );
	SetPixelFormat( window.hDC, PixelFormat, &pfd);
	window.hRC = wglCreateContext( window.hDC );
	wglMakeCurrent( window.hDC, window.hRC );

	glClearColor( 0.5f, 0.3f, 0.1f, 1.0f );

	glEnable( GL_DEPTH_TEST );
	//glEnable( GL_LIGHTING );
	glEnable( GL_NORMALIZE );
	glClearDepth(1.0f);                         
	glDepthFunc(GL_LEQUAL);      
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glShadeModel(GL_SMOOTH);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 90.0f, float(900) / 675, 0.1f, 3000.0f);

	glewInit();
}

// ���������� � ����� ������� - ����� ����� - ��� ����������
void ShutDown( void )	
{
	if( window.hRC != NULL )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( window.hRC );
		window.hRC = NULL;
	}

	if( window.hDC != NULL )
	{
		ReleaseDC( window.hWnd, window.hDC );
		window.hDC = NULL;
	}
}

// ��������� - ������� �����(������)
void Render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	SwapBuffers( window.hDC );
}

// ���������� �������� ���� � ������ ����� ����������
// ��� ������ �� ������-�� �������, ���� ���������� �� �����
void CreateSystemWindow(HINSTANCE hInstance, int nCmdShow)
{
	SystemWindow &wnd = window;
	window.winClass.lpszClassName = "GAME_WINDOWS_CLASS";
	window.winClass.cbSize        = sizeof(WNDCLASSEX);
	window.winClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window.winClass.lpfnWndProc   = WindowProc;
	window.winClass.hInstance     = hInstance;
	window.winClass.hIcon	      = LoadIcon(NULL, IDI_APPLICATION);
	window.winClass.hIconSm	      = LoadIcon(NULL, IDI_APPLICATION);
	window.winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	window.winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	window.winClass.lpszMenuName  = NULL;
	window.winClass.cbClsExtra    = 0;
	window.winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&window.winClass) ) 
	{
		Log("Can not register window class!\n");
		Log("Error code %i", (int)GetLastError());
		return;
	}

	window.hWnd = CreateWindowEx( NULL, "GAME_WINDOWS_CLASS", 
		"",
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		0, 0, window.width, window.height, NULL, NULL, hInstance, NULL );

	if ( window.hWnd == NULL )
	{
		Log("Can not create window!\n");
		Log("Error code %i\n", (int)GetLastError());
		return;
	}

	ShowWindow( window.hWnd, nCmdShow );
	UpdateWindow( window.hWnd );

	Init();
	//g_dLastTime = timeGetTime();

	while( window.uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &window.uMsg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &window.uMsg );
			DispatchMessage( &window.uMsg );
		}
		else
		{
			//g_dCurTime     = timeGetTime();
			//g_fElpasedTime = (float)((g_dCurTime - g_dLastTime) * 0.001);

			//u_time += g_fElpasedTime;

			////if(mous==2)
			////{
			//if(timegg<50)
			//{
			//	g_fElpasedTime = (float)((g_dCurTime - g_dLastTime) * 0.001);
			//	timegg+=1;
			//}
			////}

			//g_dLastTime    = g_dCurTime;

			Render();
		}
	}

	ShutDown();

	UnregisterClass( "GAME_WINDOWS_CLASS", window.winClass.hInstance );
}

};

// �������� ����� ����� � ���������� �� PC
int WINAPI WinMain(	HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR     lpCmdLine,
				   int       nCmdShow )
{
	return EngineSystem::main(hInstance, nCmdShow);
}

// ���������� ���� - ���� ���� ������ � ������� VisualStudio
// ����� ���������� �� ����� ����������
void Log( const char * str, ... )
{
	char buff[ 2048 ];
	
	va_list	args;

	va_start( args, str );
	_vsnprintf_s( buff, 2048, _TRUNCATE, str, args );
	va_end( args );

	OutputDebugString(buff);
}

// ���������� ���������� ������� �������� ���� ����������
void SetWindowSize(unsigned int width, unsigned int height)
{
	EngineSystem::window.width = width;
	EngineSystem::window.height = height;
}


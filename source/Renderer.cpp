#include <stdafx.h>
#include "renderer.h"
#include "rc_InputManager.h"

Renderer* Renderer::m_instance = nullptr;

//as there is only one renderer in game set the instance of the renderer to this object on creation
Renderer::Renderer() : m_windowWidth(0), m_windowHeight(0), m_windowHandle(nullptr), m_windowDC(nullptr),
					   m_bmpInfo(nullptr), m_bitBuffer(nullptr), m_bufferBMP(nullptr), m_bufferDC(nullptr), m_bitmapHandle(nullptr)
{
	m_instance = this;
}

//deconstruct the renderer and free m_instance to allow a new renderer to be created
Renderer::~Renderer()
{
	m_instance = nullptr;
}

//Static Windows Callback message Handle
int Renderer::Initialise(HWND a_consoleWindow, unsigned int a_width, unsigned int a_height)
{
	m_windowWidth = a_width;
	m_windowHeight = a_height;

	//register a windows class
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = InputManager::GetInstance()->HandleWindowCallbacks;
	wndClass.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = "Raycaster Framework";

	if (!RegisterClassEx(&wndClass))
	{
		return 1;
	}

	LONG x = 0; LONG y = 0;
	RECT consoleRect = { NULL };
	if (GetWindowRect(a_consoleWindow, &consoleRect))
	{
		x = consoleRect.right;
		y = consoleRect.top;
	}

	RECT windowRect = { x,y,x + (LONG)m_windowWidth, y + (LONG)m_windowHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	//Create window based on window description
	m_windowHandle = CreateWindowA("Raycaster Framework", "Main Scene",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr,
		(HINSTANCE)GetModuleHandle(NULL), nullptr);

	ShowWindow(m_windowHandle, SW_SHOW);
	MoveWindow(m_windowHandle, x, y, m_windowWidth, m_windowHeight, true);

	//Create back buffer render target
	m_windowDC = GetDC(m_windowHandle);
	if (m_windowDC == nullptr)
	{
		return 1;
	}

	//Create a bite size array that will be large enough to store bit map header information
	char* data = (char*)malloc(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
	if (data != nullptr)
	{
		m_bmpInfo = (BITMAPINFO*)data;
		m_bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_bmpInfo->bmiHeader.biWidth = (LONG)m_windowWidth;
		m_bmpInfo->bmiHeader.biHeight = -(LONG)m_windowHeight;
		m_bmpInfo->bmiHeader.biBitCount = 32;
		m_bmpInfo->bmiHeader.biPlanes = 1;
		m_bmpInfo->bmiHeader.biCompression = BI_RGB;
		m_bmpInfo->bmiHeader.biSizeImage = 0;
		m_bmpInfo->bmiHeader.biXPelsPerMeter = 0;
		m_bmpInfo->bmiHeader.biYPelsPerMeter = 0;
		m_bmpInfo->bmiHeader.biClrUsed = 256;
		m_bmpInfo->bmiHeader.biClrImportant = 256;

		//Device independent
		m_bufferBMP = CreateDIBSection(m_windowDC, m_bmpInfo, DIB_RGB_COLORS, &m_bitBuffer, NULL, 0);
		if (m_bufferBMP == nullptr)
		{
			free(data);
			return 1;
		}

		//Get Buffer device Context
		m_bufferDC = CreateCompatibleDC(m_windowDC);
		if (m_bufferDC == nullptr)
		{
			free(data);
			return 1;
		}

		m_bitmapHandle = (HBITMAP)SelectObject(m_bufferDC, m_bufferBMP);
		if (m_bitmapHandle == nullptr)
		{
			free(data);
			return 1;
		}
		free(data);
		return 0;
	}
	return 1;
}

void Renderer::ClearRenderBuffer()
{
	//Clear out bitmap Window background
	RECT clRect;
	GetClientRect(m_windowHandle, &clRect);
	FillRect(m_bufferDC, &clRect, (HBRUSH)(0x0000) + 2);
}

void Renderer::FillRenderBuffer(const unsigned int& a_x, const unsigned int& a_y,
								const unsigned int& a_width, const unsigned int& a_height, const void* a_data)
{
	//Get pointer to back buffer
	unsigned int* backBuffer = (unsigned int*)m_bitBuffer;
	//convert image data into usable pointer for drawing to back buffer
	unsigned int* imageData = (unsigned int*)a_data;
	//get the start Index in the buffer that we are going to be drawing into
	//this is the upper left corner of our quad and test to make sure we are actually trying to draw our window
	unsigned int  index = a_x + (a_y * m_windowWidth);

	if (index < (m_windowWidth * m_windowHeight))
	{
		//Copy out each row of data to bitmap buffer
		for (unsigned int y = 0; y < a_height; ++y)
		{
			if (a_y + y < m_windowHeight)
			{
				unsigned int bytesToCopy = BYTES_PER_PIXEL * a_width;
				if (a_x + a_width > m_windowWidth)
				{
					bytesToCopy = (a_width - ((a_x + a_width) - m_windowWidth)) * BYTES_PER_PIXEL;
				}
				index = a_x + ((a_y + y) * m_windowWidth);
				//copy pixel data into the back buffer
				//position on screen - BackBuffer
				//image data - pixel to copy (x, y as index) 
				memcpy(&backBuffer[index], &imageData[y * a_width], bytesToCopy);
			}	
		}
	}
}

//populate the render buffer with colour value passed in
void Renderer::FillRenderBuffer(const unsigned int& a_x, const unsigned int& a_y, const unsigned int& a_width, const unsigned int& a_height, const u32 a_colour)
{
	//get the start Index in the buffer that we are going to be drawing into
	//this is the upper left corner of our quad and test to make sure we are actually trying to draw our window
	unsigned int index = a_x + (a_y * m_windowWidth);
	if (index < (m_windowWidth * m_windowHeight))
	{
		//get a pointer to our back buffer
		unsigned int* backBuffer = (unsigned int*)m_bitBuffer;

		//calculate row size only once as we are only drawing quad shapes
		//this is done in case the dimension and starting locations passed in the function are actually outside the bounds of our renderer
		//this ensures that we do not attempt to write to memory that is outside of the render buffer
		unsigned int pixelsToFill = a_width;
		if ((a_x + a_width) > m_windowWidth)
		{
			pixelsToFill = (a_width - ((a_x + a_width) - m_windowWidth));
		}
		//fill a row buffer with the pixel data as memory is alligned horizontally for the renderer
		//can take advantage of this and fill a buffer with pixel colour data then reuse the buffer to draw each row of the quard with memcpy
		u32* rowBuffer = new u32[pixelsToFill];
		for (u32 i = 0; i < pixelsToFill; i++)
		{
			rowBuffer[i] = a_colour;
		}
		//For each row of our quad we can copy our row buffer into our render buffer
		for (unsigned int y = 0; y < a_height; ++y)
		{
			//Update our index based off our row position within the quad
			index = a_x + ((a_y + y) * m_windowWidth);
			//Test to make sure still in bounds of render buffer
			if ((a_y + y) < m_windowHeight)
			{
				//use memcpy to copy the row buffer into the backBuffer at the right index position
				memcpy(&backBuffer[index], rowBuffer, pixelsToFill * BYTES_PER_PIXEL);
				continue;
			}
			break;
		}
		delete[] rowBuffer;
	}
}

//returns the window size
void Renderer::GetWindowSize(u32& a_w, u32& a_h)
{
	a_w = m_windowWidth; a_h = m_windowHeight;
}

void Renderer::Draw()
{
	RedrawWindow(m_windowHandle, nullptr, nullptr, RDW_INVALIDATE);
}
#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <stdafx.h>

class Renderer
{
public:
	Renderer();
	~Renderer();

	static Renderer* GetInstance() { return m_instance; }
	//initialise  function
	int Initialise(HWND a_consoleWindow, unsigned int a_width, unsigned int a_height);

	//void the render buffer (clear screen)
	void ClearRenderBuffer();
	void Draw();
	//fill Render buffer with image/ texture Data
	void FillRenderBuffer(const unsigned int& a_x, const unsigned int& a_y, const unsigned int& a_width, const unsigned int& a_height, const void* a_data);
	//fill render buffer with colour value
	void FillRenderBuffer(const unsigned int& a_x, const unsigned int& a_y, const unsigned int& a_width, const unsigned int& a_height, const u32 a_colour);
	//Windows Handle
	HWND GetWindowHandle() const { return m_windowHandle; }
	//get the buffer context
	HDC	GetBufferContext() const { return m_bufferDC; }
	//Get the size of the render window
	void GetWindowSize(u32& a_w, u32& a_h);


private:
	static Renderer* m_instance;

	unsigned int m_windowWidth;
	unsigned int m_windowHeight;

	HWND			m_windowHandle;
	HDC				m_windowDC;
	void*			m_bitBuffer;
	BITMAPINFO*		m_bmpInfo;
	HBITMAP			m_bufferBMP;
	HDC				m_bufferDC;
	HBITMAP			m_bitmapHandle;
};

#endif 
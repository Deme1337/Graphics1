#pragma once
#include "stdafx.h"
#include <Windows.h>
#include <string>
class TextBox
{
public:
	TextBox();
	void CreateTextBox(HWND Parent, LPCWSTR WClass,HINSTANCE hInst, int X, int Y, int width, int height);
	~TextBox();
	HWND GetTControl();
	std::string ControlText;
	void SetText(std::string TMsg);

private:
	HWND TControl;
};


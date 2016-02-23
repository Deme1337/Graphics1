#include "stdafx.h"
#include "TextBox.h"
#include <windowsx.h>

TextBox::TextBox()
{
}

void TextBox::SetText(std::string TMsg)
{
	std::string newText = TMsg;
	this->ControlText = newText;
	std::wstring stemp = std::wstring(newText.begin(), newText.end());
	LPCWSTR sw = stemp.c_str();

	Edit_ReplaceSel(this->TControl, sw);
}

HWND TextBox::GetTControl()
{
	return this->TControl;
}


void TextBox::CreateTextBox(HWND Parent, LPCWSTR WClass,HINSTANCE hInst,  int X, int Y, int width, int height)
{
	HWND tb = CreateWindow(TEXT("edit"),  NULL, WS_CHILD | WS_VISIBLE, X, Y, width, height, Parent, NULL, hInst, NULL);
	this->TControl = tb;
}

TextBox::~TextBox()
{
}

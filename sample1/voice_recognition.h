#pragma once

#pragma warning(disable:4996)

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <locale.h>

#include "sapi.h"

void __stdcall NotifyCallback(WPARAM wParam, LPARAM lParam);

int voice_recognition();

int SetVoiceCommand();

bool GetNewHorizontalVoiceInput();

void SetNewHorizontalVoiceInput(bool _NewHorizontalVoiceInput);

bool GetNewVerticalVoiceInput();

void SetNewVerticalVoiceInput(bool _NewVerticalVoiceInput);
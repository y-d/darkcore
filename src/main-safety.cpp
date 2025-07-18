#include <windows.h>
#include <mmsystem.h>
#include <functional>
#include <iostream>
#include <cmath>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")

// --< Sounds >--
using SndCalc = std::function<char(DWORD)>;

void play(SndCalc calc) {
    HWAVEOUT Wave = 0;
    WAVEFORMATEX Format = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&Wave, WAVE_MAPPER, &Format, 0, 0, CALLBACK_NULL);
    char Sound[8000 * 30];

    for (DWORD t = 0; t < sizeof(Sound); ++t) {
		Sound[t] = calc(t);
    }

    WAVEHDR Head = { Sound, sizeof(Sound), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(Wave, &Head, sizeof(WAVEHDR));
    waveOutWrite(Wave, &Head, sizeof(WAVEHDR));
    waveOutUnprepareHeader(Wave, &Head, sizeof(WAVEHDR));
    waveOutClose(Wave);
}

VOID WINAPI sound1() {
	play([](DWORD t) -> char {
		return static_cast<char>(t % 4000 * (t >> 10));
	});
}

VOID WINAPI sound2() {
	play([](DWORD t) -> char {
        return static_cast<char>(t * (t >> 5 | t >> 8) & 127);
    });
}

VOID WINAPI sound3() {
	play([](DWORD t) -> char {
        return static_cast<char>(t * t >> 10 | t >> 5);
    });
}

VOID WINAPI sound4() {
    play([](DWORD t) -> char {
        return static_cast<char>(t >> (t & t * t >> 10 | t >> 7) | t & t >> 5);
    });
} 

VOID WINAPI sound5() {
    play([](DWORD t) -> char {
        return static_cast<char>(t*(t^t>>9));
    });
}

void WINAPI sound6() {
    play([](DWORD t) -> char {
        return static_cast<char>(((t * (t >> 9)) | ((t >> 4) | (t >> 2))));
    });
}

void WINAPI sound7() {
    play([](DWORD t) -> char {
        return static_cast<char>(t * (t & t >> 10));
    });
}

void WINAPI sound8() {
    play([](DWORD t) -> char {
        return static_cast<char>(t*(((t>>8)&(t>>10))^((t>>9)&(t>>11))));
    });
}
// --< Sounds >--

// --< Visual >--
DWORD WINAPI Dark_Shader(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    int size = width * height;

    DWORD* Pixels = (DWORD*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    while (true) {
        PRGBTRIPLE Rgb;
        HDC Screen = GetDC(0);
        HDC Memory = CreateCompatibleDC(Screen);
        BITMAPINFO bmi = { 40, width, height, 1, 24 };
        HBITMAP Bitmap = CreateDIBSection(Screen, &bmi, 0, (void**)&Rgb, 0, 0); 
        
        SelectObject(Memory, Bitmap);
        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        for (int Pixel = 0; Pixel <= size; Pixel++) {
            Rgb[Pixel].rgbtRed = std::max(50, Rgb[Pixel].rgbtRed * 5);
            Rgb[Pixel].rgbtGreen = std::max(50, Rgb[Pixel].rgbtRed * 5);
            Rgb[Pixel].rgbtBlue = std::max(50, Rgb[Pixel].rgbtRed * 5);
        }

        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
        Sleep(5);

        ReleaseDC(0, Screen);
        DeleteObject(Bitmap);
        DeleteObject(Rgb);
        DeleteDC(Memory);
        DeleteDC(Screen);
    }
    return 0;
}

DWORD WINAPI Color_Shader(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    COLORREF dwTable[] = {0xd95148,0x5969cf,0x7a5acc,0xcc5a93,0x6fc798};
    // not random values its hex color
    while (true) {
        HDC Screen = GetDC(0);

        HBRUSH brush = CreateSolidBrush(dwTable[rand() % 6]);
        
        SelectObject(Screen, brush);
        BitBlt(Screen, 0, 0, width, height, Screen, 0, 0, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(0, Screen);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI Shake(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    while (true) {
        HDC Screen = GetDC(0);
        BitBlt(Screen, 0, 0, width, height, Screen, -20, 0, SRCCOPY);
        BitBlt(Screen, 0, 0, width, height, Screen, width - 20, 0, SRCCOPY);
        BitBlt(Screen, 0, 0, width, height, Screen, 0, -20, SRCCOPY);
        BitBlt(Screen, 0, 0, width, height, Screen, 0, height - 20, SRCCOPY);
        ReleaseDC(0, Screen);
        Sleep(10);
    }
}

DWORD WINAPI Red_Shader(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    int size = width * height;

    DWORD* Pixels = (DWORD*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    while (true) {
        PRGBTRIPLE Rgb;
        HDC Screen = GetDC(0);
        HDC Memory = CreateCompatibleDC(Screen);
        BITMAPINFO bmi = { 40, width, height, 1, 24 };
        HBITMAP Bitmap = CreateDIBSection(Screen, &bmi, 0, (void**)&Rgb, 0, 0); 
        
        SelectObject(Memory, Bitmap);
        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        for (int Pixel = 0; Pixel <= size; Pixel++) {
            if (Pixel != size) {
                Rgb[Pixel].rgbtRed = Rgb[Pixel].rgbtRed % 150 + (rand() % 20);
                Rgb[Pixel].rgbtGreen =  Rgb[Pixel].rgbtGreen % 150;
                Rgb[Pixel].rgbtBlue =  Rgb[Pixel].rgbtBlue % 150;
            }
        }

        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
        Sleep(10);

        ReleaseDC(0, Screen);
        DeleteObject(Bitmap);
        DeleteObject(Rgb);
        DeleteDC(Memory);
        DeleteDC(Screen);
    }
    return 0;
}

DWORD WINAPI Furnace(LPVOID lpParam) {
    while (true) {
    	HDC Screen = GetDC(NULL);
    	int width = GetSystemMetrics(0);
        int height = GetSystemMetrics(1);
        int part = rand() % width;
        Sleep(100);
   		BitBlt(Screen, part, 10, 100, width, Screen, part, 0, SRCCOPY);
    	ReleaseDC(NULL, Screen);
	}
}

DWORD WINAPI Line_Glitch(LPVOID lpParam) {
	while (true) {
    	HDC Screen = GetDC(NULL);
    	int Width = GetSystemMetrics(0);
        int Height = GetSystemMetrics(1);
   		BitBlt(Screen, rand() % Width, 10, 100, Height, Screen, rand() % Width, 0, SRCCOPY);
    	ReleaseDC(NULL, Screen);
	}
}

DWORD WINAPI Static_Shader(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    int size = width * height;

    DWORD* Pixels = (DWORD*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    while (true) {
        PRGBTRIPLE Rgb;
        HDC Screen = GetDC(0);
        HDC Memory = CreateCompatibleDC(Screen);
        BITMAPINFO bmi = { 40, width, height, 1, 24 };
        HBITMAP Bitmap = CreateDIBSection(Screen, &bmi, 0, (void**)&Rgb, 0, 0); 
        
        SelectObject(Memory, Bitmap);
        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        for (int Pixel = 0; Pixel <= size; Pixel++) {
            int R = Rgb[Pixel].rgbtRed;
            int G = Rgb[Pixel].rgbtGreen;
            int B = Rgb[Pixel].rgbtBlue;
            Rgb[Pixel].rgbtRed = R * 2 / 4 + Pixel;
            Rgb[Pixel].rgbtGreen =  B ^ 2 / 2 + Pixel;
            Rgb[Pixel].rgbtBlue =  G + R / 3 + Pixel;
        }
        
        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
        Sleep(10);

        ReleaseDC(0, Screen);
        DeleteObject(Bitmap);
        DeleteObject(Rgb);
        DeleteDC(Memory);
        DeleteDC(Screen);
    }
    return 0;
}

DWORD WINAPI Idk_Shader(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    int size = width * height;

    DWORD* Pixels = (DWORD*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    while (true) {
        PRGBTRIPLE Rgb;
        HDC Screen = GetDC(0);
        HDC Memory = CreateCompatibleDC(Screen);
        BITMAPINFO bmi = { 40, width, height, 1, 24 };
        HBITMAP Bitmap = CreateDIBSection(Screen, &bmi, 0, (void**)&Rgb, 0, 0); 
        
        SelectObject(Memory, Bitmap);
        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        for (int i = 0; i <= size; i++) {
            // changed to i for my crazy lazyness to type Pixel
            Rgb[i].rgbtRed += 360;
            Rgb[i].rgbtGreen += 360;
            Rgb[i].rgbtBlue += 360;
        }

        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
        Sleep(10);

        ReleaseDC(0, Screen);
        DeleteObject(Bitmap);
        DeleteObject(Rgb);
        DeleteDC(Memory);
        DeleteDC(Screen);
    }
    return 0;
}

DWORD WINAPI Idk_Shader2(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    int size = width * height;

    DWORD* Pixels = (DWORD*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    while (true) {
        PRGBTRIPLE Rgb;
        HDC Screen = GetDC(0);
        HDC Memory = CreateCompatibleDC(Screen);
        BITMAPINFO bmi = { 40, width, height, 1, 24 };
        HBITMAP Bitmap = CreateDIBSection(Screen, &bmi, 0, (void**)&Rgb, 0, 0); 
        
        SelectObject(Memory, Bitmap);
        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        for (int i = 0; i <= size; i++) {
            Rgb[i].rgbtRed += i + 20;
            Rgb[i].rgbtGreen += i + 10;
            Rgb[i].rgbtBlue += i * 3;
        }

        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
        Sleep(10);

        ReleaseDC(0, Screen);
        DeleteObject(Bitmap);
        DeleteObject(Rgb);
        DeleteDC(Memory);
        DeleteDC(Screen);
    }
    return 0;
}

DWORD WINAPI ScrollDown(LPVOID lpParam) {
	while (true) {
    	HDC Screen = GetDC(NULL);
    	int Width = GetSystemMetrics(0);
        int Height = GetSystemMetrics(1);
   		BitBlt(Screen, -10, 10, Width, Height, Screen, -10, 0, SRCCOPY);
    	ReleaseDC(NULL, Screen);
        Sleep(100);
	}
}

DWORD WINAPI Idk(LPVOID lpParam) {
    while (true) {
        HDC Screen = GetDC(NULL);
        int Width = GetSystemMetrics(0);
        int Height = GetSystemMetrics(1);
        BitBlt(Screen, rand() % 100, rand() % 100, Width, Height, Screen, rand() % 100, rand() % 100, SRCCOPY);
        ReleaseDC(NULL, Screen);
        Sleep(100);
    }
}

// --< Visual >--

// --< Main >--
int main() {
    // -< Warnings >-
    if (MessageBoxW(NULL, L"This Program has epilepsy,\nClick Yes if you wanna run or No to close.", L"darkcore.exe By chinawillspyonusa", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    // -< Warnings >-

    HANDLE dark = CreateThread(0, 0, Dark_Shader, 0, 0, 0);
    HANDLE burn = CreateThread(0, 0, Furnace, 0, 0, 0);
    HANDLE down = CreateThread(0, 0, ScrollDown, 0, 0, 0);
    sound1();
    Sleep(30000);

    TerminateThread(dark, 0);
    TerminateThread(down, 0);
    HANDLE color = CreateThread(0, 0, Color_Shader, 0, 0, 0);
    HANDLE iddk = CreateThread(0, 0, Idk, 0, 0, 0);
    sound2();
    Sleep(30000);

    HANDLE shake = CreateThread(0, 0, Shake, 0, 0, 0);
    sound3();
    Sleep(30000);

    TerminateThread(shake, 0);
    TerminateThread(color, 0);
    HANDLE red = CreateThread(0, 0, Red_Shader, 0, 0, 0);
    sound4();
    Sleep(30000);

    TerminateThread(burn, 0);
    HANDLE line = CreateThread(0, 0, Line_Glitch, 0, 0, 0);
    sound5();
    Sleep(30000);

    TerminateThread(red, 0);
    TerminateThread(line, 0);
    HANDLE static_shd = CreateThread(0, 0, Static_Shader, 0, 0, 0);
    sound6();
    Sleep(30000);
    
    TerminateThread(static_shd, 0);
    HANDLE idk = CreateThread(0, 0, Idk_Shader, 0, 0, 0);
    sound7();
    Sleep(30000);

    TerminateThread(idk, 0);
    HANDLE idk2 = CreateThread(0, 0, Idk_Shader2, 0, 0, 0);
    sound8();
    Sleep(30000);
    TerminateThread(idk2, 0);
    return 1;
}
// --< Main >--

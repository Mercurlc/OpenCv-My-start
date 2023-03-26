#include <windows.h>
#include <opencv2/opencv.hpp>
#include <conio.h>

class ScreenCapturer
{
public:
    ScreenCapturer() : hdcScreen_(GetDC( nullptr ))
    {
    }

    ~ScreenCapturer()
	{
        ReleaseDC(NULL, hdcScreen_);
    }

    int mappingWeapon()
    {
        mImage_from_screen = ScreenShot();
        double minVal, maxVal;
        cv::Mat mCheck_image;
        cv::Mat weapon_temple;
        for(int i = 0; i < 3; i++)
        {
            weapon_temple = cv::imread(sfoldersWeapons[i]);
            matchTemplate(mImage_from_screen, weapon_temple, mCheck_image, cv::TM_CCORR_NORMED);
            cv::minMaxLoc(mCheck_image, &minVal, &maxVal);

            if (maxVal > 0.95)
                return i;
        }
        return 99;
    }
    int mappingWeaponMods()
    {
            double minVal, maxVal;
            cv::Mat mCheck_image;
            cv::Mat weapon_temple;
        	for (int i = 0; i < 4; i++)
        	{
        		weapon_temple = cv::imread(sfoldersmModsFlat[i]);
        		matchTemplate(mImage_from_screen, weapon_temple, mCheck_image, cv::TM_CCOEFF_NORMED);
                cv::minMaxLoc(mCheck_image, &minVal, &maxVal);

   			if (maxVal > 0.90)
        			return i;
        	}
        	return 99;
    }
    
private:
    cv::Mat ScreenShot()
    {
        const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        HDC hdcBitmap = CreateCompatibleDC(hdcScreen_);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen_, screenWidth, screenHeight);
        HGDIOBJ hOld = SelectObject(hdcBitmap, hBitmap);

        BitBlt(hdcBitmap, 0, 0, screenWidth, screenHeight, hdcScreen_, 1500, 950, SRCCOPY);

        BITMAPINFO bmpInfo = { 0 };
        bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo.bmiHeader.biWidth = screenWidth;
        bmpInfo.bmiHeader.biHeight = -screenHeight;
        bmpInfo.bmiHeader.biPlanes = 1;
        bmpInfo.bmiHeader.biBitCount = 24;
        bmpInfo.bmiHeader.biCompression = BI_RGB;

        BYTE* pBuffer = nullptr;
        HBITMAP hDIB = CreateDIBSection(hdcBitmap, &bmpInfo, DIB_RGB_COLORS, (void**)&pBuffer, NULL, 0);
        GetDIBits(hdcBitmap, hBitmap, 0, screenHeight, pBuffer, &bmpInfo, DIB_RGB_COLORS);

        cv::Mat image(screenHeight, screenWidth, CV_8UC3, pBuffer, bmpInfo.bmiHeader.biWidth * 3);

        SelectObject(hdcBitmap, hOld);
        DeleteObject(hBitmap);
        DeleteDC(hdcBitmap);

        return image.clone();
    }
    HDC hdcScreen_;
    cv::Mat mImage_from_screen;
    std::string sfoldersWeapons[3] = {"C:\\Users\\alexa\\source\\temp.png", "C:\\Users\\alexa\\source\\volt.png", "C:\\Users\\alexa\\source\\r301.png"};
    std::string sfoldersmModsFlat[4] = { "C:\\Users\\alexa\\source\\Flat_empty.png", "C:\\Users\\alexa\\source\\Flat_x1.png", "C:\\Users\\alexa\\source\\Flat_x2.png", "C:\\Users\\alexa\\source\\Flat_x3.png" };
    std::string sfoldersmModsR301[4] = { "C:\\Users\\alexa\\source\\Flat_empty.png", "C:\\Users\\alexa\\source\\Flat_x1.png", "C:\\Users\\alexa\\source\\Flat_x2.png", "C:\\Users\\alexa\\source\\Flat_x3.png" };
    std::string sfoldersmModsVolt[4] = { "C:\\Users\\alexa\\source\\Flat_empty.png", "C:\\Users\\alexa\\source\\Flat_x1.png", "C:\\Users\\alexa\\source\\Flat_x2.png", "C:\\Users\\alexa\\source\\Flat_x3.png" };
};

int main()
{
    ScreenCapturer capturer;
    int weapon_number;
	int mod_number;

    while (true)
    {
        if (GetAsyncKeyState('1'))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            weapon_number = capturer.mappingWeapon();
            //mod_number = capturer.mappingWeaponMods(0);

            if (weapon_number == 0)
            {
                //if(mod_number == 0)
                    std::cout << "it was flat empty" << std::endl;
                //if (mod_number == 1)
                    //std::cout << "it was flat x1" << std::endl;
               // if (mod_number == 2)
                    //std::cout << "it was flat x2" << std::endl;
                //if (mod_number == 3)
                    //std::cout << "it was flat x3" << std::endl;
            }

            if (weapon_number == 1)
            {
                std::cout << "it was volt" << std::endl;
            }
            if (weapon_number == 2)
            {
                std::cout << "it was r301" << std::endl;
            }
        }

        if (GetAsyncKeyState('='))
            break;
    }

    return 0;
}




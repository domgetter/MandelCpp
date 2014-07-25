#include <cstdio>
#include <cmath>
#include <ctime>
#include <windows.h>
static HWND sHwnd;
//static int count = 0;
static COLORREF redColor=RGB(255,0,0);
//static COLORREF blueColor=RGB(0,0,255);
//static COLORREF greenColor=RGB(0,255,0);

void SetWindowHandle(HWND hwnd)
{
    sHwnd=hwnd;
}

int mandelbrot(double c_real, double c_imag, int dwell){

  double z_real = c_real;
  double z_imag = c_imag;
  register double old_z_real;
  int iteration;

  //clock_t c_1 = clock();
  //int time = gettimeofday();

  for(iteration = 0; iteration < dwell; ++iteration) {
    // z_n+1 = (z_n)^2 + c
    // (a+bi)^2 == a^2 +2abi - b^2
    // z_real = a^2 - b^2 + c_a;
    // z_imag = 2ab + c_b;
    if(z_real*z_real + z_imag*z_imag > 4) break;
    old_z_real = z_real;
    z_real = z_real*z_real - z_imag*z_imag + c_real;
    z_imag = 2*old_z_real*z_imag + c_imag;
    //printf("%f\n", z_real*z_real + z_imag*z_imag);
    // printf("z_%d is %f + %fi\n", i, z_real, z_imag);
  }
  //clock_t c_2 = clock();
  //printf("%f+ %fi\n", (float)z_real, (float)z_imag);
  //printf("%d\n", iteration);
  return iteration;
}
void createBitmapHeader(){
  //BITMAPINFO bitmapInfo;
}

void setPixels(COLORREF& color=redColor)
{
  clock_t c_1 = clock();
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(sHwnd, &ps);
    HDC bufferDC;
    bufferDC = CreateCompatibleDC(NULL);

    BITMAPINFO bufferInfo;
    BITMAPINFOHEADER bufferInfoHeader;
    bufferInfo.bmiHeader = bufferInfoHeader;
    bufferInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bufferInfoHeader.biWidth = 10;
    bufferInfoHeader.biHeight = -10;
    bufferInfoHeader.biPlanes = 1;
    bufferInfoHeader.biBitCount = 24;
    bufferInfoHeader.biSizeImage = 0;
    bufferInfoHeader.biClrUsed = 0;
    bufferInfoHeader.biClrImportant = 0;
    unsigned char* bits;


    HBITMAP bufferBMP = CreateDIBSection(bufferDC, &bufferInfo, DIB_RGB_COLORS, (void**)&bits, NULL, 0);

    SelectObject(bufferDC, bufferBMP);

    int b = 100;
    printf("2");
    printf("%d", sizeof(bits));
    for(int i = 0; i < b; i += 3){
      bits[i] = 0xAA;
      bits[i+1] = 0x00;
      bits[i+2] = 0xFF;
    }
    printf("3");
    for(int i = 0; i < b; ++i){
      printf("%d\n", (int)bits[i]);
    }

    //SetDIBits(bufferDC, bufferBMP, 0, 800, &(bitmapBytes[0]), &bufferInfo, DIB_RGB_COLORS);

    BitBlt(hdc, 0, 0, 1200, 800, bufferDC, 0, 0, SRCCOPY);

    EndPaint(sHwnd, &ps);
    //clock_t c_2 = clock();
    //printf("%f", (float)c_2 - (float)c_1);


    /*
    srand(time(NULL));
    double x_lower_bound = -2.5;
    double y_upper_bound = 2.1;
    double scale = 2.2;
    double x_upper_bound = x_lower_bound+3.0*scale;
    double y_lower_bound = y_upper_bound-2.0*scale;
    int x_resolution = 1200;
    int y_resolution = 800;
    double x_pos;
    double y_pos;
    int dwell = 1200;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(sHwnd, &ps);
    HDC bufferDC;
    HBITMAP bufferBMP;

    bufferDC = CreateCompatibleDC(hdc);
    bufferBMP = CreateCompatibleBitmap(hdc, x_resolution, y_resolution);
    int bufBMP_a[sizeof(bufferBMP)];
    SelectObject(bufferDC, bufferBMP);
    SetDIBits(bufferDC, bufferBMP, 0, y_resolution, bufBMP_a);

    clock_t c_1 = clock();

    for(int y = 0; y < y_resolution; ++y){
      for(int x = 0; x < x_resolution; ++x){
        x_pos = x_lower_bound+(x)*(x_upper_bound - x_lower_bound)/(x_resolution);
        y_pos = y_upper_bound-(y)*(y_upper_bound - y_lower_bound)/(y_resolution);

        int iteration = mandelbrot(x_pos, y_pos, dwell);
        if(iteration == dwell){
          SetPixel(bufferDC, x, y, RGB(0, 0 , 0));

        } else {
          float blue = (((float)(iteration+1))/((float)dwell)) - 1.0;
          blue = 1.0-(pow(blue, 84));
          SetPixel(bufferDC, x, y, RGB(0, (int)(255.0*blue) , 0));

        }
      }
    }

    clock_t c_2 = clock();
    printf("%f", (float)c_2 - (float)c_1);
    BitBlt(hdc, 0, 0, x_resolution, y_resolution, bufferDC, 0, 0, SRCCOPY);

    EndPaint(sHwnd, &ps);
    */
    return;
}
void drawStuff() {
  HDC hdc = GetDC(sHwnd);
  SetPixel(hdc, 100, 100, RGB(255, 0, 0));
  ReleaseDC(sHwnd,hdc);

}
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    case WM_PAINT:
        SetWindowHandle(hwnd);
        //drawStuff();
        setPixels();
        break;
    case WM_CLOSE: // FAIL THROUGH to call DefWindowProc
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break; // FAIL to call DefWindowProc //
    }
    return DefWindowProc(hwnd,message,wParam,lParam);
}
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("Test");
    WNDCLASS wndclass;
    wndclass.style         = CS_HREDRAW|CS_VREDRAW ;
    wndclass.lpfnWndProc   = WndProc ;
    wndclass.cbClsExtra    = 0 ;
    wndclass.cbWndExtra    = 0 ;
    wndclass.hInstance     = hInstance ;
    wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
    wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
    wndclass.lpszMenuName  = NULL ;
    wndclass.lpszClassName = szAppName ;
// Register the window //
    if(!RegisterClass(& wndclass))
    {
        MessageBox(NULL,"Registering the class failed","Error",MB_OK|MB_ICONERROR);
        exit(0);
    }
// CreateWindow //
    HWND hwnd=CreateWindow(szAppName,"SetPixel example - programming-technique.blogspot.com",
                           WS_OVERLAPPEDWINDOW,
                           0,
                           0,
                           1200 + 16,
                           800 + 38,
                           NULL,
                           NULL,
                           hInstance,
                           NULL);
    if(!hwnd)
    {
        MessageBox(NULL,"Window Creation Failed!","Error",MB_OK);
        exit(0);
    }
// ShowWindow and UpdateWindow //
    ShowWindow(hwnd,iCmdShow);
    UpdateWindow(hwnd);
// Message Loop //
    MSG msg;
    while(GetMessage(& msg,NULL,0,0))
    {
        TranslateMessage(& msg);
        DispatchMessage(& msg);
    }
    return 0;
}

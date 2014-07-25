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

struct MandelOutput{
  int iteration;
  double degree;
};

MandelOutput mandelbrot(double c_real, double c_imag, int dwell){

  double z_real = c_real;
  double z_imag = c_imag;
  register double old_z_real;
  MandelOutput output;
  int iteration;
  double degree = 4.0;

  for(iteration = 0; iteration < dwell; ++iteration) {
    if(z_real*z_real + z_imag*z_imag > 4){
      degree = z_real*z_real + z_imag*z_imag;
      break;
    }
    old_z_real = z_real;
    z_real = z_real*z_real - z_imag*z_imag + c_real;
    z_imag = 2*old_z_real*z_imag + c_imag;
  }
  output.iteration = iteration;
  output.degree = degree;
  return output;
}
void setPixels(COLORREF& color=redColor)
{
    srand(time(NULL));
    /*//fave little mandelbrot
    double x_center = -1.2557;
    double y_center = 0.381;
    double scale = 200.0;
    */
    //default zoom out
    double x_center = 0.0;
    double y_center = 0.0;
    double scale = 0.5;

    double x_lower_bound = x_center-1.5/scale;
    double y_upper_bound = y_center+1.0/scale;
    double x_upper_bound = x_center+1.5/scale;
    double y_lower_bound = y_center-1.0/scale;
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
    SelectObject(bufferDC, bufferBMP);

    //clock_t c_1 = clock();

    for(int y = 0; y < y_resolution; ++y){
      for(int x = 0; x < x_resolution; ++x){
        x_pos = x_lower_bound+(x)*(x_upper_bound - x_lower_bound)/(x_resolution);
        y_pos = y_upper_bound-(y)*(y_upper_bound - y_lower_bound)/(y_resolution);
        int iteration;
        double degree;
        MandelOutput output = mandelbrot(x_pos, y_pos, dwell);
        iteration = output.iteration;
        degree = output.degree;

        if(iteration == dwell){
          SetPixel(bufferDC, x, y, RGB(0, 0 , 0));

        } else {
          float blue = (((float)(iteration+1))/((float)dwell)) - 1.0;
          blue = 1.0-(pow(blue, 84));
          //SetPixel(bufferDC, x, y, RGB((int)(255.0*blue), 0 , (int)(255.0*4.0/degree)));
          SetPixel(bufferDC, x, y, RGB(255-(int)(255.0*16.0/(degree*degree)), 255-(int)(255.0*16.0/(degree*degree)) , 255-(int)(255.0*16.0/(degree*degree))));

        }
      }
    }

    //clock_t c_2 = clock();
    //printf("%f", (float)c_2 - (float)c_1);
    BitBlt(hdc, 0, 0, x_resolution, y_resolution, bufferDC, 0, 0, SRCCOPY);

    EndPaint(sHwnd, &ps);
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
                           10,
                           10,
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

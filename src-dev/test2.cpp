#include "test2.h"
#include <cstdio>
#include <ctime>

void mouseMove(int x, int y){
    Display *dpy;
    Window root_window;

    dpy = XOpenDisplay(0);
    root_window = XRootWindow(dpy, 0);
    XSelectInput(dpy, root_window, KeyReleaseMask);

    XWarpPointer(dpy, None, None, 0, 0, 0, 0, x, y);

    XFlush(dpy);
}


int main(){
    //constants
    int FPS=30;
    int TIMETONEXT= 1000000 / FPS;
    int MAXITERATIONS = 30;
    bool RUNNING = true;

    //runtime numbers
    int iterations=0;
    std::clock_t start = std::clock();

    while(RUNNING){
        //if too early, wait
        std::clock_t current = std::clock();
        if(current < (start+TIMETONEXT)) continue;
        start = current;

        //else action
        //std::cout << "test " << (iterations) << std::endl;
        //mouseMove(20,0);
        Controller controller;
        // Keep this process running until Enter is pressed
        std::cout << "Press Enter to quit..." << std::endl;
        std::cin.get();

        return 0;
    }
    std::cout << "duration" << start << std::endl;
    std::cout.flush();
 }


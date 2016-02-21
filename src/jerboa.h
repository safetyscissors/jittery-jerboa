#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <stdlib.h>
#include <X11/Xmu/WinUtil.h>    // `apt-get install libxmu-dev`
//#include <gtkmm.h>
//#include <gtkmm/statusicon.h>

Display *DPY;
Window WINDOW;
int SCREEN_FAST_SCALEX = 12;
int SCREEN_FAST_SCALEY = 12;
int SCREEN_SLOW_SCALEX = 5;
int SCREEN_SLOW_SCALEY = 6;

float PINCH_THRESHOLD = 1;
float CLICK_THRESHOLD = 20;
int CLICK_DOWN = 0;

float MINMOVETHRESHOLD = .001;
float MAXMOVETHRESHOLD = 100;

float THUMB_MOVEMENT_THRESHOLD = 50;
float THUMB_ACTIVE_THRESHOLD = .2;

float INDEX_MOVEMENT_THRESHOLD = 50;
float INDEX_CLICK_THRESHOLD = .5;

bool INDEX_CLICK_ACTIVE = false;
bool THUMB_STATE_ACTIVE = false;

float Y_CLICK_THRESHOLD_OFFSET = .1;
float HAND_SPEED_CLICK_MAX = 10;
float INDEX_SPEED_CLICK_MIN = 50;

bool CURRENTLY_SCROLLING = false;
bool SCROLL_TICK = false;
int SCROLL_SPEED = 5;
int SCROLL_SPEED_COUNTER = SCROLL_SPEED;
float SCROLL_START_POS;
int SCROLL_TOLERANCE = 20;
Cursor CURSOR;

struct HandIds{
    int left;
    int right;
    int control;
};

//92712

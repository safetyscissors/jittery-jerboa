# Jittery Jerboa. 
## Ubuntu 14.04 Leap Mouse Controller.
A different kind of mouse. Its used **ONLY** to supplement a mouse. When its more convenient to not touch anything or use gesture based hotkeys. I used and tweaked on it for 4 months or so. A C++ app i run on startup.

## Problems with leapPy

+ Anytime your hands are too high, they automatically start triggering your mouse.
+ Scaling is bad. Its too precise to move across the screen, or too fast to click a button.

## Why you want to use this.

+ This is a left hand shortcut gestures and movement to supplement a mouse.
+ Only starts tracking your hand when you "pinch" the cursor.
+ Perfect for reading online without touching anything with cheeto hands.

## You dont want to use this if

+ You actually want to use the LEAP to **REPLACE** your mouse
+ You'd rather make your own. This is not the example your looking for. 

## Using jerboa

+ start listener - in terminal, ./JitteryJerboa. Needs to be run in that directory. 
+ pick up the mouse - left hand only, push your thumb against your hand. 
+ let go of the mouse - point your thumb away from your hand.
+ click - make a clicking motion with your middle finger. maybe about 1/2 to 1 inch of travel.
+ scroll - pick up the mouse first, then make a fist. Moving up and down scrolls a page. More distance steps up through three speeds
+ exit listener - press enter

## Why?

**I am right handed.** And generally most of my shortcut stuff is on my left hand. I treat the leap like a shortcut. I dont have a noticable loss of precision in using my non dominant hand because the style of interaction is so new. 

**I hate losing my mouse.** With the default softwares, it always moves when something comes into view. That made it unusable. I dont hate my physical mouse. I think its best feature is that when I let it go, it stays where I leave it. So the main point of jerboa is that you *pickup* and *putdown* the invisible mouse with your thumb.

**I need a physical mouse.** LEAP is occasionally jump when things come into focus, or false clicks. Its not 100% reliable. Also, highlighting and precise clicks of small ui buttons definitely needs a physical mouse. LEAP in no way replaces that. BUT LEAP lets you create gestures and X11 really lets you send any mouse or key command. How cool is that?! Window switching, workspace switching, sleep. LEAP and a physical mouse is the peanut butter and jelly. 

**This is not fancy.** This is the c++ example from leap's sdk. I just wrote detection functions and inserted them into their basic example. And moved my configs to the head file. [The legit sdk](https://developer.leapmotion.com/documentation/cpp/index.html?proglang=current)

## Install. 

Everything is packaged needs is included. Just run ./Jitteryjerboa

Something broke? Just run make. It'll build out of the src directory.

Configs are all in src/jerboa.h. Its a soup of arbitrary settings. Its set to what I like. Change it, then rebuild it. It needs.

+ x11 lib
+ x11 cursorfont
+ x11 xmu winutil
+ std
+ iostream
+ gcc

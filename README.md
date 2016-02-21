# Jittery Jerboa. 
## Ubuntu 14.04 Leap Mouse Controller.
A different kind of mouse. Its used **ONLY** to supplement a mouse. When its more convenient to not touch anything or use gesture based hotkeys. Its not that jittery. I do some averages things and ignore extremes. I used and tweaked on it for 4 months or so. From work, to home, to the starbucks.

## Why you want to use this

The core principle is that the LEAP mouse is a great secondary mouse input
+ You primarily use a normal mouse
+ You hate that anytime your hands are too high, they automagically do stuff
+ You have *fun* coworkers that love to give you a hand. Literally three or four hands trying to click poweroff.
+ You use linux and pyLeapMouse is the only other option
+ You eat burritos at your desk, or do howtos on lacquering wood.

## You dont want to use this 
+ You actually want to use the LEAP to **REPLACE** your mouse
+ You'd rather make your own. its fun! A challenge! This is not the example your looking for. [The legit sdk](https://developer.leapmotion.com/documentation/cpp/index.html?proglang=current) is actually really good.

## Using jerboa
+ start listener - in terminal, ./JitteryJerboa. Needs to be run in that directory. 
+ pick up the mouse - left hand only, push your thumb against your hand. 
+ let go of the mouse - point your thumb away from your hand.
+ click - make a clicking motion with your middle finger. maybe about 1/2 to 1 inch of travel.
+ scroll - pick up the mouse first, then make a fist. Moving up and down scrolls a page. More distance steps up through three speeds
+ exit listener - press enter

## What?!
**I am right handed.** And generally most of my shortcut stuff is on my left hand. I treat the leap like a shortcut. I dont have a noticable loss of precision in using my non dominant hand because the style of interaction is so new. 

**I hate losing my mouse.** With the default controllers because it always moves when something comes into view. That made it unusable. I dont hate my physical mouse. And I think thats because when I let it go, it stays where I leave it. So the main point of jerboa is that you *pickup* and *putdown* the invisible mouse with your thumb.

**I need a physical mouse.** LEAP is occasionally jitters a bit, or false clicks. Its not 100% reliable. Highlighting and precise clicks of small ui buttons definitely needs a physical mouse. LEAP in no way replaces that. BUT LEAP lets you create gestures and X11 really lets you send any mouse or key command. How cool is that?! Window switching, workspace switching, sleep. LEAP and a physical mouse is the peanut butter and jelly. 

**This is not fancy.** This is the c++ example straight from leap's sdk. I just wrote a number of functions and inserted them into their basic example. And moved my configs to the head file. 

## Install. 
**Vanilla is nice.** Vanilla ice? Chinchilla device? Everything it needs is included. Just run ./Jitteryjerboa

**You broke it.** Me too! high five. just run make. It'll build out of the src directory.

**You have man hands. or baby hands. or 5 monitors** Configs are all in src/jerboa.h. Its a soup of arbitrary settings. Its set to what I like. Change it, then rebuild it. It needs.
+ x11 lib
+ x11 cursorfont
+ x11 xmu winutil
+ std
+ iostream
+ gcc

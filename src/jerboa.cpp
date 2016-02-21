/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/
#include <iostream>
#include <string.h>
#include "Leap.h"
#include <iostream>

#include "jerboa.h"

using namespace Leap;

class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);

  private:
};

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  /*
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
  */
}

void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

        /* *************************************************************************************** *\
            send mouse click
        \* *************************************************************************************** */
        void click (int button){
        // Create and setting up the event
        /*
            XEvent event;
            memset (&event, 0, sizeof (event));
            event.xbutton.button = button;
            event.xbutton.same_screen = True;
            event.xbutton.subwindow = DefaultRootWindow (DPY);
            while (event.xbutton.subwindow){
                event.xbutton.window = event.xbutton.subwindow;
                XQueryPointer (DPY, event.xbutton.window,
                &event.xbutton.root, &event.xbutton.subwindow,
                &event.xbutton.x_root, &event.xbutton.y_root,
                &event.xbutton.x, &event.xbutton.y,
                &event.xbutton.state);
            }
            // Release
            event.type = ButtonRelease;
            if (XSendEvent (DPY, PointerWindow, True, ButtonReleaseMask, &event) == 0)
            //fprintf (stderr, "Error to send the event!\n");
            XFlush (DPY);
            //usleep (1);
            */
            std::system("xdotool click 1");
        }

        /* *************************************************************************************** *\
            detects when hands enter exit frame
            returns ""=nochange "entered" "exited"
        \* *************************************************************************************** */
        std::string handChanged( Hand oldHand, Hand newHand){
            int oldId = oldHand.id();
            int newId = newHand.id();

            if(oldId == newId) return "";
            if(oldId == -1) return (newHand.isLeft()?"left hand entered":"right hand entered");
            return (oldHand.isLeft()?"left hand exited":"right hand exited");
        }

        /* *************************************************************************************** *\
            checks position difference between frames
        \* *************************************************************************************** */
        Vector palmChanged( Hand oldHand, Hand newHand){
            Vector oldPos = oldHand.palmPosition();
            Vector newPos = newHand.palmPosition();
            Vector difference = Vector();

            difference.x = oldPos.x - newPos.x;
            difference.y = oldPos.y - newPos.y;

            //std::cout << difference << std::endl;//oldPos << newPos << std::endl;
            return difference;
        }

        /* *************************************************************************************** *\
            mouse mover through lX11
        \* *************************************************************************************** */
        void mouseMove2(Hand hand, Vector palmMove){
            if(hand.pinchStrength() < PINCH_THRESHOLD) return;
            //std::cout << hand.pinchStrength() << std::endl;
            XWarpPointer(DPY, WINDOW, None, 0, 0, 0, 0, (-SCREEN_FAST_SCALEX*palmMove.x), (SCREEN_FAST_SCALEY*palmMove.y));
            XFlush(DPY);
        }

        /* *************************************************************************************** *\
            mouse mover through lX11
        \* *************************************************************************************** */
        void mouseMove(Hand left, Hand right, Vector leftPalm){
            if(left.pinchStrength() < PINCH_THRESHOLD) return;

            if(right.id() ==-1){
                XWarpPointer(DPY, WINDOW, None, 0, 0, 0, 0, (-SCREEN_FAST_SCALEX*leftPalm.x), (SCREEN_FAST_SCALEY*leftPalm.y));
                XFlush(DPY);
            }else{
                XWarpPointer(DPY, WINDOW, None, 0, 0, 0, 0, (-SCREEN_SLOW_SCALEX*leftPalm.x), (SCREEN_SLOW_SCALEY*leftPalm.y));
                XFlush(DPY);
            }
            //std::cout << right.id() << std::endl;

        }

        /* *************************************************************************************** *\
            mouse click through lX11
        \* *************************************************************************************** */
        void mouseClick(Vector thumb, Vector pointer){
            if(std::abs(thumb.x - pointer.x) < (1-CLICK_THRESHOLD)) return;
            if(std::abs(thumb.y - pointer.y) < (1-CLICK_THRESHOLD)) return;
            if(std::abs(thumb.z - pointer.z) < (1-CLICK_THRESHOLD)) return;
            std::cout << "totally clicked" << std::endl;
        }

        /* *************************************************************************************** *\
            get active window
        \* *************************************************************************************** */
	Window get_focus_window(Display* d){
	  Window w;
	  int revert_to;
	  std::cout << ("getting input focus window ... ") << std::endl;
	  XGetInputFocus(d, &w, &revert_to); // see man
	  if(w == None){
	    std::cout << ("no focus window ") << std::endl;
	    exit(1);
	  }
	 
	  return w;
	}
        /* *************************************************************************************** *\
            get parent window
        \* *************************************************************************************** */
	Window get_top_window(Display* d, Window start){
	  Window w = start;
	  Window parent = start;
	  Window root = None;
	  Window *children;
	  unsigned int nchildren;
	  Status s;
	 
	  std::cout << ("getting top window ... ") << std::endl;
	  while (parent != root) {
	    w = parent;
	    s = XQueryTree(d, w, &root, &parent, &children, &nchildren); // see man
	 
	    if (s)
	      XFree(children);
	  }
	 
	  return w;
	}
	/* *************************************************************************************** *\
            get named window
        \* *************************************************************************************** */
	Window get_named_window(Display* d, Window start){
	  Window w;
	  std::cout << ("getting named window ... ") << std::endl;
//	  w = XmuClientWindow(d, start); // see man

//	  std::cout << ("success (window: ") << (int)w << ")"<< std::endl;
//	  return w;
	}
/* *************************************************************************************** *\
    big change
\* *************************************************************************************** */
    HandIds getHands(HandList hands){
        HandIds ids;
        ids.left = -1;
        ids.right = -1;

        for(HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl){
            const Hand hand = *hl;
            if(hand.isLeft()){
                ids.left = hand.id();
            }else{
                ids.right = hand.id();
            }
        }
        return ids;
    }


        bool detectDirectionChange(Vector *differences, int i){
            if(i<=2){
                return false;
            }

            //velocity really low
            if(differences[0].x > MINMOVETHRESHOLD){
                //different sign
                if ((differences[0].x<0)!=(differences[1].x<0)){
                    return true;
                }
            }

            //different signs
            if(differences[0].y > MINMOVETHRESHOLD){
                if ((differences[0].y<0)!=(differences[1].y<0)){
                    return true;
                }
            }
            return false;
        }

    Vector cleanMovement(Vector *positions, int i){
        Vector differences[i];
        for(int j=0;j<i;j++){ //recent to old
            differences[j].x = positions[j].x - positions[j+1].x;
            differences[j].y = positions[j].y - positions[j+1].y;
        }
        bool changed = detectDirectionChange(differences, i);
        if(changed){
            //std::cout << "change" << differences[0] << std::endl;
        }
/*
        //absolute cap movement
        differences[0].x = (differences[0].x>MAXMOVETHRESHOLD)? MAXMOVETHRESHOLD : differences[0].x;
        differences[0].y = (differences[0].y>MAXMOVETHRESHOLD)? MAXMOVETHRESHOLD : differences[0].y;
        if(differences[0].x == MAXMOVETHRESHOLD || differences[0].y == MAXMOVETHRESHOLD){
            std::cout <<"hit cap"<<std::endl;
        }

        //absolute filter movement
        differences[0].x = (abs(differences[0].x)<MINMOVETHRESHOLD)? 0 : differences[0].x;
        differences[0].y = (abs(differences[0].y)<MINMOVETHRESHOLD)? 0 : differences[0].y;
        if(differences[0].x == 0 || differences[0].y == 0){
            std::cout <<"hit min"<<std::endl;
        }
*/
        return differences[0];
    }
    void moveMouse(bool fast, bool slow, Vector handPos){
        if(fast){
            int scalex = SCREEN_FAST_SCALEX;
            int scaley = SCREEN_FAST_SCALEY;

            if(slow){
                scalex = SCREEN_SLOW_SCALEX;
                scaley = SCREEN_SLOW_SCALEY;
            }
            //std::cout << handspeed.x << handspeed.y <<std::endl;
            XWarpPointer(DPY, None, None, 0, 0, 0, 0, (scalex*handPos.x), (-scaley*handPos.y));
	    CURSOR = XCreateFontCursor(DPY, XC_pencil);
	    XDefineCursor(DPY, WINDOW, CURSOR);
            XFlush(DPY);
        }
    }
    void thumbPressed(Vector difference, Vector movement){
        //If thumb is moving or is already active, consider changing state.
        if(std::abs(movement.x)<THUMB_MOVEMENT_THRESHOLD) movement.x = 0;
        if((movement.x)!=0){
            //if thumb is pointed in, make active.
            //left if(difference.x < THUMB_ACTIVE_THRESHOLD){
            if(difference.x < THUMB_ACTIVE_THRESHOLD){
                THUMB_STATE_ACTIVE = true;
            }else{
                THUMB_STATE_ACTIVE = false;
            }
        }
    }
    void indexPressed(Vector handspeed, Vector indexSpeed, Vector compareIndex){
        if (std::abs(handspeed.x+handspeed.y) > HAND_SPEED_CLICK_MAX) return;
        if (!THUMB_STATE_ACTIVE) return;
        if (std::abs(indexSpeed.x) > INDEX_SPEED_CLICK_MIN){
            std::cout << "clickon" << std::endl;

        }
    }

    void reset (){
        bool INDEX_CLICK_ACTIVE = false;
        bool THUMB_STATE_ACTIVE = false;
    }

    void scrollDown(){
        std::system("xdotool click --clearmodifiers 5");
    }

    void scrollUp(){
        std::system("xdotool click --clearmodifiers 4");
    }

    void indexPress(Vector *boneTrail, Vector *relVelocityTrail, int trailLength, Vector handVel){
        //hand moving too fast
        if(std::abs(handVel.x+handVel.y) > HAND_SPEED_CLICK_MAX) return;
        //hand not in control
        if(!THUMB_STATE_ACTIVE) return;
        //if hand just entered
        if(trailLength<4) return;
        //if the past 5 frames were moving, accept it.
        bool mouseDown = true;
        bool mouseUp = true;
        for(int j=0;j<(trailLength-1);j++){
            if((relVelocityTrail[j]).y>(-10)){
                 mouseUp = false;
            }
            if((relVelocityTrail[j]).y<(10)){
                mouseDown = false;
            }
        }

        //change something
        if(mouseDown){
            if(!INDEX_CLICK_ACTIVE){
                if(boneTrail[0].y > (boneTrail[4].y + Y_CLICK_THRESHOLD_OFFSET)){
                    INDEX_CLICK_ACTIVE = true;
                    click(1);
                }
            }
        }
        else if(mouseUp){
            if(INDEX_CLICK_ACTIVE){
                INDEX_CLICK_ACTIVE = false;
            }
        }
    }
/* *************************************************************************************** *\
    Modified Listener
\* *************************************************************************************** */
void SampleListener::onFrame(const Controller& controller){
//TODO totally allow throwing the mouse!!
    const Frame frame=controller.frame();
    HandList hands = frame.hands();
    HandIds ids = getHands(hands);

    Hand left = frame.hand(ids.left);
    Hand right = frame.hand(ids.right);

    Hand control = left;

    if(control.isValid()){
        ids.control = control.id();
        Vector controlPos = control.palmPosition();

        Vector controlTrail[5];
        Vector thumbTrail[5];
        Vector clickDirTrail[5];
        Vector clickVelTrail[5];
        Vector boneTrail[5];

        int i;
        for( i=0; i<5; i++){
            Frame tempf = controller.frame(i);
            Hand temph = tempf.hand(ids.control);
            Finger tempt = temph.finger(ids.control*10);
            Finger tempp = temph.finger(ids.control*10+1);
            Finger tempm = temph.finger(ids.control*10+2);

            Bone tempbp = tempp.bone(static_cast<Bone::Type>(3));
            Bone tempbm = tempm.bone(static_cast<Bone::Type>(3));

            if(!temph.isValid()){ break; }

            controlTrail[i] = temph.palmPosition();
            thumbTrail[i] = tempt.direction();
            clickDirTrail[i] = tempp.direction() - tempm.direction();
            clickVelTrail[i] = tempp.tipVelocity() - tempm.tipVelocity();
            boneTrail[i] = tempbm.direction() - tempbp.direction();

        }

        bool mostFingersDown = true;
        for(int k=1;k<5;k++){
            Finger tempf = control.finger(ids.control*10+k);
            if(tempf.isExtended()){
                mostFingersDown = false;
            }
        }
        if(mostFingersDown){
            int scrollspeed=1;
            //init a scroll
            if(!CURRENTLY_SCROLLING){
                SCROLL_START_POS = controlPos.y;
                SCROLL_TICK = true;
                CURRENTLY_SCROLLING = true;
            }

            //continue a scroll
            else if(SCROLL_TICK){
                SCROLL_TICK = false;
                if(SCROLL_START_POS > controlPos.y + SCROLL_TOLERANCE){
                    scrollDown();
                }else if(SCROLL_START_POS < controlPos.y - SCROLL_TOLERANCE){
                    scrollUp();
                }

            }

            //reset scroll tick
            float diff = (std::abs(SCROLL_START_POS-controlPos.y) );
            if(diff>35){
                scrollspeed = 2;
                if(diff>50){
                    scrollspeed=4;
                }
            }

            SCROLL_SPEED_COUNTER-=scrollspeed;
            if(SCROLL_SPEED_COUNTER<=0){
                SCROLL_SPEED_COUNTER = SCROLL_SPEED;
                SCROLL_TICK = true;
            }
        }
        else{
            CURRENTLY_SCROLLING = false;
            SCROLL_SPEED_COUNTER = SCROLL_SPEED;
        }

        indexPress(boneTrail, clickVelTrail, i, control.palmVelocity());

        Finger currentThumb = control.finger(ids.control*10);
        thumbPressed(   currentThumb.direction() - control.direction(),
                        currentThumb.tipVelocity() - control.palmVelocity());
        //movements
        if(THUMB_STATE_ACTIVE){
            Vector cleanedPos = cleanMovement(controlTrail, i);
            moveMouse(control.isValid(),false, cleanedPos);
        }
    }
}


void theThingDoingEverything(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();
  std::cout << "Frame id: " << frame.id()
            << ", timestamp: " << frame.timestamp()
            << ", hands: " << frame.hands().count()
            << ", extended fingers: " << frame.fingers().extended().count()
            << ", tools: " << frame.tools().count()
            << ", gestures: " << frame.gestures().count() << std::endl;

  HandList hands = frame.hands();
  for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
    // Get the first hand
    const Hand hand = *hl;
    std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
    std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
              << ", palm position: " << hand.palmPosition() << std::endl;
    // Get the hand's normal vector and direction
    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();

    // Calculate the hand's pitch, roll, and yaw angles
    std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
              << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
              << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;

    // Get the Arm bone
    Arm arm = hand.arm();
    std::cout << std::string(2, ' ') <<  "Arm direction: " << arm.direction()
              << " wrist position: " << arm.wristPosition()
              << " elbow position: " << arm.elbowPosition() << std::endl;

    // Get fingers
    const FingerList fingers = hand.fingers();
    for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
      const Finger finger = *fl;
      std::cout << std::string(4, ' ') <<  fingerNames[finger.type()]
                << " finger, id: " << finger.id()
                << ", length: " << finger.length()
                << "mm, width: " << finger.width() << std::endl;

      // Get finger bones
      for (int b = 0; b < 4; ++b) {
        Bone::Type boneType = static_cast<Bone::Type>(b);
        Bone bone = finger.bone(boneType);
        std::cout << std::string(6, ' ') <<  boneNames[boneType]
                  << " bone, start: " << bone.prevJoint()
                  << ", end: " << bone.nextJoint()
                  << ", direction: " << bone.direction() << std::endl;
      }
    }
  }

  // Get tools
  const ToolList tools = frame.tools();
  for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
    const Tool tool = *tl;
    std::cout << std::string(2, ' ') <<  "Tool, id: " << tool.id()
              << ", position: " << tool.tipPosition()
              << ", direction: " << tool.direction() << std::endl;
  }

  // Get gestures
  const GestureList gestures = frame.gestures();

  for (int g = 0; g < gestures.count(); ++g) {
    Gesture gesture = gestures[g];

    switch (gesture.type()) {
      case Gesture::TYPE_CIRCLE:
      {
        CircleGesture circle = gesture;
        std::string clockwiseness;

        if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
          clockwiseness = "clockwise";
        } else {
          clockwiseness = "counterclockwise";
        }

        // Calculate angle swept since last frame
        float sweptAngle = 0;
        if (circle.state() != Gesture::STATE_START) {
          CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
          sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
        }
        std::cout << std::string(2, ' ')
                  << "Circle id: " << gesture.id()
                  << ", state: " << stateNames[gesture.state()]
                  << ", progress: " << circle.progress()
                  << ", radius: " << circle.radius()
                  << ", angle " << sweptAngle * RAD_TO_DEG
                  <<  ", " << clockwiseness << std::endl;
        break;
      }
      case Gesture::TYPE_SWIPE:
      {
        SwipeGesture swipe = gesture;
        std::cout << std::string(2, ' ')
          << "Swipe id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", direction: " << swipe.direction()
          << ", speed: " << swipe.speed() << std::endl;
        break;
      }
      case Gesture::TYPE_KEY_TAP:
      {
        KeyTapGesture tap = gesture;
        std::cout << std::string(2, ' ')
          << "Key Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << tap.position()
          << ", direction: " << tap.direction()<< std::endl;
        break;
      }
      case Gesture::TYPE_SCREEN_TAP:
      {
        ScreenTapGesture screentap = gesture;
        std::cout << std::string(2, ' ')
          << "Screen Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << screentap.position()
          << ", direction: " << screentap.direction()<< std::endl;
        break;
      }
      default:
        std::cout << std::string(2, ' ')  << "Unknown gesture type." << std::endl;
        break;
    }
  }

  if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
    std::cout << std::endl;
  }

}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void SampleListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char** argv) {
  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

                //my own listener
                DPY = XOpenDisplay(0);
		//WINDOW = get_focus_window(DPY);
		//WINDOW = get_top_window(DPY,WINDOW);
		//WINDOW = get_named_window(DPY,WINDOW);
                WINDOW = XDefaultRootWindow(DPY);
                //Gtk::Main::run();

  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

                //close display
                XCloseDisplay(DPY);
                //Gtk::Main::quit();

  return 0;
}

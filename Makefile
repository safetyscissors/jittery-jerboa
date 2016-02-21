LEAP_LIBRARY := ../LeapSDK/lib/x64/libLeap.so -Wl,-rpath,../LeapSDK/lib/x64

Main: src/jerboa.cpp
	$(CXX) -Wall -g src/jerboa.cpp -lX11 -o JitteryJerboa $(LEAP_LIBRARY)

Sample: example.cpp
	$(CXX) -Wall -g example.cpp -lX11 -o ListenerVs $(LEAP_LIBRARY)

Test: mouse.h
	g++ test2.cpp -g -Wall -lX11 -o test2 $(LEAP_LIBRARY)



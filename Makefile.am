#noinst_HEADERS = Assignment2.h Ball.h ControlListener.h GameObject.h OgreMotionState.h Overlay.h Player.h PlayerCamera.h PlayingField.h Simulator.h Wall.h
noinst_HEADERS = Assignment2.h Ball.h PlayingField.h BaseApplication.h GameObject.h Simulator.h OgreMotionState.h Box.h ContactSensorCallback.h Surface.h SoundSystem.h Wall.h Target.h

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir)
#assignment2_SOURCES = Assignment2.cpp Ball.cpp ControlListener.cpp GameObject.cpp Overlay.cpp Player.cpp PlayerCamera.cpp PlayingField.cpp Simulator.cpp Wall.cpp
assignment2_SOURCES = Target.cpp Assignment2.cpp Ball.cpp PlayingField.cpp BaseApplication.cpp GameObject.cpp Simulator.cpp OgreMotionState.cpp Box.cpp Surface.cpp SoundSystem.cpp Wall.cpp

assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(sdl_CFLAGS) $(SDLMIXER_CFLAGS)
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(sdl_LIBS) $(SDLMIXER_LIBS)

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign

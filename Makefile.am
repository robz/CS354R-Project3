noinst_HEADERS = Assignment3.h Ball.h BaseApplication.h GameObject.h GUI.h Simulator.h OgreMotionState.h Box.h ContactSensorCallback.h Surface.h SoundSystem.h Wall.h Target.h UDPNetEnt.h Server.h Client.h ServerToClient.h Heli.h HeliProp.h HeliChass.h

bin_PROGRAMS = assignment3
assignment3_CPPFLAGS = -I$(top_srcdir)
assignment3_SOURCES = Target.cpp Assignment3.cpp Ball.cpp BaseApplication.cpp GameObject.cpp GUI.cpp Simulator.cpp OgreMotionState.cpp Box.cpp Surface.cpp SoundSystem.cpp Wall.cpp Heli.cpp HeliProp.cpp HeliChass.cpp
 
assignment3_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(sdl_CFLAGS) $(SDLMIXER_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(SDLNET_CFLAGS)
assignment3_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(sdl_LIBS) $(SDLMIXER_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(SDLNET_LIBS)

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign

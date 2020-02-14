PLATFORM = Linux

LAB = Lab1

ifeq ($(PLATFORM),Linux)
	CFLAGS_EXT = -ffriend-injection -std=c++11
	TARGET_RELEASE = $(LAB).elf
	TARGET_DEBUG = $(LAB).elf
	SYSLIBS = -lGLEW -lglfw -lGLU -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXinerama -lXi -lXcursor	
#	SYSLPATH = -L../../../../glfw-3.1.1/src
#	SYSLPATH = -L/usr/lib/x86_64-linux-gnu
endif

LIBRARIES = $(SYSLIBS) $(EXTRAIVLIBS)  -lIvEngineOGL -lIvEngine -lIvGraphicsOGL -lIvGraphics -lIvMath -lIvUtility $(SYSLIBS)
IPATH = -I. -I../../.. -I../../../src/common/Includes

CC = g++

release: BUILD = Release
release: CFLAGS = -c -O $(IPATH) $(CFLAGS_EXT)
release: $(TARGET_RELEASE)

debug: BUILD = Debug
debug: CFLAGS = -c -g -D_DEBUG $(IPATH) $(CFLAGS_EXT)
debug: $(TARGET_DEBUG)

#------------------------------
# set based on build type

OBJSDIR = $(PLATFORM)$(BUILD)
LFLAGS = -L../../../../src/common/Libs/$(PLATFORM)$(BUILD) $(SYSLPATH)

OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
vpath %.o $(OBJSDIR)

#-------------------------------

$(TARGET_RELEASE): $(OBJS)
	cd $(OBJSDIR) && $(CC) -o ../$(TARGET_RELEASE) $(LFLAGS) $(OBJS) $(LIBRARIES)

$(TARGET_DEBUG): $(OBJS)
	cd $(OBJSDIR) && $(CC) -o ../$(TARGET_DEBUG) $(LFLAGS) $(OBJS) $(LIBRARIES)

$(OBJS): $(OBJSDIR)

.cpp.o: 
	$(CC) $(CFLAGS) -DPLATFORM_$(PLATFORM) $< -o $(OBJSDIR)/$@

$(OBJSDIR):
	-mkdir -p $(OBJSDIR)

#-------------------------------

clean:
	-rm -f $(PLATFORM)Release/$(OBJS)
	-rm -f $(PLATFORM)Debug/$(OBJS)
	-rm -f $(TARGET_RELEASE) 
	-rm -f $(TARGET_DEBUG) 

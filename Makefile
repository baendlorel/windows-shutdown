# Windows Shutdown Makefile for VS Code
# use MSVC compiler

CXX = cl.exe
RC = rc.exe
LINK = link.exe

# compiler and linker flags
CXXFLAGS = /std:c++20 /EHsc /W3 /MD /D_WINDOWS /DUNICODE /D_UNICODE /Zi
LINKFLAGS = /SUBSYSTEM:WINDOWS /DEBUG
LIBS = user32.lib gdi32.lib gdiplus.lib kernel32.lib shell32.lib comctl32.lib

# directories
SRCDIR = windows-shutdown
OUTDIR = x64\Debug
RELDIR = x64\Release

# source files
SOURCES = $(SRCDIR)\main.cpp $(SRCDIR)\config.cpp $(SRCDIR)\controller.cpp $(SRCDIR)\i18n.cpp $(SRCDIR)\image-button.cpp $(SRCDIR)\render.cpp $(SRCDIR)\window.cpp
RESOURCE = $(SRCDIR)\windows-shutdown.rc
TARGET = $(OUTDIR)\windows-shutdown.exe
RESOURCE_OBJ = $(OUTDIR)\windows-shutdown.res

# default target
all: $(TARGET)

# create out directory
$(OUTDIR):
	@if not exist "$(OUTDIR)" mkdir "$(OUTDIR)"

# compile resource file
$(RESOURCE_OBJ): $(RESOURCE) | $(OUTDIR)
	$(RC) /fo $(RESOURCE_OBJ) $(RESOURCE)

# compile main program
$(TARGET): $(SOURCES) $(RESOURCE_OBJ) | $(OUTDIR)
	$(CXX) $(CXXFLAGS) /Fe:$(TARGET) /Fo:$(OUTDIR)\ /Fd:$(OUTDIR)\ $(SOURCES) /link $(RESOURCE_OBJ) $(LIBS) $(LINKFLAGS)

clean:
	@if exist "$(OUTDIR)" rmdir /s /q "$(OUTDIR)"
	@if exist "$(RELDIR)" rmdir /s /q "$(RELDIR)"

release: CXXFLAGS = /std:c++20 /EHsc /W3 /MD /O2 /D_WINDOWS /DUNICODE /D_UNICODE /DNDEBUG
release: LINKFLAGS = /SUBSYSTEM:WINDOWS
release: TARGET = $(RELDIR)\windows-shutdown.exe
release: RESOURCE_OBJ = $(RELDIR)\windows-shutdown.res
release: $(RELDIR) $(RELDIR)\windows-shutdown.res $(RELDIR)\windows-shutdown.exe

$(RELDIR):
	@if not exist "$(RELDIR)" mkdir "$(RELDIR)"

$(RELDIR)\windows-shutdown.res: $(RESOURCE) | $(RELDIR)
	$(RC) /fo $(RELDIR)\windows-shutdown.res $(RESOURCE)

$(RELDIR)\windows-shutdown.exe: $(SOURCES) $(RELDIR)\windows-shutdown.res | $(RELDIR)
	$(CXX) $(CXXFLAGS) /Fe:$(RELDIR)\windows-shutdown.exe /Fo:$(RELDIR)\ $(SOURCES) /link $(RELDIR)\windows-shutdown.res $(LIBS) $(LINKFLAGS)

.PHONY: all clean release

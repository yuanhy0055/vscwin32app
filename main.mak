CCDEBUG    = -DWIN32 -D_DEBUG -D_CONSOLE
#定义release版本的预处理器
CCNODBG    = -DWIN32 -D_NDEBUG -D_CONSOLE

#CFLAG     = $(CFLAGS) /GS- -nologo -c -W3
CCFLAG     = $(CFLAGS) /GS -nologo -c -W3 /EHsc

!IFDEF debug
CCFLAG     = $(CCDEBUG)
CFGSET     =  TRUE
!ELSE IFDEF release
CCFLAG     = $(CCNODBG)
CFGSET     =  TRUE
!ENDIF

all: go.exe

# compile
#main.obj: main.cpp
#	cl $(CCFLAG) /Fo $@ $*.cpp
   
# link
go.exe: main.obj
	link /machine:x86 /NOLOGO /subsystem:console /out:go.exe main.obj kernel32.lib
#	link /machine:x86 /INCREMENTAL:YES /NOLOGO /subsystem:console /out:go.exe main.obj kernel32.lib

.cpp.obj:
	cl $(CCFLAG) /Fo$(<D)/$@ $*.cpp
#	cl $(CFLAGS) /Fo$(<D)/ $<

clean :  
	del /q main.obj main.exe go.exe


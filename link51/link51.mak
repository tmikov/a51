#àlink51.mak

!ifdef _DEBUG
BCCFLAGS = /ml /v /r- /p /1 /D_DEBUG /DHEAP_DBG=1
!else
BCCFLAGS = /ml /O1 /N- /p /1
!endif

.autodepend

.c.obj:
  bcc.exe /c /H=link51.sym $(BCCFLAGS) $<

link51.exe: link51.obj err.obj misc.obj collect.obj debug.obj readrex.obj process.obj symbols.obj segments.obj output.obj map.obj
  bcc $(BCCFLAGS) @&&|
link51.obj err.obj misc.obj collect.obj
debug.obj readrex.obj process.obj
symbols.obj segments.obj output.obj
map.obj
|


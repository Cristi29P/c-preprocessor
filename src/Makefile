CC = cl
L = link
CFLAGS = /MD /c /W3 /nologo /I.

build: so-cpp.exe

so-cpp.exe: hashmap.obj linkedlist.obj so-cpp.obj
    $(L) /out:$@ /nologo $**

hashmap.obj: hashmap.c
    $(CC) $(CFLAGS) hashmap.c

linkedlist.obj: linkedlist.c
    $(CC) $(CFLAGS) linkedlist.c

so-cpp.obj: so-cpp.c
    $(CC) $(CFLAGS) so-cpp.c

.IGNORE:
clean:
    del /Q /S /F *.obj so-cpp.exe
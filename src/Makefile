CC = cl
L = link
CFLAGS = /MD /c /W3 /nologo /I.

build: so-cpp.exe

so-cpp.exe: hashmap.obj linkedlist.obj so-cpp.obj so_cpp_utils.obj utils.obj cmd_line.obj
    $(L) /out:$@ /nologo $**

hashmap.obj: hashmap.c
    $(CC) $(CFLAGS) hashmap.c

linkedlist.obj: linkedlist.c
    $(CC) $(CFLAGS) linkedlist.c

cmd_line.obj: cmd_line.c
    $(CC) $(CFLAGS) cmd_line.c

utils.obj: utils.c
    $(CC) $(CFLAGS) utils.c

so_cpp_utils.obj: so_cpp_utils.c
    $(CC) $(CFLAGS) so_cpp_utils.c

so-cpp.obj: so-cpp.c
    $(CC) $(CFLAGS) so-cpp.c

.IGNORE:
clean:
    del /Q /S /F *.obj so-cpp.exe
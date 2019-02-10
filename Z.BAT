mkdir compact
copy dupldel.exe compact
start /w \develop\aspack\aspack.exe compact\dupldel.exe
del ..\duplsrc3.zip
pkzip25 -add -pass=n0n0 ..\duplsrc3 *.c* *.mak *.dfm *.h* *.bmp *.dsk *.res *.ico *.lic *.txt *.us *.jpg *.fr *.it *.bpr *.def *.pas *.bpk *.bat
del ..\duplexe3.zip
pkzip25 -add ..\duplexe3 release\*.exe release\*.dll *.lic *.txt *.txt *.us *.fr *.it

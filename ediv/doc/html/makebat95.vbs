dim fs,f,bat,fil,c
Set fs=CreateObject("Scripting.FileSystemObject")
Set f=fs.getfolder(".").files
set bat=fs.opentextfile("makedok95.bat",2,true)
c=0
for each fil in f
if lcase(fs.getextensionname(fil.path))="dok" or lcase(fs.getextensionname(fil.path))="doku" then
bat.write "..\doku.exe " & fil.name & vbcrlf
c=c+1
end if
next
bat.close
msgbox "Encontrados " & c & " fichero(s)"

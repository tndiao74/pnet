
set zip="C:\Program Files\7-Zip\7z.exe"

%zip% x .\aws\windows\windows.zip -y -o.\aws\windows
%zip% x .\curl\windows\windows.zip -y -o.\curl\windows
%zip% x .\ssl\windows\windows.zip -y -o.\ssl\windows
%zip% x .\zlib\windows\windows.zip -y -o.\zlib\windows

pause
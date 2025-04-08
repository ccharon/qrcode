# qrcode - a msdos qrcode generator

This is a qrcode generator running under MS-DOS, it needs a 8088 cpu and a CGA compatible graphics card. The application is based on the C++ Version of Nayuki QR Code-generator https://github.com/nayuki/QR-Code-generator modified to compile with openwatcom 1.9 and to run under dos. (or dosbox if you like)

The project is ment as a demo trying things for some other application. For a smaller qrcode like this my NEC V20 needs about 15-20s.

![qrcode dosbox](./doc/qrcode.png)

## usage
Start the program with 2 parameters. The first parameter is the codepage you are using. It is used for UTF-8 conversion of the text in parameter 2. Right now the program supports codepages 437, 850, 852, 866, 737. The QR Code should handle all characters of these codepages.

```
C:\>qrcode.exe 437 "this is a short text that i want to see as qrcode"
C:\>qrcode.exe 850 "auch dieser Text $%! sollte ohne größere Probleme gehn! Bähh."

C:\>qrcode.exe https://reddit.com
```

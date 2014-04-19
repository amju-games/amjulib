Building for Wii
----------------

MSVC workspace
--------------
F7 to build as normal - you don't see output until the end of the build, though.

Command line
------------
CD to this directory. Type "make". You immediately see output and can stop the build with Ctrl-C.

Uploading to Wii
----------------

Make sure the Wii is showing the Homebrew channel. Press Home to check the IP address of the Wii. (E.g. 192.168.1.4, displayed at top left.) You may need to click on the 'Earth' icon at bottom right to initialise.

On the PC, make sure the environment variable WIILOAD is set to this same IP address.

You can use Control Panel/System/Environment Variables, but then must restart MSVC or CMD to pick up the change. If you are using command-line, use set, e.g.
set WIILOAD=tcp:192.168.1.4

Command-line: 
Run wiiload.exe, giving the .dol executable name as argument, e.g.
\devkitPro\devkitPPC\bin\wiiload ..\..\Build\Wii\boot.dol
from the Script\WiiProject directory.

Sending from MSVC: you should be able to F5, but if there is a problem you won't know what it is.

Homebrew channel and devkitpro need to be in sync. E.g. when wiiload started compressing the file!

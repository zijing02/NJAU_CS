1. Don't edit the script on the Windows OS, otherwise the script's format will become the DOS or the windows,
   that can't run on the Linux OS.

2. You must set the correct APN in the "wcdma-chat-connect" script file, such as AT+CGDCONT=1,"IP","3gnet",,0,0, 
   which depend on local operator.
   

3. If the user's APN need user name and password, you must set it in the "wcdma" script file.
   If the user's APN do not need user name and password, you can only leave a blank behind the "user" and "password"
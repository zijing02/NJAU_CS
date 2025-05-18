
            SecureCRT(R) 8.7.2 (Official) -- May 14, 2020

            Copyright (C) 1995-2020 VanDyke Software, Inc.
                        All rights reserved.


Product Overview
----------------

SecureCRT protects your passwords, user accounts, and data, combining
rock-solid terminal emulation with the strong encryption, broad
authentication options, and data integrity of the Secure Shell (SSH)
protocol.

SecureCRT provides versatile solutions for business, network manage-
ment, information security, and development tasks, from accessing 
host-based applications and administering servers to securely 
accessing behind-the-firewall network resources like email, files,
and printers.

SecureCRT supports SSH2, SSH1, Telnet, serial, and Raw on all 
platforms.  On Windows, Telnet/TLS, RLogin, and TAPI are supported.
Authentication methods include password, public key, X.509 
certificate, Kerberos v5 via GSSAPI, and keyboard interactive.  
Ciphers include ChaCha20/Poly1305, AES-GCM, AES-CTR, and AES.

Choose from a wide range of emulations, most with ANSI color.  
Unicode support includes the ability to display character sets from 
multiple languages, support for languages with right-to-left reading 
order, and a character encoding list that includes commonly-used 
encodings as well as all encodings supported by the local system.

SecureCRT features a multi-session tabbed or tiled interface with 
extensive session management and customization features.  Customi-
zation options include keymaps, button bar, and login scripts, as 
well as fonts, cursors, and color schemes.  A dockable session 
manager provides quick connection to sessions.

Named sessions and firewalls let you create individual configurations
that can be used on a session-specific basis.  A personal data folder
provides private and separate storage of sensitive information so that 
other configuration data can be stored on a network drive or in the 
cloud for use on different systems or for sharing with colleagues.  
Other features include auto logon, printing, Emacs mode, and SOCKS 
firewall support.

Routine configuration tasks can be automated using powerful scripting 
capabilities including ActiveX scripting support for VBScript, 
JScript, and PerlScript on Windows, embedded Python support on all 
platforms, and the script recorder.  Securely transfer files using 
Zmodem, Xmodem, Ymodem, or Kermit from an SSH1, SSH2, or TLS session.
Upload files easily by dragging and dropping them onto an SFTP tab or 
session window.  A built-in TFTP server provides additional flexi-
bility for transferring files.

Save steps with the close integration of SecureCRT and the SecureFX(R)
file transfer client, which share sessions and settings that let you
run SFTP, FTP, SCP, and FTP/TLS file transfer sessions without
reentering passwords.

SecureCRT includes a 30-day try-before-you-buy evaluation license for
the fully functional application and access to VanDyke Software(R)
technical support.


Contents
--------

   1. New for SecureCRT 8.7
   2. Enhancement Requests, Updates, and Questions
   3. Encryption Export
   4. Upgrades
   5. Product History
   6. SecureCRT Features
   7. Reporting Bugs
   8. Contact Information


1. New For SecureCRT 8.7
------------------------

SecureCRT 8.7 increases your efficiency with a keyword highlighting 
enhancement that allows phrase and substring matches.  On Windows, a 
dockable Command Manager lets you organize, filter, and launch 
commands, and local shell support lets you work in a tabbed CMD or 
PowerShell session.  Also new are Xterm True Color (24-bit) support 
and the ability to include folders when filtering sessions in the 
Session Manager.  On macOS, Dark Mode is supported.

Here are some of the new features in SecureCRT 8.7:

Keyword highlighting enhancement

  Keyword highlighting is more versatile with the ability to match 
  phrases and substrings for literal strings and regular expressions.

Command Manager (Windows only)

  Streamline repetitive tasks with the dockable Command Manager, 
  which makes it easy to organize commands into named folders, filter 
  commands by name, and launch them with a double-click or by 
  pressing ENTER.  Commands and command folders can also be shared 
  with the button bar and vice versa.

Local shell session (Windows)

  On Windows 10 and Windows Server 2019 or later, you can open a local 
  shell session within SecureCRT.  A tabbed CMD or PowerShell session 
  makes it easy to work on the local system without having to leave 
  SecureCRT.  Local shell was already supported on macOS and Linux.

True Color support

  Xterm 24-bit color (True Color) is now supported.

UI enhancements

  Session folder filter: Include top-level folders in the Session 
  Manager or Connect dialog filtered sessions by adding a forward 
  slash to the end of the search text in the session filter box.  
  Folders that match and the sessions in those folders are included 
  in the filtered sessions.  Partial matches are supported, making it 
  even faster to find the session you’re looking for.

  Dark Mode support (Mac): Dark Mode support has been added for macOS 
  10.14 and later.

  Horizontal and vertical tiling (Mac and Linux):  Tile session 
  windows in both horizontal and vertical layouts on all platforms.

  Hide output for locked sessions: An option was added to hide all 
  output when a session is locked, adding an extra layer of security.

Improved Hex View interface

  The Hex View now shows hex data independently for each connected 
  session, making it easier to work with multiple Serial sessions.  
  A selection made on the hexadecimal pane or the ASCII pane is 
  reflected in the other pane and the ASCII pane can also be hidden.  
  The performance of the Hex View has been improved when the window 
  contains thousands of lines.

Other enhancements

  New algorithm support:  SecureCRT now supports x509v3-ecdsa-sha2 
  algorithms per RFC 6187 for PKCS #11 as well as SSH2 support for 
  the diffie-hellman-group14-sha256, diffie-hellman-group16-sha512, 
  and diffie-hellman-group18-sha512 key-exchange algorithms.

  Local proxy command: A new firewall type, "Proxy command", allows a 
  local proxy command to be run when connecting to a remote server.

  Audio bell: The audio bell can be configured to play a sound file.

  Command window option: A new global option allows Send Characters 
  Immediately to be enabled by default for the Command window.

  Import/export button bars: You can now import and export just the 
  button bars.

  Log all data: A "Log Screen" INI-file-only session option allows 
  all data displayed in the terminal to be logged to a file.

  Command-line key exchange: A new /KEX flag allows the key-exchange 
  algorithm(s) to be specified on the command line.

  TFTP server: New TFTP server enhancements include honoring the block 
  size set by the TFTP client as specified in RFC 2347 and a global 
  option Allow overwrite of existing files that allows TFTP transfers 
  to overwrite existing files.  Also new is status bar display of the 
  running TFTP server listen address and port, and the ability to use 
  templated paths for TFTP upload, download, and log file paths, which 
  get modified to an appropriate path on each platform.

  Scripting: The script functions WaitForCursor, WaitForKey, 
  WaitForString, and WaitForStrings allow the wait time to be 
  specified in milliseconds. Also, global configuration options can 
  be retrieved and set from a script.

Platform support

  Support for Ubuntu 19.x and macOS 10.15 Catalina has been added.


Please see the History tab in the SecureCRT About Box for information
on changes and bug fixes.


2. Enhancement Requests, Updates, and Questions
-----------------------------------------------

We want to hear from you.  Let us know what features you would like to
see in future releases of SecureCRT by visiting our website at:

  https://www.vandyke.com/feedback.php

Every VanDyke Software license includes a full year of upgrades and
technical support.  To receive email notification when new releases
of SecureCRT are available, sign up for our SecureCRT mailing list at:

  https://www.vandyke.com/cgi-bin/subscribe.php?RMF=3

For tips, ideas, and product news, subscribe to VanDyke Software's
"What's New" at:

  https://whatsnew.vandyke.com

Join the online forums to exchange useful tips and ideas with your 
peers and VanDyke developers:

  https://forums.vandyke.com/index.php?RMF=3

Get online video how-tos on the VanDyke Software YouTube Channel:

  http://youtube.com/vandykesoftware

If you have any questions, please visit our website at:

  https://www.vandyke.com/feedback.php

SecureCRT for Windows supports Windows 10, Windows 8.1, Windows Server
2019, Windows Server 2016, Windows Server 2012 R2, Windows Server 
2012, and Windows 7.  SecureCRT for macOS supports 10.15, 10.14, and 
10.13.  SecureCRT for Linux supports Ubuntu 19.x, 18.04 LTS, and 
16.04 LTS, and Red Hat Enterprise Linux 7.x.


3. Encryption Export
--------------------

This Software is subject to export control.  The Software may be
transmitted, exported, or re-exported only under applicable export
laws and restrictions and regulations of the United States Bureau of
Industry and Security or foreign agencies or authorities.  By
downloading or using the Software, you are agreeing to comply with
export controls.  For more information see:

  https://www.vandyke.com/download/export.html


4. Upgrades
-----------

Users who purchased SecureCRT within a year prior to the official 
release are eligible for a free upgrade to SecureCRT 8.7.  All users 
can evaluate SecureCRT 8.7 for 30 days free of charge.  For more 
information, please visit:

  https://www.vandyke.com/pricing/index.html?pid=upgrades

* Release dates are subject to change without notice.


5. Product History
------------------

The list of changes made for this release of SecureCRT can be found
on the History tab in the About box.


6. SecureCRT Features 
---------------------

Support for SSH1 and SSH2 Secure Shell protocols.
  - SSH2 Protocol support:
    - ChaCha20/Poly1305, AES-GCM, AES-128, AES-192, AES-256, 
      AES-128-CTR, AES-192-CTR, AES-256-CTR, Twofish, Blowfish, 
      3DES, and RC4 ciphers.
    - RSA, Ed25519, ECDSA (RFC 5656), DSA, and X.509 (Windows) host 
      key support.
    - Multiple ordered authentication methods, ciphers, and MACs.
    - Public Key Assistant makes it easier to upload public keys.
    - Support for GSSAPI secure key exchange.
    - Local port forwarding, X11 forwarding, remote forwarding, and
      dynamic forwarding.
    - OpenSSH Agent forwarding.
    - SHA2, SHA1, UMAC, and MD5 MACs.
    - Public key with support for RSA (up to 16,384 bits), Ed25519, 
      ECDSA (RFC 5656), DSA, PuTTY PPK, OpenSSH certificates, and
      X.509 certificate including SmartCards, PKCS #11, PKCS #12 
      (Windows only), and Kerberos v5 via GSSAPI.  Password and 
      keyboard-interactive authentication methods are also supported.
    - SFTP tab creates an SFTP session to an existing SSH session.
    - Passphrase and password caching options.
  - Built-in SSH agent allows keys to be explicitly added or removed.
  - SSH1 Protocol support:
    - Blowfish, DES, 3DES, and RC4 ciphers.
    - RSA, TIS, and password authentication.
    - Local port forwarding, X11 forwarding.

Session Management.
  - Dockable session manager.
  - Named sessions store different preferences for different hosts.
  - Tabbed sessions allow multiple sessions in the same window.
  - Tab groups make it easier to group related sessions.
  - Launch multiple selected sessions in tabs with a single click.
  - Tiling allows multiple sessions to be viewed at once.
  - A session can be logged to a file, including options for logging
    custom data and a option for creating a new log file at midnight.

Configuration & Customization.
  - Easy configuration of basic SSH, port forwarding, remote
    forwarding, and other settings in Session Options dialog.
  - Named firewalls.
  - Personal data folder for separate storage of logon credentials.
  - Dependent session option (jump host).
  - User-defined number of savelines (scrollback) up to 128,000.
  - User-configurable number of rows and columns.
  - User-defined foreground, background, and bold colors.
  - User-defined keymaps.
  - User-defined button bar.
  - User-defined word delimiter characters for double-click.
  - Emacs mode maps ALT+<key> to send ESC+<key>.
  - Real-time keyword highlighting.

Advanced Terminal Emulation.
  - Quality VT100, VT102, VT220, VT320, Linux console, SCO ANSI, 
    TN3270, TVI910, TVI925, Wyse 50/60, and ANSI emulations.
  - VT line drawing.
  - Support for bold, underline, and reverse attributes.
  - Double-width and double-height fonts.
  - 80/132 column switching.
  - VT100 and VT220 keyboard emulation.
  - Optional ANSI color.
  - 256-color Xterm.
  - 24-bit color (True Color) Xterm.
  - Xterm extensions for mouse support and changing title bar.
  - Multi-byte character set support for Japanese, Korean, and
    Chinese.
  - Unicode support includes the ability to display character sets
    from multiple languages, support for multi-byte character sets,
    right-to-left reading order languages, and an extensive
    character encoding list.
 
Other Features.
  - FIPS 140-2 validated cryptographic library support (Windows 
    only).
  - Variable compression increases performance on slow connections.
  - Simple interface for automating logins.
  - Support for Telnet, Telnet/TLS (Windows only), and RLogin 
    (Windows only) protocols:
    - Telnet supports Negotiate About Window Size (NAWS).
    - Telnet supports Local Flow Control (LFLOW).
  - Local shell support.
  - Serial (COM) device support.
  - Hex view.
  - Integration with SecureFX.
  - Scripting language support for VBScript, JScript, and Perlscript
    (Windows only).
  - Embedded support for Python scripting.
  - Script recorder.
  - Zmodem, Xmodem, Ymodem, and Kermit file transfer (upload and 
    download).
  - Built-in TFTP server.
  - Drag-and-drop file transfer (upload).
  - Auto print, selection, screen, and pass-through printing.
  - Modem dialer support: configure and save modem, country code,
    phone, and redial settings for TAPI sessions (Windows only).
  - SOCKS firewall support with password authentication.
  - Unauthenticated and basic HTTP proxy support.
  - Generic and local proxy firewall support.
  - Copy and paste, including an "auto copy" option and a "paste on
    middle or right mouse click" option.
  - Command window option provides an editable type-ahead buffer with
    history support and the ability to send text to the active 
    session, all sessions, specified tab groups, and visible sessions. 
    Use "Send characters immediately" mode to stop commands, edit 
    files using vi, send escape sequences, and do tab completion.
  - Support for use from the command line or web browsers (Windows 
    only).
  - Support for standard insertion caret so that it can be tracked
    by screen access technology for the blind.
  - Import/export tool makes it easy to copy settings between 
    systems.


7. Reporting Bugs
-----------------

If you experience something you believe is a bug, please fill out
our online form at:

  https://www.vandyke.com/feedback.php

Please do not assume someone else will report it.  We will try to
resolve reported bugs as quickly as possible.  However, we can't
resolve bugs that are not reported.

Please describe the problem in as much detail as possible. Please
include the following information:

  - Version of SecureCRT (as shown in the About dialog)
  - Operating system and version


8. Contact Information
-----------------------

For information on ordering licenses, please visit the VanDyke
Software website:

  https://www.vandyke.com


All other inquiries should be directed to:

  VanDyke Software, Inc.
  4848 Tramway Ridge Dr. NE
  Suite 101
  Albuquerque, NM 87111
  USA

  Inquiry form: https://www.vandyke.com/feedback.php


VanDyke Software, SecureCRT, and SecureFX are trademarks or registered
trademarks of VanDyke Software, Inc. in the United States and/or other
countries.

All other products and services mentioned are trademarks or registered
trademarks of their respective companies.
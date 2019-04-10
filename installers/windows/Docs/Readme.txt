Windows Installers
==================
This document is intended to be brief notes on how to install the necessary components and create installers
for Windows 32-bit and 64-bit versions of OSCAR.  This documentation will be fleshed out later and probably
placed in the Wiki for the OSCAR repository.

On my computer, I have QT installed in E:\QT and the OSCAR code base in E:\oscar\oscar-code.  I don't believe
there are any dependencies on these locations.

Required Programs
-----------------

The following programs and files are required to create Windows installers:

# Inno Setup 5.6.1 from http://www.jrsoftware.org/isdl.php.  Download and install innosetup-qsp-5.6.1.exe.
  Unicode version may be ok but I have not tested it.  The deployment batch file assumes that Inno Setup is
  installed into its default location: C:\Program Files (x86)\Inno Setup 5.  If you put it somewhere else, you
  will have to change the batch file. Do I need to install the InnoSetup Preprocessor???
# GIT for windows, from https://gitforwindows.org/.  Again, you want git but not the mingw-tools to be in your PATH.
# QT Open Source edition from https://www.qt.io/download.  I use version 5.12.2.  More recent versions may
  also work but I have not tested any. qt-unified-3.0.6???

Configuring QT
--------------

Here is how I configure QT 5.12.2.  You can configure during the installation or with the QT maintenance tool:

# Select the MinGW 7.3.0 32-bit and 64-bit components, If you want to build with Visual Studio 2017 select also
MSVC 2017 32-bit and 64-bit.
# In the developer and designer tools, select
## Qt Createor 4.8.2 CDB Debugger Support
## MinGW 7.3.0 32-bit
## MinGW 7.3.0 64-bit

Note that I did not select the installer frameworks.
When you start QT, it will ask you to select your kits and configure them.  Select both MinGW 7.3.0 32-bit and
64-bit kits.

Next, we have to fix a problem in the 32-bit kit, as it is created using a 64-bit compiler.
Select Tools/Options and click on the Compilers tab.  Select the 32-bit C compiler and notice that it invokes
the 64-bit compiler (C++ has the same issue):

We cannot change the compiler path, but we can define new compilers.  So I define a "Real 32-bit C compiler" by
copying the compiler path and changing the 64 to 32:

Then I create a "real" C++ compiler similarly with the 32-bit path:

Now, back to the Kits tab and set the 32-bit kit to use the "real" 32 bit compilers:

Close this configuration dialog.  One the main screen, you should see two kits:

Adjusting the Build Steps
-------------------------

Select "Release" rather than the default "Debug" in the pull-down at the top of the Build Settings.  Click on

Details for the qmake build step.  By default, "Enable Qt Quick Compiler" is checked.  Remove that check -
errors result if it is on.  QT will ask if you want to recompile everything now.  Don't there is more to do.
Make this same change for the Release build for both 32-bit and 64-bit kits.  And if you ever want to use the
Build Debug pull-down, you probably need to do the same (I don't remember and haven't used Debug in a while).

To create a "release" directory and installer, you need to add a custom process step to the Release build
configuration.  Be sure to select "Release" rather than the default "Debug" in the pull-down at the top of
the Build Settings:

Create a custom process step as the final build step.  Put the fully qualified path for deploy.bat in the
command field.  Don't touch "working directory."  Any string you care to place in the "arguments" field
will be appended to the installer executable file name.

Do the same for both 32-bit and 64-bit Build settings.

Now you should be able to build OSCAR:

To make 32-bit or 64-bit builds, just make sure the correct Build item is selected in the Build & Run section
on the left:

The Deploy.BAT file
-------------------

The deployment batch file creates two directories:
	Release - everything needed to run OSCAR.  You can run OSCAR from this directory just by clicking on it.
	Installer - contains an installer exe file that will install this product.

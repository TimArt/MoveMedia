# MoveMedia
MoveMedia is an application which allows a user to control MacOS Media functions with foot gestures. The application makes use of PlayStation Move controllers as the motion sensing hardware and uses the [PS Move API by Thomas Perl](https://github.com/thp/psmoveapi) to communicate with PlayStation Move controllers. It uses Apple's AppKit and IOKit libraries and MacOS system accessibility functions to send media commands such as volume up, volume down, play/pause, etc.

## Repository Overview

**bin** : Pre-compiled binary executables for MoveMedia. This folder contains `MoveMedia`, a precompiled unix executable of our application. And `psmove`, a utility compiled from the PS Move API allowing for the pairing of PlayStation Move controllers to MacOS over Bluetooth.

**LICENSE.md** : License information for external libraries used.

**MoveMedia** : Directory containing our source code.

**MoveMedia.xcodeproj** : Xcode project to build the MoveMedia application.

**psmoveapi** : Directory containing the PS Move API header files and a pre-compiled static library of the PS Move API.

### Source Code Overview (MoveMedia Directory)

**MoveMedia.c** : The main program which interprets gestures from the PlayStation Move controller and triggers MacOS media commands.

**MoveMediaKeys.h** : Functions to press and release media keys virtually using accessibility features. Provides C/C++ interface to access Objective-C functionality of MacOS APIs.

**MoveMediaKeys.m** : Functions to generate key events given certain key types.


## Building, Running, and Using Our Prototype

### Required Materials
- Computer running the MacOS Operating System (**our program will only compile and run on the MacOS Operating System**)
- 1 PlayStation Move controller (can be bought from Walmart, Best Buy, Amazon, etc)
- 1 mini usb cable (comes with a PlayStation Move controller).
- Some tape to adhere the PlayStation Move controller to some kind of shoe.
- A shoe to attach the PlayStation Move controller to.
- Our digital project folder including source code and pre-compiled programs.

### External Libraries Used (Already Included In Our Submitted Project Folder)
- PS Move API by Thomas Perl (and its dependencies) : [https://github.com/thp/psmoveapi] : for communicating with the PlayStation Move Controllers
- Apple’s AppKit and IOKit libraries : for MacOS and system accessibility functions.

### PlayStation Move Controller Bluetooth Setup Instructions
- Connect the PlayStation Move Controller to the computer via a USB connection.
- Turn on Bluetooth on your Mac.
- Navigate to the `bin` folder within the main project directory in the command line (using `cd` command).
- Run the shell command in the command line: `./psmove pair`.
- You may need enter your password or run the `psmove` command using sudo : `sudo ./psmove pair` to let the utility modify the system Bluetooth settings and whitelist the PlayStation Move controller for connection.
- Unplug the USB cable and press the PS button on the controller. The red status LED will start blinking and **should eventually remain lit to indicate a working Bluetooth connection**. If it continues blinking, it might not be paired via Bluetooth, or - if you can see the connection in your computer’s bluetooth settings - the battery is low and needs charging via USB or a charger. To verify the connection, check the Bluetooth devices list of your computer and see if there is an entry for “Motion Controller”.
- On recent versions of OS X, a dialog might pop up asking for a PIN. Close it and pair the controller again using `./psmove pair`. After that, it should connect successfully.

### Running from Our Pre-Compiled Program on Mac
**This section shows how to run our pre-compiled program, while optional build steps are listed in later sections.**
- Make sure the PlayStation Controller is paired over Bluetooth to your computer following the above steps.
- Navigate to the `bin` directory within the main project directory in the command line (using `cd` command).
- Run the MoveMedia Program : `./MoveMedia`
- You may be prompted to give the program Accessibility privileges with your computer.
- Click allow and/or navigate to System Preferences -> Security & Privacy -> Privacy (tab along top) -> Accessibility (item along left sidebar) -> Scroll through the items in the right side window till you find “MoveMedia” -> Check the checkbox (You may need to first clock the lock icon in the bottom left corner of this window and provide your password to allow changes).

### Prototype Usage Instructions
Once the PlayStation Move controller has been comfortably and securely been affixed to a foot, has been connected via bluetooth to the computer, and the program is running on the computer:
- The PlayStation Move controller orb should be glowing red. This means it is in disabled mode.
- Place the controller-wearing foot flat on the floor.
- Press the large, central, oval shaped “Move” button with the “Move” logo on it. This will reset the natural orientation of the controller to match your current foot position.
- Open a media program such as iTunes on your Mac.
- Select an album or media or initially click a song to bring focus onto the iTunes application.
- Enable gesture tracking for MoveMedia by pressing the triangle button on the PlayStation Move controller. The controller orb should now be off and not glowing.
- Tap your foot vertically into the ground to play or pause the media item.
- Tilt your foot upward, toe to the ceiling, heel to the ground, to increase the volume.
- Tilt your foot downward, toe to the ground, heel to the ceiling, to decrease volume.
- Swing your foot to the right in the air and bring it back again to the neutral ground position to skip to the next media item.
- Swing your foot to the left in the air and bring it back again to the neutral ground position to skip to the previous media item.
- Swing your foot to the right in the air and hold it in the air to scrub forwards through the media item and bring your foot back to the neutral ground position when you are finished scrubbing.
- Swing your foot to the left in the air and hold it in the air to scrub backwards through the media item and bring your foot back to the neutral ground position when you are finished scrubbing.
- To disable gesture tracking, press the square button on the PlayStation Move controller. This allows you to walk around or move your foot without accidentally triggering a command.
- To close the program, press `CNTRL-c` in the command line.
- To disconnect and turn off the PlayStation Move Controller, turn off the Bluetooth setting on your Mac.


### Optional Build Steps
#### Building MoveMedia Program from Our Source (using pre-built PS Move API library)
- Unzip the source.
- Open the Xcode Project file : `MoveMedia.xcodeproj `
- Choose the Xcode Build command (Product Menu -> Build). Build output will go to `/Build/Products/Debug/MoveMedia`.
- Optionally run the program by choosing the Xcode Run command (Product Menu -> Run). This will run the program located at `/Build/Products/Debug/MoveMedia`

#### PS Move API Build Instructions
**(Follow these instructions if you want to rebuild the PSMoveAPI library. This is not a required build step for building or running our source code program, but these steps may be useful for any further extensions to the project that want to extend PSMoveAPI functionality or change how the library is related to our source code.)**

The following describes building the PS Move API as a static library as opposed to the default dynamic library build scheme.

Commands to run are designated by code formatted text: `a command to run`
All line numbers refer to the original, unedited file.

1. Clone the PS Move API repository
    `git clone https://github.com/thp/psmoveapi.git`
2. Change into the repository's directory
    `cd psmoveapi`
3. Checkout the 4.0.7 tag
    `git checkout 4.0.7`
4. Change the git modules to use HTTPS.
    Edit ./.gitmodules
    Replace "git://" with "https://" (#3 and #6)
5. Make psmoveapi\_tracker static
    Edit ./src/tracker/CMakeLists.txt
    Replace "SHARED" with "STATIC" on the line (#147) starting with "add\_library(psmoveapi\_tracker ..."
    Delete the line (#154) starting with "set\_target\_properties(psmoveapi\_tracker ..." and the one below it (#155)
6. Make psmoveapi static
    Edit ./src/CMakeLists.txt
    Replace "SHARED" with "STATIC" on the line (#162) starting with "add\_library(psmoveapi ..."
    Delete the line (#164) starting with "set\_target\_properties(psmoveapi ..." and the one below it (#165)
7. Fix the build script
    Edit ./scripts/macos/build-macos
    Delete the block (from #55 to #62) starting with "# Patch for Mac OS X deprecation ..." and ending with "git checkout pr7266"
    Change "ON" to "OFF" on the line (#102) starting with "cmake -DPSMOVE\_USE\_PS3EYE\_DRIVER=..."
    Insert the line (between #102 and #103) "      -DPSMOVE\_USE\_PSEYE=OFF \" between it (#102) and the one below it (#103)
    Change "ON" to "OFF" on the line (#103) starting with "-DPSMOVE\_BUILD\_TRACKER=..."
8. Run the build script
    `bash -e -x ./scripts/macos/build-macos`
9. Copy the resources
    The include headers are "psmove.h" and "psmoveapi.h" in ./include and "psmove\_config.h" in ./build
    The static library is "libpsmoveapi.a" in ./build

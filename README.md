# PiMusiic: Interactive Music Instrument Standalone

PiMusiic was a project developed by @CaptainBidou and me using the Joy-IT kit and a Raspberry Pi. This project aimed to create musical instrument, allowing users to compose, load, and play music. The project includes a server with a custom protocol created especially for the PiMusiic.

This version is a standalone version of the project, which mean it didn't use the Joy-IT kit. The project was developed using the ncurses library for the user interface and the ALSA library for sound generation.

## Features:
- **User-friendly Interface:** Implemented using the ncurses library for intuitive navigation and interaction.
- **Music Creation:** Enables users to create new music by specifying the beats per minute (BPM), with a sequencer for composing melodies.
- **Music Loading:** Allows users to connect to the Pi2serv server using credentials and load music from the server.
- **Sound Generation:** Using multiple channels for sound generation, including 3 audio channels
- **Pi2iserv Communication:** Communication with the Pi2iserv server for user authentication and music management operations (List/Add/Modify/Delete).

## Standalone Version Features: 
- **All Features:** All features of the original project are available in this standalone version. Except for the Joy-IT kit obviously. **(TODO)**
- **More instruments:** The standalone version includes more instruments and instrument using sound samples. **(TODO)**
- **Better protocol (MPP 2.0):** The protocol is less verbose with smaller messages & better error handling. **(TODO)**
- **Better navigation & commands:** The Joy-kit commands weren't very intuitive & the navigation was a bit clunky. **(TODO)**
- And more... **(TODO)** 

## Current Status:
- **In Development:** The standalone version is currently in development. **(TODO)**

Actually the project is just a copy of the original without all the Joy-IT kit stuff. A big part of the project will be rewritten to clean the code and make it more efficient. 

## Installation:
1. Clone the repository: `git clone https://github.com/your_username/PiMusiic.git`
2. Set up environment variables for cross-compilation (optional):
3. Compile the project: `make`
5. Run the executable: `./bin/PiMusiic`

## Usage:
- Follow the on-screen instructions to navigate the menu, create music, load music, and play music. 

## Requirements:
- ALSA library installed
- ncurses library installed (for user interface)


<a name="top"></a>

<!-- LOGO -->
<br />
<div align = "center">
    <img src = "readme/logo.png" alt = "Logo" width = "100" height = "100">

<h3 align = "center">Embedded 2D SCARA-Model Pen Plotter</h3>

  <p align = "center">
    A Computer-Vision-based IoT robotic arm with pen plotting functionalities
    <br />
    <a href="https://www.youtube.com/watch?v=2bIokQEKmMY"><strong>Watch the demo »</strong></a>
    <br />
    <br />
    <h6>
    <a href="https://github.com/AlessiaRinaldi">Alessia Rinaldi</a>
    ·
    <a href="https://github.com/TianShi14">Angelo Nutu</a>
    ·
    <a href="https://github.com/alterlleo">Leonardo Pasquato</a>
    </h6>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
- [Overview](#overview)
- [About The Project](#about-the-project)
  - [Structure](#structure)
  - [Telegram Bot](#telegram-bot)
  - [Communication and actuation](#communication-and-actuation)
- [Getting started](#getting-started)
  - [Raspberry setup](#raspberry-setup)
    - [UART communication setup](#uart-communication-setup)
    - [Telegram bot and vectorialization setup](#telegram-bot-and-vectorialization-setup)
- [Files Organization](#files-organization)
- [Work Organization](#work-organization)
- [Roadmap](#roadmap)


<!-- Overview -->
## Overview

This project revolves around transforming digital images into computer-crafted printed drawings. 

In contrast to the conventional Cartesian structure commonly found in 3D printers, our apparatus is fashioned in the likeness of a human arm, introducing a distinctive approach to the process.

Users are prompted to effortlessly upload their desired images to our <a href = "https://web.telegram.org/k/#@pen_plotter_bot">Telegram bot</a>, which proceeds to carry out a vectorization process, resulting in a comprehensive `JSON` file containing vectors.
Following the image vectorization process, the `JSON` document will detail the exact positions determined to be followed by the manipulator. This document will be available in the Raspberry pi Zero memory and a serial communication between the `Raspberry` and the `MSP432` will enable the line-to-line interpretation of the file by the second one. Then the `MSP432` will compute the motor angles starting by the positions read and will actuate them to carry out the movement

<!--Following the image vectorization process, a text document will detail the exact positions determined to be followed by the manipulator, which, once interpreted by the `MSP432 microcontroller`, will be computed into motor angles necessary to carry out the movement.
-->

<p align="right">(<a href="#top"> back to top </a>)</p>



<!-- ### Built With

* ![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
* ![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54)
* ![Raspberry Pi](https://img.shields.io/badge/-RaspberryPi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)

<p align="right">(<a href="#readme-top">back to top</a>)</p> -->



<!-- ABOUT THE PROJECT -->
## About The Project

In the following sections, we'll break down the project's components.

### Structure

The structure of the 2D pen plotter comprises two links: one linking the first servo to the second, and the other connecting the second servo to the third. To establish clarity, we define the following reference frames:

- Frame 0: this represents the absolute frame, identical to the paper's frame.
- Frame 1: aligned with the first link, its x-axis parallels the link's axis, and its origin coincides with the servo 1 pin.
- Frame 2: similar to frame 1 but relative to the second link and motor. 

The outcome is a manipulator akin to a human arm, with its end-effector capable of drawing along points specified in the file aforementioned.

For visual clarification, refer to the accompanying image:

<div align = "center"> <img src = "readme/photo1.jpg" alt = "Frame01" width = "500"> </div>

The third servo serves to raise and lower the attached pen:

<div align = "center"> <img src = "readme/photo2.jpg" alt = "Frame2" width = "650" height = ""> </div>

### Telegram Bot

Our interface of choice for users looking to convert their chosen image into a drawing is a `Telegram bot`. Upon receiving the user's selected image, it initiates a structured sequence of Computer Vision algorithms. It systematically begins with a grayscale conversion, followed by Canny edge detection for precise boundary identification. Subsequently, a Gaussian blur is applied for image smoothing, and a Sobel operator to enhance edge definition.

These algorithms work alongside contouring techniques shaping distinct forms and structures within the image. Line sorting is then employed to organize the details. The outcome is a meticulously processed, vectorized image prepared for submission to the microcontroller. Throughout this process, the Telegram bot provides real-time updates on the processing status, and furthermore submits the vectorized image to the user for confirmation.

We've included a visual representation of the state machine to make things clearer:

**TBD**

<br/>

### Communication and actuation

Following the image processing, the Telegram bot proceeds to save a `.json` file in a common folder included in the Raspberry Pi memory, encapsulating the vectors that constitute the image. The file is then parsed by another `Python` script, which extracts vector points. Those are then individually communicated to the MSP432.

The aforementioned communication between the two boards works via UART protocol, and upon receiving these points the MSP432 coordinates the movement of the servomotors to the specified position.

Here is the working FSM that drives the communication between Raspberry and MSP432 and its actuation:


## Getting started

This project is designed for execution on the MSP432 embedded platform, utilizing `Texas Instruments' DriverLib` to operate at a higher abstraction level. While the MSP432 can be programmed using Code Composer Studio, this project has been configured to leverage Visual Studio Code with the `PlatformIO extension` due to more familiarity with the tool.

PlatformIO is an open-source extension for Visual Studio Code, providing compatibility with a wide range of embedded systems. Although commonly used for Arduino development without its native IDE, it also supports the coding of STM32 and MSP boards.

To facilitate the integration of the DriverLib into the project, it is included in the _lib_ folder. Within this folder, sub-folders exist for each pair of header and definition files (.h and .c). Consequently, every #include statement for the DriverLib has been modified to exclude the path of the sub-libraries, as they are all housed in the same folder.

To use this project, install PlatformIO on your Visual Studio Code, clone this repository, and open the project from PlatformIO. All the necessary components for the MSP432 board are consolidated here.

An additional element of the project involves the Raspberry Pi 0 W for Human Machine Interaction (HMI). The setup requires the necessary connections for serial communcation between the MSP432 and Raspberry Pi, while it's only required to execute the primary Python script _plotter_bot.py_ to initialize and engage with the Telegram Bot.

### Raspberry setup

Testing of the platform has been conducted on `Raspbian version 11.0` and later, as well as Ubuntu 22.02 and above. The prerequisites for the runtime environment include a minimum `Python 3.10 version`, and all additional Python package dependencies.

Before installing dependencies, ensure that the `pip` command is installed using the following:

``` bash
sudo apt install pip 
```
On the _Raspberry Pi 0_ there might be insufficient memory space in the directory `/var/cache/apt/archives`. It's possible to solve this issue: expand the filesystem to SD Card (it's recommended to use at least an SD card with at least 8Gb) by doing `sudo raspi-config` ⟶ `Advanced` ⟶ `Expand Filesystem`.

The same is also achievable through the command line by executing:

``` bash
sudo raspi-config --expand-rootfs
```    

#### UART communication setup

For initiating serial communication, it's necessary to install the `pyserial` module using the command:

``` bash
pip install pyserial
```

In this project, the `/dev/serial0` port is utilized. Before the first run, enable it by editing the file `/boot/config.txt`

``` bash
sudo nano /boot/config.txt
```
and append the following line:

```
enable_uart=1
```

#### Telegram bot and vectorialization setup

Additionally, for booting and usage of the Telegram Bot, several additional modules are needed. 

Tu run the bot, install `python-telegram-bot`, as well as `cairo-svg` for SVG conversion (to display the vectorized image to the user) and `numpy` for serial decoding of the image sent by the user. Execute the following command:
```
pip install python-telegram-bot cairosvg numpy
```
For the backend image-related processes, the required modules include the headless version of `opencv` for the various processing algorithms and `PIL` for image handling. Install them with the following:

``` bash
pip install opencv-python-headless Pillow
```

<br/>
<br/>

Reboot the system, and everything should be set up and ready for execution.

## Files Organization
```
.
├── 3D-structure
├── MSP-codes
|   ├── include
|   ├── lib
|   |   ├── driverlib
|   |   ├── Crystalfontz128x128_ST7735
|   |   └── HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735
|   ├── src
|   |   ├── pictures
|   |   |   ├── Default.c
|   |   |   ├── DrawingBar.c
|   |   |   └── images.h
|   |   ├── main.c
|   |   ├── motors.h
|   |   ├── motors.c
|   |   ├── communication.h
|   |   ├── communication.c
|   |   ├── screen.h
|   |   ├── screen.c
|   |   ├── inithw.h
|   |   └── inithw.c
|   └── test
├── rasp-communication
|   └── communication_rasp.py
├── Vectorization-bot
|   ├── linedraw.py
|   └── plotter_bot.py
└── README.md

```

<!-- Working Organization -->
## Work Organization

The entire group began discussions on the project's core concept starting from the outset of the course, eventually settling on the designing and manfacturing of the 2D pen plotter. Over the subsequent weeks, individual work was precluded, and all decisions related to the project were made collectively. Consequently, tasks were distributed among team members as follows:

- Angelo Nutu assumed responsibility for image processing and setting up the Telegram bot [more](https://github.com/AlessiaRinaldi/2D-plotter-manipulator/tree/main/Vectorization-Bot/README.md);
- Alessia Rinaldi took charge of serial communication between the Raspberry Pi and the MSP432 [more](https://github.com/AlessiaRinaldi/2D-plotter-manipulator/tree/main/MSP-codes);
- Leonardo Pasquato was tasked with designing the 3D structure and managing motor control actuation for the MSP432 [more](https://github.com/AlessiaRinaldi/2D-plotter-manipulator/tree/main/MSP-codes);

<p align="right">( <a href="#top">back to top</a> )</p>

<!-- ROADMAP -->
## Roadmap

- [ ] Telegram Bot
- [ ] Point-to-Point connection
    - [ ] Protocol implementation
    - [ ] Transmission 
- [ ] Motor control
    - [ ] Serial connection between microcontroller and wi-fi module
    - [ ] Math implementation
    - [ ] Motor PWM definition
- [ ] Working organization

<p align="right">( <a href="#top">back to top</a> )</p>

<!-- ACKNOWLEDGMENTS -->
<!--## Acknowledgments

* []()
* []()
* []()

<p align="right">(<a href="#top">back to top</a>)</p> Per librerie specifiche e citazioni varie, direi formale e carino da fare ma si vede alla fine -->

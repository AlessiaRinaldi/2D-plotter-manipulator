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
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#overview">Overview</a></li>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#structure">Structure</a></li>
        <li><a href="#telegram-bot">Telegram Bot</a></li>
      </ul>
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#work-organization">Work Organization</a></li>
  </ol>
</details>



<!-- Overview -->
## Overview

This project revolves around transforming digital images into computer-crafted printed drawings. Users are prompted to effortlessly upload their desired images to our <a href = "https://web.telegram.org/k/#@pen_plotter_bot">Telegram bot</a>, which proceeds to carry out a vectorization process, resulting in a comprehensive `JSON` file containing vectors.

In contrast to the conventional Cartesian structure commonly found in 3D printers, our apparatus is fashioned in the likeness of a human arm, introducing a distinctive approach to the process.

Following the image vectorization process, a text document will detail the exact positions determined to be followed by the manipulator, which, once interpreted by the `MSP432 microcontroller`, will be computed into motor angles necessary to carry out the movement.

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

Our interface of choice for users looking to convert their chosen image into a drawing is a `Telegram bot`. Upon receiving the user's selected image, our it initiates a structured sequence of Computer Vision algorithms. It systematically begins with a grayscale conversion, followed by Canny edge detection for precise boundary identification. Subsequently, a Gaussian blur is applied for image smoothing, and a Sobel operator to enhance edge definition.

These algorithms work alongside contouring techniques shaping distinct forms and structures within the image. Line sorting is then employed to organize the details. The outcome is a meticulously processed, vectorized image prepared for submission to the microcontroller. Throughout this process, the Telegram bot provides real-time updates on the processing status, and furthermore submits the vectorized image to the user for confirmation.

We've included a visual representation of the state machine to make things clearer:

**TBD**

<br/>

<!-- Working Organization -->
### Work Organization

The entire group began discussions on the project's core concept starting from the outset of the course, eventually settling on the designing and manfacturing of the 2D pen plotter. Over the subsequent weeks, individual work was precluded, and all decisions related to the project were made collectively. Consequently, tasks were distributed among team members as follows:

- Angelo Nutu assumed responsibility for image processing and setting up the Telegram bot [more](https://github.com/AlessiaRinaldi/2D-plotter-manipulator/tree/main/Vectorization-Bot/README.md);
- Alessia Rinaldi took charge of serial communication between the Raspberry Pi and the MSP432 [more](https://github.com/AlessiaRinaldi/2D-plotter-manipulator/tree/main/MSP-codes);
- Leonardo Pasquato was tasked with designing the 3D structure and managing motor control attenuation for the MSP432 [more](https://github.com/AlessiaRinaldi/2D-plotter-manipulator/tree/main/MSP-codes);

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

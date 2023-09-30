
<a name="top"></a>

<!-- LOGO -->
<br />
<div align = "center">
    <img src = "readme/logo.png" alt = "Logo" width = "100" height = "100">

<h3 align = "center">Embedded 2D SCARA-Model Pen Plotter</h3>

  <p align = "center">
    A Computer-Vision-based IoT robotic arm with pen plotting functionalities
    <br />
    <a href="https://youtu.be/dQw4w9WgXcQ?si=npVwkW-gNgec-lmm"><strong>Watch the demo »</strong></a>
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

<div align = "center"> <img src = "readme/photo1.jpg" alt = "Logo" width = "500"> </div>

The third servo serves to raise and lower the attached pen:

<div align = "center"> <img src = "readme/photo2.jpg" alt = "Logo" width = "650" height = ""> </div>

### Telegram Bot



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

<p align="right">( <a href="#top">back to top</a> )</p>

<!-- ACKNOWLEDGMENTS -->
<!--## Acknowledgments

* []()
* []()
* []()

<p align="right">(<a href="#top">back to top</a>)</p> Per librerie specifiche e citazioni varie, direi formale e carino da fare ma si vede alla fine -->

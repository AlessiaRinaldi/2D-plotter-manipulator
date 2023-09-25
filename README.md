## 2D pen plotter manipulator
#### Index
* 
* 
* 

### Overview
The main focus of this project is to obtain a printed drawing from a digital image.
The user can load every image he wants and then the mobile-app processes it in order to execute 
the vectoralization and the generation of a gcode-like text. The manipulator is the executor of the drawing. It's not the same old structur of the 3D printer:
the structure will not be a cartesian structur, insted it will be an human-arm-like structure. 2 links are motorized with 2 servomotors, like in the figure:
---figure---
So, after the image processing, it will be avaible a text file containg the positions that the manipulator must follow in order to recreate the drawing.
An MSP will aknowledge these positions and it will calculate the motors angle.


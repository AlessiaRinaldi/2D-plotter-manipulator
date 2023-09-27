## Overview
The structure for the 2D pen plotter needs 2 links: one connects the first servo with the second, the second one connect the second servo with the third.
It's better to define some reference frames:
* frame 0: it's the absolute frame, the same frame of the paper;
* frame 1: it's the frame united to the first link. The x-axis is parallel to the axis of the link and the origin of the frame 1 coincides with the servo 1 pin.
* frame 2: it's the same as the frame 1, but relative to the second link.

The next image will clarify this notation:
---inserisci immagine Leonardo----

## 3D printer specs
|piece|fill density|precision|time|
|---------|---------|---------|---------|
support 0 |   20%   |  0.2 mm | |
link 1    |   10%   |  0.2 mm | |
link 2    |   10%   |  0.2 mm | |
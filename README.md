# Cell controller

This is a not-so-originally named application for controlling multiple instruments in cell microfluidic experiments. A suspension of cells in KCl solution is driven through a microfluidic channel by an external syringe pump. Cell passage is monitored via the resistive pulse (or [Coulter counter](https://en.wikipedia.org/wiki/Coulter_counter)) technique with a measured current sampled by a data acquisition card ([National Instruments](http://www.ni.com/data-acquisition/)). When a cell is detected, the syringe is reversed so that the cell begins to change direction after passing through the channel. At the same time, an external camera (Phantom v7 high-speed camera, [Vision Research](https://www.phantomhighspeed.com/)) is triggered to record the passage. Cells can be driven through the sensing region of the device multiple times to get good measurement statistics.

The program is written in C++ and Qt, and uses threading to ensure the devices can respond to commands without delay. Each device has its own unique communication protocol:

| **Device**   | **Comm. type**|
|--------------|---------------|
| Syringe pump | Serial RS-232 |
| Camera       | TCP sockets   |
| DAQ card     | NI-DAQ C API  |

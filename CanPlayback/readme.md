# **RealDash-extras**

RealDash examples and technical materials

All materials in this repository are licensed with [NoLicense](https://github.com/janimm/RealDash/blob/master/LICENSE)

[realdash.net](https://www.realdash.net)

&nbsp;
## **CanPlayback**

This is a simple tool to stream RealDash CAN frames from CAN log files recorded in RealDash CAN Monitor. Usage:

     $ python3 canplayback.py CSVFILE PORT

For example:

     $ python3 canplayback.py rdcan_2023-03-31_11-33-36.csv 35000

Limitations:

- Only works on <=8 byte frames (0x11223344 frames).
- Frames with less than 8 bytes are padded with zeroes.
- Has no timing, it pushes the frames as quickly as possible.
- Enable 'time.sleep(0.01)' in the code to slow down.
- Has no proper error checking

&nbsp;

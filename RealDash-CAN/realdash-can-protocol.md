# **RealDash-extras**
[realdash.net](https://www.realdash.net)

&nbsp;
## **RealDash CAN protocol**
The *RealDash CAN* protocol enables custom implementations and DIY projects to connect to RealDash. The protocol uses typical 8-byte CAN frames with a header that specifies the stream start-tag and CAN frame ID number. RealDash CAN protocol was designed to easily connect to existing CAN network and transfer the data over to RealDash. However, this protocol can also be used with custom and DIY devices.

The RealDash CAN protocol is a stream of data in following structure:

    CAN DATA FRAME:
    4 bytes - 0x44,0x33,0x22,0x11
    4 bytes - CAN frame id number (as 32bit little endian value)
    8 bytes - CAN frame payload (data)

First 4 bytes are an identifier at the beginning of each CAN frame. This is required for RealDash to 'catch-up' on ongoing stream of protocol data.

Each data frame payload can contain *any data* in *any format*. How data is arranged and how it links to RealDash inputs is described in XML file that is imported to RealDash connection settings. The structure of the RealDash CAN XML description file is explained later in this document.

In addition, there is a *Text Extension Frame* that allows easy transfer of text data between devices. The text in Text Extension Frame must be UTF8 compatible string terminated with 0. A text extension frame contains one, and only one text string. It always consumes the entire frame.

    TEXT EXTENSION FRAME:
    4 bytes - 0x55,0x33,0x22,0x11
    4 bytes - CAN frame id number (as 32bit little endian value)
    N bytes - text in UTF8 format, including null-terminator


&nbsp;
### **Writing CAN frames from RealDash to device**
RealDash sends a *SET VALUE* frame over connection when value is updated based on RealDash actions, like buttons or data triggers. *SET VALUE* frame is always 17 bytes with following layout:

    4 bytes - always 0x44,0x33,0x22,0x11
    4 bytes - CAN frame id number (as 32bit little endian value)
    8 bytes - CAN frame payload (data)
    1 byte - checksum byte

- First 4 bytes are RealDash CAN protocol identification tag.
- Next 4 bytes are CAN frame id number.
- Next 8 bytes are the can frame payload data containing values to write.
- Last byte is a simple checksum of preceeding 16 bytes. Checksum value overflow is ignored.



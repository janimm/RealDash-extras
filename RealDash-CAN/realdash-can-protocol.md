&nbsp;
# **RealDash CAN protocol**

The *RealDash CAN* protocol was designed to easily connect to existing CAN network and transfer the data to RealDash. However, this protocol can also be used with custom and DIY devices.

A single *RealDash CAN* frame uses 8 byte header that specifies the frame identification tag, and CAN frame ID number. The header is followed by frame payload data, and on some frame types the CRC check value.

Three different frame types can be received by RealDash. In all frame types, first 4 bytes identify the frame type. This *tag* is also required for RealDash to 'catch-up' and synchronize on ongoing stream of protocol data.

Each frame payload can contain *any data* in *any arrangement/format*. How data is arranged and how RealDash interprets it is described in XML file that is imported to RealDash connection settings. The RealDash CAN XML description file is explained in detail [here](./realdash-can-description-file.md).


&nbsp;
## **The CAN *44* frame**

The *44* frame is a simple, constant size frame that contains 8 byte header and 8 bytes of payload data. Name *44* is from first header byte *0x44* that identifies the frame. This frame type allows easily pass CAN device data to RealDash, just by setting the header and copying the 8 CAN frame bytes.

    RealDash CAN '44' frame:
    4 bytes - 0x44,0x33,0x22,0x11
    4 bytes - CAN frame id number (32bit little endian value)
    8 bytes - CAN frame payload (data)

Payload size is always 8 bytes. In case of transferring CAN data with less than 8 bytes, pad the payload with zeroes. 


&nbsp;
## **The CAN FD *66* frame (requires RealDash version 1.9.1)**

The *66* frame allows to send more than 8 bytes in a single frame and contains 32 bit CRC value. This frame type can be used to transfer [CAN FD](https://en.wikipedia.org/wiki/CAN_FD) frame data, as CAN FD is capable of variable payload size.

    RealDash CAN '66' frame:
    3 bytes - 0x66,0x33,0x22
    1 byte  - payload size identifier
    4 bytes - CAN frame id number (32bit little endian value)
    # bytes - CAN frame payload (data), number of bytes identified by frame 4th byte
    4 bytes - CRC32 checksum (32bit little endian value)

4th byte (payload size identifier) in frame header describes the payload size by following table.

| Header 4th byte | payload size (bytes) |
|:--------|:----------:|
| 0x11 | 8 |
| 0x12 | 12 |
| 0x13 | 16 |
| 0x14 | 20 |
| 0x15 | 24 |
| 0x16 | 28 |
| 0x17 | 32 |
| 0x18 | 36 |
| 0x19 | 40 |
| 0x1a | 44 |
| 0x1b | 48 |
| 0x1c | 52 |
| 0x1d | 56 |
| 0x1e | 60 |
| 0x1f | 64 |
----



&nbsp;
This frame ends with 4 byte little endian CRC32 checksum value. Checksum is calculated from all frame bytes, except for the CRC value itself. Following function is used to calculate the checksum:

    uint32_t Crc32(const uint8_t* data, size_t numBytes)
    {
        static const uint32_t table[256] = {
            0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,
            0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,
            0xE7B82D07,0x90BF1D91,0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,
            0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,
            0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,0x3B6E20C8,0x4C69105E,0xD56041E4,
            0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,0x35B5A8FA,0x42B2986C,
            0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,0x26D930AC,
            0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
            0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,
            0xB6662D3D,0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,
            0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,
            0x086D3D2D,0x91646C97,0xE6635C01,0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,
            0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,0x65B0D9C6,0x12B7E950,0x8BBEB8EA,
            0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,0x4DB26158,0x3AB551CE,
            0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,0x4369E96A,
            0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
            0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,
            0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,
            0xB7BD5C3B,0xC0BA6CAD,0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,
            0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,
            0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,0xF00F9344,0x8708A3D2,0x1E01F268,
            0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,0xFED41B76,0x89D32BE0,
            0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,0xD6D6A3E8,
            0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
            0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,
            0x4669BE79,0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,
            0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,
            0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,
            0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,0x95BF4A82,0xE2B87A14,0x7BB12BAE,
            0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,0x86D3D2D4,0xF1D4E242,
            0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,0x88085AE6,
            0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
            0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,
            0x3E6E77DB,0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,
            0x47B2CF7F,0x30B5FFE9,0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,
            0xCDD70693,0x54DE5729,0x23D967BF,0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,
            0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D };

        uint32_t crc32 = 0xFFFFFFFF;
        for (size_t i = 0; i < numBytes; i++) {
            crc32 = (crc32 >> 8) ^ table[(crc32 ^ data[i]) & 0xFF];
        }
        return (crc32 ^ 0xFFFFFFFF);
    }


&nbsp;
## **The *55* frame**

In addition, there is a *Text Extension Frame* that allows easy transfer of text from device to RealDash. The text payload in this frame must be UTF8 compatible string terminated by 0-byte. A text extension frame contains one, and only one text string. It always consumes the entire frame.

    RealDash CAN '55' text extension frame:
    4 bytes - 0x55,0x33,0x22,0x11
    4 bytes - CAN frame id number (32bit little endian value)
    # bytes - text in UTF8 format, including null-terminator


&nbsp;
## **Writing CAN frames from RealDash to device**

RealDash sends a *SET VALUE* frame to the connected device when value is updated based on RealDash actions, like buttons or data triggers. RealDash uses two different frames to send data from RealDash. Frames are equivalent of *44* and *66* incoming frames, with the exception that *SET VALUE 44* frame contains single byte checksum.


 *SET VALUE 44* frame is always 17 bytes with following layout:

    4 bytes - always 0x44,0x33,0x22,0x11
    4 bytes - CAN frame id number (32bit little endian value)
    8 bytes - CAN frame payload (data)
    1 byte  - checksum byte (unsigned)

- First 4 bytes are RealDash CAN protocol identification tag.
- Next 4 bytes are CAN frame id number.
- Next 8 bytes are the can frame payload data containing values to write.
- Last byte is a unsigned checksum of preceeding 16 bytes. Checksum value overflow is ignored.


*SET VALUE 66* frame has variable payload size. Frame structure is identical to the *66* frame received by RealDash:

    3 bytes - always 0x66,0x33,0x22
    1 byte  - payload size identifier (same as *66* frame described earlier)
    4 bytes - CAN frame id number (32bit little endian value)
    # bytes - CAN frame payload (data), number of bytes identified by frame 4th byte
    4 bytes - CRC32 checksum (32bit little endian value)



"""
canplayback.py

This program reads a RealDash CAN log file and streams the CAN frames from log
to RealDash.

Usage:
- Start the canplayback program
$ python3 canplayback.py CSVFILE PORT

- Start RealDash and create 'RealDash CAN' connection
- Configure connection to IP address and port shown by the canplayback program
"""

import socket
import csv
import time
import sys

def get_next_line(csvfile):
    line = csvfile.readline()
    if not line:
        csvfile.seek(0)
        _ = csvfile.readline()
        line = csvfile.readline()

    return line


def send_next_frame(client, csvfile):
    line = get_next_line(csvfile)
    if line:
        values = next(csv.reader([line], delimiter = ',', skipinitialspace=True))
        #index = values[0]
        #time = values[1]
        #channel = values[2]
        #direction = values[3]
        #frame_id = values[4]
        #frame_data = values[5]
        
        print("send:", values[4], values[5])
        frame_id = int(values[4], 0)
        client.sendall(bytes([0x44, 0x33, 0x22, 0x11]))
        client.sendall(frame_id.to_bytes(4, 'little'))
        
        frame_data = [int(value, 16) for value in values[5].split(' ') if value != 'x|']
        frame_data.extend([0] * (8 - len(frame_data)))
        client.sendall(bytes(frame_data))
        #time.sleep(0.01)

def main(filename, port):
    with open(filename) as csvfile:
        _ = csvfile.readline()

        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.bind(("localhost", port))
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server.listen(2)
        print("Listening at", server.getsockname())

        try:
            while True:
                client, address = server.accept()
                print("Connected to ", address)

                try:
                    while True:
                        send_next_frame(client, csvfile)
                except Exception as e:
                    print(e)
                    client.close()
                    csvfile.seek(0)
                    _ = csvfile.readline()

                print("Listening at", server.getsockname())
            
        except (KeyboardInterrupt, Exception):
            server.close()

if __name__ == "__main__":
    CSV_FILE = sys.argv[1]
    port = int(sys.argv[2])

    main(CSV_FILE, port)


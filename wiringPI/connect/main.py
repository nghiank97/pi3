#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import socket
UDP_IP = "192.168.7.47"
UDP_PORT = 5004
MESSAGE = b"Hello, World!"
print("UDP target IP: %s" % UDP_IP)
print("UDP target port: %s" % UDP_PORT)
print("message: %s" % MESSAGE)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

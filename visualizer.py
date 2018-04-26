import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
import serial
import sys
import re

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=100)

def get_data():
    line = ser.readline()
    line = line.decode('utf8')
    re.sub(r'^[0-9 ]', "", line)
    line.strip()
    arr = line.split()
    try:
     arr = list(map(int, arr))
    except:
        return []
    return arr

def update(frame):
    # frame is irrelavent but required
    plt.cla()
    arr = get_data()
    while len(arr) < 8:
        arr = get_data()
        
    x = plt.bar(range(8), arr, 1)
    return x

fig = plt.figure()
d = get_data()
dummy = [0, 3, 5, 6, 2, 1, 8, 20]
bar = plt.bar(range(8), dummy, 1)

ani = animation.FuncAnimation(fig, update, frames=60, interval=70, blit=True)
plt.show()

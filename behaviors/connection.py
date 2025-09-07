import serial

SERIAL_PORT = 9600
START_BYTE = 0xAA
CHECKSUM_MASK = 0xFF

class SerialConnection:
    def __init__(self):
        self._ser = serial.Serial('COM3', SERIAL_PORT)

    def send_packet(self, cmd, joint, angle):
        checksum = (cmd + joint + angle) & CHECKSUM_MASK
        packet = bytes([START_BYTE, cmd, joint, angle, checksum])
        self._ser.write(packet)
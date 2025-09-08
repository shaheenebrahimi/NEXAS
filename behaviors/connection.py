import serial
import serial.tools.list_ports
import time

BAUD_RATE = 9600
TIMEOUT = 3
START_BYTE = 0xAA
CHECKSUM_MASK = 0xFF

name_to_serial = {
    "arm" : "035363832363515062B1"
}

class SerialConnection:
    def __init__(self, name):
        self.device_id = SerialConnection.get_device(name)
        self.ser = serial.Serial(self.device_id, BAUD_RATE, timeout=TIMEOUT)
        self.wait_for_ready()
        self.ser.reset_input_buffer()

    def wait_for_ready(self):
        print("Waiting for Arduino to be ready...", end="")
        while True:
            line = self.ser.readline().decode().strip()
            if line == "READY":
                print("Arduino is ready!")
                break
            time.sleep(0.01) # reduce cpu load
    
    @staticmethod
    def get_device(name):
        serial_number = name_to_serial.get(name)
        ports = serial.tools.list_ports.comports()
        for port in ports:
            if port.serial_number == serial_number:
                return port.device
        raise ValueError(f"Couldn't find device with serial number: {serial_number}")

    @staticmethod
    def print_all_devices():
        print("Finding devices...")
        ports = serial.tools.list_ports.comports()
        for port in ports:
            print(f"Device: {port.device}")
            print(f"  VID:PID   = {port.vid}:{port.pid}")
            print(f"  Serial#   = {port.serial_number}")
            print(f"  Description = {port.description}")

    def send_packet(self, cmd, joint, angle):
        checksum = (cmd + joint + angle) & CHECKSUM_MASK
        packet = bytes([START_BYTE, cmd, joint, angle, checksum])
        self.ser.write(packet)
    
    def read_packet(self):
        line = self.ser.readline()  # reads until \n or timeout
        print("Arduino says:", line.decode().strip())

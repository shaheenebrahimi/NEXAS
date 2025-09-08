from behaviors.connection import SerialConnection
from protocol.generator import generate_protocol_files

if __name__ == "__main__":
    generate_protocol_files()

    conn = SerialConnection(name="arm")
    conn.send_packet(0x01, 0x02, 120)

    line = conn.ser.readline()  # reads until \n or timeout
    print("Arduino says:", line.decode().strip())
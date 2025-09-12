from behaviors.connection import SerialConnection
from protocol.generator import generate_protocol_files
import behaviors.protocol as proto

if __name__ == "__main__":
    generate_protocol_files()

    conn = SerialConnection(name="arm")
    conn.send_packet(proto.COMMANDS.MOVE, 0x00, 50)

    line = conn.ser.readline()  # reads until \n or timeout
    print("Arduino says:", line.decode().strip())
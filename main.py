from behaviors.connection import SerialConnection

if __name__ == "__main__":
    conn = SerialConnection(name="arm")
    conn.send_packet(0x01, 0x02, 120)

    line = conn.ser.readline()  # reads until \n or timeout
    print("Arduino says:", line.decode().strip())
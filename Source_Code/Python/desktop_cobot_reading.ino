//update program for the desktop cobot reading command
//=======================

import serial

PORT = "/dev/ttyACM0"
BAUD = 115200
COMMAND_FILE = "command.txt"

command_names = {
    "x": "STOP",
    "c": "CLOSE_GRIPPER",
    "o": "OPEN_GRIPPER",
    "w": "+X",
    "s": "-X",
    "a": "+Y",
    "d": "-Y",
    "u": "+Z",
    "n": "-Z"
}

ser = serial.Serial(PORT, BAUD, timeout=1)

last_cmd = ""

print("Reading ESP32 glove commands...")
print("Writing latest command to command.txt")
print("Press CTRL+C to stop")

while True:
    line = ser.readline().decode(errors="ignore").strip()

    if line:
        cmd = line[0]

        if cmd != last_cmd:
            last_cmd = cmd
            meaning = command_names.get(cmd, "UNKNOWN")

            print("Received:", cmd, "=", meaning)

            with open(COMMAND_FILE, "w") as file:
                file.write(cmd)
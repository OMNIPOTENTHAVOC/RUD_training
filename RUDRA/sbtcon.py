import serial
import time
import pygame
import math

# Attempt to create a serial connection
try:
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    print(f"Connected")
except serial.SerialException as e:
    print(f"Error: {e}")
    exit(1)

def send_command(command):
    try:
        command_byte = bytes([command])
        ser.write(command_byte)
        print(f"Command sent: {command} (0x{command:02X})")  
    except Exception as e:
        print(f"Failed to send command: {e}")

pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() > 0:
    joystick = pygame.joystick.Joystick(0)
    joystick.init()
    print(f"Joystick {joystick.get_name()} initialized")
else:
    print("Joystick not connected")
    exit()


def joystick_to_motor_power(x, y, max_motor_power=100,max_joystick=1.0):
    
    theta = math.atan2(y, x)

    magnitude = math.sqrt(x*2 + y*2)

    magnitude = min(magnitude, 1.0)

    x_cartesian = magnitude * math.cos(theta)
    y_cartesian = magnitude * math.sin(theta)

    x_scaled = (max_motor_power * x_cartesian) / max_joystick
    y_scaled = (max_motor_power * y_cartesian) / max_joystick

    right_motor_power = y_scaled-x_scaled
    left_motor_power = x_scaled+y_scaled


    #right_motor_power = (max_motor_power * (y_cartesian - x_cartesian)) / max_joystick
    #left_motor_power = (max_motor_power * (y_cartesian + x_cartesian)) / max_joystick

    return left_motor_power, right_motor_power


def control_motors(y_axis, x_axis):
    left_power, right_power = joystick_to_motor_power(y_axis, x_axis)

    if abs(y_axis) > 0.1 or abs(x_axis) > 0.1:
        left_speed = int(left_power * 63 / 100)  
        right_speed = int(right_power * 63 / 100)

        send_command(64 + left_speed if left_power >= 0 else 64 - abs(left_speed))  # Motor 1
        send_command(192 + right_speed if right_power >= 0 else 192 - abs(right_speed))  # Motor 2

      #  send_command(64 + right_speed if right_power >= 0 else 64 - abs(right_speed))  # Motor 1
      #  send_command(192 + left_speed if left_power >= 0 else 192 - abs(left_speed))  # Motor 2

    else:
        send_command(64)  # Stop motor 1
        send_command(192)  # Stop motor 2
        print("Motors stopped")


try:
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        
        x_axis = joystick.get_axis(0)  
        y_axis = joystick.get_axis(1) 

        control_motors(x_axis, y_axis)

        time.sleep(0.1)


except KeyboardInterrupt:
    print("Program interrupted.")

finally:
    send_command(64)  
    send_command(192)  
    ser.close()
    pygame.quit()

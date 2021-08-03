import RPi.GPIO as GPIO
import time


GPIO.setmode(GPIO.BCM)
GPIO_DIR1 = 16
GPIO_DIR2 = 18

GPIO.setup(GPIO_DIR1, GPIO.OUT)
GPIO.setup(GPIO_DIR2, GPIO.OUT)

def setspeed(speed1, speed2):
    p1.ChangeDutyCycle(speed1)
    p2.ChangeDutyCycle(speed2)

try:
    p1 = GPIO.PWM(GPIO_PWM1, 10)  # 100hz
    p1.start(10)  # start the PWM on 0% duty cycle
    p2 = GPIO.PWM(GPIO_PWM2, 10)  # 100hz
    p2.start(10)  # start the PWM on 0% duty cycle

finally:
    setspeed(0, 0)
    GPIO.cleanup()
    print("finally")

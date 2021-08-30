from adafruit_servokit import ServoKit                          #서보모터 드라이버를 사용하기 위해 패키지를 불러옵니다
import board                                                    #서보모터 드라이버 패키지에 종속된 패키지입니다
import busio                                                    #위와같이 서보모터 드라이버 패키지에 종속된 패키지입니다
import time    

float way_x, way_y , x , y

del_longi = way_x - x
del_lati = way_y - y

while True:
    if( del_longi > 0):
       motor_move(self,60,100)

    elif( del_longi < 0):
        motor_move(self,120,100)

    time.sleep(0.02)



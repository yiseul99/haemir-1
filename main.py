# -*- coding: utf8 -*- 
from lidar import *


# ################### MODE LIST ####################
# mode1 : 사용자가 직접 조종
# mode2 : camera + lidar 사용해서 자율 주행
# ##################################################


# 메인 함수(프로그램 실행)
if __name__ == "__main__":
    # 모드 입력
    # mode = int(input("어떤 모드를 실행할까요?(1: 직접 조종, 2: 카메라+라이다)\n"))
    mode = 2

    # mode1(직접 주행일 때)
    if mode == 1:
        # 클래스 불러오기
        control = ControlMode1(True)

        # 주행 시작
        control.drive_myself()

    # mode2(camera + lidar)
    elif mode == 2:
        # 클래스 불러오기
        lidar = Lidar()
        blocks = lidar.block_listen()
        for idx, blocks in enumerate(blocks[0:500]):
            print("idx = ", idx, "\n blocks[", i, "] = ", blocks[idx])

        # 주행 시작
      #  while True:
       #     control.set_destination()

    del lidar

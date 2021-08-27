

i = 0

way_x=[way1,way2,way3,way4]

while True:
    if(gps_x > way_x[i]) :
        turn left

    elif(gps_x < way_x[i]):
        turn right
    
    elif abs((gps_x-way_x[i]))<0.5:
        i += 1
    
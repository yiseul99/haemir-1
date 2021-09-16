i = int(0)
way_x=[]
way_y=[]
while True:
    a = input("way_x[%.f]:"%int(i+1))
    b = input("way_y[%.f]:"%int(i+1))
    #way_x[i] = a
    #way_y[i] = b


    if a == "end" or b == "end":
        break
    way_x.append(a)
    way_y.append(b)
    i = i+1
    
for z in range (i):
    print("way_x[z]:%f" %float(way_x[z]))
    print("way_y[z]:%f" %float(way_y[z]))

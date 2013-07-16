import time

n = 10

for i in range(n):
	print("frame:%d" % i)
	print("box:10")
	print("particles:2")
	print("x:0\ny:5\nm:1")
	print("x:%d\ny:0\nm:10" % (i-(n/2)))
	time.sleep(0.5)
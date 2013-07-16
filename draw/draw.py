import threading
import time
import io
import sys
from tkinter import *

class TextStream:
	def __init__(self, fileobject):
		self.f = fileobject

	def nextNamedToken(self, expected):
		token = self.nextToken()
		start_line = self.line
		
		# Search ahead to see if some next token is the right one
		# (skip unexpected tokens)
		while (token[0] != expected):
				token = self.nextToken()
			
		return token[1]

	def nextToken(self):
		while True:
			self.line = self.f.readline()

			if (len(self.line) == 0):
				raise IOError('End of file')

			ss = [s.strip() for s in self.line.split(':') ]
			if (len(ss) > 1):
				return ss

	# def nextFloat(self):
	# 	return float(self.f.readline())



class Box:
	def __init__(self, size):
		self.size = size

	def fromStream(stream):
		return Box(2*float(stream.nextNamedToken('box')))

	def computeScale(self, canvas, margin):
		return (canvas.width / (1+margin)) / self.size

	def draw(self, canvas):
		sz = self.size/2
		canvas.create_rectangle(-sz, -sz, sz, sz)


class Vector2d:
	def __init__(self, x, y):
		self.x = x
		self.y = y
		
class Element:
	def __init__(self, name, mass, charge, radius):
		self.name   = name
		self.mass   = mass
		self.charge = charge
		self.radius = radius
	
	def fromStream(stream):
		return Element(
			name  =stream.nextNamedToken('name'),
			mass  =float(stream.nextNamedToken('mass')),
			charge=float(stream.nextNamedToken('charge')),
			radius=float(stream.nextNamedToken('radius')))

	def elementsFromStream(stream):
		num_elements = int(stream.nextNamedToken('elements'))
		return [ Element.fromStream(stream) for i in range(num_elements)]
			
class Particle (Vector2d):
	def __init__(self, x, y, element):
		self.x = x
		self.y = y
		self.element = element

		self.size = element.radius

	def draw(self, canvas):
		canvas.create_oval(
			self.x - self.size, self.y - self.size,
			self.x + self.size, self.y + self.size)

	def fromStream(elements, stream):
		return Particle(
			element=elements[stream.nextNamedToken('element')],
			x=float(stream.nextNamedToken('x')), 
			y=float(stream.nextNamedToken('y')))

	def particlesFromStream(elements, stream):
		num_particles = int(stream.nextNamedToken('particles'))
		return [ Particle.fromStream(elements, stream) for i in range(num_particles)]

class Frame:
	def __init__(self, time, box, elements, particles):
		self.time = time
		self.box  = box
		self.elements  = elements
		self.particles = particles

	def fromStream(stream):
		time = float(stream.nextNamedToken('frame'))
		box  = Box.fromStream(stream)
		elements = { e.name : e for e in Element.elementsFromStream(stream) }
		
		return Frame(
			time = time, box = box, elements  = elements,
			particles = Particle.particlesFromStream(elements, stream))

	def draw(self, canvas):
		self.box.draw(canvas)
		pass

class DrawCanvas(Canvas):
	def __init__(self, master, width, height, timer, drawFunction):
		Canvas.__init__(self, master, width=width, height=height)

		self.width  = width
		self.height = height
		self.master = master
		self.timer  = timer
		self.scale  = 1.0

		self.setCenter()

		self.draw = drawFunction
		self.pack(fill=BOTH, expand=YES)
		self.bind('<Configure>', self.onCanvasResize)
		self.draw(self)
		master.after(timer, self.onTimer)

	"""Keep track of canvas size"""
	def onCanvasResize(self, event):
		self.width = event.width
		self.height = event.height
		self.setCenter()
		# print('(%d, %d)' % (event.width, event.height))
		self.draw(self)

	def onTimer(self):
		self.draw(self)
		self.master.after(self.timer, self.onTimer)

	def setCenter(self):
		self.cx = self.width /2
		self.cy = self.height/2

	def create_rectangle(self, x0, y0, x1, y1, **options):
		Canvas.create_rectangle(self, 
			x0*self.scale+self.cx, 
			y0*self.scale+self.cy, 
			x1*self.scale+self.cx, 
			y1*self.scale+self.cy, 
			**options)

	def create_oval(self, x0, y0, x1, y1, **options):
		Canvas.create_oval(self, 
			x0*self.scale+self.cx, 
			y0*self.scale+self.cy, 
			x1*self.scale+self.cx, 
			y1*self.scale+self.cy, 
			**options)




""" Main function """
if __name__ == "__main__":
	
	margin = 0.2
	framerate = 30
	refreshrate = framerate * 4

	input = TextStream(sys.stdin)
	test=(		io.StringIO("""frame: 1024
							   box: 12.5
							   particles: 2
							   x:0.1
							   y:0.2
							   m:1.0
							   x:0.3
							   y:0.4
							   m:1.0
							   bonds: 1
							   0 1"""))

	frames = [Frame.fromStream(input)]

	def draw(canvas):
		w = canvas.width
		h = canvas.height
		canvas.delete(ALL)
		# canvas.create_line(0, 0, w, h)
		# canvas.create_line(0, h, w, 0, fill="red", dash=(4, 4))
		# canvas.create_line(x[0], 0, x[0], h, fill="black")
		lframe = frames[0]
		canvas.scale = lframe.box.computeScale(canvas, margin)
		lframe.draw(canvas)
		for particle in lframe.particles:
			particle.draw(canvas)

		canvas.create_text(10, 10, anchor="nw", text="time: %0.2f" % lframe.time)
		#canvas.create_rectangle(w/4, h/4, 3*w/4, 3*h/4, fill="blue")

	master = Tk()
	cv = DrawCanvas(master, width=500, height=500, timer=int(1000/framerate), drawFunction = draw)

	class IOThread(threading.Thread):
		def __init__(self):
			threading.Thread.__init__(self)
			self.running = True

		def run(self):
			while (self.running):
				time.sleep(1/refreshrate)
				try:
					frames[0] = Frame.fromStream(input)
				except IOError:
					# Sleep and try again
					pass

	iothread = IOThread()
	iothread.start()

	master.mainloop()

	iothread.running = False
	iothread.join()

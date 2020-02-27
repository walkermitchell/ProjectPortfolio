# This is a module with variables and functions at module scope.

import math
import os
import re
import random

# Module functions
def forVideo( xval, yval, zval ):
	return "camera { location < " +str(xval) + "," + str(yval) + "," + str(zval) + "> look_at <0,0,0>}" 
	
def Picture( x ):
	pov_cmd = " pvengine.exe +I%s +O%s -D -V +A +H600 +W800 /exit"
	cmd = pov_cmd % ( 'tmp.pov', "tmp" + str(x).zfill(4) + ".png")
	os.system(cmd)
	
# Module variables
frames = 250
loop_counter = 0
	
# This is a tuple	
changeColor = (25, 50, 75, 100, 125, 150, 175, 200, 225)

# These are dictionaries
colors = { "White" : "<1,1,1>", "Gray" : "<0.5,0.5,0.5>" , "Red" : "<1,0,0>", "Green" : "<0,1,0>",
		   "Blue" : "<0,0,1>", "Yellow" : "<1,1,0>", "Cyan" : "<1,0,1>", "Magenta" : "<0,1,1>" }
colorIndex = { 0 : "White", 1 : "Gray", 2 : "Red", 3 : "Green", 4 : "Blue", 
			   5 : "Yellow", 6 : "Cyan", 7 : "Magenta" }


# Class time!
class Ball:
	def __init__(self, xpos, ypos, zpos, radius, color):
		self.xpos = xpos
		self.ypos = ypos
		self.zpos = zpos
		self.radius = radius
		self.color = color
	
	def getPos(self):
		return "<" + str(xpos) + "," + str(ypos) + "," + str(zpos) + ">" 
	
	def getColor(self):
		return colors[color]
		
	def getRadius(self):
		return str(radius)
	
	def moveTo(self, x, y, z):
		self.xpos = x
		self.ypos = y
		self.zpos = z
	
	def changeSize(self, r):
		self.radius *= r
		
		
def BalltoPOV(ball):
		povString = " sphere { <" + str(ball.xpos) + "," + str(ball.ypos) + "," + str(ball.zpos) + ">, " + str(ball.radius)
		povString += " texture { pigment { color rgb" + colors[ball.color] + " } } }"
		return povString

fin = open( 'base.pov' )
sdl = fin.read() # Read the entire file into a string
fin.close() 
# Now you have the entire POV source code in the string sdl


# New object to be added to POV file, calling constructor with keyword args
sphere = Ball( xpos=2.0, ypos=2.0, zpos=2.0, radius=0.25, color="White" )
changeBallSize = 1.02
	
for i in range( 0, frames ):
	# Manipulate the ball
	ballX = 2 * math.sin( math.radians(-1*i) )
	ballZ = 2 * math.cos( math.radians(-1*i) )
	sphere.moveTo( ballX, 2, ballZ )
	sphere.changeSize(changeBallSize)
	
	# Change things up if it's time to do so
	if i in changeColor :
		changeBallSize = 1/changeBallSize
		objectColors = re.findall(r"rgb<.*>", sdl)		#This is my list
		for j in range( 0, len(objectColors) ):
			sdl = re.sub( objectColors[j] , "rgb" +  colors[colorIndex[random.randint(0,7)]] , sdl )
			print( objectColors )
	
	# Manipulate the camera
	radius = 10
	xval = ( radius * math.sin( math.radians( i+2 ) ) )
	zval = ( radius * math.cos( math.radians( i+2 ) ) )
	yval = i/25
	
	# Write out data
	outfile = "tmp.pov"
	sdl_new = sdl + forVideo( xval, yval, zval )
	sdl_new += BalltoPOV(sphere) 
	fout = open( outfile, 'w' )
	fout.write( sdl_new )
	fout.close()
	Picture( i ) 
	
	# Increment the counter
	loop_counter += 1
	

# Movie time!

print( 'Encoding Movie!' )
os.system( "mencoder.exe mf://tmp*.png -mf type=png:fps=25 -ovc lavc -lavcopts \
vcodec=msmpeg4:vbitrate=2160000:keyint=5:vhq -o movie.avi " )

# Double check how many frames were completed
print( "Number of Frames: " + str( loop_counter ) )



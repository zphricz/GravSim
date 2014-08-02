import spaceSystem
import sys
import tdvector
import body
from PySide import QtGui, QtCore
from PySide.QtGui import *

SCALE_DENOM = 2.25
SIMULATION_PERIOD = 10

class Example(QtGui.QWidget):

    def __init__(self, spaceSys, width, height):
        # Inherits functions from Example
        super(Example, self).__init__()

        # Initializes the system from passed data
        self.spaceSys = spaceSys

        # Sets the size of 'space' and its location in the window
        self.height = height
        self.width = width
        self.doScale = True

        # Initializes a pause variable
        #self.pause = False

        # Finds the magnitude of the largest x and y components
        maxval = 0.0
        for i in self.spaceSys.bodies:
            if abs(i.position.xComp) > maxval:
                maxval = abs(i.position.xComp)
            if abs(i.position.yComp) > maxval:
                maxval = abs(i.position.yComp)
        self.scale = maxval / (min(width, height) / SCALE_DENOM)
        self.startScale = self.scale

        # Sets the desired # of calculations per draw
        self.interval = 150

        # Initializes the GUI
        self.initGUI()

    def initGUI(self):

        # Creates a timer to run upon application startup with a speed in mS.
        self.simulation_timer = QtCore.QBasicTimer()
        self.draw_timer = QtCore.QBasicTimer()

        # Sets the currently selected body and pixel movement
        self.selection = 0
        self.pixelnum = 15

        # Repositions the screen to the top left with a valid size
        self.setGeometry(0, 0, self.width, self.height)
        self.setWindowTitle('Points')
        self.show()
        self.draw_timer.start(1000.0 / 60.0, self)
        self.simulation_timer.start(SIMULATION_PERIOD, self)

################################################################################
# Events Section - These occur from specific types of interaction with the GUI
################################################################################

    def paintEvent(self, event):

            # Initializes painter variable
            painter = QtGui.QPainter()
            painter.begin(self)

            # Draws the system
            self.drawSystem(painter)

            # Terminates the painter variable
            painter.end()

    # Performs regular actions to the table/space system
    def timerEvent(self, event):

        if event.timerId() == self.simulation_timer.timerId():

            # Performs *interval* timestep calculations on the system
            for i in range(100):
                self.spaceSys.gravitate()
                self.spaceSys.move()

            for body in self.spaceSys.bodies:
                if body.head == body.trackLength:
                    body.head = 0
                else: body.head += 1
                body.track[body.head] = tdvector.TDVector(body.position.xComp, body.position.yComp)
        else:
            # Signals a painting event to occur
            self.update()

    def keyPressEvent(self, event):

        # Shortcuts to the currently selected body
        if self.spaceSys.numBodies != 0:
            sel = self.spaceSys.bodies[self.selection]
        else:
            sel = body.Body(0,0,0,0,0,False,"NULL", 0)

        # Shift key shifts to the next body
        if event.key() == QtCore.Qt.Key_Shift:
            self.selection += 1
            if self.selection >= self.spaceSys.numBodies:
                self.selection = 0

        # arrow keys move the planet in the given logical direction
        elif event.key() == QtCore.Qt.Key_Left:
            sel.position.xComp -= self.pixelnum * self.scale
            sel.track[sel.head].xComp = sel.position.xComp
        elif event.key() == QtCore.Qt.Key_Right:
            sel.position.xComp += self.pixelnum * self.scale
            sel.track[sel.head].xComp = sel.position.xComp
        elif event.key() == QtCore.Qt.Key_Up:
            sel.position.yComp += self.pixelnum * self.scale
            sel.track[sel.head].yComp = sel.position.yComp
        elif event.key() == QtCore.Qt.Key_Down:
            sel.position.yComp -= self.pixelnum * self.scale
            sel.track[sel.head].yComp = sel.position.yComp

        # F1 and F2 double and half the timestep, respectively
        elif event.key() == QtCore.Qt.Key_F1:
            self.spaceSys.timeStep *= 2.0
        elif event.key() == QtCore.Qt.Key_F2:
            self.spaceSys.timeStep /= 2.0

        # F3 toggles isMoveable, making the selected body moveable or not
        elif event.key() == QtCore.Qt.Key_F3:
            sel.isMoveable = not sel.isMoveable
            sel.velocity.xComp = 0.0
            sel.velocity.yComp = 0.0

        # F4 toggles pause, halting the simulation
        elif event.key() == QtCore.Qt.Key_F4:
            if self.simulation_timer.isActive():
                self.simulation_timer.stop()
            else:
                self.simulation_timer.start(SIMULATION_PERIOD, self)

        # F5 stops the program from performing scaling
        elif event.key() == QtCore.Qt.Key_F5:
            self.doScale = not self.doScale

        # Esc closes the program and stores data
        elif event.key() == QtCore.Qt.Key_Escape:
            self.close()

        elif event.key() == QtCore.Qt.Key_Delete and self.spaceSys.numBodies != 0:
            self.spaceSys.removeBody(sel)
            if self.selection >= self.spaceSys.numBodies:
                self.selection = 0

################################################################################

    # Scans if scaling effects are necessary for viewing
    def scaler(self):
        maxX = 0
        maxY = 0

        # Finds the magnitude of the largest x and y components
        for i in self.spaceSys.bodies:
            if abs(i.position.xComp) > maxX:
                maxX = abs(i.position.xComp)
            if abs(i.position.yComp) > maxY:
                maxY = abs(i.position.yComp)

        ## If the drawing is too compressed or extended, scale it
        newScaleX = maxX / (self.width / SCALE_DENOM)
        newScaleY = maxY / (self.height / SCALE_DENOM)
        newScale = max(newScaleX, newScaleY)
        if newScale > self.startScale:
            self.scale = newScale

    # Draws the system
    def drawSystem(self, painter):

        # Scales the system if toggled on
        if self.doScale:
            self.scaler()

        # Fills in the screen with Black, deleting everything present
        painter.fillRect(0, 0, self.width, self.height, QtGui.QColor(0,0,0))

        # Finds the speed of the fastest body. Used to determine the color for each body.
        maxSpeed = 1.0
        for i in self.spaceSys.bodies:
            if i.speed() > maxSpeed:
                maxSpeed = i.speed()

        # Draws the trails and bodies
        for i in self.spaceSys.bodies:

            # Loop through all positions in the track list, drawing lines between each one
            # except for i.track[i.head] -> i.track[tail]
            if i.head == i.trackLength: tail = 0
            else: tail = i.head + 1
            j = tail
            if j == i.trackLength: k = 0
            else: k = j + 1
            while k != tail:
                painter.drawLine(self.width/2 + i.track[j].xComp/self.scale,
                                 self.height/2 - i.track[j].yComp/self.scale,
                                 self.width/2 + i.track[k].xComp/self.scale,
                                 self.height/2 - i.track[k].yComp/self.scale)

                j = k
                if k == i.trackLength: k = 0
                else: k += 1

            x = i.position.xComp/self.scale
            y = i.position.yComp/self.scale

            # Fills in a body at the given x and y coordinate with a color that
            # is derived from its speed
            magnitude = 511.0 * i.speed() / maxSpeed
            blue = 0.0
            if magnitude <= 255.0:
                red = 255.0
                green = magnitude
            else:
                red = 511.0 - magnitude
                green = 255.0

            if not i.isMoveable:
                red = 0.0
                green = 0.0
                blue = 255.0

            painter.fillRect(self.width/2 + x - 2,
                             self.height/2 - y - 2,
                             5, 5, QtGui.QColor(red, green, blue))

            # Adds crosshairs for the currently selected body
            if i == self.spaceSys.bodies[self.selection]:

                painter.fillRect(self.width /2 + x - 5,
                                 self.height/2 - y + 0,
                                 11, 1, QtGui.QColor(0,0,255) )
                painter.fillRect(self.width /2 + x + 0,
                                 self.height/2 - y - 5,
                                 1, 11, QtGui.QColor(0,0,255) )


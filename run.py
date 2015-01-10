#!/usr/bin/python2.7
import drawSystem
import spaceSystem
import body
import sys
from PySide import QtGui, QtCore
from PySide.QtGui import *

import sys

if len(sys.argv) != 3:
    sys.stderr.write("USAGE: ./run.py [Width] [Height]\n")
    sys.exit(1)

try:
    width = int(sys.argv[1])
    height = int(sys.argv[2])
except ValueError:
    sys.stderr.write("Please input numbers for width and height parameters\n")
    sys.exit(1)

# Bodies
Star = body.Body(0.0, 0.0, 0.0, 0.0, 6.0 * 10 ** 24, False, 'Star', 0)
Planet1 = body.Body(400000000.0, 0.0, 0.0, 1006.0, 7.0 * 10.0 ** 22.0, True, 'Planet1', 150)
Moon1 = body.Body(420000000.0, 0.0, 0.0, 1500.0, 4.0 * 10.0 ** 20.0, True, 'Moon1', 150)
Planet2 = body.Body(0.0, 400000000.0, -1006.0, 0.0, 7.0 * 10.0 ** 22.0, True, 'Planet2', 150)
Moon2 = body.Body(0.0, 420000000.0, -1500.0, 0.0, 4.0 * 10.0 ** 20.0, True, 'Moon2', 150)
Planet3 = body.Body(-400000000.0, 0.0, 0.0, -1006.0, 7.0 * 10.0 ** 22.0, True, 'Planet3', 150)
Moon3 = body.Body(-420000000.0, 0.0, 0.0, -1500.0, 4.0 * 10.0 ** 20.0, True, 'Moon3', 150)
Planet4 = body.Body(0.0, -400000000.0, 1006.0, 0.0, 7.0 * 10.0 ** 22.0, True, 'Planet4', 150)
Moon4 = body.Body(0.0, -420000000.0, 1500.0, 0.0, 4.0 * 10.0 ** 20.0, True, 'Moon4', 150)

spaceSys = spaceSystem.SpaceSystem()
spaceSys.addBody(Star)
spaceSys.addBody(Planet1)
spaceSys.addBody(Moon1)
"""spaceSys.addBody(Planet2)
spaceSys.addBody(Moon2)
spaceSys.addBody(Planet3)
spaceSys.addBody(Moon3)
spaceSys.addBody(Planet4)
spaceSys.addBody(Moon4)"""

# Starts the application and passes system information
app = QtGui.QApplication(sys.argv)
ex = drawSystem.Example(spaceSys, width, height)

sys.exit(app.exec_())

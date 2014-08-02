import tdvector

# A class to hold all of the oribtal information of a body.
class Body():

    # xPos and yPos are the coordinates of the body (meters).
    # xVel and yVel are the velocity components of the body (meters/second).
    # mass is the mass of the body (kilograms).
    # head is used to determine where in the track to start drawing the trail from.
    # isMovable determines if the position of the body is allowed to change in time.
    # trackLength is the number of positions which are kept track of to show the
    # trail of the Body.
    # The list track holds previous position of the body for use in
    # drawing the trail. It is initially filled with the body's initial
    # position.
    def __init__(self, xPos, yPos, xVel, yVel, mass,
                 isMoveable, name, trackLength):
        self.position = tdvector.TDVector(xPos, yPos)
        self.velocity = tdvector.TDVector(xVel, yVel)
        self.forceSum = tdvector.TDVector(0.0, 0.0)
        self.mass = mass
        self.isMoveable = isMoveable
        self.name = name
        self.head = 0
        if isMoveable == 0:
            self.velocity = tdvector.TDVector(0.0,0.0)
        self.track = []
        for i in range(trackLength + 1):
            self.track.append(tdvector.TDVector(xPos, yPos))
        self.trackLength = trackLength

    # Returns the speed at which the Body is currently moving.
    def speed(self):
        return self.velocity.magnitude()

    # Sets the value of the forceSum vector's components to zero.
    def zero(self):
        self.forceSum.xComp = 0.0
        self.forceSum.yComp = 0.0



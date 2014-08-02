import math

# A simple vector class that holds two components of data.
class TDVector():

    def __init__(self, xComp, yComp):
        self.xComp = xComp
        self.yComp = yComp

    # Returns the scalar magnitude of the TDVector.
    def magnitude(self):
        return math.sqrt(self.xComp * self.xComp + self.yComp * self.yComp)

    # Returns the square of the scalar magnitude of the TDVector.
    def magnitudeSquare(self):
        return self.xComp * self.xComp + self.yComp * self.yComp

    # Returns the angle of the TDVector relative to the origin,
    # counter-clockwise from the positive x-axis.
    def angle(self):
        if self.xComp > 0:
            if self.yComp >= 0:
                return math.atan(self.yComp/self.xComp)
            return math.atan(self.yComp/self.xComp) + 2 * math.pi
        if self.xComp < 0:
            return math.atan(self.yComp/self.xComp) + math.pi
        if self.xComp == 0:
            if self.yComp > 0:
                return math.pi / 2
            if self.yComp < 0:
                return 3 * math.pi / 2
            return float('NaN')

    # Returns the TDVector multiplied by the scalar "factor".
    def scale(self, factor):
        return TDVector(self.xComp * factor, self.yComp * factor)

    # Adds the components of Vec to the TDVector that calls this
    def add(self, Vec):
        self.xComp += Vec.xComp
        self.yComp += Vec.yComp

    # Subtracts the components of Vec from the TDVector that calls this
    def subtract(self, Vec):
        self.xComp -= Vec.xComp
        self.yComp -= Vec.yComp

# Returns the result of Vec1 - Vec2.
def difference(Vec1, Vec2):
    return TDVector(Vec1.xComp - Vec2.xComp, Vec1.yComp - Vec2.yComp)



import body
import tdvector
import math

# A class that allows members of the Body class to interact with eachother.
class SpaceSystem():

    # timestep is the amount of time between intervals of simulation (seconds).
    # UGC is the universal gravitational constant.
    # bodies contains every Body to be simulated.
    def __init__(self, UGC = 6.67384 * 10 ** (-11), timeStep = 50):
        self.bodies = []
        self.numBodies = 0
        self.timeStep = timeStep
        self.UGC = UGC

    # Add a new Body to the spaceSystem.
    def addBody(self, newBody):
        self.bodies.append(newBody)
        self.numBodies += 1

    def removeBody(self, removeBody):
        self.bodies.remove(removeBody)
        self.numBodies -= 1

    # Modify the acceleration TDVectors of all bodies in the space along one
    # time interval. Uses the Newtonian gravitational equation.
    def gravitate(self):

        # Every Body only needs to interact with every other Body once. These
        # loops are set to loop through every Body pair only once.
        for i, iBody in enumerate(self.bodies[:self.numBodies - 1]):
            for jBody in self.bodies[i+1:]:

                # This TDVector describes the difference in position between
                # two bodies.
                deltaPos = tdvector.difference(jBody.position, iBody.position)

                # The angle that jBody makes relative to iBody.
                theta = deltaPos.angle()

                # The scalar magnitude of force that each body exerts on
                # eachother.
                forceMag = (self.UGC * iBody.mass * jBody.mass /
                            deltaPos.magnitudeSquare())

                # The TDVector quantity of force that is exerted on iBody.
                # The TDVector quantity of force that is extered on jBody is
                # merely the opposite of this vector.
                forceVec = tdvector.TDVector(forceMag * math.cos(theta),
                                             forceMag * math.sin(theta))

                # Modify each body's forceSum accordingly
                iBody.forceSum.add(forceVec)
                jBody.forceSum.subtract(forceVec)

    # Change the velocity and position vectors of each body accordingly.
    def move(self):
        for body in self.bodies:
            if body.isMoveable:
                # Delta-Velocity = Force * Time / Mass
                body.velocity.add(body.forceSum.scale(self.timeStep/body.mass))

                # Delta-Position = Velocity * Time
                body.position.add(body.velocity.scale(self.timeStep))

            # Reset the forceSum vector
            body.zero()



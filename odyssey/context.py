from enum import Enum

import zmq


class QueueStatus(Enum):
    NEW_MESSAGE = 0
    EMPTY = 1
    ERROR = 2


class Message:
    def __init__(self, data, address):
        self.data = data
        self.address = address


class Context:
    def __init__(self):
        context = zmq.Context()
<<<<<<< HEAD
        data_out = context.socket(zmq.PUB)  # ruff: ignore[F841]
        data_in = context.socket(zmq.SUB)  # ruff: ignore[F841]
        command = context.socket(zmq.REQ)  # ruff: ignore[F841]

    """Sends a CAN message"""

    def send_sim_can(self, message):
=======
        dataOut = context.socket(zmq.PUB)
        dataIn = context.socket(zmq.SUB)
        command = context.socket(zmq.REQ)

    """Sends a CAN message"""

    def sendSimCAN(message):
>>>>>>> e0b3ecc (fix formatting)
        return

    """Stores the message into the provided buffer and returns a QueueStatus"""

<<<<<<< HEAD
    def poll_sim_can(self, bus, message):
=======
    def pollSimCAN(bus, message):
>>>>>>> e0b3ecc (fix formatting)
        return

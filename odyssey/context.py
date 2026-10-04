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
        data_out = context.socket(zmq.PUB)
        data_in = context.socket(zmq.SUB)
        command = context.socket(zmq.REQ)

    """Sends a CAN message"""

    def send_sim_can(self, message):
        return

    """Stores the message into the provided buffer and returns a QueueStatus"""

    def poll_sim_can(self, bus, message):
        return

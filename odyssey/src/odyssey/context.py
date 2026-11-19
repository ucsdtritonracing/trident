import zmq

from .peripherals import CANPeripheral
from .ports import Peripherals


class Poller:
    def __init__(self):
        self.poller = zmq.Poller()
        self.handlers = {}  # (socket: peripheral)

    def register(self, peripheral):
        self.poller.register(peripheral.get_recv_sock(), zmq.POLLIN)
        self.handlers[peripheral.get_recv_sock()] = peripheral

    def poll(self):
        return self.poller.poll(0)


class Context:
    def __init__(self, peripherals: Peripherals):
        self.zmq_ctx = zmq.Context()
        self.poller = Poller()

        self.can1 = CANPeripheral(self.zmq_ctx, self.poller, peripherals.can1) if peripherals.can1 is not None else None
        self.command = self.zmq_ctx.socket(zmq.REQ)  # ruff: ignore[F841]

    def poll(self):
        events = self.poller.poll()
        for sock in events:
            p = self.poller.handlers[sock[0]]
            p.recv()  # get message from sock

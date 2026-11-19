import zmq

from .peripherals import CANPeripheral
from .ports import Peripherals


class Context:
    def __init__(self, peripherals: Peripherals):
        self.zmq_ctx = zmq.Context()
        self.handlers = {}  # (socket: peripheral)
        if peripherals.can1 is not None:
            self.can1 = CANPeripheral(self.zmq_ctx, peripherals.can1)
        self.command = self.zmq_ctx.socket(zmq.REQ)  # ruff: ignore[F841]

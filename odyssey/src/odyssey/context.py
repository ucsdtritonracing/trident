import zmq

from .peripherals import CANPeripheral
from .ports import AppConfig


class Context:
    def __init__(self, peripherals: AppConfig):
        self.zmq_ctx = zmq.Context()
        self.peripherals = peripherals.value
        if self.peripherals.can1 is not None:
            self.can1 = CANPeripheral(self.zmq_ctx, self.peripherals.can1)
        self.command = self.zmq_ctx.socket(zmq.REQ)  # ruff: ignore[F841]

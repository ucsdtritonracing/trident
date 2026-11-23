import zmq

from .peripherals import CANPeripheral
from .ports import AppConfig


class Context:
    def __init__(self, app_config: AppConfig):
        self.zmq_ctx = zmq.Context()
        self.app_config = app_config.value
        if self.app_config.can1 is not None:
            self.can1 = CANPeripheral(self.zmq_ctx, self.app_config.can1)
        self.command = self.zmq_ctx.socket(zmq.REQ)  # ruff: ignore[F841]

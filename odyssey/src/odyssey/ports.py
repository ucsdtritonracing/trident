from dataclasses import dataclass
from enum import Enum


@dataclass(frozen=True, eq=False)
class Peripherals:
    can1: SocketConfig | None = None


class SocketConfig:
    def __init__(self, in_port: int, out_port: int):
        self.in_port = in_port
        self.out_port = out_port


class AppConfig(Enum):
    Front = Peripherals(can1=SocketConfig(1234, 123213))

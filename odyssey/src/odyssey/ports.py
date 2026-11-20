from dataclasses import dataclass
from enum import Enum


class SocketConfig:
    def __init__(self, in_port: int, out_port: int):
        self.in_port = in_port
        self.out_port = out_port


@dataclass(frozen=True, eq=False)
class AppSockets:
    can1: SocketConfig | None = None


class AppConfig(Enum):
    FRONT = AppSockets(can1=SocketConfig(1234, 123213))

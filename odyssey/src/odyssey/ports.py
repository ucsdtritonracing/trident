from dataclasses import dataclass
from enum import Enum

from .peripherals import CANPeripheral


@dataclass(frozen=True, eq=False)
class Ports:
    can1: CANPeripheral | None = None


class PortConfig(Enum):
    Front = Ports(can1=CANPeripheral())
    #TODO: FIX PROPER INSTANTIATON add extra init method that takes in context and then finally inits sockets

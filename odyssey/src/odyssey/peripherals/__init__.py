# peripherals/__init__.py
from .base_peripheral import BasePeripheral
from .can_peripheral import CANPeripheral

__all__ = ["BasePeripheral", "CANPeripheral"]

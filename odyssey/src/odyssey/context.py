from dataclasses import fields
from .peripherals import CANPeripheral 
import zmq

from .ports import Peripherals

class Context:
    
    def __init__(self, peripherals: Peripherals):
        self.can1 = CANPeripheral(peripherals.can1) if not None else None

        self.zmq_ctx = zmq.Context()
        self.command = self.zmq_ctx.socket(zmq.REQ)  # ruff: ignore[F841]
        self.poller = zmq.Poller()
        self.peripherals = {}

        for p in self.peripherals:
            self.poller.register(p.get_recv_socket(), zmq.POLLIN)

    def poll(self):
        events = self.poller.poll(0)
        for sock in events:
            sock[0].recv()  #get message from sock = (socket, event_mask) 

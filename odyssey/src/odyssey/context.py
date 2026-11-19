from dataclasses import fields

import zmq

from .ports import PortConfig


class Context:
    def __init__(self, config: PortConfig):
        self.zmq_ctx = zmq.Context()
        self.command = self.context.socket(zmq.REQ)  # ruff: ignore[F841]
        self.poller = zmq.Poller()
        self.peripherals = {}
        for f in fields(config.value):
            p = getattr(config.value, f.name)
            if p is not None:
                self.peripherals[f.name] = p
        for p in self.peripherals:
            self.poller.register(p.getRecvSocket(), zmq.POLLIN)

    def __getattr__(self, name: str):
        try:
            return self.peripherals[name]
        except KeyError:
            print(name + " is not supported by " + self.config.name)

    def poll(self):
        events = self.poller.poll(0)
        for sock in events:
            sock.recv()  # get message and queue it


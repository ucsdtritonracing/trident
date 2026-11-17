import zmq
from ports import PortConfig


class Context:
    def __init__(self, config: PortConfig):
        context = zmq.Context()
        command = context.socket(zmq.REQ)  # ruff: ignore[F841]
        self.poller = zmq.Poller()
        ports = config.value
        for port in ports:
            self.poller.register(port.getRecvSocket(), zmq.POLLIN)

    def poll(self):
        events = self.poller.poll()
        for sock in events:
            sock.recv()  # put message in that queue
            # should it

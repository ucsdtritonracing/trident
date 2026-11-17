import queue

import zmq


class BasePeripheral:
    def __init__(self, context, out_port, in_port):
        self.recvQueue = queue.Queue(500)
        self.data_out = context.socket(zmq.PUB)
        self.data_in = context.socket(zmq.SUB)
        self.data_out.bind("tcp://localhost:" + out_port)
        self.data_in.connect("tcp://localhost:" + in_port)
        # subscribe to everything, as sockets autofilter
        self.data_out.setsockopt(zmq.SUBSCRIBE, "")
        self.data_in.setsockopt(zmq.SUBSCRIBE, "")

    def send(self, msg):
        self.data_out.send()

    def recv(self):
        try:
            self.recvQueue.put(self.data_in.recv())
        except queue.Full:
            print("Internal Queue Full! (Not HWM) @TCP Port:" + self.data_in.underlying)

    def get_send_socket(self):
        return self.data_out

    def get_recv_socket(self):
        return self.data_in

    def get_last_msg(self):
        try:
            return self.recvQueue.get()
        except queue.Empty:
            return None

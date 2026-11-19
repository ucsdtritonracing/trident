from collections import deque

import zmq


class BasePeripheral:
    def __init__(self, out_port, in_port):
        self.out_port = out_port
        self.in_port = in_port
        self.recv_queue = deque.Queue(500)

    def init_sockets(self, context):
        self.out_sock = context.socket(zmq.PUB)
        self.in_sock = context.socket(zmq.SUB)
        self.out_sock.bind(f"tcp://localhost: {self.out_port}")
        self.in_sock.connect(f"tcp://localhost: {self.in_port}")
        # subscribe to everything, as sockets autofilter
        self.in_sock.setsockopt(zmq.SUBSCRIBE, b"")

    def serialize(self, msg):
        # Should serialize one protobuf message
        # Returns its bytestring in a single element list with SerializeToString()
        raise NotImplementedError("Must be implemented!")

    def deserialize(self, msg):
        # Should deserialize a bytestring inside a single element list with ParseFromString()
        # Returns the message, not in a list
        raise NotImplementedError("Must be implemented!")

    def send(self, msg):
        self.out_sock.send_serialized(msg, self.serialize)

    def recv(self):
        self.recv_queue.append(self.in_sock.recv_serialized(self.deserialize))
    
    def get_send_socket(self):
        return self.out_sock

    def get_recv_socket(self):
        return self.in_sock

    def get_next_msg(self):
        try:
            return self.recvQueue.pop()
        except IndexError:
            return None

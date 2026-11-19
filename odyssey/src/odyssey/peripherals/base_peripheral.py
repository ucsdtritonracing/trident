
import zmq

from odyssey import SocketConfig


class BasePeripheral:
    def __init__(self, context, sock_config: SocketConfig):
        self.out_port = sock_config.out_port
        self.in_port = sock_config.in_port
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
        # Should deserialize a ZMQ Frame inside a single element list with ParseFromString()
        # Returns the message, not in a list
        raise NotImplementedError("Must be implemented!")

    def send(self, msg):
        self.out_sock.send_serialized(msg, self.serialize)

    def recv(self):
        return self.in_sock.recv_serialized(self.deserialize)

    def recv_all(self):
        msg_list = []
        while True:
            try:
                msg_list.append(self.in_sock.recv_serialized(self.deserialize, flags=zmq.NOBLOCK))
            except zmq.Again:
                break  # no more messages as queue empty
        return msg_list

    def get_send_socket(self):
        return self.out_sock

    def get_recv_socket(self):
        return self.in_sock

import queue

import zmq


class BasePeripheral:
    def __init__(self, context, out_port, in_port):
        self.recv_queue = queue.Queue(500)
        self.out_sock = context.socket(zmq.PUB)
        self.in_sock = context.socket(zmq.SUB)
        self.out_sock.bind(f"tcp://localhost: {out_port}")
        self.in_sock.connect(f"tcp://localhost: {in_port}")
        # subscribe to everything, as sockets autofilter
        self.in_sock.setsockopt(zmq.SUBSCRIBE, b"")

    def serialize(self, msg):
        # Should serialize one protobuf message
        # Returns its bytestring in a single element list with SerializeToString()
        raise NotImplementedError("Must be implemented!")
    
    def deserialize(self,msg):
        # Should deserialize one bytestring inside a single element list in place with ParseFromString()
        # Returns nothing 
        raise NotImplementedError("Must be implemented!")

    def send(self, msg):
        self.out_sock.send_serialized(self.serialize)

    def recv(self):
        try:
            self.recv_queue.put(self.in_sock.recv_serialized(self.deserialize))
        except queue.Full:
            print("Internal Queue Full! (Not HWM) @TCP Port:" + self.in_sock.underlying)

    def get_send_socket(self):
        return self.out_sock

    def get_recv_socket(self):
        return self.in_sock

    def get_next_msg(self):
        try:
            return self.recvQueue.get()
        except queue.Empty:
            return None

from peripherals import BasePeripheral


class CANPeripheral(BasePeripheral):
    def __init__(self, context, out_port, in_port):
        super.__init__(context=context, out_port=out_port, in_port=in_port)

    def send_can(self, msg):
        # serialize with protobuf first
        super.send(msg)  # then send over zmq

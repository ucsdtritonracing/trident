from .base_peripheral import BasePeripheral


class CANPeripheral(BasePeripheral):
    def __init__(self, context, out_port, in_port):
        super().__init__(context=context, out_port=out_port, in_port=in_port)

    def serialize(self, msg):
        # implement serialize function here
        pass

    def deserialize(self, msg):
        # implement deserialize function here
        pass

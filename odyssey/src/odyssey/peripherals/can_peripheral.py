from .base_peripheral import BasePeripheral
from odyssey.protocol import can_pb2 

class CANPeripheral(BasePeripheral):
    def __init__(self, context, out_port, in_port):
        super().__init__(context=context, out_port=out_port, in_port=in_port)

    
    def serialize(self, msg):
        pass
    
    def deserialize(self, msg):
        pass

    

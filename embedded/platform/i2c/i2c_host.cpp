#include "platform/i2c/i2c_host.h"

#include "platform/i2c/i2c.h"

namespace Platform::I2c {

StartStatus HostPeripheral::StartRawWrite(Transfer& transfer) { return StartStatus::ERROR; }

StartStatus HostPeripheral::StartRawRead(Transfer& transfer) { return StartStatus::ERROR; }

StartStatus HostPeripheral::StartRegisterWrite(Transfer& transfer, uint16_t reg,
                                               RegisterSize reg_size) {
    return StartStatus::ERROR;
}

StartStatus HostPeripheral::StartRegisterRead(Transfer& transfer, uint16_t reg,
                                              RegisterSize reg_size) {
    return StartStatus::ERROR;
}

TransferStatus HostPeripheral::Poll(const Transfer& transfer) { return transfer.status; }

} // namespace Platform::I2c

#include "platform/i2c/i2c_stm32.h"

#include "platform/i2c/i2c.h"

namespace Platform::I2C {

StartStatus Stm32Peripheral::StartRawWrite(Transfer& transfer) { return StartStatus::ERROR; }

StartStatus Stm32Peripheral::StartRawRead(Transfer& transfer) { return StartStatus::ERROR; }

StartStatus Stm32Peripheral::StartRegisterWrite(Transfer& transfer, uint16_t reg,
                                                RegisterSize reg_size) {
    return StartStatus::ERROR;
}

StartStatus Stm32Peripheral::StartRegisterRead(Transfer& transfer, uint16_t reg,
                                               RegisterSize reg_size) {
    return StartStatus::ERROR;
}

TransferStatus Stm32Peripheral::Poll(const Transfer& transfer) { return transfer.status; }

} // namespace Platform::I2C

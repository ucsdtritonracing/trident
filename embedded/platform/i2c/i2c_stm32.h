#pragma once

#include "platform/i2c/i2c.h"

namespace Platform::I2C {

/// STM32 I2C peripheral. Stub for now with every Start*() call set to fail
class Stm32Peripheral : public Peripheral {
public:
    StartStatus StartRawWrite(Transfer& transfer) override;
    StartStatus StartRawRead(Transfer& transfer) override;
    StartStatus StartRegisterWrite(Transfer& transfer, uint16_t reg,
                                   RegisterSize reg_size) override;
    StartStatus StartRegisterRead(Transfer& transfer, uint16_t reg, RegisterSize reg_size) override;
    TransferStatus Poll(const Transfer& transfer) override;
};

} // namespace Platform::I2C

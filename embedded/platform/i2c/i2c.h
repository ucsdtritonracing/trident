#pragma once

#include <cstdint>
#include <span>

namespace Platform::I2C {

/// Result of a Start*() call.
enum class StartStatus : uint8_t {
    STARTED,
    BUSY, ///< Another transfer is in progress, so this one was rejected.
    INVALID,
    ERROR,
};

/// State of a transfer, set by Start*() and updated when the transfer finishes.
enum class TransferStatus : uint8_t {
    IDLE,
    IN_PROGRESS,
    DONE,
    NACK,
    BUS_ERROR,
    ARBITRATION_LOST,
    OVERRUN,
    DMA_ERROR,
    TIMEOUT,
    ERROR,
};

/// Size of a device's register address.
enum class RegisterSize : uint8_t {
    ONE_BYTE,
    TWO_BYTES,
};

/**
 * One I2C transaction, owned by the driver that starts it. The transfer and its buffer must stay
 * alive and unchanged until Poll() returns something other than IN_PROGRESS.
 */
struct Transfer {
    /// 7-bit device address, unshifted.
    uint8_t address;
    std::span<uint8_t> buffer;
    /// Updated from an interrupt, so volatile forces a fresh read each time.
    volatile TransferStatus status = TransferStatus::IDLE;
};

/// Non-blocking I2C controller that runs one transfer at a time.
class Peripheral {
public:
    virtual ~Peripheral() = default;

    /// Start writing the buffer to the device.
    virtual StartStatus StartRawWrite(Transfer& transfer) = 0;

    /// Start reading from the device into the buffer.
    virtual StartStatus StartRawRead(Transfer& transfer) = 0;

    /// Start writing the buffer to a register on the device.
    virtual StartStatus StartRegisterWrite(Transfer& transfer, uint16_t reg, RegisterSize reg_size) = 0;
                                           

    /// Start reading from a register on the device into the buffer.
    virtual StartStatus StartRegisterRead(Transfer& transfer, uint16_t reg, RegisterSize reg_size) = 0;

    /// Get the current status of a transfer.
    virtual TransferStatus Poll(const Transfer& transfer) = 0;
};

} // namespace Platform::I2C

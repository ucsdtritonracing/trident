#pragma once

#include <atomic>
#include <cstdint>
#include <span>

#include "core/time.h"

namespace Platform::I2c {

/// Result of a Start*() call.
enum class StartStatus : uint8_t {
    STARTED, ///< The transfer was added to the queue.
    BUSY,    ///< The queue is full, so this transfer was rejected.
    INVALID,
    ERROR,
};

/// State of a transfer, set to QUEUED by Start*() and updated as the transfer runs.
enum class TransferStatus : uint8_t {
    IDLE,
    QUEUED,      ///< Waiting in the queue, not on the bus yet.
    IN_PROGRESS, ///< On the bus now.
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
 * alive and unchanged until Poll() returns something other than QUEUED or IN_PROGRESS.
 */
struct Transfer {
    /// 7-bit device address, unshifted.
    uint8_t address;
    std::span<uint8_t> buffer;
    /// Max time on the bus before the transfer is stopped with TIMEOUT. 0 means no limit.
    Core::Duration timeout = 0us;
    /// Updated from an interrupt. Atomic so a DONE read is ordered after the buffer was filled.
    std::atomic<TransferStatus> status{TransferStatus::IDLE};
};

/// Non-blocking I2C controller. Start*() adds a transfer to a queue, and transfers run in order.
class Peripheral {
public:
    virtual ~Peripheral() = default;

    /// Queue a write of the buffer to the device.
    virtual StartStatus StartRawWrite(Transfer& transfer) = 0;

    /// Queue a read from the device into the buffer.
    virtual StartStatus StartRawRead(Transfer& transfer) = 0;

    /// Queue a write of the buffer to a register on the device.
    virtual StartStatus StartRegisterWrite(Transfer& transfer, uint16_t reg,
                                           RegisterSize reg_size) = 0;

    /// Queue a read from a register on the device into the buffer.
    virtual StartStatus StartRegisterRead(Transfer& transfer, uint16_t reg,
                                          RegisterSize reg_size) = 0;

    /// Get the current status of a transfer.
    virtual TransferStatus Poll(const Transfer& transfer) = 0;
};

} // namespace Platform::I2c

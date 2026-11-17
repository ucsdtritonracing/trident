#include "targets/compute_module/stm32/peripherals.h"

#include "fdcan.h"

namespace Target::Stm32 {

// can1 is routed to FDCAN2, and can2 is routed to FDCAN3.
constinit Platform::Can::Stm32Peripheral can1{hfdcan2};
constinit Platform::Can::Stm32Peripheral can2{hfdcan3};

} // namespace Target::Stm32

namespace {

// HAL callbacks are shared by every FDCAN instance, so map the handle back to its peripheral.
Platform::Can::Stm32Peripheral* FromHandle(const FDCAN_HandleTypeDef* hfdcan) {
    if (hfdcan == &hfdcan2) {
        return &Target::Stm32::can1;
    }
    if (hfdcan == &hfdcan3) {
        return &Target::Stm32::can2;
    }
    return nullptr;
}

} // namespace

// Overrides the weak HAL definitions. Called from HAL_FDCAN_IRQHandler during the interrupt.
extern "C" {

// NOLINTNEXTLINE(readability-identifier-naming): HAL callback signature
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs) {
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) == 0) {
        return;
    }

    Platform::Can::Stm32Peripheral* peripheral = FromHandle(hfdcan);
    if (peripheral != nullptr) {
        peripheral->OnRxFifo0Interrupt();
    }
}
}

#include "can_stm32.h"

#include "can.h"

namespace Platform::Can {

SendStatus Stm32Peripheral::Send(const Message& message) { return SendStatus::ERROR; }

PollStatus Stm32Peripheral::Poll(Message& out) { return PollStatus::ERROR; }

BusState Stm32Peripheral::GetStatus() { return BusState::BUS_OFF; }

} // namespace Platform::Can

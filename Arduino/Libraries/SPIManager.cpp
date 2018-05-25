#include <Arduino.h>
#include <SPIManager.h>

SPIManager* spi_manager = nullptr;

SPIManager::SPIManager(GameState* gameState) {
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

  // Update reference to the current SPI manager.
  spi_manager = this;

  // Store a reference to the game state.
  this->gameState = gameState;
}

uint8_t SPIManager::dataISR(uint8_t data) {
  uint8_t nextByte;

  // If a message has not been received in a while assume that the first
  // byte is the start of the message.
  uint32_t currentTime = micros();
  if (currentTime - lastByteTransaction > 1000) {
    state = MessageState::Address;
  }
  lastByteTransaction = currentTime;

  // Determine what byte to send next.
  switch (state) {
    case MessageState::Address:
      // Calculate the receive and transmit op codes.
      transmitOpCode = OpCode(data & 0x0F);
      receiveOpCode = OpCode(data >> 4);

      // Logic for handling the receive op code.
      switch (receiveOpCode) {
        case OpCode::ModuleType:
          nextByte = uint8_t(moduleType);
          transmitByte = 0xFF;
          break;
        case OpCode::PlaySound:
          nextByte = uint8_t(sound);
          transmitByte = 0xFF;
          sound = PlaySound::NoSound;
          break;
        default:
          nextByte = 0x00;
          transmitByte = 0x00;
          break;
      }

      // Reset all flags that may have been set.
      transmitAddress = 0x00;

      break;

    // Handle the first byte transmitted.
    case MessageState::Transmit1:
      // The first byte has just been received, so the next byte
      // transmitted should be transmit byte 1.
      nextByte = transmitByte;

      // Store the receive byte 1.
      receiveByte = data;
      break;

    // Handle the second byte transmitted.
    case MessageState::Transmit2:

      // Apply the message that has just been transmitted.
      gameState->setField(transmitOpCode, receiveByte | (data << 8));

      nextByte = transmitAddress;
      break;

    default:
      nextByte = transmitAddress;
      break;
  };

  // Update the next state of the FSM.
  switch (state) {
    case MessageState::Address:
      state = MessageState::Transmit1;
      break;
    case MessageState::Transmit1:
      state = MessageState::Transmit2;
      break;
    case MessageState::Transmit2:
      state = MessageState::Address;
      break;
    default:
      state = MessageState::Address;
      break;
  }

  // Return the next byte to be transmitted.
  return nextByte;
}

void SPIManager::setModuleType(ModuleType type) { moduleType = type; }

void SPIManager::playSound(PlaySound sound) {
  this->sound = sound;
  // Add PlaySound flag to extra information to be send to the host system.
  transmitAddress |= 1 << uint8_t(ExtraInformation::PlaySound);

  // If the message is not curretly transmitting set the tranmit address.
  if (state == MessageState::Address && !(SPSR & (1 << SPIF))) {
    SPDR = transmitAddress;
  }
}

void SPIManager::strikeModule() {
  transmitAddress |= 1 << uint8_t(ExtraInformation::Strike);

  // If the message is not curretly transmitting set the tranmit address.
  if (state == MessageState::Address && !(SPSR & (1 << SPIF))) {
    SPDR = transmitAddress;
  }
}

void SPIManager::disarmModule() {
  transmitAddress |= 1 << uint8_t(ExtraInformation::Disarm);

  // If the message is not curretly transmitting set the tranmit address.
  if (state == MessageState::Address && !(SPSR & (1 << SPIF))) {
    SPDR = transmitAddress;
  }
}

void SPIManager::startGame() {
  transmitAddress |= 1 << uint8_t(ExtraInformation::StartGame);

  // If the message is not curretly transmitting set the tranmit address.
  if (state == MessageState::Address && !(SPSR & (1 << SPIF))) {
    SPDR = transmitAddress;
  }
}

void SPIManager::stopGame() {
  transmitAddress |= 1 << uint8_t(ExtraInformation::StopGame);

  // If the message is not curretly transmitting set the tranmit address.
  if (state == MessageState::Address && !(SPSR & (1 << SPIF))) {
    SPDR = transmitAddress;
  }
}

ISR(SPI_STC_vect) {
  if (spi_manager != nullptr) {
    SPDR = spi_manager->dataISR(SPDR);
  } else {
    SPDR = 0xFF;
  }
}

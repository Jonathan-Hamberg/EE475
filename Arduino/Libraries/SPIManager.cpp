#include <SPIManager.h>
#include <Arduino.h>

SPIManager *spi_manager = nullptr;

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
    if(currentTime - lastByteTransaction > 200) {
        state = MessageState::Address;
    }
    lastByteTransaction = currentTime;

    // Determine what byte to send next.
    switch(state) {
        case MessageState::Address:
        // Calculate the receive and transmit op codes.
            transmitOpCode = OpCode(data & 0x0F);
            receiveOpCode = OpCode(data >> 4);

            switch(receiveOpCode) {
                case OpCode::ModuleType:
                    nextByte = uint8_t(moduleType);
                    break;
                case OpCode::PlaySound:
                    nextByte = uint8_t(sound);
                    break;
                default:
                    nextByte = 0x00;
                    break;
            }


            switch(transmitOpCode) {
                case OpCode::Acknowledge:
                    // Acknoledge that the strike has been received by the
                    // host system.
                    transmitAddress &=
                        ~((1 << uint8_t(ExtraInformation::Strike)) |
                          (1 << uint8_t(ExtraInformation::Disarm)));


                    nextByte = 0x01;
                    transmitByte = 0x00;
                    break;
                case OpCode::PlaySound:
                    // Acknoledge that the play sound has been received by the
                    // host system.
                    transmitAddress &=
                        ~(1 << uint8_t(ExtraInformation::PlaySound));

                    nextByte = uint8_t(sound);
                    transmitByte = 0x00;
                    sound = PlaySound::NoSound;

                default:
                    break;
            }

            break;

        case MessageState::Transmit1:
            // The first byte has just been received, so the next byte
            // transmitted should be transmit byte 1.
            nextByte = transmitByte;

            // Store the receive byte 1.
            receiveByte = data;
            break;

        case MessageState::Transmit2:

            // Apply the message that has just been transmitted.
            gameState->setField(transmitOpCode, receiveByte | (data << 8));

            nextByte = transmitAddress;
            break;

        default:
            nextByte = transmitByte;
            break;
    };


    // Update the next state of the FSM.
    switch(state) {
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

    return nextByte;
}

void SPIManager::setModuleType(ModuleType type) {
    moduleType = type;
}

void SPIManager::playSound(PlaySound sound) {
    this->sound = sound;
    // Add PlaySound flag to extra information to be send to the host system.
    transmitAddress |= 1 << uint8_t(ExtraInformation::PlaySound);

    // If the message is not curretly transmitting set the tranmit address.
    if(state == MessageState::Address && !(SPSR & (1<<SPIF))) {
        SPDR = transmitAddress;
    }
}

void SPIManager::strikeModule() {
    transmitAddress |= 1 << uint8_t(ExtraInformation::Strike);

    // If the message is not curretly transmitting set the tranmit address.
    if(state == MessageState::Address && !(SPSR & (1<<SPIF))) {
        SPDR = transmitAddress;
    }
}

void SPIManager::disarmModule() {
    transmitAddress |= 1 << uint8_t(ExtraInformation::Disarm);

    // If the message is not curretly transmitting set the tranmit address.
    if(state == MessageState::Address && !(SPSR & (1<<SPIF))) {
        SPDR = transmitAddress;
    }
}

ISR (SPI_STC_vect)
{
    if(spi_manager != nullptr) {
        SPDR = spi_manager->dataISR(SPDR);
    } else {
        SPDR = 0xFF;
    }
}


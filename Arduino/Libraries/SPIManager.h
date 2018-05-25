#ifndef SPI_MANAGER_H
#define SPI_MANAGER_H

#include <Definitions.h>
#include <GameState.h>

enum class MessageState : uint8_t {
    Address = 0,
    Transmit1,
    Transmit2,
};

/**
 *
 */
class SPIManager final {
public:

    SPIManager() = delete;

    /**
     *
     */
    SPIManager(GameState* gameState);

    /**
     *
     *
     * @param data
     *
     * @return
     */
    uint8_t dataISR(uint8_t data);

    void setModuleType(ModuleType type);

    void playSound(PlaySound sound);

    void strikeModule();

    void disarmModule();

    void startGame();

    void stopGame();
private:
    
    ///
    volatile MessageState state = MessageState::Address;

    ///
    GameState* gameState;

    ///
    uint8_t transmitByte;

    ///
    uint8_t receiveByte;

    ///
    OpCode transmitOpCode = OpCode::None;

    ///
    OpCode receiveOpCode = OpCode::None;

    /// T
    volatile uint8_t transmitAddress = 0;

    uint32_t lastByteTransaction;

    ModuleType moduleType;

    PlaySound sound;
};

#endif // SPI_MANAGER_H

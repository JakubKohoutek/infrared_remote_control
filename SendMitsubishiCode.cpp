#include <IRremote.h> // library by shirriff, https://github.com/z3t0/Arduino-IRremote

#define MITSUBISHI_HDR_MARK   8000U
#define MITSUBISHI_HDR_SPACE  4000U
#define MITSUBISHI_BIT_MARK   526U
#define MITSUBISHI_ZERO_SPACE 474U
#define MITSUBISHI_ONE_SPACE  1474U

#define SETTINGS_MENU    0x20DFC23D
#define OK               0x20DF22DD
#define DOWN             0x20DF827D
#define UP               0x20DF02FD
#define BACK             0x20DFE01F
#define GAP              200

IRsend irsend;

void dimMonitor() {
  irsend.sendNEC(SETTINGS_MENU, 32);
  delay(GAP);
  irsend.sendNEC(OK, 32);
  delay(GAP);
  irsend.sendNEC(OK, 32);
  delay(GAP);
  irsend.sendNEC(DOWN, 32);
  delay(GAP);
  irsend.sendNEC(DOWN, 32);
  delay(GAP);
  irsend.sendNEC(OK, 32);
  delay(GAP);
  irsend.sendNEC(BACK, 32);
  delay(GAP);
  irsend.sendNEC(BACK, 32);
}

void brightenMonitor() {
  irsend.sendNEC(SETTINGS_MENU, 32);
  delay(GAP);
  irsend.sendNEC(OK, 32);
  delay(GAP);
  irsend.sendNEC(OK, 32);
  delay(GAP);
  irsend.sendNEC(UP, 32);
  delay(GAP);
  irsend.sendNEC(UP, 32);
  delay(GAP);
  irsend.sendNEC(OK, 32);
  delay(GAP);
  irsend.sendNEC(BACK, 32);
  delay(GAP);
  irsend.sendNEC(BACK, 32);
}

void sendMitsubishiCode (unsigned long data, int nbits, int repeat){
  // Set IR carrier frequency
  irsend.enableIROut(38); // kHz

  while(repeat--){
    uint8_t address = data >> (nbits / 2);
    uint8_t command = data & ((1 << (nbits / 2)) - 1);
    unsigned int dataLength = 0;

    // Header
    irsend.mark(MITSUBISHI_HDR_MARK);
    irsend.space(MITSUBISHI_HDR_SPACE);

    // ------- first part of data: address ------- //
    for (uint64_t mask = 1ULL << 7; mask; mask >>= 1)
      if (address & mask) {
        // Send 1
        irsend.mark(MITSUBISHI_BIT_MARK);
        irsend.space(MITSUBISHI_ONE_SPACE);
        dataLength +=2;
      } else {
        // Send 0
        irsend.mark(MITSUBISHI_BIT_MARK);
        irsend.space(MITSUBISHI_ZERO_SPACE);
        dataLength +=1;
      }

    // Gap between the address and the command
    irsend.mark(MITSUBISHI_BIT_MARK);
    irsend.space(MITSUBISHI_HDR_SPACE);

    // ------- second part of data: command ------- //
    for (uint64_t mask = 1ULL << 7; mask; mask >>= 1)
      if (command & mask) {
        // Send 1
        irsend.mark(MITSUBISHI_BIT_MARK);
        irsend.space(MITSUBISHI_ONE_SPACE);
        dataLength +=2;
      } else {
        // Send 0
        irsend.mark(MITSUBISHI_BIT_MARK);
        irsend.space(MITSUBISHI_ZERO_SPACE);
        dataLength +=1;
      }

    // Footer
    irsend.mark(MITSUBISHI_BIT_MARK);
    unsigned int gap = 60000 - (dataLength * 1000) - MITSUBISHI_HDR_MARK - MITSUBISHI_HDR_SPACE * 2 - MITSUBISHI_BIT_MARK * 2;
    irsend.space(gap);
  }
};

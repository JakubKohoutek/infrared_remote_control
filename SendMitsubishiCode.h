#define MITSUBISHI_DEFAULT_BITS   16
#define MITSUBISHI_DEFAULT_REPEAT 2

/**
 * My device require IR Mitsubishi protocol (https://sbprojects.net/knowledge/ir/xsat.php) 
 * that isn't currently supported by IRremote library. Therefore I had to implement this
 * custom function to send Mitsubishi encoded codes.
 */
void sendMitsubishiCode (
  unsigned long data,
  int           nbits  = MITSUBISHI_DEFAULT_BITS,
  int           repeat = MITSUBISHI_DEFAULT_REPEAT
);

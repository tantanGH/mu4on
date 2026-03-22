#include <stdio.h>
#include <stdint.h>
#include <doslib.h>
#include <iocslib.h>

#define MERCURY_BOARD_CHECK_ADDR1  (0xECC000)
#define MERCURY_BOARD_CHECK_ADDR2  (0xECC080)

#define MERCURY_DATA_PORT          (0xECC080)
#define MERCURY_V4_PORT            (0xECC090)
#define MERCURY_COMMAND_PORT       (0xECC091)
#define MERCURY_STATUS_PORT        (0xECC0A1)

//
//  mercury version check
//
static int16_t mercury_get_unit_version() {

  int16_t rc = 0;   // not exists

  // exists?
  if (BUS_ERR((uint8_t*)MERCURY_BOARD_CHECK_ADDR2, (uint8_t*)MERCURY_BOARD_CHECK_ADDR2, 2) == 0) {

    // 3.1 or 3.5/4.0
    uint8_t status = B_BPEEK((uint8_t*)MERCURY_STATUS_PORT);
    B_BPOKE((uint8_t*)MERCURY_COMMAND_PORT, status & 0b01111111);
    uint8_t status2 = B_BPEEK((uint8_t*)MERCURY_STATUS_PORT);
    rc = status2 & 0x80 ? 31 : 35;

    // 3.5 or 4.0
    if (rc == 35) {
      if (BUS_ERR((uint8_t*)MERCURY_BOARD_CHECK_ADDR1, (uint8_t*)MERCURY_BOARD_CHECK_ADDR1, 2) != 0) {
        rc = 40;
      }
    }
  }

  return rc;    // 0:none 31:~V3.1 35:V3.5 40:V4.0
}

//
//  enable mercury unit v4 
//
static void mercury_v4_enable() {
  int32_t usp = B_SUPER(0);
  asm volatile (
    "ori.b #0x03, %0"
    :
    : "m" (*(volatile uint8_t*)MERCURY_V4_PORT)
    : "memory"
  );
  if (usp > 0) {
    B_SUPER(usp);
  }
}

//
//  main
//
int32_t main(int32_t argc, uint8_t* argv[]) {

    int32_t rc = -1;

    int16_t mu_version = mercury_get_unit_version();
    if (mu_version == 0) {
        printf("Mercury-UNIT is not installed.\n");
        goto exit;
    } else if (mu_version < 40) {
        printf("Mercury-UNIT version is not 4.0.\n");
        goto exit;
    }

    mercury_v4_enable();
    printf("Enabled Mercury-UNIT version 4.0.\n");

    rc = 0;

exit:
    return rc;
}
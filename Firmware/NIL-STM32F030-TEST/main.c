#include "hal.h"
#include "ch.h"

#define LED_LINE    PAL_LINE(GPIOA, 2U)
#define PWR_LINE    PAL_LINE(GPIOA, 5U)
#define BTN_LINE    PAL_LINE(GPIOA, 1U)


bool status = true;

THD_WORKING_AREA(waLedBlinker, 128);
THD_FUNCTION(thdLedBlinker, arg) {

  (void)arg;

  palSetLineMode( LED_LINE, PAL_MODE_OUTPUT_PUSHPULL );

  while (true) {
    if( status ) {
      palSetLine( LED_LINE );
      chThdSleepMilliseconds(500);
      palClearLine( LED_LINE );
      chThdSleepMilliseconds(500);
    } else {
      palSetLine( LED_LINE );
      chThdSleepMilliseconds(200);
      palClearLine( LED_LINE );
      chThdSleepMilliseconds(200);
    }
  }
}

THD_WORKING_AREA(waPowerManagement, 256);
THD_FUNCTION(thdPowerManagement, arg) {

  (void)arg;

  palSetLineMode( PWR_LINE, PAL_MODE_OUTPUT_PUSHPULL );
  palSetLineMode( BTN_LINE, PAL_MODE_INPUT );


  chThdSleepMilliseconds(1000);
  palSetLine( PWR_LINE );

  while (true) {
    if( palReadLine( BTN_LINE ) == 0 ) {
      status = false;
      palClearLine( PWR_LINE );
      chThdSleepMilliseconds(5000);
      status = true;
      palSetLine( PWR_LINE );
    }
    chThdSleepMilliseconds(500);
  }
}


THD_TABLE_BEGIN
  THD_TABLE_THREAD(1, "LedBlinker",     waLedBlinker,       thdLedBlinker,      NULL)
  THD_TABLE_THREAD(0, "PowerManagement",waPowerManagement,  thdPowerManagement,      NULL)
THD_TABLE_END

int main(void) {

  halInit();
  chSysInit();

  while (true) {
  }
}

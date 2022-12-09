#include <avr/interrupt.h>
#include <avr/io.h>

#include <rotaryencoder/debounced_encoder.h>

static encoder_state es;
static volatile uint8_t encoder_value;

ISR(PORTA_PORT_vect)
{
  if (PORTA.INTFLAGS & (PIN1_bm | PIN2_bm))
  {
    PORTA.INTFLAGS = PIN1_bm | PIN2_bm; // Clear interrupt flags
    // Update encoder state
    switch (encoder_debounced_full_step_update(&es, (PORTA.IN >> 1) & 0x3))
    {
    case ENCODER_ACTION_TURN_CW:
      encoder_value++;
      break;
    case ENCODER_ACTION_TURN_CCW:
      encoder_value--;
      break;
    default:
      break;
    }
  }
}

int main(void)
{
  // Configure edge-triggered interrupts for both terminal inputs
  PORTA.PIN1CTRL = PORT_ISC_BOTHEDGES_gc; // Encoder Terminal A
  PORTA.PIN2CTRL = PORT_ISC_BOTHEDGES_gc; // Encoder Terminal B

  // Initialize encoder state
  encoder_debounced_full_step_init(&es, (PORTA.IN >> 1) & 0x3);

  sei(); // Enable interrupts

  uint8_t last_encoder_value = 0;

  for (;;)
  {
    if (encoder_value != last_encoder_value)
    {
      last_encoder_value = encoder_value;
      // Do something with last_encoder_value
    }
    else
    {
      SLPCTRL.CTRLA = SLPCTRL_SMODE_IDLE_gc | SLPCTRL_SEN_bm;
      __builtin_avr_sleep();
    }
  }

  return 0;
}

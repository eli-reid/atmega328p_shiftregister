// code for ATMEGA328P MCU

//edit these values for control of outputs
#define DELAY 50
#define DELAY_US true
#define TEST_CIR false	//----- sets a delays so you can see the shift in the shift registers
#define TEST_DELAY 0
#define NUMBER_SHIFT_REG 8
#define GROUPED false	//----- SET TRUE FOR GROUP	EX: 01110000000000 .....(3 BIT GROUPED)
#define SPACED true		//----- SET FOR SPACE		EX: 10010010000000......(3 BITS, 2 SPACES)
#define NUMBER_BITS 2	//----- SET NUMBER OF BITS IN GROUPED OR SPACED
#define NUMBER_SPACES 3	//----- SET NUMBER OF SPACES FOR SPACED
#define F_CPU 100000UL	//----- MATCH SYSTEM CLOCK SPEED FOR DELAY TO WORK RIGHT
#define CLK_DIV  clock_div_1

//########################################################################    DON'T EDIT BELOW THIS LINE  #############################################################################//

//DEFINE PIN BIT VALUES
#define SER_PIN	_BV(PINB3)
#define SER_DDR	_BV(DDB3)
#define RCLK_PIN _BV(PINB2) 
#define RCLK_DDR _BV(DDB2)
#define SRCLK_PIN _BV(PINB5)
#define SRCLK_DDR _BV(DDB5)
#define OE_PIN _BV(PINB1)
#define OE_DDR _BV(DDB1)
#define PORT_VALUE PORTB
#define DDR_VALUE DDRB
//DEFINE PWM 
#define PWM_PIN 

//INCLUDES
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

int main(void)
{
	
	//clock_prescale_set(CLK_DIV);
	int regspaces;//DECLARE VARS
	
	
	//INITAILIZE OUTPUT PINS
	DDR_VALUE = RCLK_DDR | SRCLK_DDR | SER_DDR | OE_DDR; // set pin to output
	PORT_VALUE |= OE_PIN | SRCLK_PIN; // SET PINS HIGH
	//INITALIZE SHIFT REGISTERS
	for (int i = 0; i < (NUMBER_SHIFT_REG*8) + 1 ; i++ ) //LOAD SHIFT REGISTERS 00000000
	{
		PORT_VALUE ^= SRCLK_PIN | RCLK_PIN;//TOGGLE
		PORT_VALUE ^= SRCLK_PIN | RCLK_PIN;//TOGGLE
	}
	PORT_VALUE &= ~OE_PIN; //ENABLE OUTPUT
	
	while (1)
	{
		if (GROUPED)
		{
			PORT_VALUE |= SER_PIN; // SET PIN
			for ( int i = 0; i < NUMBER_BITS; i++)
			{
				PORT_VALUE ^= SRCLK_PIN | RCLK_PIN ;//first half of cycle
				if (DELAY_US){TEST_CIR ? _delay_us(TEST_DELAY):_delay_us(DELAY);}else{TEST_CIR ? _delay_ms(TEST_DELAY):_delay_ms(DELAY);}
				PORT_VALUE ^= SRCLK_PIN | RCLK_PIN; // TOGGLE PINS second half of cycle
				if (DELAY_US){TEST_CIR ? _delay_us(TEST_DELAY):_delay_us(DELAY);}else{TEST_CIR ? _delay_ms(TEST_DELAY):_delay_ms(DELAY);}
			}
			PORT_VALUE &= ~SER_PIN; //CLEAR PIN
			regspaces=(NUMBER_BITS);	
		}
		
		else if (SPACED)
		{
			for (int i = 0; i < NUMBER_BITS ; i++)
			{
				PORT_VALUE ^= SER_PIN; // SET PIN
				PORT_VALUE ^= SRCLK_PIN | RCLK_PIN; //TOGGLE PINS
				if (DELAY_US){TEST_CIR ? _delay_us(TEST_DELAY):_delay_us(DELAY);}else{TEST_CIR ? _delay_ms(TEST_DELAY):_delay_ms(DELAY);}//DEALAY
				PORT_VALUE ^= SRCLK_PIN | RCLK_PIN; //TOGGLE PINS
				PORT_VALUE ^= SER_PIN; //CLEAR PIN
				for (int j = 0; j < NUMBER_SPACES; j++)
				{
					PORT_VALUE ^= SRCLK_PIN | RCLK_PIN; //TOGGLE PINS
					if (DELAY_US){TEST_CIR ? _delay_us(TEST_DELAY):_delay_us(DELAY);}else{TEST_CIR ? _delay_ms(TEST_DELAY):_delay_ms(DELAY);}//DEALAY
					PORT_VALUE ^= SRCLK_PIN | RCLK_PIN; //TOGGLE PINS
					if (DELAY_US){TEST_CIR ? _delay_us(TEST_DELAY):_delay_us(DELAY);}else{TEST_CIR ? _delay_ms(TEST_DELAY):_delay_ms(DELAY);}//DEALAY
				}
			}
			regspaces=(NUMBER_BITS + NUMBER_SPACES);////CALCULATE CHANNELS USED
		}
		else
		{
			PORT_VALUE ^= SER_PIN | SRCLK_PIN | RCLK_PIN; //TOGGLE PINS
			PORT_VALUE ^= SER_PIN | SRCLK_PIN | RCLK_PIN; //TOGGLE PINS
			regspaces = 0;
		}//END IF STATEMENT
		
		int pushbits =(( NUMBER_SHIFT_REG * 8) - regspaces);//CALCULATE THE REMAINDER OF CHANNELS  
		
		for (int i = 0; i < pushbits; i++ ) //MOVE BITS DOWN THE SHIFT REGISTERS
		{
			PORT_VALUE ^= SRCLK_PIN | RCLK_PIN;//TOGGLE PIN
			if (DELAY_US){TEST_CIR ? _delay_us(TEST_DELAY):_delay_us(DELAY);}else{TEST_CIR ? _delay_ms(TEST_DELAY):_delay_ms(DELAY);}//DELAY
			PORT_VALUE ^= SRCLK_PIN | RCLK_PIN;//TOGGLE PIN
			if (DELAY_US){TEST_CIR ? _delay_us(TEST_DELAY):_delay_us(DELAY);}else{TEST_CIR ? _delay_ms(TEST_DELAY):_delay_ms(DELAY);}//DELAY
		}//END FOR LOOP
	}//END WHILE LOOP
}//END MAIN()
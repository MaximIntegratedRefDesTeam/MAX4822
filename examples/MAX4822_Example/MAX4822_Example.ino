#include <MAX4822.h>

MAX4822 rly_drvr;

const int CS_2 = 8;
const int SET = 7;
const int RESET = 6;

void setup() 
{
  Serial.begin(9600);
  while(!Serial);

  pinMode(CS_2, OUTPUT);
  digitalWrite(CS_2, HIGH);

  //Config MAX4822
  //defaults to 11, 13, 10 for MOSI, SCLK, CS
  rly_drvr.begin();

  //Config SET pin
  pinMode(SET, OUTPUT);
  digitalWrite(SET, HIGH);

  //Config RESET pin
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);
}

void loop() 
{
  uint8_t user_entry = 0;
  uint8_t relay_ch;
  
  while(user_entry != 5)
  {
    Serial.println();
    Serial.println(F("Please select an option below"));
    Serial.println(F("1. Set Relay"));
    Serial.println(F("2. Reset Relay"));
    Serial.println(F("3. Set All Relays"));
    Serial.println(F("4. Reset All Relays"));
    Serial.println(F("5. Quit"));

    user_entry = get_user_int(NULL, 5);

    if((user_entry == 1) || (user_entry == 2))
    {
      do
      {
        relay_ch = get_user_int("Wich relay? (1 - 8): ", 8);
        if(relay_ch == 0)
        {
          Serial.println(F("Please select a ch > 0"));
        }
      }
      while(relay_ch == 0);

      Serial.print("You selected relay channel: ");
      Serial.println(static_cast<MAX4822::RelayChannel>(relay_ch), DEC);
    }

    switch(user_entry)
    {
      case 1:
          rly_drvr.set_relay(static_cast<MAX4822::RelayChannel>(relay_ch));
      break;

      case 2:
          rly_drvr.reset_relay(static_cast<MAX4822::RelayChannel>(relay_ch));
      break;

      case 3:
          rly_drvr.set_all_relays(SET);
      break;

      case 4:
          rly_drvr.reset_all_relays(RESET);
      break;

      case 5:
          Serial.println(F("Quitting example"));
      break;

      default:
        arduino_die();
      break;
    }
  }

  arduino_die();
}

//*********************************************************************
uint8_t get_user_int(char *msg, uint8_t max_val)
{
  uint8_t user_int;
  uint8_t bytes_available;
  uint8_t input_buffer[64];

  do
  {
    Serial.println(msg);
    do
    {
      bytes_available = Serial.readBytesUntil(0x0A, input_buffer, 64);
    }
    while(!bytes_available);

    if(bytes_available > 4)
    {
      Serial.println(F("Invalid Entry"));
      user_int = 0xFF;
    }
    else if(bytes_available > 3)
    {
      user_int = (100*(input_buffer[0] - '0')); //get 100s place
      user_int += (10*(input_buffer[1] - '0')); //get 10s place
      user_int += (input_buffer[2] - '0'); //get 1s place
    }
    else if(bytes_available > 2)
    {
      user_int = (10*(input_buffer[0] - '0'));
      user_int += (input_buffer[1] - '0');
    }
    else
    {
      user_int = (input_buffer[0] - '0');
    }

    Serial.print(F("You entered "));
    Serial.println(user_int, DEC);
    if(user_int > max_val)
    {
      Serial.println(F("Value out of range"));
    }
    
  }
  while(user_int > max_val);

  return(user_int);
}

//*********************************************************************
void arduino_die(void)
{
  pinMode(13, OUTPUT);

  while(1)
  {
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13, LOW);
    delay(250);
  }
}


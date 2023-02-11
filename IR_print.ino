#include <IRremote.h> //Lib for IR remote

// Connect the IR receiver IO pin to arduino pin 5
int RECV_PIN = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  /*
  Set up the serial connection and IR receiver
  */
  Serial.begin(9600);
  Serial.println("Setup starting");
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    //If we sense an IR key pressed, report the value to the console
    Serial.print("Key pressed:");
    Serial.println(results.value);
    delay(500); //Debounce for next value so no spam
    irrecv.resume(); // Receive the next value
  }
}

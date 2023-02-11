#include <IRremote.h> //Lib for IR remote
#include "SD.h" //Lib to read SD card
#include "SPI.h" //SPI lib for SD card
#include "TMRpcm.h" //Lib to play audio

#define SD_ChipSelectPin 4 //Chip select is pin number 4
//SD card pinout:
//local pin - arduino pin
//gnd - gnd
//miso - pin 12
//sck - pin 13
//mosi - pin11
//cs - pin4
//+5v - +5v

//music setup
TMRpcm music; //Lib object is named "music"
#define SPEAKER_PIN 9

//IR receiver
#define RECV_PIN 5
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
/*
  Set up the serial, sd card, music,
  and pinouts
*/
{
  Serial.begin(9600);
  Serial.println("Setup starting");
  irrecv.enableIRIn(); // Start the receiver
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }
  music.quality(9);
  music.speakerPin = SPEAKER_PIN;
  music.setVolume(5);
  Serial.println("Setup finished");
}

//variable to keep track if we want music to play or not
bool isPlaying = false;

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    // PUT YOUR REMOTE VALUES BELOW
    // IN HEX OR DECIMAL VALUE:
    if(results.value == 0x807F48B7 ||
        results.value == 2704){
      // Remote was pressed
      if(!isPlaying){
        // We were not playing music, so we toggle on
        Serial.println("Playing now");
        isPlaying=true;
        music.play("fire.wav");
      }
      else{
        // We were playing, so we toggle off
        Serial.println("Stopping now");
        isPlaying=false;
        //stop playback then write 0 because if we dont then it makes 
        //a weird eeeeeeeeEEeeeeeeeeErreeeeeEEeEeee noise
        music.stopPlayback();
        digitalWrite(SPEAKER_PIN,0); //force stop
      }
    }
    delay(500); //'debouce' delay on next receive so we dont spam
    irrecv.resume(); //receive the next value
  }
  else{
    //We have not received a value from the ir remote
    if(isPlaying && !music.isPlaying()){
      //In the case that we want to play music, but the song reached its end,
      //restart the song from the beginning
      Serial.println("Restarting from beginning");
      music.play("fire.wav");
    }
  }
}

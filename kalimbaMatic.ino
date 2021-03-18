#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define TIME_DELAY 400 // How long to wait between each note
#define NUM_SERVOS 14  // The total number of servos in the configuration

//Each servo has its own servo objest which contains the distance it needs to move to play, its current state, and the note it plays
class servoPlayer {
  private:
    int minPos;  //Where the servo is positioned on the up stroke
    int maxPos;  //Where the servo is positioned on the down stroke
  public:
    int state;   //0 for up 1 for down
    int note;    //MIDI note number
    int pin;     //The pin the servo is tied to

    servoPlayer(int s, int n, int p, int m, int x){
        state = s;
        note = n;
        pin = p;
        minPos = m;
        maxPos = x;
    }

    //Play down if we are up and play up if we are down
    void play(){
        if (state == 0){
            for (uint16_t pulselen = minPos; pulselen < maxPos; pulselen++) {
               pwm.setPWM(pin, 0, maxPos);
             }
            this->state = 1;
        } else if (state == 1){
            for (uint16_t pulselen = minPos; pulselen < maxPos; pulselen++) {
               pwm.setPWM(pin, 0, minPos);
            }
            this->state = 0;
        }
    }

    //Pulls a servo to its highest point, used at the beginning to home servos
    void pullUp(){
      for (uint16_t pulselen = minPos; pulselen < maxPos; pulselen++) {
               pwm.setPWM(pin, 0, minPos);
      }
    }
};


//Manually initialize every servo player object
servoPlayer players[14] = {
      players[0] = servoPlayer(0, 48, 1, 400, 500),
      players[1] = servoPlayer(0, 50, 2, 400, 500),
      players[2] = servoPlayer(0, 52, 3, 400, 500),
      players[3] = servoPlayer(0, 53, 4, 400, 500),
      players[4] = servoPlayer(0, 55, 5, 400, 500),
      players[5] = servoPlayer(0, 57, 6, 400, 500),
      players[6] = servoPlayer(0, 59, 7, 400, 500),
      players[7] = servoPlayer(0, 60, 8, 400, 500),
      players[8] = servoPlayer(0, 62, 9, 400, 500),
      players[9] = servoPlayer(0, 64, 10, 400, 500),
      players[10] = servoPlayer(0, 65, 11, 400, 500),
      players[11] = servoPlayer(0, 67, 12, 400, 500),
      players[12] = servoPlayer(0, 69, 13, 400, 500),
      players[13] = servoPlayer(0, 71, 14, 400, 500)
};

//A song is comprised of a list of MIDI notes, maybe add a delay list in the future
int song[] = {50,50,62,55,55};
int songLen = sizeof(song)/sizeof(int);

void setup() {
  Serial.begin(9600);
  pwm.begin();

  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  //Home servos before playing
  pullServosUp();
  
  delay(10);
}


//Wrapper function to call pullUp on each individual servo, homes all servos
void pullServosUp(){
    for (int i = 0; i < NUM_SERVOS; i++){
        players[i].pullUp();
        delay(25);
    }
}


//Takes a note and finds the servo player corresponding to the requested note
//This function essentially := map(String -> servoPlayer)
int getPlayerIndexForNote(int note){
    for (int i = 0; i < NUM_SERVOS; i++){
        if (note == players[i].note){
            return i;
        }
    }
}


void loop() {

  //Go through every note in the song
  for (int i = 0; i < songLen; i++){

    //Extract a single note from the song array
    int note = song[i];

    //Get the index of the player object, we get the index because we want to change
    //the state member within the method of the class itself
    int playerIdx = getPlayerIndexForNote(note);

    //Play the note and flip the state
    players[playerIdx].play();
    
    //Wait to play next note, this will be read from an array in the future
    delay(TIME_DELAY);

  }

  pullServosUp();
  

}

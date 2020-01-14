#include <SPI.h>
#include "HackBoy.h"
#include <Keypad.h>

// COLOUR CODE
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF

//CODE FOR NOTES
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// SPEAKER PIN
#define melodyPin 3

//Underworld melody
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};
//Underwolrd tempo
int underworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};


// Game level
uint8_t level = 0;
//player 1 paddle y
uint8_t paddle_1 = 63;
//player 2 paddle y
uint8_t paddle_2 = 63;
//
uint16_t touches = 0, Vball = 1;
//ballX and ballY
uint8_t ball[] = {63,63};
//{player1 movement, player2 movement, ball x movement, ball y movement}
int8_t movement[] = {0,0,-1,0};
//Points for the two players
uint8_t points[] = {0,0};

//TIP120 pin number (control vibrator)
int TIP120pin = 6; 


const byte ROWS = 2; 
const byte COLS = 2; 

char hexaKeys[ROWS][COLS] = {
  {'5', '6'},
  {'8', '9'}
};

byte rowPins[ROWS] = {2, 4}; 
byte colPins[COLS] = {5, 7}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void vibrating() {  //function for the vibrator

  pinMode(TIP120pin, OUTPUT); // Set pin for output to control TIP120 Base pin
  analogWrite(TIP120pin, 80); // By changing values from 0 to 255 you can control motor speed
  delay(50);
  analogWrite(TIP120pin, 0);
//  delay(100);
  
}

// function for returning (Up : Down)
uint8_t getMovement() {
  /* 
   
     LEFT_KEY_BIT  0b1000
     UP_KEY_BIT    0b0100
     RIGHT_KEY_BIT 0b0010
     DOWN_KEY_BIT  0b0001
    
    */
  char buttons = customKeypad.getKey();
  if (buttons) vibrating();
  if (buttons == '8') {
    //vibrating();
    return DOWN_KEY_BIT;
  }
  if (buttons == '6') {
    //vibrating();
    return UP_KEY_BIT;
  }
//  if (buttons == '6') return RIGHT_KEY_BIT;
//  if (buttons == '5') return DOWN_KEY_BIT;
    
}

//function for getting the speaker buzzing
void buzz(int targetPin, int frequency, int lengths) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * lengths / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
//  Serial.println("Buzz works");
}

//make the speaker buzz in different Herzs and in different rhythms
void sing(int song) {
  // iterate over the notes of the melody:
  if (song == 2) {
//    Serial.println(" 'Underworld Theme'");
    int Size = sizeof(underworld_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < Size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / underworld_tempo[thisNote];

      buzz(melodyPin, underworld_melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }

  }
}



void setup(void) {
  pinMode(12, OUTPUT);//buzzer
//  Serial.begin(9600);
  HackBoy.begin();  // built in function from Hackboy, do the animations and stuff
  showMenu();
}

// draw the field
void drawField(boolean clear) {
  if (clear)
    tft.fillScreen(BLACK);
  for (uint8_t i = 0; i < 8; i++) {
    tft.drawLine(63, i*16, 63, i*16+8, CYAN);  // The middle point
  }
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.setCursor(40, 10);
  tft.print( points[0] );
  tft.setCursor(70, 10);
  tft.print( points[1] );
}

//starting the game
//These are the initial values
void startGame(uint8_t mode) {
  points[0] = 0;
  points[1] = 0;
  level = mode;
  // The paddles and the ball are initially in the middle
  paddle_1 = 63;
  paddle_2 = 63;
  ball[0] = 63;
  ball[1] = 63;
  movement[0] = 0;
  movement[1] = 0;
  movement[2] = -1;
  movement[3] = 0;
  touches = 0;
  drawField(true);
}

//showing the menu
void showMenu() {
  tft.fillScreen(BLACK);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.setCursor(30, 10);
  tft.print("PONG");
  tft.setTextSize(1);
  tft.setCursor(0, 50);
  tft.println("  Easy");
  tft.println("");
  tft.println("  Medium");
  tft.println("");
  tft.println("  Hard");
  //what you selected
  uint8_t selected = 0;
  //old movement value
  uint8_t old_mov = 0;
  tft.fillRect(4, 43, 4, 20, WHITE);
  while (true) {
    uint8_t mov = getMovement();//return the movement 
    if ((mov & 5) != (old_mov & 5)) {   // only could be up and down
      tft.fillRect(4, 43+selected*16, 4, 20, BLACK);//return to be black
      selected = (selected + ((mov&1)-((mov>>2)&1)) + 4) & 3;  //selected could only be 0,1,2
      tft.fillRect(4, 43+selected*16, 4, 20, WHITE);
    }
    old_mov = mov;
    char customKey = customKeypad.getKey(); 
    if (customKey == '5') {                   // Press select key to start
      vibrating();
      return startGame(selected);
    }
  }
  
  
}

int16_t estimated_pos = 0; //estimated y of the ball that is going to hit player2
int8_t hit_diff = 0;
void loop() {
    movement[1] = 0;
    tft.drawPixel(116, estimated_pos, BLACK);
    if (movement[2] > 0) {                        //If the ball is heading towards player2, player 2 starts to move
      if (level == 2) {                         // If the game is in Hard Mode, paddle2 will estimate where the ball goes
        estimated_pos = abs((ball[1] - 2) + (116 - ball[0]) * movement[3] / movement[2]);
        //estimated_pos may be larger than 124, which is the max y for the ball
        while (estimated_pos >= 124) estimated_pos = abs(246 - estimated_pos);
        estimated_pos += 2;
        estimated_pos += hit_diff;
        // The max y velocity of paddle 2 will be plus or minus 4
        movement[1] = min(4, max(-4, estimated_pos - paddle_2));
        //if (paddle_2 - estimated_pos > 0) movement[1] = max(-4, estimated_pos - paddle_2);
        //else if (estimated_pos - paddle_2 > 0) movement[1] = min(4, estimated_pos - paddle_2);
        // paddle2 shouldd not move outside of the range
        if (paddle_2 + movement[1] >= 117) movement[1] = 117-paddle_2;
        if (paddle_2 + movement[1] <= 10) movement[1] = 10-paddle_2;
      } else {
        //If the mode of the game is EASY or MEDIUM, paddle 2 will only follow the ball and move occationally, with the max speed of 3
        if (random(4-level) < 2) {   
          if (paddle_2 - ball[1] > 2) movement[1] = -3;
          else if (ball[1] - paddle_2 > 2) movement[1] = 3;
          if (paddle_2 + movement[1] >= 117) movement[1] = 117-paddle_2;
          if (paddle_2 + movement[1] <= 10) movement[1] = 10-paddle_2;
        }
      }
    } else {
      //The ball is heaing towards player1, player 2 starts to move to the middle
      if (abs(paddle_2 - 63) > 2) movement[1] = (63 - paddle_2) > 0 ? 3 : -3;
    }

  
  tft.fillRect( (ball[0] - 2), ball[1]-2, 4, 4, RED);
  if (movement[0] != 0)
    tft.fillRect( 4 , paddle_1-10, 4, 20, WHITE);
  if (movement[1] != 0)
    tft.fillRect( 119 , paddle_2-10, 4, 20, GREEN);
  
  paddle_1+=movement[0];
  paddle_2+=movement[1];
  int8_t ball_y = ball[1] + movement[3]; 
  //ball collides with the side
  if (ball_y < 2) {
    ball[1] = 2 - ball_y;
    movement[3] *= -1; 
  } else if (ball_y > 125) {
    ball[1] = 250 - ball_y;
    movement[3] *= -1;
  } else {
    ball[1] += movement[3];
  }
  ball[0] += movement[2];
  
  tft.fillRect( (ball[0] - 2), ball[1]-2, 4, 4, RED);
  tft.fillRect( 4 , paddle_1-10, 4, 20, WHITE);
  tft.fillRect( 119 , paddle_2-10, 4, 20, GREEN);
  drawField(false);      //Show Score
  if (ball[1] <= 2 || ball[1] >= 125) {
    movement[3] *= -1;
  }
  //Check if there is collision between the ball and the paddles
  if (ball[0] <= 10 && ball[0] > 6 && movement[2] < 0) {
    //Paddle1 hits the ball
    if (paddle_1+10 >= ball[1] && paddle_1-10 <= ball[1]) {
      touches++;
      //The x speed of the ball increases every two hits
      //The x speed will never be greater than 110
      Vball = min(110, (touches >> 2) + 1);
      movement[2] = Vball;
      //move with an angle
      movement[3] = (ball[1] - paddle_1) / 3 * movement[2];
//      HackBoy.note(NOTE_A, 50);
      buzz(melodyPin,NOTE_A3,12);
    }
  } else if (ball[0] >= 116 && ball[0] < 121 && movement[2] > 0) {
    //Paddle2 hits the ball
    //Same as previous (paddle1)
    if (paddle_2+10 >= ball[1] && paddle_2-10 <= ball[1]) {
      touches++;
      Vball = min(110, (touches >> 2) + 1);
      movement[2] = - Vball;
      movement[3] = (ball[1] - paddle_2) / 3 * -movement[2];
      hit_diff = 7 - random(17);
//      HackBoy.note(NOTE_A, 50);
      buzz(melodyPin,NOTE_A3,12);
    }
   //The paddle didn't hit the ball
  } else if (ball[0] <= 6 || ball[0] >= 122) {
    tft.fillRect( (ball[0] - 2) , ball[1]-2, 4, 4, BLACK);
    tft.setTextSize(3);
    tft.setTextColor(BLACK);
    tft.fillRect(40,10,50,24,BLACK);
    
    points[ball[0] < 64]++;
    //if the ball is on the left, then player1 lose
    if (points[ball[0] < 64] == 10) {
      tft.fillScreen(BLACK);
      tft.setTextSize(3);
      tft.setTextColor(MAGENTA);
      tft.setCursor(40,40);
      tft.println("You");
      tft.setCursor(0,64);
//      HackBoy.note(110+330*(ball[0] < 64), 600);
      tft.print( (ball[0] < 64) == 0 ) ? "  won!" : "lost :(");
//      char customKey = customKeypad.getKey();
//      while(customKey != '1'){
//        char customKey = customKeypad.getKey(); 
//        }                                                              /////

      sing(2); //Play music
//      delay(3000);
      showMenu(); //return to menu
      return;
    } else {
//      HackBoy.note(110, 400);
        buzz(melodyPin,NOTE_C4,24);
    }
    //the ball starts moving from where it goes out of range
    movement[3] = 0; 
    movement[2] = 1 - ((ball[0] < 64) * 2);
    ball[0] = 63;
    ball[1] = min(125, max(2, ball[1]));
    touches = 0;
    //Update Score
    drawField(false);
  }
  //Avoid overlapping
  if ((ball[0] + movement[2] <= 7 && movement[2] < 0)) {
    movement[3] /= movement[2];
    movement[2] = min(-1, 8 - ball[0]);
    movement[3] *= movement[2];
  }
  if (ball[0] + movement[2] >= 121 && movement[2] > 0) {
    movement[3] /= movement[2];
    movement[2] = max(1, 120 - ball[0]);
    movement[3] *= movement[2];
  }
  delay(20);
  //get paddle1 moving
  uint8_t mvmnt = getMovement();    
  movement[0] = ((mvmnt&1)-((mvmnt>>2)&1)) * 3;
  if (paddle_1 + movement[0] >= 117) movement[0] = 117-paddle_1;
  if (paddle_1 + movement[0] <= 10) movement[0] = 10-paddle_1;

}


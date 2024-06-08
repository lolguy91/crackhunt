
// include the library code:
#include <LiquidCrystal.h>
#include "AntiDelay.h"

byte guyu[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b11111,
  0b11111,
  0b00100,
  0b01010,
  0b10001,
};

byte guyd[8] = {
  0b10001,
  0b01010,
  0b00100,
  0b11111,
  0b11111,
  0b10101,
  0b01110,
  0b00100,
};

byte guyl[8] = {
  0b00000,
  0b00010,
  0b00110,
  0b01110,
  0b01110,
  0b00110,
  0b00010,
  0b00000,
};

byte guyr[8] = {
  0b00000,
  0b01000,
  0b01100,
  0b01110,
  0b01110,
  0b01100,
  0b01000,
  0b00000,
};
byte crack[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00010,
  0b00110,
  0b11110,
  0b01111,
  0b11111,
};

byte wall[8] = {
  0b11111,
  0b10101,
  0b11111,
  0b10101,
  0b11111,
  0b10101,
  0b11111,
  0b10101,
};
byte bawx[8] = {
  0b01110,
  0b11111,
  0b10101,
  0b10001,
  0b11011,
  0b11111,
  0b11111,
  0b01110,
};
byte pp[8] = {
  0b00000,
  0b01110,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00000,
};

#define PIN_SPEAKER 6

#define KEY_UP     119
#define KEY_DOWN   115
#define KEY_LEFT   97
#define KEY_RIGHT  100
#define KEY_HELP   104
#define KEY_RETRY  114

#define SPRITE_UP       0
#define SPRITE_DOWN     1  
#define SPRITE_LEFT     2
#define SPRITE_RIGHT    3
#define SPRITE_CRACK    4
#define SPRITE_WALL     5
#define SPRITE_BOXX     6
#define SPRITE_PP       7
#define SPRITE_BADCRACK 'X'

byte PlayerSprite = SPRITE_UP;
byte PlayerX      = 0;
byte PlayerXDelta = 0;
byte PlayerYDelta = 0;
byte PlayerLastX  = 0;
byte PlayerLastY  = 0;

byte PlayerY      = 0;
byte CurrentLevel = 0;


// Music stuff
AntiDelay MuzikDelay(0);

struct Note{
  public:
  Note(unsigned int p, unsigned long i){
    pitch = p;
    interval = i;
  }
  unsigned int pitch; // in hertz
  unsigned long interval; // in milliseconds
};


Note track1[] = {
  Note(440, 500),   // A4
  Note(494, 500),   // B4
  Note(523, 500),   // C5
  Note(494, 500),   // B4
  Note(440, 500),   // A4
  Note(392, 500),   // G4
  Note(349, 500),   // F4
  Note(392, 500),   // G4
  Note(440, 500),   // A4
  Note(392, 500),   // G4
  Note(349, 500),   // F4
  Note(330, 500),   // E4
  Note(294, 500),   // D4
  Note(330, 500),   // E4
  Note(349, 500),   // F4
  Note(392, 500),   // G4
  Note(440, 1000),  // A4
};

Note track2[] = {
  Note(523, 400),   // C5
  Note(587, 400),   // D5
  Note(659, 400),   // E5
  Note(698, 400),   // F5
  Note(784, 400),   // G5
  Note(880, 400),   // A5
  Note(988, 400),   // B5
  Note(1047, 800),  // C6
  Note(988, 400),   // B5
  Note(880, 400),   // A5
  Note(784, 400),   // G5
  Note(698, 400),   // F5
  Note(659, 400),   // E5
  Note(587, 400),   // D5
  Note(523, 800),   // C5
};
Note track3[] = {
  Note(880, 200),   // A5
  Note(988, 200),   // B5
  Note(1047, 200),  // C6
  Note(1175, 200),  // D6
  Note(1047, 200),  // C6
  Note(988, 200),   // B5
  Note(880, 200),   // A5
  Note(784, 400),   // G5
  Note(880, 200),   // A5
  Note(988, 200),   // B5
  Note(1047, 200),  // C6
  Note(1175, 200),  // D6
  Note(1319, 400),  // E6
  Note(1175, 200),  // D6
  Note(1047, 200),  // C6
  Note(988, 200),   // B5
  Note(880, 200),   // A5
  Note(784, 200),   // G5
  Note(880, 400),   // A5
  Note(784, 400),   // G5
};

Note* musics[] = {
  track1,
  track2,
  track3
};

unsigned short lengths[] = { // length -1 cuz arrays
  16,
  14,
  19
};

byte CurrentSong         = 0;
unsigned int CurrentNote = 0;

bool isMusicInited = false;

byte levels[10][2][16] = {
  {
    {' ',' ',' ',' ',' ',5  ,' ',' ',' ',' ',' ',5  ,' ',' ',' ',' '},
    {' ',' ',' ',5  ,' ',' ',' ',' ',5  ,' ',' ',' ',' ',' ',' ',4  }
  },
  {
    {' ',' ',6  ,' ',' ',' ',' ',6  ,' ',' ',' ',6  ,' ',' ',' ',4  },
    {' ',' ',6  ,' ',' ',6  ,' ',' ',' ',6  ,' ',' ',' ',6  ,' ',5  }
  },
  {
    {' ',' ',6  ,' ',' ',5  ,' ',' ',' ',6  ,' ',' ',5  ,5  ,5  ,4  },
    {5  ,5  ,5  ,' ',' ',6  ,' ',' ',5  ,5  ,' ',' ',' ',' ',' ',4  }
  },
  {
    {' ',' ',' ',' ',' ',' ',' ',' ',7  ,' ',6  ,' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'}
  },
  {
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',4  }
  },
  {
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',4  }
  },
  {
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',4  }
  },
  {
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',4  }
  },
  {
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',4  }
  },
  {
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',4  }
  }
};
char* hints[10] = {
  "HINT: Moron...",
  "HINT: Push...",
  "HINT: Push...",
  "HINT: WIP",
  "HINT: WIP",
  "HINT: WIP",
  "HINT: WIP",
  "HINT: WIP",
  "HINT: WIP",
  "HINT: WIP",
};

byte CurLevel[2][16] = {0};


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void DrawLevel(){
  for(byte i = 0; i < 16; i++){
    for(byte j = 0; j < 2; j++){
      lcd.setCursor(i,j);
      lcd.write(CurLevel[j][i]);
    }
  }
}

void ReloadLevel(){
  for(byte i = 0; i < 16; i++){
    for(byte j = 0; j < 2; j++){
      CurLevel[j][i] = levels[CurrentLevel][j][i];
    }
  }
  DrawLevel();
}

void UpdatePlayerSprite(){
   lcd.setCursor(PlayerLastX,PlayerLastY);
   lcd.write(CurLevel[PlayerLastY][PlayerLastX]);
   PlayerLastX = PlayerX;
   PlayerLastY = PlayerY;
   lcd.setCursor(PlayerX,PlayerY);
   lcd.write(PlayerSprite);
}

void CheckCollisions(){
  if (PlayerX > 100){ // detect overflows with no false positves
    PlayerX = 0;
  }
  if (PlayerY > 100){ // detect overflows with no false positves
    PlayerY = 0;
  }
  if (PlayerX > 15){
    PlayerX = 15;
  }
  if (PlayerY > 1){
    PlayerY = 1;
  }
  if(CurLevel[PlayerY][PlayerX] == SPRITE_CRACK){
    PlayerX = 0;
    PlayerY = 0;
    CurrentLevel++;
    CurrentSong = CurrentLevel;
    ReloadLevel();
    return;
  } 
  if(CurLevel[PlayerY][PlayerX] == SPRITE_BOXX){
    CurLevel[PlayerY][PlayerX] = ' ';
    byte BoxX = PlayerX + PlayerXDelta;
    byte BoxY = PlayerY + PlayerYDelta;
    
    if (BoxX > 100){ // detect overflows with no false positves
      BoxX = 0;
      PlayerX = PlayerLastX;
      PlayerY = PlayerLastY;
    }
    if (BoxY > 100){ // detect overflows with no false positves
      BoxY = 0;
      PlayerX = PlayerLastX;
      PlayerY = PlayerLastY;
    }
    if (BoxX > 15){
      BoxX = 15;
      PlayerX = PlayerLastX;
      PlayerY = PlayerLastY;
    }
    if (BoxY > 1){
      BoxY = 1;
      PlayerX = PlayerLastX;
      PlayerY = PlayerLastY;
    }
    if(CurLevel[BoxY][BoxX] != ' ' && CurLevel[BoxY][BoxX] != SPRITE_PP){
      BoxX = PlayerX;
      BoxY = PlayerY;
      PlayerX = PlayerLastX;
      PlayerY = PlayerLastY;
    }
    if(CurLevel[BoxY][BoxX] == SPRITE_PP){
      for(byte i = 0; i < 16; i++){
        for(byte j = 0; j < 2; j++){
          if(CurLevel[j][i] == SPRITE_BADCRACK){
            CurLevel[j][i] = SPRITE_CRACK;
          }
        }
      }
    }
    CurLevel[BoxY][BoxX] = SPRITE_BOXX;
    DrawLevel();
    return;
  }
  if(CurLevel[PlayerY][PlayerX] != ' '){
    PlayerX = PlayerLastX;
    PlayerY = PlayerLastY;
  }
}

void setup() {
  Serial.begin(9600);
  lcd.createChar(0,guyu);
  lcd.createChar(1,guyd);
  lcd.createChar(2,guyl);
  lcd.createChar(3,guyr);
  lcd.createChar(4,crack);
  lcd.createChar(5,wall);
  lcd.createChar(6,bawx);
  lcd.createChar(7,pp);
  lcd.begin(16, 2);
  ReloadLevel();
  UpdatePlayerSprite();

  //init Music
  Note note = musics[CurrentSong][CurrentNote];
  tone(PIN_SPEAKER,note.pitch,note.interval);
  MuzikDelay.setInterval(note.interval);
}

void UpdateMuzik(){
  if(MuzikDelay){
    noTone(PIN_SPEAKER);
    if(CurrentNote++ >= lengths[CurrentSong]){
      CurrentNote = 0;
    }
    Note note = musics[CurrentSong][CurrentNote];
    tone(PIN_SPEAKER,note.pitch);
    MuzikDelay.setInterval(note.interval);
    MuzikDelay.reset();
  }
}

void loop() {
  UpdateMuzik();
  if (Serial.available() > 0) {
    byte ib = Serial.read();
    PlayerXDelta = 0;
    PlayerYDelta = 0;
    switch(ib){
      case KEY_UP:
        PlayerY--;
        PlayerYDelta = -1;
        PlayerSprite = SPRITE_UP;
        break;
      case KEY_DOWN:
        PlayerY++;
        PlayerYDelta = 1;
        PlayerSprite = SPRITE_DOWN;
        break;
      case KEY_LEFT:
        PlayerX--;
        PlayerXDelta = -1;
        PlayerSprite = SPRITE_LEFT;
        break;
      case KEY_RIGHT:
        PlayerX++;
        PlayerXDelta = 1;
        PlayerSprite = SPRITE_RIGHT;
        break;
      case KEY_HELP:
        Serial.println(hints[CurrentLevel]);
        break;
      case KEY_RETRY:
        PlayerX = 0;
        PlayerY = 0;
        ReloadLevel();
        break;
    }
    CheckCollisions();
    UpdatePlayerSprite();
  }
}

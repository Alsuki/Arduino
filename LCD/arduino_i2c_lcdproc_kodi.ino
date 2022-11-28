#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_COLS 16
#define LCD_ROWS 2
#define BAUDRATE 9600
int S = 0;

// Class to "animate" simple messages
class Porcessor {
private:
  struct {
    int rowSize;
    int colSize;
  } Geometry;

  String completeMessage(String msg){
    int a;
    for (a = 0; msg[a] != '\0'; a++);

    for (int b = 0; b < Geometry.colSize; b++){
      if (b < Geometry.colSize - a) {
        msg = msg + ' ';
      }
    }
        
    msg = msg + '\0';
    return msg;
    }

public:
  void setGeometry(int x, int y){
    Geometry.rowSize = y;
    Geometry.colSize = x;
  }

  String newMessage(String msg, char c, int repeat){
    if (repeat == 0 ) {
      return completeMessage(msg);
    } else {
      int a;
      for (a = 0; msg[a] != '\0'; a++);
      for(int b = 0; b < repeat; b++)
      {
        if (b < Geometry.colSize - a){
          msg = msg + c;
        }
      }

      return completeMessage(msg);
    }
  }

  String animateMessage(String msg, int pos){
    String out = "";
    for (int a = 0; a < pos; a++) {
      if (msg[a] != '\0'){
        out = out + msg[a];
      }
    }
    
    return completeMessage(out);
  }

  String camelMessage(String msg){
    String out = "";
    char c;
    for (int a = 0; msg[a] != '\0'; a++){
      if (a % 2 == 0){
        if (msg[a] >= 'A' && msg[a] <= 'Z'){
          out = out + msg[a];
        } else {
          c = msg[a] & ~(0x20);
          out = out + c;
        }
      } else {
        if (msg[a] >= 'A' && msg[a] <= 'Z'){
          c = tolower(msg[a]);
          out = out + c;
        } else {
          out = out + msg[a];
        }
      }

    }
    return out;
  }
  
  String reverseCamelMessage(String msg){
    String out = "";
    char c;
    for (int a = 0; msg[a] != '\0'; a++){
      if (a % 2 == 0){
        c = tolower(msg[a]);
        out = out + c;   
      } else {
        c = msg[a] & ~(0x20);
        out = out + c;
      }      
    }
    return out;
  }

  String toLowerMessage(String msg){
    String out = "";
    char c;
    for (int a = 0; msg[a] != '\0'; a++){
      c = tolower(msg[a]);
      out = out + c;
    }
    return out;
  }

  String toUpperMessage(String msg){
    String out = "";
    char c;
    for (int a = 0; msg[a] != '\0'; a++){
      c = msg[a] & ~(0x20);
      out = out + c;
    }
    return out; 
  }
};

// Initialize LCD 
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

// The simpler way to extend LiquidCrystal to also have a reset
void reset() {
  lcd.noDisplay();
  lcd.display();
}

void setup() {
  Serial.begin(BAUDRATE);   
  lcd.init(); 
  lcd.backlight();
  lcd.clear();
}

byte serial_getch(){
  int incoming;
  int c = 0;
  // Print Starting message
  while (Serial.available() == 0){
    starting_msg(c);
    c += 1;
    if (c == 4) {c = 0;}
  }

  // read the incoming byte:
  incoming = Serial.read();
  return (incoming &0xff);
}

void starting_msg(int i) {
  if(S == 0) {
    Porcessor p;
    p.setGeometry(LCD_COLS, LCD_ROWS);
    lcd.setCursor(0, 0);
    lcd.print(p.newMessage("Starting", '.', i));
    if(LCD_ROWS > 1) {
      lcd.setCursor(0, 1);
      lcd.print(p.newMessage("Media-Centre", ' ', 0));
    }
    delay(500);
  }
}

void loop(){
  // byte temp;
  byte rxbyte;

  // Read from Serial
  rxbyte = serial_getch();    

  // Read Serial and Write to LCD
  if(rxbyte == 0xFE) {
    lcd.command(serial_getch());
  } else {
    // Reset S and LCD
    // This prevents LCD Buffer overflow
    if (S == 0) {
      S = 1;
      reset();   
    }

    if (Serial) {
      // Write to LCD
      lcd.write(rxbyte);
    }
  }
}

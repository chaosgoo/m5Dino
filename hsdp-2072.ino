#include <Wire.h>
int data = 7;
int myclock = 6;
int column[] = {
    10, 9, 11, 8, 12};
struct CharMap
{
  char c;
  int v1;
  byte b1;
  byte b2;
  byte b3;
  byte b4;
  byte b5;
};
const int cmap_len = 95;
struct CharMap cmap[] = {
    {' ', 32, 0x00, 0x00, 0x00, 0x00, 0x00},
    {'!', 33, 0x00, 0x00, 0xF2, 0x00, 0x00},
    {'#', 35, 0x28, 0xFE, 0x28, 0xFE, 0x28},
    {'$', 36, 0x24, 0x54, 0xFE, 0x54, 0x48},
    {'%', 37, 0xC4, 0xC8, 0x10, 0x26, 0x46},
    {'&', 39, 0x6C, 0x92, 0xAA, 0x44, 0x0A},
    {'(', 40, 0x00, 0x38, 0x44, 0x82, 0x00},
    {')', 41, 0x00, 0x82, 0x44, 0x38, 0x00},
    {'*', 42, 0x28, 0x10, 0x7C, 0x10, 0x28},
    {'+', 43, 0x10, 0x10, 0x7C, 0x10, 0x10},
    {',', 44, 0x00, 0x0A, 0x0C, 0x00, 0x00},
    {'-', 45, 0x10, 0x10, 0x10, 0x10, 0x10},
    {'.', 46, 0x00, 0x06, 0x06, 0x00, 0x00},
    {'0', 48, 0x7C, 0x8A, 0x92, 0xA2, 0x7C},
    {'1', 49, 0x00, 0x42, 0xFE, 0x02, 0x00},
    {'2', 50, 0x42, 0x86, 0x8A, 0x92, 0x62},
    {'3', 51, 0x84, 0x82, 0xA2, 0xD2, 0x8C},
    {'4', 52, 0x18, 0x28, 0x48, 0xFE, 0x08},
    {'5', 53, 0xE4, 0xA2, 0xA2, 0xA2, 0x9C},
    {'6', 54, 0x3C, 0x52, 0x92, 0x92, 0x0C},
    {'7', 55, 0x80, 0x8E, 0x90, 0xA0, 0xC0},
    {'8', 56, 0x6C, 0x92, 0x92, 0x92, 0x6C},
    {'9', 57, 0x60, 0x92, 0x92, 0x94, 0x78},
    {':', 58, 0x00, 0x00, 0x10, 0x00, 0x00},
    {';', 59, 0x00, 0x6A, 0x6C, 0x00, 0x00},
    {'<', 60, 0x10, 0x28, 0x44, 0x82, 0x00},
    {'=', 61, 0x28, 0x28, 0x28, 0x28, 0x28},
    {'>', 62, 0x00, 0x82, 0x44, 0x28, 0x10},
    {'?', 63, 0x40, 0x80, 0x8A, 0x90, 0x60},
    {'@', 64, 0x4C, 0x92, 0x9E, 0x82, 0x7C},
    {'A', 65, 0x7E, 0x88, 0x88, 0x88, 0x7E},
    {'B', 66, 0xFE, 0x92, 0x92, 0x92, 0x6C},
    {'C', 67, 0x7C, 0x82, 0x82, 0x82, 0x44},
    {'D', 68, 0xFE, 0x82, 0x82, 0x44, 0x38},
    {'E', 69, 0xFE, 0x92, 0x92, 0x92, 0x82},
    {'F', 70, 0xFE, 0x90, 0x90, 0x90, 0x80},
    {'G', 71, 0x7C, 0x82, 0x92, 0x92, 0x5E},
    {'H', 72, 0xFE, 0x10, 0x10, 0x10, 0xFE},
    {'I', 73, 0x00, 0x82, 0xFE, 0x82, 0x00},
    {'J', 74, 0x04, 0x02, 0x82, 0xFC, 0x80},
    {'K', 75, 0xFE, 0x10, 0x28, 0x44, 0x82},
    {'L', 76, 0xFE, 0x02, 0x02, 0x02, 0x02},
    {'M', 77, 0xFE, 0x40, 0x30, 0x40, 0xFE},
    {'N', 78, 0xFE, 0x20, 0x10, 0x08, 0xFE},
    {'O', 79, 0x7C, 0x82, 0x82, 0x82, 0x7C},
    {'P', 80, 0xFE, 0x90, 0x90, 0x90, 0x60},
    {'Q', 81, 0x7C, 0x82, 0x8A, 0x84, 0x7A},
    {'R', 82, 0xFE, 0x90, 0x98, 0x94, 0x62},
    {'S', 83, 0x62, 0x92, 0x92, 0x92, 0x8C},
    {'T', 84, 0x80, 0x80, 0xFE, 0x80, 0x80},
    {'U', 85, 0xFC, 0x02, 0x02, 0x02, 0xFC},
    {'V', 86, 0xF8, 0x04, 0x02, 0x04, 0xF8},
    {'W', 87, 0xFC, 0x02, 0x0C, 0x02, 0xFC},
    {'X', 88, 0xC6, 0x28, 0x10, 0x28, 0xC6},
    {'Y', 89, 0xE0, 0x10, 0x0E, 0x10, 0xE0},
    {'Z', 90, 0x86, 0x8A, 0x92, 0xA2, 0xC2},
    {'[', 91, 0x00, 0xFE, 0x82, 0x82, 0x00},
    {']', 93, 0x00, 0x82, 0x82, 0xFE, 0x00},
    {'^', 94, 0x20, 0x40, 0x80, 0x40, 0x20},
    {'_', 95, 0x02, 0x02, 0x02, 0x02, 0x02},
    {'a', 97, 0x04, 0x2A, 0x2A, 0x2A, 0x1E},
    {'b', 98, 0xFE, 0x12, 0x12, 0x12, 0x0C},
    {'c', 99, 0x1C, 0x22, 0x22, 0x22, 0x22},
    {'d', 100, 0x0C, 0x12, 0x12, 0x12, 0xFE},
    {'e', 101, 0x1C, 0x2A, 0x2A, 0x2A, 0x1A},
    {'f', 102, 0x00, 0x10, 0x7E, 0x90, 0x40},
    {'g', 103, 0x12, 0x2A, 0x2A, 0x2A, 0x3C},
    {'h', 104, 0xFE, 0x10, 0x10, 0x10, 0x0E},
    {'i', 105, 0x00, 0x00, 0x5E, 0x00, 0x00},
    {'j', 106, 0x04, 0x02, 0x02, 0xBC, 0x00},
    {'k', 107, 0x00, 0xFE, 0x08, 0x14, 0x22},
    {'l', 108, 0x00, 0x82, 0xFE, 0x02, 0x00},
    {'m', 109, 0x3E, 0x20, 0x1C, 0x20, 0x3E},
    {'n', 110, 0x3E, 0x10, 0x20, 0x20, 0x1E},
    {'o', 111, 0x1C, 0x22, 0x22, 0x22, 0x1C},
    {'p', 112, 0x3E, 0x28, 0x28, 0x28, 0x10},
    {'q', 113, 0x10, 0x28, 0x28, 0x28, 0x3E},
    {'r', 114, 0x3E, 0x10, 0x20, 0x20, 0x10},
    {'s', 115, 0x12, 0x2A, 0x2A, 0x2A, 0x24},
    {'t', 116, 0x20, 0x20, 0xFC, 0x22, 0x24},
    {'u', 117, 0x3C, 0x02, 0x02, 0x02, 0x3C},
    {'v', 118, 0x38, 0x04, 0x02, 0x04, 0x38},
    {'w', 119, 0x3C, 0x02, 0x0C, 0x02, 0x3C},
    {'x', 120, 0x22, 0x14, 0x08, 0x14, 0x22},
    {'y', 121, 0x20, 0x12, 0x0C, 0x10, 0x20},
    {'z', 122, 0x22, 0x26, 0x2A, 0x32, 0x22},
    {'{', 123, 0x00, 0x10, 0x6C, 0x82, 0x82},
    {'|', 124, 0x00, 0x00, 0xEE, 0x00, 0x00},
    {'}', 125, 0x82, 0x82, 0x6C, 0x10, 0x00},
    {'~', 126, 0x20, 0x40, 0x40, 0x40, 0x80}};

byte getCode1(char c, int k)
{
  byte chardata;
  //Serial.print ("the letter is - ");
  //Serial.print (c);
  for (int i = 0; i < cmap_len; i++)
  {
    //Serial.print (" i= ");
    //Serial.print (i);
    //Serial.print (" char = ");
    //Serial.println (cmap[i].c);
    if (c == cmap[i].c)
    {
      switch (k)
      {
      case 0:
        chardata = cmap[i].b1;
        break;
      case 1:
        chardata = cmap[i].b2;
        break;
      case 2:
        chardata = cmap[i].b3;
        break;
      case 3:
        chardata = cmap[i].b4;
        break;
      case 4:
        chardata = cmap[i].b5;
        break;
      }

      //Serial.print("found it - ");
      //Serial.println(chardata, BIN);
      break;
    }
  }
  //Serial.print ( "char data = ");
  //Serial.println (chardata, BIN);
  return chardata;
}

void ShowWord(char c8, char c7, char c6, char c5, char c4, char c3, char c2, char c1)
{

  // for each column
  //Serial.println("Starting to look for data");
  for (int i = 0; i < 5; i++) // 5 columns
  {
    // display the first column in 0, 1, 2, 3, 4, 5, 6, 7
    // case i = 0

    // look up column i(0) for char 1
    byte thisCharData;
    thisCharData = getCode1(c1, i);

    // write 8 values
    for (int j = 1; j < 8; j++)
    {
      digitalWrite(myclock, HIGH);
      //Serial.println(thisCharData & (1<<j));
      digitalWrite(data, (thisCharData & (1 << j))); // first binary value character 8, reads for right to left, binary from left to right
      digitalWrite(myclock, LOW);
    }
    thisCharData = getCode1(c2, i);

    // write 8 values
    for (int j = 1; j < 8; j++)
    {
      digitalWrite(myclock, HIGH);
      digitalWrite(data, (thisCharData & (1 << j))); // first binary value character 8, reads for right to left, binary from left to right
      digitalWrite(myclock, LOW);
    }
    thisCharData = getCode1(c3, i);

    // write 8 values
    for (int j = 1; j < 8; j++)
    {
      digitalWrite(myclock, HIGH);
      digitalWrite(data, (thisCharData & (1 << j))); // first binary value character 8, reads for right to left, binary from left to right
      digitalWrite(myclock, LOW);
    }
    thisCharData = getCode1(c4, i);

    // write 8 values
    for (int j = 1; j < 8; j++)
    {
      digitalWrite(myclock, HIGH);
      digitalWrite(data, (thisCharData & (1 << j))); // first binary value character 8, reads for right to left, binary from left to right
      digitalWrite(myclock, LOW);
    }
    thisCharData = getCode1(c5, i);

    // write 8 values
    for (int j = 1; j < 8; j++)
    {
      digitalWrite(myclock, HIGH);
      digitalWrite(data, (thisCharData & (1 << j))); // first binary value character 8, reads for right to left, binary from left to right
      digitalWrite(myclock, LOW);
    }
    thisCharData = getCode1(c6, i);

    // write 8 values
    for (int j = 1; j < 8; j++)
    {
      digitalWrite(myclock, HIGH);
      digitalWrite(data, (thisCharData & (1 << j))); // first binary value character 8, reads for right to left, binary from left to right
      digitalWrite(myclock, LOW);
    }
    thisCharData = getCode1(c7, i);

    //    // write 8 values
    for (int j = 1; j < 8; j++)
    {
      digitalWrite(myclock, HIGH);
      digitalWrite(data, (thisCharData & (1 << j))); // first binary value character 8, reads for right to left, binary from left to right
      digitalWrite(myclock, LOW);
    }

    thisCharData = getCode1(c8, i);

    //    // write 8 values
    for (int j = 1; j < 8; j++)
    {
      digitalWrite(myclock, HIGH);
      digitalWrite(data, (thisCharData & (1 << j))); // first binary value character 8, reads for right to left, binary from left to right
      digitalWrite(myclock, LOW);
    }
    digitalWrite(column[i], HIGH);
    delayMicroseconds(2000);
    digitalWrite(column[i], LOW);
  }
}
void setup()
{
  pinMode(myclock, OUTPUT);
  // put your setup code here, to run once:
  for (int i = 0; i < 5; i++)
  {
    pinMode(column[i], OUTPUT);
  }
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(column[i], LOW);
  }
  pinMode(data, OUTPUT);
  Serial.begin(9600);
}

void showNum()
{
  for (char a = char('0'); a <= char('9'); a++)
  {
    for (int d = 0; d < 10; d++)
    {
      ShowWord('0', a, a, a, a, a, a, a);
    }
  }
}

void showNum(char num)
{
  if (num <= char('9') && num >= char('0'))
  {
    for (int d = 0; d < 10; d++)
    {
      ShowWord('0', '0', '0', '0', '0', '0', '0', num);
    }
  }
  else if (num > char('9') )
  {
    for (int d = 0; d < 10; d++)
    {
      ShowWord('0', '0', '0', '0', '0', '0', '0', num);
    }
  }
}

char error[] = {' ', 'E', 'R', 'R', 'O', 'R', '!', '!'};
char unknow[] = {' ', 'U', 'N', 'K', 'N', 'O', 'W', 'N'};
char user[] = {' ', 'U', 'S', 'E', 'R', '!', ' ', ' '};

void showCharArrary(char a[])
{
  for (int d = 0; d < 30; d++)
  {
    ShowWord(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
  }
}

void test()
{
  // put your main code here, to run repeatedly:
  showCharArrary(error);
  delay(500);
  showCharArrary(error);
  delay(500);
  showCharArrary(unknow);
  delay(500);
  showCharArrary(unknow);
  delay(500);
  showCharArrary(user);
  delay(500);
  showCharArrary(user);
  delay(500);
  showNum();
}
int kills = 0;
void loop()
{
  if (Serial.available() > 0)
  {
    kills = Serial.read();
    showNum(kills);
  }else{
    showNum(kills);
  }
}

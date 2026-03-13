/*
2~9:LED_K1~LED_K7
1:LED_A1
0:LED_A2
16:LED_A3
15:LED_A4
12:LED_A5
10:USERBTN
*/
uint8_t an[5] = { 1, 0, 16, 15, 12 };
uint8_t ca[7] = { 2, 3, 4, 5, 6, 7, 8 };
uint8_t txt[6]{ 4, 0, 1, 2, 2, 3 };
uint8_t vram[5] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};
uint8_t font[5][5] = {
  { 0b01111111,
    0b00001000,
    0b00001000,
    0b00001000,
    0b01111111 },

  { 0b01001001,
    0b01001001,
    0b01001001,
    0b01001001,
    0b01111111 },

  { 0b00000001,
    0b00000001,
    0b00000001,
    0b00000001,
    0b01111111 },

  { 0b01111111,
    0b01000001,
    0b01000001,
    0b01000001,
    0b01111111 },

  { 0b01111111,
    0b01111111,
    0b01111111,
    0b01111111,
    0b01111111 }
};
uint8_t drawcount = 0;
uint timer = 0;
uint8_t button_state = 0;  //短く押すと1,長押しで2
uint button_timer = 0;
uint txtnum = 0;
uint8_t dot_x = 0;
uint8_t dot_y = 0;

void led_draw();
void led_init();
void vram_fill(uint8_t c);
void vram_txt(uint8_t txtn);
void button_detec();

void setup() {
  // put your setup code here, to run once:
  led_init();
  pinMode(10, INPUT_PULLUP);
  delay(500);
}

void loop() {
  while (txtnum <= 5) {
    if (button_state == 1) {
      txtnum++;
      button_state = 0;
    }
    vram_txt(txt[txtnum]);
    led_draw();
    button_detec();
    delay(2);
  }
  while (1) {
    vram_fill(0);
    dot_x = 0;
    dot_y = 0;
    for (int i = 0; i < 35; i++) {

      while ((vram[dot_x] & (1 << dot_y))) {
        dot_x = random(0, 5);
        dot_y = random(0, 7);
      }
      vram[dot_x] |= (1 << dot_y);
      for (int j = 0; j < 10; j++) {
        led_draw();
        delay(2);
      }
    }
    for (int j = 0; j < 250; j++) {
      led_draw();
      delay(2);
    }
  }
}

void led_init() {
  for (int i = 0; i < 7; i++) pinMode(ca[i], OUTPUT);
  for (int i = 0; i < 5; i++) pinMode(an[i], OUTPUT);
}

void led_draw() {
  for (int i = 0; i < 7; i++) digitalWrite(ca[i], HIGH);
  for (int i = 0; i < 5; i++) digitalWrite(an[i], LOW);
  for (int i = 0; i < 7; i++) {
    digitalWrite(ca[i], !(vram[drawcount] & (1 << 6 - i)));
  }
  digitalWrite(an[4 - drawcount], HIGH);
  drawcount++;
  if (drawcount >= 5) drawcount = 0;
}

void vram_fill(uint8_t c) {
  if (c == 0) {
    for (int i = 0; i < 5; i++) vram[i] = 0x00;
  } else if (c == 1) {
    for (int i = 0; i < 5; i++) vram[i] = 0xff;
  }
}

void vram_txt(uint8_t txtn) {
  for (int i = 0; i < 5; i++) vram[i] = font[txtn][i];
}

void button_detec() {
  if (!digitalRead(10)) {
    button_timer++;
  } else {
    if (button_timer >= 500) {
      button_state = 2;
    } else if (button_timer >= 10) {
      button_state = 1;
    }
    button_timer = 0;
  }
}

//  EIZO supports VESA 
//  must run on 125mhz

//PICO LIPO
    
// The user LED is wired to GP25, just like on an ordinary Pico. You can blink it in exactly the same way!
// As well as being useful for putting your Pimoroni Pico LiPo into bootloader mode, you can also use the BOOT button as a user button. It's wired to GP23 and active low.
// You can read GP29 to monitor/Users/brorgustav/Documents/Arduino/Sketches/bgw_pico_vga_sync/bgw_pico_vga_sync_rgb_buffer_640x480_60hz/vizAsciiHorizontal.ino /Users/brorgustav/Documents/Arduino/Sketches/bgw_pico_vga_sync/bgw_pico_vga_sync_rgb_buffer_640x480_60hz/vizTunnel.ino the voltage of the battery (and use that measurement to calculate how much charge is left in it). This example shows you                   how to do that in MicroPython!
//The power button can also be used as a reset button, yay! Just double press it to cut and reinstate the power whilst holding down the BOOTSEL button on the Pimoroni Pico LiPo to get into bootloader mode, with no plugging and unplugging of cables required.


// /**
//    CODIGO ORIGINAL BY Hunter Adams (vha3@cornell.edu)
//    Modificado by San Tarcisio (https://www.instagram.com/san_tarcisio/)

//    vvvvvvvvvvvvvvvvvvvvv
// Pin1 (RED): Red video (75 ohms, 0.7V peak-to-peak)
// Pin2 (GREEN): Green video
// Pin3 (BLUE): Blue video
// Pin4 (ID2 / RES) : Monitor ID Bit-2 or Reserved
// Pin5 (GND): Ground Pin
// Pin6 (RGND): Red Ground
// Pin7 (GGND): Green Ground
// Pin8 (BGND): Blue Ground
// Pin9 (KEY): +5V DC output from graphic card
// Pin10 (SGND): Sync Ground
// Pin11 (ID0 / RES): Monitor ID Bit0 or Reserved
// Pin12 (ID1 / SDA): Monitor ID Bit-1 / Data line with I2C bi-directional
// Pin13 (HSYNC): Horizontal Sync
// Pin14 (VSYNC): Vertical Sync
// Pin15 (ID3 / SCL): Monitor ID Bit-3 or I2C data clock


//    HARDWARE CONNECTIONS
//     - GPIO 16 ---> VGA Hsync
//     - GPIO 17 ---> VGA Vsync
//     - GPIO 18 ---> 330 ohm resistor ---> VGA Red
//     - GPIO 19 ---> 330 ohm resistor ---> VGA Green
//     - GPIO 20 ---> 330 ohm resistor ---> VGA Blue
//     - RP2040 GND ---> VGA GND

//    RESOURCES USED
//     - PIO state machines 0, 1, and 2 on PIO instance 0
//     - DMA channels 0, 1, 2, and 3
//     - 153.6 kBytes of RAM (for pixel color data)

//*/

#include "vga_graphics.h"  // VGA graphics library
#include "buffer.h"
#define debug Serial
// #include "pico/stdlib.h"

//TIMER
const byte frameRate = 20;
const unsigned long FRAME_INTERVAL = 1000 / frameRate;  // Intervalo de tiempo para cada frame
unsigned long previousFrameTime = 0; // Tiempo previo para el inicio de cada ciclo
unsigned long currentTime;

// int delay1 = 100;
// int delay2 = 150; 

float divide = 5;
float drift_divide = divide-0.003;

unsigned long hperiod = 0;
bool hsync_now;
unsigned long hdrift_rate = 5000;
unsigned long hreturn_period = hperiod;
unsigned long hdrift_last;
unsigned long hreturn_last;
const int numRectangles = 40;             // Número de rectángulos
static float scaleFactors[numRectangles];  // Factores de escala de los rectángulos
static float scaleSpeed = 0.2;            // Velocidad de cambio de escala
static float maxScale = 2;               // Escala máxima
static float finalX = screenWidth / 2;
static float finalY = screenHeight / 2;
//float transparencies[numRectangles];  // Transparencia de los rectángulos
//byte colors[numRectangles];          // Colores aleatorios de los rectángulos

void initTunnel() {
  // Asignar valores iniciales a los factores de escala y transparencias
  for (int i = 0; i < numRectangles; i++) {
    scaleFactors[i] = maxScale / numRectangles * i;
    //transparencies[i] = 255;
  }

  //  // Generar colores aleatorios para los rectángulos
  //  for (int i = 0; i < numRectangles; i++) {
  //    colors[i] = color(random(255), random(255), random(255));
  //  }
}

void tunnel() {
  // Calcular la posición inicial en función del tiempo
  float time = millis() * 0.001;  // Tiempo en segundos
  int initialX = screenWidth / 2 + cos(time) * 100;
  int initialY = screenHeight / 2 + sin(time) * 100;

  // Actualizar y dibujar los rectángulos
  for (int i = 0; i < numRectangles; i++) {
    // Incrementar el factor de escala
    scaleFactors[i] = scaleFactors[i] + scaleSpeed / 10;
//    Serial.print("sc fact:");
//    Serial.println(scaleFactors[i]);
    // Verificar si se alcanzó el límite máximo
    if (scaleFactors[i] > maxScale) {
      scaleFactors[i] = 0;  // Restablecer el factor de escala a cero
      //Serial.println(scaleFactors[i]);
    }

    // Calcular la posición del rectángulo en función del factor de escala
    float x = initialX * (1 - scaleFactors[i]) + finalX * scaleFactors[i];
    float y = initialY * (1 - scaleFactors[i]) + finalY * scaleFactors[i];

    // Calcular el tamaño del rectángulo en función del factor de escala
    int rectSize = 100 * scaleFactors[i] * scaleFactors[i] * scaleFactors[i];
    //Serial.println(rectSize);

    // // Calcular la transparencia en función de la posición del mouse
    // if (i == 0) {
    //   transparencies[i] = map(mouseX, 0, width, 0, 255);
    //   fill(colors[i], transparencies[i]);
    // } else {
    //   transparencies[i] = 255;
    //   noFill();
    // }

    // stroke(255);
    drawRectCenter(x, y, rectSize, rectSize, RED);
    //    Serial.print("x:");
    //    Serial.println(x);
    //    Serial.print("y:");
    //    Serial.println(y);
    //    Serial.print("size:");
    //    Serial.println(rectSize);
  }
}
//ejemplo original (https://play.ertdfgcvb.xyz/#/src/basics/rendering_to_canvas)
//by @andreasgysin (https://twitter.com/andreasgysin)

const int cols = 62;        // Número de columnas de celdas
const int rows = 22;        // Número de filas de celdas
const int cellWidth = 11;   // Ancho de cada celda
const int cellHeight = 20;  // Alto de cada celda
const byte textSize = 2;
const byte textColor = RED;
const byte scrollTime = 4;

String chars PROGMEM = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.:!?";  // Caracteres disponibles

void asciiHorizontal() {
  for (int i = -1; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      char character;

      if (i % 2 == 1) {
        int index = (i + j + currentFrame / scrollTime) % chars.length();  // Calcula el índice del carácter
        character = chars.charAt(index);                          // Obtiene el carácter correspondiente
      } else {
        int index = (i + cols - j + currentFrame / scrollTime) % chars.length();  // Calcula el índice del carácter
        character = chars.charAt(index);                                 // Obtiene el carácter correspondiente
      }

      //setTextColor(RED);
      //setTextSize(2);  // Establece el tamaño del texto
      //setTextCursor(j * cellWidth + cellWidth/2,  i * cellHeight + cellHeight/2);
      //textAlign(CENTER, CENTER);               // Alinea el texto en el centro de la celda
      const short int x = (j * cellWidth + cellWidth / 2)+5;
      const short int y = i * cellHeight + cellHeight / 2;
      drawChar(x,y, character,textColor,BLACK,textSize);  // Muestra el carácter en la celda correspondiente
    }
  }
}

void setup() {
  debug.begin(115200);
  // delay(5500);
  debug.println("Started debug montor,,");
  //  digitalWrite(LED_BUILTIN, true);
  debug.print("CPU RUNNING:");
  Serial.println(rp2040.f_cpu());
    initVGA();
  initTunnel();
}



void frame_rate() {
  //CODE HERE RUNS AT FRAMERATE
  //  static bool programa;
  // change_color();
  if (currentTime % 5000 <= 50) {
    // set_hsync(0);
  }
}
void run_hdrift() {
  test();
  if (hsync_now == false) {
    if (millis() > hdrift_last + hreturn_period) {
      debug.print("HSYNC REGULAR: ");
      debug.print(125 / divide);
      debug.println("mhz");
      hsync_return(divide);
      hsync_now = true;
      hdrift_last = millis();
    }
  }
}
void test() {
  if (hsync_now == true) {
    if (millis() > hdrift_last + hdrift_rate) {
      // set_hsync(0);
      debug.print("HSYNC DRIFT:  ");
      debug.print(125 / drift_divide);
      debug.println("mhz");
      hsync_drift(drift_divide);
      // debug.print("CPU RUNNING: ");
      // debug.println(rp2040.f_cpu());
      hdrift_last = millis();  // Obtener el tiempo actual
      hsync_now = false;
    }
  }
}
void loop() {
  // test();
  currentTime = millis();  // Obtener el tiempo actual
  if (currentTime - previousFrameTime >= FRAME_INTERVAL) {
    previousFrameTime = currentTime;
    // run_hdrift();rr           
    // nextFrame(); 
    //clearScreen();
    frame_rate();
     draw();


  }
}
    //CODE HERE RUNS AT CPU SPEED
void draw() {
  //CODE HERE RUNS AT FRAMERATE
  static bool programa;

  if (currentTime % 5000 <= 50) {
    programa = !programa; //changes the example program
  }

  if (programa == 1) {
    tunnel();           //example
  } else {
    asciiHorizontal(); //example
  }
  escribir();         //example
  nextFrame(); //copies temporary buffer to the vga output buffer
  clearScreen(); //deletes temporary buffer, then next frame will be black
}

void escribir() {
  setTextColor2(RED, BLUE);
  setTextCursor(100, 100);
  setTextSize(2);

  char statusTemp[4];
  itoa(currentFrame, statusTemp, 10);
  writeString(statusTemp);
}
// This #include statement was automatically added by the Particle IDE.
#include <InternetButton.h>

InternetButton b = InternetButton();

#define BUT_TOP_ONOFF 3
#define BUT_LEFT_TIME 2
#define BUT_BOTT_COLOR 1
#define BUT_RIGHT_MODE 4

#define MODE_BREATHE 0
#define MODE_RANDOM 1
#define MODE_CYLON 2
#define MODE_CYCLE 3
#define MODE_FLASH 4
#define MODE_STAR 5
#define MODE_RAINBOW 6
#define MODE_POLICE 7
#define MODE_CHASER 8
#define MODE_TEST 9

#define MAX_MODES 9

#define MAX_COLORS 26
// rows of r/g/b
// various blues followed by litegreen/medgreen/darkgreen/brightgreen/lightlime/yellow/dullyellow/brown/dullred/brightred/blueish/purples
int colors[MAX_COLORS][3] = {
    {0, 0, 255},
    {0, 0, 150},
    {0, 0, 75},
    {0, 255, 255},
    {255, 0, 255},
    {71, 32, 188},
    {32, 118, 188},
    {171, 11, 196},
    {64, 11, 196},
    {27, 30, 196}, 
    {27, 145, 196},
    {1, 6, 91},
    {1, 25, 125},
    {1, 69, 112},
    {0, 125, 145},
    {0, 147, 130},
    {0, 109, 49},
    {31, 61, 45},
    {0, 200, 0},
    {112, 198, 0},
    {255, 255, 0},
    {155, 144, 18}, 
    {155, 87, 18},
    {155, 30, 18},
    {255, 21, 0},
    {60, 50, 168}
};
int curr_color_ofs = 0;         // default to dark blue

int curr_duration_mins = 30;     // default to 30 mins
int curr_cycle = 0;
int max_cycles = 0;

int curr_mode = MODE_BREATHE;

bool is_running = false;


void setup() {   
    b.begin();      // initialize InternetButton
}

void loop() {

    if (b.buttonOn(BUT_TOP_ONOFF)) {            // top button: on/pause
        if (!is_running) {
            is_running = true;
            randomSeed(millis());
        }
        else {
            is_running = false;
        }
        b.playNote("G3", 8);
        b.ledOn(6, 0, 0, 255);
        delay(250);
        b.ledOff(6);
    }
    
    else if (b.buttonOn(BUT_LEFT_TIME)) {       // left button: time
        b.playNote("E3", 8);
        change_duration();
    }
    
    else if (b.buttonOn(BUT_BOTT_COLOR)) {       // bottom button: color
        b.playNote("D3", 8);
        change_color();
    }
    
    else if (b.buttonOn(BUT_RIGHT_MODE)) {       // right button: mode
        b.playNote("C3", 8);
        change_mode();
    }
    
    if (is_running) {
        if (curr_cycle <= max_cycles) {
            
            switch (curr_mode) {
                case MODE_BREATHE:
                    one_breath();
                    break;
                case MODE_RANDOM:
                    run_random();
                    break;
                case MODE_CYLON:
                    run_cylon();
                    break;
                case MODE_CYCLE:
                    run_cycle();
                    break;
                case MODE_FLASH:
                    run_flash();
                    break;
                case MODE_STAR:
                    run_star();
                    break;
                case MODE_RAINBOW:
                    run_rainbow();
                    if (curr_cycle % 3 == 0) {
                        curr_cycle--;
                    }
                    break;
                case MODE_POLICE:
                    run_police(curr_cycle);
                    break;
                case MODE_CHASER:
                    run_chaser();
                    break;                    
                case MODE_TEST:
                    breathe_in(random(0, 255), random(0, 255), random(0, 255));
                    break;
            }

            curr_cycle++;
        }
        else {
            curr_cycle = 0;
            is_running = false;
            b.allLedsOff();
        }
    }

}

void change_duration() {
    int light_leds = 6;
    
    switch (curr_duration_mins) {
        case 30:
            curr_duration_mins = 5;
            light_leds = 1;
            break;
        case 5:
            curr_duration_mins = 10;
            light_leds = 2;
            break;
        case 10:
            curr_duration_mins = 15;
            light_leds = 3;
            break;
        case 15:
            curr_duration_mins = 25;
            light_leds = 5;
            break;
        case 25:
            curr_duration_mins = 31;
            light_leds = 6;
            break;
        case 31:
            curr_duration_mins = 45;
            light_leds = 9;
            break;
        case 45:
            curr_duration_mins = 60;
            light_leds = 11;
            break;
        case 60:
            curr_duration_mins = 5;
            light_leds = 1;
            break;
    }
    
    max_cycles = curr_duration_mins * 60 / 10;
    
    show_led_time(light_leds);
}

void show_led_time(int max_leds) {
    int lightled;

    for (lightled = 1; lightled <= max_leds; lightled++) {
        b.ledOn(lightled, colors[curr_color_ofs][0], colors[curr_color_ofs][1], colors[curr_color_ofs][2]);
    }
    
    delay(250);
    
    b.allLedsOff();
}

void change_color() {
    curr_color_ofs++;
    
    if (curr_color_ofs > MAX_COLORS) {
        curr_color_ofs = 0;
    }

    b.allLedsOn(colors[curr_color_ofs][0], colors[curr_color_ofs][1], colors[curr_color_ofs][2]);
    delay(250);
    b.allLedsOff();
}

void change_mode() {
    int light_led = 11;

    curr_mode++;
    if (curr_mode > MAX_MODES) {
        curr_mode = MODE_BREATHE;
    }

    // Note: will need to do something different if we have more than 11 modes
    light_led = 11 - curr_mode;

    b.ledOn(light_led, 230, 230, 0);       // semi yellow color
    delay(250);
    b.ledOff(light_led);
}

void one_breath() {
    double cycle;
    int color_r;
    int color_g;
    int color_b;
    int individual_delay = 20;      // 20ms for a 255 step cycle, 40 for 128, to add up for a 10 second breath
    
    for (cycle = 0; cycle < 256; cycle++) {
        color_r = (cycle / 256) * colors[curr_color_ofs][0];
        color_g = (cycle / 256) * colors[curr_color_ofs][1];
        color_b = (cycle / 256) * colors[curr_color_ofs][2];
        
        b.allLedsOn(color_r, color_g, color_b);
        
        delay(individual_delay);
    }
    
    delay(500);
    
    for (cycle = 255; cycle >= 0; cycle--) {
        color_r = (cycle / 256) * colors[curr_color_ofs][0];
        color_g = (cycle / 256) * colors[curr_color_ofs][1];
        color_b = (cycle / 256) * colors[curr_color_ofs][2];
        
        b.allLedsOn(color_r, color_g, color_b);
        
        delay(individual_delay);
    }
       
    delay(500);
}

void run_random() {
    int rnd_color_r = random(0, 255);
    int rnd_color_g = random(0, 255);
    int rnd_color_b = random(0, 255);
    int rnd_light = random(1, 12);
    int rnd_delay = random(1, 500);
    
    b.ledOn(rnd_light, rnd_color_r, rnd_color_g, rnd_color_b);
    delay(rnd_delay);
    b.ledOff(rnd_light);   
}

void run_flash() {
    int rnd_light = random(1, 12);
    int rnd_delay = random(1, 500);
    
    if (random(1,11) >= 5) {
        b.ledOn(rnd_light, 128, 128, 128);
    }
    else {
        b.ledOn(rnd_light, 255, 255, 255);
    }
    
    delay(5);
    b.ledOff(rnd_light);
    delay(rnd_delay);
}

void run_cylon() {
    int curr_led;
    
    for (curr_led = 1; curr_led < 12; curr_led++) {
        b.ledOn(curr_led, 255, 0, 0);
        delay(50);
        b.ledOff(curr_led);
    }
    
    delay(10);
    
    for (curr_led = 11; curr_led > 0; curr_led--) {
        b.ledOn(curr_led, 255, 0, 0);
        delay(50);
        b.ledOff(curr_led);
    }
    
    delay(30);
}

void run_cycle() {
    int curr_led;
    int rnd_color_r = random(0, 255);
    int rnd_color_g = random(0, 255);
    int rnd_color_b = random(0, 255);

    for (curr_led = 1; curr_led < 12; curr_led++) {
        b.ledOn(curr_led, rnd_color_r, rnd_color_g, rnd_color_b);
        delay(100);
        b.ledOff(curr_led);
    }
    
    delay(50);
}

void run_star() {
    int rnd_color_r = random(0, 255);
    int rnd_color_g = random(0, 255);
    int rnd_color_b = random(0, 255);

    b.ledOn(6, rnd_color_r, rnd_color_g, rnd_color_b);
    delay(25);
    b.ledOn(2, rnd_color_r, rnd_color_g, rnd_color_b);
    delay(25);
    b.ledOn(8, rnd_color_r, rnd_color_g, rnd_color_b);
    delay(25);
    b.ledOn(4, rnd_color_r, rnd_color_g, rnd_color_b);
    delay(25);
    b.ledOn(10, rnd_color_r, rnd_color_g, rnd_color_b);
    delay(25);
    b.allLedsOff();
}

void run_rainbow() {
    b.advanceRainbow(5, 10);
    delay(10);
}

void run_police(int cycleno) {
    int curr_led;
    int alt_led;
    int ofs;
    
    for (curr_led = 1; curr_led < 12; curr_led++) {
        alt_led = alternate_led(curr_led);
        b.ledOn(curr_led, 255, 0, 0);
        b.ledOn(alt_led, 0, 0, 255);
        delay(50);
        b.ledOff(curr_led);
        b.ledOff(alt_led);
    }
    
    delay(10);
    
    for (curr_led = 11; curr_led > 0; curr_led--) {
        alt_led = alternate_led(curr_led);
        b.ledOn(curr_led, 255, 0, 0);
        b.ledOn(alt_led, 0, 0, 255);
        delay(50);
        b.ledOff(curr_led);
        b.ledOff(alt_led);
    }
    
    delay(30);
        
    if (cycleno % 4 == 0) {
        curr_led = random(1, 11);
        alt_led = alternate_led(curr_led);
        
        delay(100);
        
        for (ofs = 0; ofs < 25; ofs++) {
            b.ledOn(curr_led, 255, 0 , 0);
            b.ledOn(alt_led, 0, 0 , 255);
            delay(10);
            b.allLedsOff();
            delay(30);
            
            if (ofs % 3 == 0) {
                curr_led = alt_led;
                alt_led = alternate_led(curr_led);
            }
        }
        
        delay(100);
        
    }
}

int alternate_led(int curr_led) {
    if (curr_led < 6) {
        return curr_led + 6;
    }
    else if (curr_led > 6) {
        return curr_led - 6;
    }
    else {
        return 11;
    }
}

void run_chaser() {
    int *curr_color;
    int col;
    int led_set;
    int led;

    for (col = 0; col < 256; col++) {

        for (led_set = 0; led_set < 3; led_set++) {

            for (led = 0; led < 12; led+=3) {

                curr_color = color_wheel((col + led) % 255);

                b.ledOn(led_set + led, *curr_color, *(curr_color+1), *(curr_color+2));
            }

            if (b.buttonOn(BUT_TOP_ONOFF)) {
                return;
            }

            delay(75);

            for (led = 0; led < 12; led+=3) {
                // turning off every 3rd led produces the chasing effect
                b.ledOff(led_set + led);
            }

        }

    }
}

int *color_wheel(int wheel_position) {
    static int rgb[3];
  
    if(wheel_position < 85) {
        rgb[0]=wheel_position * 3;
        rgb[1]=255 - wheel_position * 3;
        rgb[2]=0;
    } 
    else if(wheel_position < 170) {
        wheel_position -= 85;
        rgb[0]=255 - wheel_position * 3;
        rgb[1]=0;
        rgb[2]=wheel_position * 3;
    } 
    else {
        wheel_position -= 170;
        rgb[0]=0;
        rgb[1]=wheel_position * 3;
        rgb[2]=255 - wheel_position * 3;
    }

    return rgb;
}

void breathe_in(int red, int green, int blue) {
    double cycle;
    int color_r;
    int color_g;
    int color_b;
    int individual_delay = 20;
    
    for (cycle = 0; cycle < 256; cycle++) {
        color_r = (cycle / 256) * red;
        color_g = (cycle / 256) * green;
        color_b = (cycle / 256) * blue;
        
        b.allLedsOn(color_r, color_g, color_b);
        
        delay(individual_delay);
    }
    
    delay(500);
    
    b.allLedsOff();
}

void run_train() {
    int curr_led;
    int ofs;
    int rnd_green = random(0, 10);
    int rnd_gold = random(0, 10);
    
    for (curr_led = 1; curr_led < 6; curr_led++) {
        b.ledOn(curr_led, 0, 128, 0);
        b.ledOn(12 - curr_led, 255, 215, 0);     // gold
        delay(300);
        b.ledOff(curr_led);
        b.ledOff(12 - curr_led);
    }
    
    if (rnd_green > 6 || rnd_gold > 7) {
        delay(300);
        for (ofs = 0; ofs < 5; ofs++) {
            if (rnd_green > 6) {
                b.ledOn(1, 0, 128, 0);
                b.ledOn(2, 0, 128, 0);
                b.ledOn(3, 0, 128, 0);
                b.ledOn(4, 0, 128, 0);
                b.ledOn(5, 0, 128, 0);
            }
            if (rnd_gold > 7) {
                b.ledOn(11, 255, 215, 0);
                b.ledOn(10, 255, 215, 0);
                b.ledOn(9, 255, 215, 0);
                b.ledOn(8, 255, 215, 0);
                b.ledOn(7, 255, 215, 0);
            }
            
            delay(200);
            b.allLedsOff();
            delay(200);
        }
    }
}

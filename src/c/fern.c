#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten.h>

typedef struct Point Point;
typedef struct FernCanvas FernCanvas;
typedef void (*ButtonCallback)(void);

typedef struct {
    int x;
    int y;
    int width;
    int height;
    uint32_t normal_color;
    uint32_t hover_color;
    uint32_t press_color;
    const char* label;
    int text_scale;
    uint32_t text_color;
    ButtonCallback on_click;
} ButtonConfig;

struct Point {
    int x;
    int y;
};

struct FernCanvas {
    uint32_t* pixels;
    size_t height;
    size_t width;
};

typedef struct {
    uint32_t color;
    float position;  // 0-1
} GradientStop;

typedef struct {
    GradientStop* stops;
    int stop_count;
    int direction;  // 0 = horizontal, 1 = vertical
} LinearGradient;

typedef struct {
    int mouse_x;
    int mouse_y;
    int mouse_down;
    int mouse_clicked;
} InputState;


#define GRADIENT_HORIZONTAL 0
#define GRADIENT_VERTICAL 1

#define Colors_green 0xFF00FF00
#define Colors_blue  0xFF0000FF
#define Colors_red   0xFFFF0000
#define Colors_gray  0xFF202020
#define Colors_black 0xFF000000
#define Colors_white 0xFFFFFFFF

typedef void (*FernDrawCallback)(void);
static FernDrawCallback current_draw_callback = NULL;

void fern_init_wasm(uint32_t* pixel_buffer, size_t height, size_t width);
void ffill(uint32_t* pixels, size_t height, size_t width, uint32_t color);
void frect(uint32_t* pixels, size_t height, size_t width, uint32_t color, 
          size_t x, size_t y, size_t w, size_t h);
void fcircle(uint32_t* pixels, size_t height, size_t width, uint32_t color, 
            size_t cx, size_t cy, size_t r);
void fline(uint32_t* pixels, size_t height, size_t width, uint32_t color, 
          int x1, int y1, int x2, int y2, int thickness);
void fchar(uint32_t* pixels, int width, int height, char c, int x, int y, int scale, uint32_t color);
void ftext(uint32_t* pixels, int width, int height, const char* text, int x, int y, int scale, uint32_t color);
uint32_t fblend_color(uint32_t color1, uint32_t color2, float t);
uint32_t gradient_color_at(LinearGradient grad, float position);

void fern_start_render_loop(void);
void reset_input_events(void);
void setup_event_listeners(void);
void fernPrintf(const char* string);

Point Point_create(int x, int y);

void runApp(FernCanvas canvas);
void Container(uint32_t color, int x, int y, int width, int height);
void CenteredContainer(int width, int height, uint32_t color);
void CircleWidget(int radius, Point position, uint32_t color);
void LineWidget(Point start, Point end, int thickness, uint32_t color);
void TextWidget(Point start, const char* text, int scale, uint32_t color);
void LinearGradientContainer(int x, int y, int width, int height, LinearGradient gradient);

#define color(c) c
#define x(val) val
#define y(val) val
#define width(w) w
#define height(h) h
#define radius(r) r
#define position(p) p
#define start(p) p
#define end(p) p
#define thickness(t) t
#define text(t) t
#define scale(s) s
#define gradient(g) g

static uint32_t* buffer_ptr = NULL;
static size_t buffer_width = 0;
static size_t buffer_height = 0;
static InputState current_input = {0};
FernCanvas current_canvas;

#define return_defer(value) do {result = (value); goto defer;} while(0)
typedef int Errno;

Point Point_create(int x, int y) {
    Point p = {x, y};
    return p;
}

void runApp(FernCanvas canvas) {
    current_canvas = canvas;
    fern_init_wasm(canvas.pixels, canvas.height, canvas.width);
    setup_event_listeners(); 
    ffill(canvas.pixels, canvas.height, canvas.width, Colors_gray);
}

void Container(uint32_t color, int x, int y, int width, int height) {
    frect(current_canvas.pixels, current_canvas.height, current_canvas.width,
          color, x, y, width, height);
}

void CenteredContainer(int width, int height, uint32_t color) {
    int centered_x = (current_canvas.width - width) / 2;
    int centered_y = (current_canvas.height - height) / 2;    
    Container(color, centered_x, centered_y, width, height);
}

void CircleWidget(int radius, Point position, uint32_t color) {
    fcircle(current_canvas.pixels, current_canvas.height, current_canvas.width,
            color, position.x, position.y, radius);
}

void LineWidget(Point start, Point end, int thickness, uint32_t color) {
    fline(current_canvas.pixels, current_canvas.height, current_canvas.width,
          color, start.x, start.y, end.x, end.y, thickness);
}

void TextWidget(Point start, const char* text, int scale, uint32_t color) {
    ftext(current_canvas.pixels, current_canvas.width, current_canvas.height, text, start.x, start.y, scale, color);
}

void fern_set_draw_callback(FernDrawCallback callback) {
    current_draw_callback = callback;
}


static void internal_render_loop() {
     if (current_draw_callback != NULL) {
        current_draw_callback();
    }

    if (!buffer_ptr) return;    
    EM_ASM({
        var canvas = document.getElementById('canvas');
        var ctx = canvas.getContext('2d');        
        if (canvas.width !== $1 || canvas.height !== $0) {
            canvas.width = $1;  
            canvas.height = $0; 
        }
        var imageData = ctx.createImageData(canvas.width, canvas.height);
        
        var data = imageData.data;
        var buffer = $2; 
        var size = $0 * $1;
        
        for (var i = 0; i < size; i++) {
            var pixel = HEAP32[buffer/4 + i]; 
            var r = pixel & 0xFF;
            var g = (pixel >> 8) & 0xFF;
            var b = (pixel >> 16) & 0xFF;
            var a = (pixel >> 24) & 0xFF;            
            var j = i * 4;
            data[j + 0] = r;
            data[j + 1] = g;
            data[j + 2] = b;
            data[j + 3] = a;
        }
        
        ctx.putImageData(imageData, 0, 0);       
    }, buffer_height, buffer_width, buffer_ptr);
    reset_input_events();
    //  EM_ASM({
    //     console.log("Mouse position:", $0, $1, "Mouse down:", $2, "Clicked:", $3);
    // }, current_input.mouse_x, current_input.mouse_y, 
    //    current_input.mouse_down, current_input.mouse_clicked);
}

void fern_init_wasm(uint32_t* pixel_buffer, size_t height, size_t width) {
    buffer_ptr = pixel_buffer;
    buffer_height = height;
    buffer_width = width;
}

EMSCRIPTEN_KEEPALIVE
void update_mouse_position(int x, int y) {
    current_input.mouse_x = x;
    current_input.mouse_y = y;
}

EMSCRIPTEN_KEEPALIVE
void update_mouse_button(int down) {
    if (!current_input.mouse_down && down) {
        current_input.mouse_clicked = 1;
    }
    current_input.mouse_down = down;
}

void reset_input_events() {
    current_input.mouse_clicked = 0;
}

EMSCRIPTEN_KEEPALIVE
void setup_event_listeners() {
    EM_ASM({
        var canvas = document.getElementById('canvas');
        
        canvas.addEventListener('mousemove', function(e) {
            var rect = canvas.getBoundingClientRect();
            var mouseX = Math.floor((e.clientX - rect.left) * (canvas.width / rect.width));
            var mouseY = Math.floor((e.clientY - rect.top) * (canvas.height / rect.height));
            
            Module._update_mouse_position(mouseX, mouseY);
        });
        
        canvas.addEventListener('mousedown', function(e) {
            Module._update_mouse_button(1);
        });
        
        canvas.addEventListener('mouseup', function(e) {
            Module._update_mouse_button(0);
        });
        

    });
}

EMSCRIPTEN_KEEPALIVE
void fernPrintf(const char* string) {
    
#ifdef __EMSCRIPTEN__
    EM_ASM({
        console.log("🌿 Fern: " + UTF8ToString($0));
    }, string);
#else
    printf("🌿 Fern: %s\n", string);
#endif
}


void fern_start_render_loop(void) {
    emscripten_set_main_loop(internal_render_loop, 0, 1);
}

void ffill(uint32_t* pixels, size_t height, size_t width, uint32_t color){
    for(size_t i = 0; i < height*width; ++i){
        pixels[i] = color;
    }
}

Errno fsave_ppm(uint32_t* pixels, size_t width, size_t height, const char* filename){
    int result = 0;
    FILE* file = NULL;

    {
        file = fopen(filename, "wb");
        if (!file) return_defer(errno);
        fprintf(file, "P6\n%zu %zu\n255\n", width, height);
        if(ferror(file)) return_defer(errno);

        for (size_t i = 0; i < width*height; i++){
            // 0xAABBGGRR
            uint32_t pixel = pixels[i];
            uint8_t bytes[3] = {
                pixel&0xFF,
                (pixel>>8)&0xFF,
                (pixel>>16)&0xFF
            };

            fwrite(bytes, sizeof(bytes), 1, file);
            if(ferror(file)) return_defer(errno);
        }
        
    }
    defer: 
    if(file) fclose(file);
    return result;
}

void frect(uint32_t* pixels, size_t height, size_t width, uint32_t color, size_t x, size_t y, size_t w, size_t h) {
    for (int dx = 0; dx < (int) h; ++dx) {
        for (int dy = 0; dy < (int) w; ++dy) {
            if (x + dy < width && y + dx < height) {
                pixels[(y + dx) * width + (x + dy)] = color;
            }
        }
    }
}

void fcircle(uint32_t* pixels, size_t height, size_t width, uint32_t color, size_t cx, size_t cy, size_t r) {
    for (int x = -r; x <= (int)r; x++){
        for(int y = -r; y <= (int) r; y++){
            if (x*x + y*y <= (int) (r*r)){
                int px = cx + x;
                int py = cy + y;
                if (px >= 0 && px < (int)width && py >= 0 && py < (int)height){
                    pixels[py * width + px] = color;
                }
            }
        }
    }
}

// based on bresenham's algo
void fline(uint32_t* pixels, size_t px_height, size_t px_width, uint32_t color, int x1, int y1, int x2, int y2, int thickness){
    int t = thickness;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;

    int err = (dx - dy);
    int e2;
 
    while(1){
        if (x1 >= 0 && x1 < px_width && y1 >= 0 && y1 < px_height) {
            // pixels[y1 * px_width + x1] = color; 
            fcircle(pixels, px_height, px_width, color, x1, y1, t);
        }

        if(x1 == x2 && y1 == y2) break;

        e2 = 2 * err;
        if(e2 > -dy){
            e2 -= dy;
            x1 += sx;
        }

        if(e2 < dx) {
            e2 += dx;
            y1 += sy;
        }
    }
}

uint32_t fblend_color(uint32_t color1, uint32_t color2, float t) {
    uint8_t r1 = (color1 >> 16) & 0xFF;
    uint8_t g1 = (color1 >> 8) & 0xFF;
    uint8_t b1 = color1 & 0xFF;
    
    uint8_t r2 = (color2 >> 16) & 0xFF;
    uint8_t g2 = (color2 >> 8) & 0xFF;
    uint8_t b2 = color2 & 0xFF;
    
    uint8_t r = r1 + (r2 - r1) * t;
    uint8_t g = g1 + (g2 - g1) * t;
    uint8_t b = b1 + (b2 - b1) * t;
    
    return 0xFF000000 | (r << 16) | (g << 8) | b;
}

uint32_t gradient_color_at(LinearGradient grad, float position) {
    if (position <= grad.stops[0].position) return grad.stops[0].color;
    if (position >= grad.stops[grad.stop_count-1].position) return grad.stops[grad.stop_count-1].color;
    
    for (int i = 0; i < grad.stop_count - 1; i++) {
        if (position >= grad.stops[i].position && position <= grad.stops[i+1].position) {
            float local_pos = (position - grad.stops[i].position) / 
                              (grad.stops[i+1].position - grad.stops[i].position);
            return fblend_color(grad.stops[i].color, grad.stops[i+1].color, local_pos);
        }
    }
    return 0xFF000000;
}

void ButtonWidget(ButtonConfig config) {
    int mouse_over = 
        current_input.mouse_x >= config.x && 
        current_input.mouse_x < config.x + config.width &&
        current_input.mouse_y >= config.y && 
        current_input.mouse_y < config.y + config.height;
    
    uint32_t button_color = config.normal_color;
    if (mouse_over) {
        button_color = current_input.mouse_down ? config.press_color : config.hover_color;
        
        if (current_input.mouse_clicked && config.on_click != NULL) {
            config.on_click();
        }
    }
    
    Container(
        button_color,
        config.x, 
        config.y, 
        config.width, 
        config.height
    );
    

    if (config.label != NULL) {
        int text_width = strlen(config.label) * 8 * config.text_scale;
        int text_x = config.x + (config.width - text_width) / 2;
        int text_y = config.y + (config.height - 8 * config.text_scale) / 2;
        
        TextWidget(
            Point_create(text_x, text_y),
            config.label,
            config.text_scale,
            config.text_color
        );
    }
}

void LinearGradientContainer(int x, int y, int width, int height, LinearGradient gradient) {
    if (gradient.direction == GRADIENT_VERTICAL) {
        for (int row = 0; row < height; row++) {
            float pos = (float)row / height;
            uint32_t color = gradient_color_at(gradient, pos);
            
            Container(
                color(color),
                x(x),
                y(y + row),
                width(width),
                height(1)
            );
        }
    } else {
        for (int col = 0; col < width; col++) {
            float pos = (float)col / width;
            uint32_t color = gradient_color_at(gradient, pos);
            
            Container(
                color(color),
                x(x + col),
                y(y),
                width(1),
                height(height)
            );
        }
    }
}


static const unsigned char SIMPLE_FONT[][8] = {
    /* A */
    {
        0b00111000,  // ░░███░░
        0b01101100,  // ░██░██░
        0b11000110,  // █░░░░█░
        0b11111110,  // ███████
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* B */
    {
        0b11111100,  // █████░░
        0b01100110,  // ░██░░█░
        0b01111100,  // ░████░░
        0b01100110,  // ░██░░█░
        0b01100110,  // ░██░░█░
        0b11111100,  // █████░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* C */
    {
        0b01111100,  // ░████░░
        0b11000110,  // █░░░░█░
        0b11000000,  // █░░░░░░
        0b11000000,  // █░░░░░░
        0b11000110,  // █░░░░█░
        0b01111100,  // ░████░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* D */
    {
        0b11111000,  // █████░░
        0b01101100,  // ░██░██░
        0b01100110,  // ░██░░█░
        0b01100110,  // ░██░░█░
        0b01101100,  // ░██░██░
        0b11111000,  // █████░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* E */
    {
        0b11111110,  // ██████░
        0b11000000,  // █░░░░░░
        0b11111100,  // █████░░
        0b11000000,  // █░░░░░░
        0b11000000,  // █░░░░░░
        0b11111110,  // ██████░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* F */
    {
        0b11111110,  // ██████░
        0b11000000,  // █░░░░░░
        0b11111100,  // █████░░
        0b11000000,  // █░░░░░░
        0b11000000,  // █░░░░░░
        0b11000000,  // █░░░░░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* G */
    {
        0b01111100,  // ░████░░
        0b11000110,  // █░░░░█░
        0b11000000,  // █░░░░░░
        0b11001110,  // █░░███░
        0b11000110,  // █░░░░█░
        0b01111110,  // ░█████░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* H */
    {
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11111110,  // ██████░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* I */
    {
        0b01111000,  // ░████░░
        0b00110000,  // ░░██░░░
        0b00110000,  // ░░██░░░
        0b00110000,  // ░░██░░░
        0b00110000,  // ░░██░░░
        0b01111000,  // ░████░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* J */
    {
        0b00011110,  // ░░░███░
        0b00001100,  // ░░░░██░
        0b00001100,  // ░░░░██░
        0b00001100,  // ░░░░██░
        0b11001100,  // █░░░██░
        0b01111000,  // ░████░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* K */
    {
        0b11000110,  // █░░░░█░
        0b11001100,  // █░░░██░
        0b11011000,  // █░██░░░
        0b11110000,  // ████░░░
        0b11011000,  // █░██░░░
        0b11001110,  // █░░░██░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* L */
    {
        0b11000000,  // █░░░░░░
        0b11000000,  // █░░░░░░
        0b11000000,  // █░░░░░░
        0b11000000,  // █░░░░░░
        0b11000000,  // █░░░░░░
        0b11111110,  // ██████░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* M */
    {
        0b11000110,  // █░░░░█░
        0b11101110,  // █░███░░
        0b11111110,  // ██████░
        0b11010110,  // █░█░█░░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* N */
    {
        0b11000110,  // █░░░░█░
        0b11100110,  // ███░░█░
        0b11110110,  // ████░█░
        0b11011110,  // █░███░░
        0b11001110,  // █░░░██░
        0b11000110,  // █░░░░█░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* O */
    {
        0b01111100,  // ░████░░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b01111100,  // ░████░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* P */
    {
        0b11111100,  // █████░░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11111100,  // █████░░
        0b11000000,  // █░░░░░░
        0b11000000,  // █░░░░░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* Q */
    {
        0b01111100,  // ░████░░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11011110,  // █░███░░
        0b01111110,  // ░█████░
        0b00000110,  // ░░░░░█░
        0b00000000   // ░░░░░░░
    },
    /* R */
    {
        0b11111100,  // █████░░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11111100,  // █████░░
        0b11011000,  // █░██░░░
        0b11001110,  // █░░░██░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* S */
    {
        0b01111100,  // ░████░░
        0b11000110,  // █░░░░█░
        0b01110000,  // ░███░░░
        0b00011100,  // ░░░███░
        0b11000110,  // █░░░░█░
        0b01111100,  // ░████░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* T */
    {
        0b11111110,  // ██████░
        0b00110000,  // ░░██░░░
        0b00110000,  // ░░██░░░
        0b00110000,  // ░░██░░░
        0b00110000,  // ░░██░░░
        0b00110000,  // ░░██░░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* U */
    {
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b01111100,  // ░████░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* V */
    {
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b01101100,  // ░██░██░
        0b00111000,  // ░░███░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* W */
    {
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b11010110,  // █░█░█░░
        0b11111110,  // ██████░
        0b01101100,  // ░██░██░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* X */
    {
        0b11000110,  // █░░░░█░
        0b01101100,  // ░██░██░
        0b00111000,  // ░░███░░
        0b00111000,  // ░░███░░
        0b01101100,  // ░██░██░
        0b11000110,  // █░░░░█░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* Y */
    {
        0b11000110,  // █░░░░█░
        0b11000110,  // █░░░░█░
        0b01101100,  // ░██░██░
        0b00111000,  // ░░███░░
        0b00110000,  // ░░██░░░
        0b00110000,  // ░░██░░░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },
    /* Z */
    {
        0b11111110,  // ██████░
        0b00001100,  // ░░░░██░
        0b00011000,  // ░░░██░░
        0b00110000,  // ░░██░░░
        0b01100000,  // ░██░░░░
        0b11111110,  // ██████░
        0b00000000,  // ░░░░░░░
        0b00000000   // ░░░░░░░
    },

        // Numbers 0-9
    {
        0b00111100,  // ░░████░░
        0b01100110,  // ░██░░██░
        0b01101110,  // ░██░███░
        0b01110110,  // ░███░██░
        0b01100110,  // ░██░░██░
        0b00111100,  // ░░████░░
        0b00000000,  // ░░░░░░░░
        0b00000000   // ░░░░░░░░
    },  // 0

    {
        0b00011000,  // ░░░██░░░
        0b00111000,  // ░░███░░░
        0b00011000,  // ░░░██░░░
        0b00011000,  // ░░░██░░░
        0b00011000,  // ░░░██░░░
        0b01111110,  // ░██████░
        0b00000000,  // ░░░░░░░░
        0b00000000   // ░░░░░░░░
    },  // 1

    {
        0b00111100,  // ░░████░░
        0b01100110,  // ░██░░██░
        0b00001100,  // ░░░░██░░
        0b00011000,  // ░░░██░░░
        0b00110000,  // ░░██░░░░
        0b01111110,  // ░██████░
        0b00000000,  // ░░░░░░░░
        0b00000000   // ░░░░░░░░
    },  // 2

    {
        0b00111100,  // ░░████░░
        0b01100110,  // ░██░░██░
        0b00001100,  // ░░░░██░░
        0b00011100,  // ░░░███░░
        0b01100110,  // ░██░░██░
        0b00111100,  // ░░████░░
        0b00000000,  // ░░░░░░░░
        0b00000000   // ░░░░░░░░
    },  // 3

    {
        0b00001100,  // ░░░░██░░
        0b00011100,  // ░░░███░░
        0b00101100,  // ░░█░██░░
        0b01001100,  // ░█░░██░░
        0b01111110,  // ░██████░
        0b00001100,  // ░░░░██░░
        0b00000000,  // ░░░░░░░░
        0b00000000   // ░░░░░░░░
    },  // 4

    {
        0b01111110,  // ░██████░
        0b01100000,  // ░██░░░░░
        0b01111100,  // ░█████░░
        0b00000110,  // ░░░░░██░
        0b01100110,  // ░██░░██░
        0b00111100,  // ░░████░░
        0b00000000,  // ░░░░░░░░
        0b00000000   // ░░░░░░░░
    },  // 5

    {
        0b00111100,  // ░░████░░
        0b01100110,  // ░██░░██░
        0b01100000,  // ░██░░░░░
        0b01111100,  // ░█████░░
        0b01100110,  // ░██░░██░
        0b00111100,  // ░░████░░
        0b00000000,  // ░░░░░░░░
        0b00000000   // ░░░░░░░░
    },  // 6

    {
        0b01111110,  // ░██████░
        0b00000110,  // ░░░░░██░
        0b00001100,  // ░░░░██░░
        0b00011000,  // ░░░██░░░
        0b00110000,  // ░░██░░░░
        0b00110000,  // ░░██░░░░
        0b00000000,  // ░░░░░░░░
        0b00000000   // ░░░░░░░░
    },  // 7

    {
        0b00111100,  // ░░████░░
        0b01100110,  // ░██░░██░
        0b00111100,  // ░░████░░
        0b01100110,  // ░██░░██░
        0b01100110,  // ░██░░██░
        0b00111100,  // ░░████░░
        0b00000000,  // ░░░░░░░░
        0b00000000   // ░░░░░░░░
    },  // 8

    {
        0b00111100,  // ░░████░░
        0b01100110,  // ░██░░██░
        0b01100110,  // ░██░░██░
        0b00111110,  // ░░█████░
        0b00000110,  // ░░░░░██░
        0b00111100,  // ░░████░░
        0b00000000,  // ░░░░░░░░
        0b00000000   // ░░░░░░░░
    }   // 9
};



void fchar(uint32_t* pixels, int width, int height, char c, int x, int y, int scale, uint32_t color) {
    int char_index;
    
    if (c >= 'A' && c <= 'Z') {
        char_index = c - 'A';  
    } else if (c >= '0' && c <= '9') {
        switch (c) {
            case '0': char_index = 26; break;
            case '1': char_index = 27; break;
            case '2': char_index = 28; break;
            case '3': char_index = 29; break;
            case '4': char_index = 30; break;
            case '5': char_index = 31; break;
            case '6': char_index = 32; break;
            case '7': char_index = 33; break;
            case '8': char_index = 34; break;
            case '9': char_index = 35; break;
            default: return; 
        }
    } else {
        return;
    }

    
    for (int row = 0; row < 8; row++) {
        unsigned char row_bits = SIMPLE_FONT[char_index][row];
        
        for (int col = 0; col < 8; col++) {
            if (row_bits & (1 << (7 - col))) {  
                int base_x = x + col * scale;
                int base_y = y + row * scale;
                
                for (int sy = 0; sy < scale; sy++) {
                    for (int sx = 0; sx < scale; sx++) {
                        int pixel_x = base_x + sx;
                        int pixel_y = base_y + sy;
                        
                        if (pixel_x >= 0 && pixel_x < width && pixel_y >= 0 && pixel_y < height) {
                            pixels[pixel_y * width + pixel_x] = color;
                        }
                    }
                }
            }
        }
    }
}

void ftext(uint32_t* pixels, int width, int height, const char* text, int x, int y, int scale, uint32_t color) {
    int cursor_x = x;
    for(const char* p = text; *p!='\0'; p++){
       if (*p == ' ' || ((*p < 'A' || *p > 'Z') && (*p < '0' || *p > '9'))) {
            cursor_x += 4 * scale;
            continue;
        }
        fchar(pixels, width, height, *p, cursor_x, y, scale, color);
        cursor_x +=8 * scale;
    }
}
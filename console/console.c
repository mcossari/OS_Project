#include "console.h"
#include "portmap.h"

char* const VGA_BUFFER =(char*) 0xb8000;

const int VGA_WIDTH = 80;

const int VGA_HEIGHT = 25;

const int VGA_BYTES_PER_CHARACTER = 2;

static VGA_Color terminal_font_color = LIGHT_GRAY; // Default font color will be light gray

static VGA_Color terminal_background_color = BLACK; // Default background color is black

static int terminal_position = 0;

void update_cursor() {

     uint16_t cursor_position = terminal_position >> 1;

     outb(0x3D4, 0x0F);

     outb(0x3D5, (uint8_t) (cursor_position));

     outb(0x3D4, 0x0E);

     outb(0x3D5, (uint8_t) (cursor_position >> 8));

}


void clear_terminal() {

	terminal_position = 0;

	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * VGA_BYTES_PER_CHARACTER; i+=VGA_BYTES_PER_CHARACTER) {
		VGA_BUFFER[i]= 0;
        	VGA_BUFFER[i+1] = 0x07;
    	}

	update_cursor();

 }

void print_character(char c) {

	print_character_with_color(c, terminal_background_color, terminal_font_color);
}

void print_string(char* str){

	print_string_with_color(str, terminal_background_color, terminal_font_color);
}

void print_line(char* str) {

	print_line_with_color(str, terminal_background_color, terminal_font_color);
}

void print_character_with_color(char c, VGA_Color bg_color, VGA_Color font_color){

	char color = (bg_color << 4) | font_color;

	if (c == '\n'){
                terminal_position = (terminal_position / (VGA_WIDTH * VGA_BYTES_PER_CHARACTER) + 1) * (VGA_WIDTH * VGA_BYTES_PER_CHARACTER);
        }
        else {
                VGA_BUFFER[terminal_position] = c;
                VGA_BUFFER[terminal_position + 1] = color;
                terminal_position += VGA_BYTES_PER_CHARACTER;
        }

	update_cursor();
}

void print_string_with_color(char* str, VGA_Color bg_color, VGA_Color font_color) {

	for (int i=0; str[i] != '\0'; i++) {

        	print_character_with_color(str[i], bg_color, font_color);
     	}
}

void print_line_with_color(char* str, VGA_Color bg_color, VGA_Color font_color) {

	print_string_with_color(str, bg_color, font_color);
	print_character_with_color('\n', bg_color, font_color);
}

void set_terminal_font_color(VGA_Color col) {

     terminal_font_color = col;
}

void set_terminal_background_color(VGA_Color col) {

     terminal_background_color = col;
}

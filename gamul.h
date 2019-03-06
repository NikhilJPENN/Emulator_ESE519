#ifndef GAMUL_H
#define GAMUL_H

#define SCREEN_WIDTH	64
#define SCREEN_HEIGHT	32
#define FONTSET_SIZE	80
#define MEMORY_SIZE     4096
#define  STACK_SIZE     16

typedef struct gamul8 {
	unsigned char display[SCREEN_WIDTH][SCREEN_HEIGHT];
        
} gamul8;

struct gamul8 gamul8_val;
extern int flag_key;
extern unsigned char key_state[16];
extern void display_func(gamul8 *gamer);
extern unsigned char memory_gamulator[MEMORY_SIZE];
extern unsigned char delay_timer;
extern unsigned char sound_timer;
extern unsigned short pc;
extern int load_file(char *file_name, unsigned char *buffer);
extern void fill_font_table(unsigned char *buffer);
extern void decode_opcode(short ,struct gamul8 *);
extern unsigned short fetch_opcode(unsigned char *);


#endif

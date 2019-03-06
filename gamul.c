#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gamul.h"

// font set for rendering
const unsigned char fontset[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
	0x20, 0x60, 0x20, 0x20, 0x70,		// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
	0xF0, 0x80, 0xF0, 0x80, 0x80		// F
};


unsigned short pc = 0x200;
//int memory_gamulator_length = 0xDFF;
unsigned char memory_gamulator[4096] = {0};
unsigned char array_register[16];  //V0 -F
unsigned short index_r;  //Index register
unsigned char delay_timer;
unsigned char sound_timer;

struct stack {
   unsigned short s[STACK_SIZE];
   int top;
} st;
 
int stfull() {
   if (st.top >= STACK_SIZE )
      return 1;
   else
      return 0;
}
 
void push(unsigned short item) {
   
   st.s[st.top] = item;
   st.top++;
}
 
int stempty() {
   if (st.top == -1)
      return 1;
   else
      return 0;
}
 
unsigned short pop() {
   unsigned short item;
   st.top--;
   item = st.s[st.top];
   
   return (item);
}

/*	FUNCTION: load_file
 *  -------------------
 *	Loads the given program/game
 *	PARAMETERS: 
 *  file_name: name of file to be loaded
 *  buffer: system memory which will hold program
 *	RETURNS: 0 if successful, -1 if file error
 */
int load_file(char *file_name, unsigned char *buffer)
{
	FILE *file;
	int file_size;

	// open file stream in binary read-only mode
       // printf("buffer before %x",*(&buffer));
	file = fopen(file_name, "rb");	//man 3 fopen
	
	fseek(file, 0, SEEK_END);	//man 3 fseek

	file_size = ftell(file);	//man 3 ftell
        printf("file size: %d\n",file_size);
        buffer = buffer + 512;
	rewind(file);	
       
	printf("bytes read %d\n",fread(buffer, 1, file_size, file));	//man 3 fread
	st.top = 0;
	fclose(file);
	return 0;

}

void fill_font_table(unsigned char* buffer)
{  
    int i;
   // printf("%x\n",*(&buffer));
     for( i= 0; i<80;i++)
      {     
            buffer[i] =  fontset[i];
           // printf("buffer[%d] = %x", i,buffer[i]);
       
      }

      
 
}
/*	FUNCTION: display_func
 *  ----------------------
 *	Sample function that displays a pixel on the screen
 *	PARAMETERS: 
 *  gamer: architecture to be emulated, defined in gamul.h
 *	RETURNS: none
 */
void display_func(gamul8 *gamer)
{
	gamer->display[20][40] = 1;
	gamer->display[10][30] = 0;
}


unsigned short fetch_opcode(unsigned char *buffer)
{
     unsigned short  opcode;
     unsigned short  opcode1;
     unsigned short  opcode2;
     opcode1 = buffer[pc] ;
    // printf("pc = %x\n",pc);
    // pc = pc +1;
    // printf("opcode1 %x\n",opcode1);
     opcode2 = buffer[pc+1];
     opcode  =((opcode1 <<8) |  opcode2);
     //printf("Opcode =  %x\n", opcode);
      pc = pc + 2;
     return opcode;
}


void decode_opcode(short opcode,struct gamul8 *gamulc)
{
    unsigned short new_opcode;
    unsigned short address;
    unsigned char value;
    unsigned char value_x;
    unsigned char value_y;
    unsigned char m_s_b;
    unsigned char l_s_b;
    unsigned char rand_2;
    unsigned char middle_digit;
    unsigned char last_digit;
    unsigned char remainder;
    unsigned char first_digit;
    unsigned char pixel_state;
    unsigned short loop_var;
    unsigned short loop_var_x;
    unsigned short n_of_rows;
    unsigned short operand_x;
    unsigned short operand_y;
    unsigned short operand_z;
    unsigned char pixel;
    unsigned int i;
    unsigned int j;
    unsigned char current_evaluated_pixel;
    unsigned char operand_nn;
    unsigned short new_val;
    unsigned short register_opcode_x;
    unsigned short register_opcode_y;
    unsigned short opcode_subscript;
    unsigned short last_fbits;
    time_t t;
    new_opcode = opcode & 0xf000;
    new_opcode = new_opcode >>12;
   // printf("new_opcode:%x\n",new_opcode);
    new_opcode = new_opcode &0x000f;
   
    if (new_opcode == 0x0000)
      {
         if((opcode & 0xff) == 0x00EE)
            { 
              pc =  pop();
            }
               
         if((opcode & 0xff) == 0x00E0)
           {
              for(i = 0;i <64;i++)
              for(j = 0;j <32;j++)
           {gamulc->display[i][j] = 0;
           }
               
           }
      }
     


   else if(new_opcode == 0x0001)
     {   address =opcode &0x0fff;
         printf("address = %x\n",address);
         pc = address;
         printf("pc: %x\n",pc);
     }

  else if(new_opcode == 0x0002)
   {  
     if(stfull() == 0)
      push(pc); 
     else
      printf("Stack Full\n");
     address = opcode &0x0fff;
     pc = address;  
     // pop(pc);
      
   }

 else if(new_opcode == 0x0003)
   {  
      short register_opcode = opcode & 0x0f00;
           register_opcode = register_opcode >>8;
           printf("register_opcode = %x\n",register_opcode);
           if(register_opcode == 0x00)
           	printf("array_register 0 = %d\n", array_register[register_opcode]);
           opcode_subscript = opcode &0x00ff;
           if(array_register[register_opcode] == opcode_subscript)
                 pc = pc+2;
     
     // pop(pc);
      
   }

else if(new_opcode == 0x0004)
{

          // short opcode_subscript;
           short register_opcode = opcode & 0x0f00;
           register_opcode = register_opcode >>8;

            opcode_subscript = opcode &0x00ff;
           if(array_register[register_opcode] != opcode_subscript)
                 pc = pc+2;

}

else if(new_opcode == 0x0005)
{
           //short register_opcode_y;
           //short opcode_subscript;
            register_opcode_x = opcode & 0x0f00;
           register_opcode_x = register_opcode_x >>8;
           register_opcode_y = opcode & 0x00f0;
           register_opcode_y = register_opcode_y >>4;

            opcode_subscript = opcode &0x00ff;
           if(array_register[register_opcode_x] == array_register[register_opcode_y])
                 pc = pc+2;

}



else if(new_opcode == 0x0006)
{
    //short opcode_subscript;
    register_opcode_x = opcode & 0x0f00;
    register_opcode_x = register_opcode_x >>8;
     opcode_subscript = opcode &0x00ff;
    if(register_opcode_x == 9)
       {  printf("%d\n",opcode_subscript);
          
       }
    
    array_register[register_opcode_x] = (unsigned char)(opcode_subscript);
    printf("array_register[%x] = %d\n",register_opcode_x,array_register[register_opcode_x]);
    
}



else if(new_opcode == 0x0007)
{
  //  short opcode_subscript;
    register_opcode_x = opcode & 0x0f00;
    register_opcode_x = register_opcode_x >>8;
     opcode_subscript = opcode &0x00ff;
     printf("register_opcode7 :%x",register_opcode_x);
     value =  array_register[register_opcode_x];
     array_register[register_opcode_x] = value + opcode_subscript;
     printf("array_register[register_opcode_x] :%x",array_register[register_opcode_x] );
}


else if(new_opcode == 0x008)
{
   last_fbits =  opcode & 0x000f;
   operand_x = opcode &0x0f00;
   operand_x = operand_x >> 8;
   operand_y = opcode & 0x00f0;
   operand_y = operand_y >> 4;
   operand_z = opcode & 0x00ff;
   
    if(last_fbits == 0x00)
    {
        value_y = array_register[operand_y];
        value_x = array_register[operand_x];
        array_register[operand_x] = value_y;


    }



     if(last_fbits == 0x1)
     { 

       value_y = array_register[operand_y];  
       value_x = array_register[operand_x];
       new_val = value_x | value_y;
       array_register[operand_x] = new_val;
      }
       

      if(last_fbits == 0x2)
     { 

       value_y = array_register[operand_y];  
       value_x = array_register[operand_x];
       new_val = value_x & value_y;
       array_register[operand_x] = new_val;
      }

     if(last_fbits == 0x3)
     { 

       value_y = array_register[operand_y];  
       value_x = array_register[operand_x];
       new_val = value_x ^ value_y;
       array_register[operand_x] = new_val;
      }

     if(last_fbits == 0x4)
     { 

       value_y = array_register[operand_y];  
       value_x = array_register[operand_x];
       new_val = value_x + value_y;
       if(new_val > 0xff)
         array_register[15] = 1;
       else
         array_register[15] = 0;

	array_register[operand_x] = value_x + value_y;
     }

     
     if(last_fbits == 0x5)
     { 

       value_y = array_register[operand_y];  
       value_x = array_register[operand_x];
       int temp = value_x - value_y;
       if(temp < 0)
         array_register[15] = 0;
       else
         array_register[15] = 1;

	array_register[operand_x] = value_x - value_y;

     }

     if(last_fbits == 0x6)
     { 

       //value_y = array_register[operand_y];  
       value_x = array_register[operand_x];

       new_val = value_x >>1;
        l_s_b = value_x & 0x01;
       
      
         array_register[15] = l_s_b;
      array_register[operand_x] = new_val;
     }

      if(last_fbits == 0x7)  //8XY7
     { 

       value_y = array_register[operand_y];  
       value_x = array_register[operand_x];
	int temp = value_y - value_x;
       if(temp < 0)
         array_register[15] = 0;
       else
         array_register[15] = 1;

	array_register[operand_x] = value_y - value_x;
      
     }

    if(last_fbits == 0xE)  //8XY8
     { 

       //value_y = array_register[operand_y];  
       value_x = array_register[operand_x];

       new_val = value_x <<1;
        m_s_b = value_x & 0x80;
       
      
         array_register[15] = m_s_b;  //set to m_s_b
	array_register[operand_x] = new_val;
      
     }


}


else if(new_opcode == 0x0009)
{

   operand_x = opcode &0x0f00;
   operand_x = operand_x >> 8;
   operand_y = opcode & 0x00f0;
   operand_y = operand_y >> 4;
   value_y = array_register[operand_y];  
   value_x = array_register[operand_x];
   if(value_x != value_y)
       pc = pc+2;


}


else if(new_opcode == 0x000A)
{
   operand_x =  opcode & 0x0fff;
   index_r = operand_x;
   printf("index_r: %x\n",index_r);
}

else if(new_opcode == 0x000B)
{
   operand_x = opcode & 0x0fff;
   value_x = array_register[0];
   operand_x = value_x + operand_x;
   pc = operand_x;

}
else if(new_opcode == 0x000C)
{  
    operand_x = opcode &0x0f00;
    srand((unsigned)time(&t));
    rand_2    = (char) (rand() %255);
    operand_nn =(char) opcode & 0x00ff;
    value_x =   rand_2 & operand_nn;
    array_register[operand_x] = value_x;

}


else if(new_opcode == 0x000D)
{
   operand_x = opcode &0x0f00;
   operand_x = operand_x >> 8;
    printf("operandx:%x",operand_x);

   operand_y = opcode & 0x00f0;
   operand_y = operand_y >> 4;
    printf("operandy:%x",operand_y);
   n_of_rows = opcode & 0x000f;
   value_y = array_register[operand_y];  
   value_x = array_register[operand_x];

   array_register[15] = 0;
for(loop_var = 0 ;loop_var <n_of_rows;loop_var ++)
{   pixel =  memory_gamulator[index_r + loop_var];

    for(loop_var_x = 0;loop_var_x <8;loop_var_x ++)
       {   
            pixel_state =  0x1<<loop_var_x;
           current_evaluated_pixel = pixel & pixel_state;
           if(current_evaluated_pixel != 0)
             {
                if(gamulc->display[value_x +(7-loop_var_x)][value_y + loop_var]  == 1)
                   array_register[15] = 1;
                    
                gamulc->display[value_x + (7-loop_var_x) ][value_y +loop_var] ^= 1;
             
             }
       }
}
}
else if(new_opcode == 0x000E)
{
   operand_x = opcode &0x0f00;
   operand_x = operand_x >> 8;
   operand_y = opcode & 0x00f0;
   operand_y = operand_y >> 4;
   value_x = array_register[operand_x];
    operand_x = (short)value_x;
    
    if(operand_y == 0x0009)
  {
     if(key_state[operand_x] == 1)
        pc = pc + 2;
   }

  if(operand_y == 0x000A)
{
    if(key_state[operand_x] == 0 )
        pc = pc + 2;
}



}






else if(new_opcode == 0x000F)
{
   operand_x = opcode &0x0f00;
   operand_x = operand_x >> 8;
   operand_y = opcode &0x00FF;
   //printf("Inside f\n");
   if(operand_y == 0x0007)
{
   array_register[operand_x] = delay_timer;
}

  if(operand_y == 0x000A)
 {
 while(1)
{
   if(flag_key == 1)
    {  
      for ( i =0;i < 16;i++)
     {
          if(key_state[i] == 1)
             value = i;
      }
      array_register[operand_x] = value;
     break;
    } 
   
}
 }


  else if(operand_y == 0x0015)
 {
     operand_x = opcode &0x0f00;
     operand_x = operand_x >> 8;
     delay_timer = array_register[operand_x];  
     printf("delay_timer f015: %d\n",delay_timer);           


 }


else if(operand_y == 0x0018)
 {
     operand_x = opcode &0x0f00;
     operand_x = operand_x >> 8;
     sound_timer = array_register[operand_x];             


 }


else if(operand_y == 0x001E)
 {
     operand_x = opcode &0x0f00;
     operand_x = operand_x >> 8;
     value = array_register[operand_x]; 
     index_r = index_r + value;
 }

if(operand_y == 0x0029)
{
     operand_x = opcode &0x0f00;
     operand_x = operand_x >> 8;
     value = array_register[operand_x];
      index_r = value * 5;
 
}

else if(operand_y == 0x0033)
{
   operand_x = opcode &0x0f00;
   operand_x = operand_x >> 8; 
   value = array_register[operand_x];
   last_digit = value % 10;
   remainder  = value /10;
   middle_digit = remainder % 10;
   first_digit = remainder/10;
   memory_gamulator[index_r + 2] = last_digit;
   memory_gamulator[index_r + 1] = middle_digit;
   memory_gamulator[index_r] = first_digit;       
 
}

else if(operand_y == 0x0055)
{
   operand_x = opcode &0x0f00;
   operand_x = operand_x >> 8; 
   for( i = 0; i<= operand_x; i++)
     {
      memory_gamulator[index_r+i] = array_register[i] ;
      //index_r++;
     }
 
}

else if(operand_y == 0x0065)
{

   operand_x = opcode &0x0f00;
   operand_x = operand_x >> 8; 
   for( i = 0; i<= operand_x; i++)
     {
      array_register[i] = memory_gamulator[index_r+i] ;
      //index_r++;
     }


}

}
}







/*void clear_display(gamul8 *gamulc)
{
  int i = 0;
  int j = 0;
   
for(i = 0;i <64;i++)
   for(j = 0;j <32;j++)
  {gamulc->display[i][j] = 0;
   }

}*/
    
    













/* ncc.c - number converted command: convert betweem binary, hexadecimal, and decimal */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(void){
  printf("incorrect number of arguemnets\n");
  printf("usage : ncc <number>\n");
  printf("Where one of binary, hexadecimal, decimal.\n" );
  printf("   binary (0b prefix, e.g., 0b0110\n" );
  printf("   hexadecimal (0x prefix), e.g., 0xF118\n" );
  printf("   decimal (no prefix) \n");
  printf("\n");
}

//--------------Check Functions----------------------------

bool is_binary(const char *numstr){

  bool rv= false;

  if( (strlen(numstr) >2) && (numstr[0]=='0')&& (numstr[1] == 'b') ){
    rv = true;
    int i;
    for(i = 2; i<strlen(numstr); i++ ){
      if( ! ( (numstr[i] == '0') || (numstr[i] == '1')  ) ) {
	rv = false;
	break;
      }
    }
  }
  return rv;
}

bool is_decimal(const char *numstr){
  bool is_dec = false;
  int i;
  int len = strlen(numstr);
  for(i = 0; i<len; i++){

    if( isalpha(numstr[i])) {
	return false;
    }
    
  }   
  return true;
}


bool is_hexadecimal(const char *numstr){
  int i;
  bool is_hex= false;
  char check[6] = {'A','B','C','D','E','F'};
  if( (strlen(numstr) > 2) && (numstr[0] == '0') && (numstr[1]=='x')){
   
    for(i = 2; i<strlen(numstr); i++){
      if( (numstr[i]>='A') && (numstr[i] <='F') || (isdigit(numstr[i]))  ){
	is_hex = true;
      }else{
	is_hex= false;
	break;
      }
    }
  }
  return is_hex;
}


//-------------- Coversion Methods -----------------------------//
    
int conv_binary_to_signed_int(char *binstr){
  /* converts binary to signed by creating temp string of inverted values
   * and finding the decimal representation inverted binary then adding by 1
   * at end reads the msb to see if its led by '1.' If so, make neg, else leave
   * alone
   */ 
  
  int len = strlen(binstr);
  int i;
  char temp[32];
  strcpy(temp,binstr);

  for( i = 0; i<len; i++){
    if(binstr[i]=='1'){
      temp[i]='0';
    }else{
      temp[i] = '1';
    }
  }

  int decimal_value_of_invert = conv_binary_to_unsigned_int(temp);
  decimal_value_of_invert +=1;

  if(binstr[0] == '1'){
    decimal_value_of_invert *= -1;
  }

  return decimal_value_of_invert;
}

int conv_binary_to_unsigned_int(char *binstr){
  int powsize = strlen(binstr)-1;
  int len = strlen(binstr);
  int i;
  
  int pfactor = 1<<powsize;
  unsigned int bin_value;
  unsigned int uint_value = 0;

  for (i = 0; i<len; i++){
    // converts binstring[i] to ascii and then substracts asccii 0 aka 48
    // if binstr[i] == "0" bin value will = 0
    // "              " "1" bin value will = 1

    bin_value = (int)(binstr[i] - '0');
    // the computaion
    uint_value += bin_value * pfactor;
    // divides by two
    pfactor = pfactor>>1;
  }
  
  return uint_value;
}



/* decimal to ~ functions ---------------------------*/

void  conv_decimal_to_binary_unsigned(int dec_val,char* bin_unsigned){
  int counter=0;
  char bin[32];

  int temp = dec_val*-1;
 
  if(temp != 0){
    while( temp  !=  0){
   
      if( (temp%2) == 0){
	bin[counter] ='0';
      }else{
	bin[counter] = '1';
      }
      temp=temp/2;
      counter++;
    }
 
    while(counter%4!= 0){
 
      bin[counter]='0';
      counter++;
    }
  }else{


    
 
    bin[counter] = '0';
 
    while (counter < 4){
      bin[counter] = '0';
      counter++;
    }
  }


  bin[counter] ='\0';
   
  char reversed_bin[32];
  int i;
 
  int c = strlen(bin)-1;
 
  for(i = 0; i<strlen(bin); i++){    
 
    reversed_bin[c-i] = bin[i];
  }
 
  reversed_bin[++c] = '\0';
  
 
  strcpy(bin_unsigned,reversed_bin);
}





void invert_bins(char* temp,char* bin_signed){

 
  //Two's Complement
  int len = strlen(temp);
  int i;
  for(i = 0; i<len;i++){
    if(temp[i] == '1'){
      temp[i]='0';
    }else if(temp[i] == '0'){
      temp[i] = '1';
    }
  }
}
  




void conv_decimal_to_binary_signed(int dec_val,char* bin_signed){

  // do decimal to unsigned binary, then two's complement


  char temp[32];
  char ifmsb[32];

  conv_decimal_to_binary_unsigned(dec_val,bin_signed);
  // check to see if there is a 1 in the msb
  // if there is, then add a 1 infront of it.
 

  if(bin_signed[0]=='1'){

     temp[0]='0';
     temp[1]='\0';
     strcat(temp,bin_signed);

     
     if( strlen(temp)%4!=0){
       int i= 0;
       int len = strlen(temp);
       while(len%4 != 0){
	 ifmsb[i]='0';
	 
	 i++;
	 len++;
       }
       ifmsb[i]='\0';
      
       strcat(ifmsb,temp);
      
     }
     strcpy(temp,ifmsb);
     
  }else{
    
    strcpy(temp,bin_signed);
  }
    
  
  
  invert_bins(temp,bin_signed);
  
  //sumulating addig by one
  int c =strlen(temp)-1;
 
  int i;
  for(i = 0; i<strlen(temp); i++){
    char carry = '1';
  
    if( temp[c] == '0'){
      temp[c] = '1';
  
      break;
  
    }else if( temp[c] == '1'){
      temp[c] = '0';
  
      c--;
    }
  }
  
  
  strcpy(bin_signed,temp);
 
}


char hex_table(int dec_val){
  char temp;
  switch(dec_val){
  case 0:
    temp='0';
    break;
  case 1:
    temp = '1';
    break;
  case 2:
    temp = '2';
    break;
  case 3:
    temp='3';
    break;
  case 4:
    temp='4';
    break;
  case 5:
    temp = '5';
    break;
  case 6:
    temp = '6';
    break;
  case 7:
    temp = '7';
    break;
  case 8:
    temp = '8';
    break;
  case 9:
    temp = '9';
    break;
  case 10:
    temp = 'A';
    break;
  case 11:
    temp = 'B';
    break;
  case 12:
    temp = 'C';
    break;
  case 13:
    temp = 'D';
    break;
  case 14:
    temp = 'E';
    break;
  case 15:
    temp = 'F';
    break;
  }
  return temp;
}

void hex_binary_table(char hex_val,char* temp_bin){

  char temp[32];
 
  switch(hex_val){
  case '0':
    strcat(temp_bin,"0000");
    break;
  case '1':
   strcat(temp_bin, "0001");
   break;
  case '2':
    strcat(temp_bin,"0010");
    break;
  case '3':
    strcat(temp_bin,"0011");
    break;
  case '4':
    strcat(temp_bin,"0100");
    break;
  case '5':
    strcat(temp_bin, "0101");
    break;
  case '6':
    strcat(temp_bin, "0110");
    break;
  case '7':
    strcat(temp_bin,"0111");
    break;
  case '8':
    strcat(temp_bin,"1000");
    break;
  case '9':
    strcat(temp_bin, "1001");
    break;
  case 'A':
    strcat(temp_bin, "1010");
    break;
  case 'B':
    strcat(temp_bin,"1011");
    break;
  case 'C':
    strcat(temp_bin,"1100");
    break;
  case 'D':
    strcat(temp_bin,"1101");
    break;
  case 'E':
    strcat(temp_bin,"1110");
    break;
  case 'F':
    strcat(temp_bin,"1111");
    break;
  }
 
 
  
}

void conv_unsigned_dec_to_hex(int dec_val,char* hex){
 
  char hex_str[32];
  int i;
  int temp= dec_val;
  int counter = 0;
  //check to see whether its one or not
  if(dec_val== 0){
    hex_str[counter++] = '0';
  }else{
    while(temp != 0){
      int remainder = temp%16;
      
      hex_str[counter] = hex_table(remainder); 
      counter+=1;
      temp = temp/16;
    }
  }
  
  hex_str[counter++] ='x';
  hex_str[counter++] = '0';
  
 
  hex_str[counter++] = '\0';

 
  char reversed_hex[32];
  
  int c = strlen(hex_str)-1;
  
  for(i = 0; i<strlen(hex_str); i++){     
    reversed_hex[c-i] = hex_str[i];
  }
  
  reversed_hex[++c] = '\0';
  
  strcpy(hex,reversed_hex);
}


void conv_signed_dec_to_hex(int dec_val,char * hex){
  char hex_str[32];
  char temp_str[32];
  int i;
  int temp= dec_val;
  int counter = 0;

  conv_decimal_to_binary_signed(temp,temp_str);
 
  int num = conv_binary_to_unsigned_int(temp_str);
 
    while(num != 0){
   
   
    int remainder = (num%16);
    
    temp_str[32];
        
    int lookup_num =remainder;

    hex_str[counter] = hex_table(lookup_num);
    counter+=1;
    num = num/16;
      
  }
  
  hex_str[counter++] ='x';
  hex_str[counter++] = '0';
 
  
  strcpy(temp_str,hex_str); 
  
  hex_str[counter++] = '\0';
 
  
  // To reverse string containing hex

 
 
  int c = strlen(hex_str)-1;
  
  for(i = 0; i<strlen(hex_str); i++){    
  
    hex_str[c-i] = temp_str[i];
  }
 
 
 
  strcpy(hex,hex_str);
}


void conv_hex_to_binary(char* hex_str,char* result){
  char bin_result[32];
  bin_result[0] = '0';
  bin_result[1] = 'b';
  bin_result[2] = '\0';

  int i;
   for(i =0 ; i< strlen(hex_str); i++){
   
     hex_binary_table(hex_str[i],bin_result);

   }
 

   strcpy(result,bin_result);
  

}

void conv_binary_to_hexadecimal(char *binstr ){
  int i;
  char four_comb[32];
  i = conv_binary_to_unsigned_int(binstr);
 
  conv_unsigned_dec_to_hex(i,four_comb);
 
  strcpy(binstr,four_comb);
}

int conv_hex_to_unsigned_decimal(char* hex_str){
  char temp_bin[32];
  conv_hex_to_binary(hex_str,temp_bin);
 
  char* noPrefix_bin = temp_bin+2;
 
  int result = conv_binary_to_unsigned_int(noPrefix_bin);
 
  return result;
}

int conv_hex_to_signed_decimal(char* hex_str){
  char temp_bin[32];
  conv_hex_to_binary(hex_str,temp_bin);
 
  char* noPrefix_bin = temp_bin+2;
 
  int result = conv_binary_to_signed_int(noPrefix_bin);
 
  return result;
}




void conv_decimal(char *numstr){
  int dec_val = atoi(numstr);
  char bin_signed[32];
  char bin_unsigned[32];
  char* spaced_bin;
  char hex[32];

  //decimal to base 2 unsigned

  printf("%d (base 10) ",dec_val);
  if(dec_val>=0){
    conv_decimal_to_binary_unsigned(dec_val, bin_unsigned);
    spaced_bin = bin_unsigned;

    //to space binary out
    
    int index =0;
    int counter = 1;
    while(index<strlen(spaced_bin)){
      if(counter%4 == 0){

	printf("%c ",spaced_bin[index]);
      }else{
	printf("%c",spaced_bin[index]);

      }
      index++;
      counter++;
    }

    
    printf(" (base 2) ");
    printf( "0b%s (base 2) ",bin_unsigned);
    conv_unsigned_dec_to_hex(dec_val,hex);
    printf(" %s (base 16)",hex);
  }else{
  
    conv_decimal_to_binary_signed(dec_val,bin_signed);
    spaced_bin = bin_signed;

    //to space binary out

    int index =0;
    int counter = 1;
    while(index<strlen(spaced_bin)){
      if(counter%4 == 0){

	printf("%c ",spaced_bin[index]);
      }else{
	printf("%c",spaced_bin[index]);

      }
      index++;
      counter++;
    }


    printf("(base 2) ");
   

    printf("0b%s (base 2) ",bin_signed);
    conv_signed_dec_to_hex(dec_val,hex);
    printf("%s (base 16)",hex);
  }

  
}

void conv_binary(char* numstr){

  char* bin_string;
  char* hex;
  
  // get pointer to start of binary digit in numstring

  bin_string =  numstr+2;
  hex =numstr+2;
  printf("%s (base 2)  ",numstr);

  // convert from binary to usigned
  int conv_to_unsigned = conv_binary_to_unsigned_int(bin_string);
  printf("%d (base 10 unsigned) ",conv_to_unsigned);

  //convert from binary to signed

  int conv_to_signed = conv_binary_to_signed_int(bin_string);
  printf("%d (base 10 signed) ",conv_to_signed);
  conv_binary_to_hexadecimal(hex);
  printf(" %s (base 16)", hex);
}


void conv_hexadecimal(char *numstr){
  char* hex_str;
  char* spaced_bin;
  char bin[32];
  printf("%s (base 16) ",numstr);
  hex_str = numstr+2;
 
    
  conv_hex_to_binary(hex_str,bin);
  spaced_bin = bin+2;
  int index =0;
  int counter = 1;
  while(index<strlen(spaced_bin)){
    if(counter%4 == 0){
      
      printf("%c ",spaced_bin[index]);
    }else{
      printf("%c",spaced_bin[index]);
      
    }
    index++;
    counter++;
  }
  printf(" (base 2) ");  
  printf(" %s (base 2) ",bin);
  int unsigned_decimal = conv_hex_to_unsigned_decimal(hex_str);
  printf("%d (base 10) ",unsigned_decimal);
  int signed_decimal = conv_hex_to_signed_decimal(hex_str);
  printf(" %d (base 10 signed) \n", signed_decimal);
}



int main(int argc, char **argv)
{
  char *numstr;

  if( argc != 2){
    print_usage();
    exit(-1);
  }

  

  numstr = argv[1];
  if(strlen(argv[1])>32){
    print_usage();
    exit(-1);
  }

  
  if( is_binary(numstr) ){
    conv_binary(numstr);

  }else if( is_decimal(numstr)){
    conv_decimal(numstr);
    
  }else if( is_hexadecimal(numstr) ){
    if(strlen(numstr) >10){
      print_usage();
      exit(-1);
    }
    conv_hexadecimal(numstr);
  }else{
    printf("number not a valid number\n");
    print_usage();
  }
  return 0;
}

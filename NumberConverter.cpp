#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <climits>

using namespace std;

void deallocate_array(int *);
void print_menu();
double todec(int *,int &,string,int);
void fromdec(int *, int &,double,int);
void push(int *,int &, int);
int pop(int *,int &);
bool check_full(int);
bool check_empty(int);
int hex_char_dec(char);
char get_hex_char(int);
void ask_value(string &, int&);
int get_desired_base(int);

/**
*		@author Victor Romero
*   		
*		This program converts numbers from any base into any other base.
*
*   Main driver of the number conversion program.  Here I deal with reading from user and selecting
*   the parameters of the methods I use to convert.
*
*/
int main()
{
   int from,*p, to,pos = 0;
   double decimal;
   string value;
   const int stack_size = 100;
   cout << "\t...Welcome To Your Friendly Converter...\n" << endl;
   p = new int[stack_size];
   do
   {
      print_menu();
      cout << "Convert from or exit: ";
      cin >> from;
      cin.ignore(INT_MAX,'\n');
      switch(from)
      {
               case 1: //binary
		 ask_value(value,to);
		 if(from == to) //base converting from and to is same print out value
		 {
		    cout << "From: " << value << " to " << value << endl;
		    break;
		 }
		 decimal = todec(p,pos,value,2); 
		 if(to == 3) //if desired output is decimal then print decimal
		 {
		    cout << "From: " << value << " to " << decimal << endl;
	  	    break;
		 }
		 to = get_desired_base(to);
		 if(to == -1)
		    break;
		 fromdec(p,pos,decimal,to); //pointer, position,number,base wanted
		 break;
                case 2: //octal
		 ask_value(value,to);
		 if(from == to)
		 {
		     cout << "From: " << value << " to " << value << endl;
		     break;
		 }
		 decimal = todec(p,pos,value,8);
		 if(to == 3)
		 {
		     cout << "From: " << value << " to " << decimal << endl;
	  	     break;
		 }
		 to = get_desired_base(to);
		 if(to == -1)
		    break;
		 fromdec(p,pos,decimal,to);
	 	 break;
               case 3: //decimal
		 ask_value(value,to);
		 if(from == to)
		 {
		    cout << "From: " << value << " to " << value << endl;
	                    break;
		 }
		 decimal = atof(value.c_str());
		 if(to == 3)
		 {
		    cout << "From: " << value << " to " << decimal << endl;
 		    break;
		 }
		 to = get_desired_base(to);
		 if(to == -1)
		    break;
		 fromdec(p,pos,decimal,to);
		 break;
               case 4://hex
		 ask_value(value,to);
		 if(from == to)
		 {
		    cout << "From: " << value << " to " << value <<endl ;
		    break;
		 }
		 decimal = todec(p,pos,value,16);
		 if(to == 3)
		 {
		    cout << "From: " << value << " to " << decimal << endl;
		    break;
		 }
		 to = get_desired_base(to);
		 if(to == -1)
		    break;
		 fromdec(p,pos,decimal,to);
		 break;
              case 5:
		 deallocate_array(p);
		 cout << "\nProgram now Terminated!!" << endl;
		 system("pause");
		 return 0;
         }	
         system("pause");
         system("cls");
   }while(true);
   system("pause");
   return 0;
}

/**
*  This function asks the user for the value he wishes to convert.
*  It then asks the user for the base he wishes to convert to.
*/
void ask_value(string &value, int &to)
{
   cout << "Enter a value you wish to convert: ";
   cin >> value;
   cout << "Enter the base you wish to convert to: ";
   cin >> to;
}

/**
*  This program takes in the base the user entered as a 1-4
*  option and returns the number of the base ex) 1 ==> 2
*  2 ==> 8 3==> 10 4 ==> 16
*/
int get_desired_base(int x)
{
   switch(x)
   {
      case 1:
	  return 2;
      case 2: 
	  return 8;
      case 3: 
	  return 10;
      case 4:
	  return 16;
      case 5: 
	  cout << "Invalid selection!!" << endl;
	  return -1;
   }
}

/**
*  This function converts a decimal number to any base. The desired base is passed as a parameter.
*   The result is not returned but it is printed directly.
*/
void fromdec(int *p, int &pos, double value, int base)
{
   int quotient = (int)value;
   while(quotient >= base)
   {
      push(p,pos,quotient%base);
      quotient /= base;
   }
   push(p,pos,quotient);
   if(base == 16)
      cout << "0x";
   while(!check_empty(pos))
   {
      if(base == 16)
         cout << get_hex_char(pop(p,pos));
     else
         cout << pop(p,pos);
   }
   cout << endl;
}

/**
*   This function receives a hex number and returns a character.
*   The purpose of this is to print letters A-F.
*/
char get_hex_char(int x)
{
   switch(x)
   {
         case 10:
  	     return 'A';
         case 11:
	     return 'B';
         case 12:
	     return 'C';
         case 13:
	     return 'D';
         case 14:
	     return 'E';
         case 15:
	     return 'F';
         default:
	    return to_string(x)[0];
   }
}

/**
*  This function receives a value in any base and returns the decimal value of it.
*/
double todec(int *p, int &pos, string value,int base)
{
   double result = 0;
   int temp;
   string curr;
   while(value.length() > 0)
   {
       curr = value.substr(value.length()-1);
       if(base == 16)
           temp = hex_char_dec(curr[0]);
       else
           temp = atoi(curr.c_str());
       push(p,pos,temp); //gets last digit of string and makes it to int
       value = value.substr(0,value.length()-1); //removes last element from string
   }
   while(!check_empty(pos))
   {
      temp = pop(p,pos);
      result += temp * (pow(base,pos)); //pop returns the value of index pos-1 which is top of stack and decrements 
 	   		      //pos so no decrement is needed with the power pos
   }
   return result;
}

/**
*  This function takes in a character which may be a hex letter and returns 
*  the equivalent value of that letter or number in decimal format. 
*
*/
int hex_char_dec(char c)
{
   switch(c)
  {
        case 'a': 
	return 10;
       case 'A':
	return 10;
       case 'b': 
	return 11;
       case 'B': 
	return 11;
       case 'c':
	return 12;
       case 'C':
	return 12;
       case 'd': 
 	return 13;
       case 'D':
	return 13;
       case 'e':
	return 14;
       case 'E':
	return 14;
       case 'f':
	return 15;
       case 'F': 
 	return 15;
       default:
	string t = "";
	t += c;
	return atoi(t.c_str());
   }
}

/**
*  This function pushes a value into the stack. 
*  This also checks if the stack is full and if it is then an error is printed
*  the program is then terminated.
*/
void push(int *ptr,int &pos, int val)
{
   if(check_full(pos))
   {
       cout << "Stack overflow!!" << endl;
      deallocate_array(ptr);
      system("pause");
      exit(1);
   }
   *(ptr + pos++) = val;
}

/**
*  This function pops and returns a value from the stack.
*  This function also checks if the stack is already empty and if it is
*  a message is printed and the program exits.
*/
int pop(int *ptr, int &pos)
{
   if(check_empty(pos))
   {
      cout << "Stack is empty!!" << endl;
      deallocate_array(ptr);
      system("pause");
      exit(1);
   }
   return *(ptr + (--pos));
}

/**
*   Function to check if stack is full
*/
bool check_full(int position)
{
   if(position >99)
      return true;
   return false;
}

/**
*  Function to check if stack is empty
*/
bool check_empty(int pos)
{
   if(pos == 0)
      return true;
   else
      return false;
}

/**
*  This function prints out the menu which asks the user to select
*  an option and it then prints out the possible options.
*
*/
void print_menu()
{
   cout << "\t...Select from the following options...\n" << endl;
   cout << "1) Binary\n" 
          << "2) Octal\n"
          << "3) Decimal\n"
          << "4) Hexadecimal\n" 
          << "5) exit program"
          << endl;
}

/**
*   This function deletes the dynamically allocated array. 
*   It sets the address of pointer p to 0 for security.
*/

void deallocate_array(int *p)
{
   delete [] p;
   p = 0;
}
 

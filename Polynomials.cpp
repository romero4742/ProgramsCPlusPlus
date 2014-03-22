#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <cmath>


using namespace std;

/**
*	@author Victor Romero
*
*	This program performs mathematical operations on polynomial functions.
*
*/

class Polynomial
{
private:
	double **p;
	int rows,cols;

public:
	Polynomial(){}
	Polynomial(string);
	Polynomial(const Polynomial &);
	~Polynomial();
	void print();
	void setPolynomial(string);
	Polynomial operator+(const Polynomial &);
	Polynomial operator-(const Polynomial &);
	Polynomial operator*(const Polynomial &);
	Polynomial& operator=(const Polynomial &);
	void sort();
	void swap(int,int);
	void removeVariables(string &);
	int getRequiredSize(const Polynomial &) const;
};

/**
*
*   Prototype function to print the main menu of the program
*/
void displayMenu();

/**
*   This is the copy constructor.
*/
Polynomial::Polynomial(const Polynomial &obj)
{
	rows = obj.rows;
	cols = 2;
	p = new double*[rows];
	for(int i = 0; i < rows; i++)
	{
		*(p+i) = *(obj.p+i);
	}
}

/**
*  constructor which allows the Polynomial to be initialized with a string
*  
*/
Polynomial::Polynomial(string str)
{
	rows = 0;
	cols = 2;
	setPolynomial(str);
}

/**
*
*Destructor which deletes all the dynamically allocated memory
*/
Polynomial::~Polynomial()
{
   for(int i = 0; i < rows; i++)
      delete [] *(p+i);
   delete [] p;
   p = 0;
}

/**
*   This function receives a string and removes all variables to allow my 
*   other functions to process the string
*   ex) in: 1x^2+2x^3    out: 1^2+2^3
*/
void Polynomial::removeVariables(string &str)
{
	for(int i = 0; i < str.length(); i++)
	{
	   if(str[i] == 'x' || str[i] == 'X')
	   {
		   str = str.substr(0,i)+str.substr(i+1,str.length()-1);
	   }
	}
}

/**
*  This function is one of the main drivers of the program.
*  Here I calculate the space needed for the dynamic memory allocation.
*  I then allocate the memory and follow by adding the bases and powers to 
*  the array. After completing the array I sort the array as to have
*  it from least power to greatest power.
*/
void Polynomial::setPolynomial(string str)
{
	removeVariables(str);
	rows = 1; //leave at one this is right
	cols = 2;
	int i = 1,currentIndex = 0;
	string temp= "";
	for(; i < str.length();i++) //count the number of rows required for dynamic allocation
	{
	   if((str[i] == '+' || str[i] == '-') && str[i-1] != '^' && str[i-1] != '+' && str[i-1] != '-') //if operator read is not a unary then add rows
		   rows++;
	}
	//allocate memory
	p = new double*[rows];
	for(i = 0; i < rows; i++)
		p[i] = new double[cols];
	
	i = 1;
	while(str.length() > 0) //begin string traversal
	{
		temp = str[0];//first character negative or a digit
		while(i < str.length() && str[i] != '^' && str[i] != '+' && str[i] != '-')//get the digits until we hit an operator this is the base
		   temp += str[i++];
		*(*(p+currentIndex)+0) = atof(temp.c_str()); //add the base to array
		temp = ""; //reset to hold power
		if(str[i] == '^') //if we reached a power then read the power
		{
		   if(str[++i] == '-') //check if first character is not a digit but a unary -
			   temp += str[i++];
		   while(i < str.length() && str[i] != '+' && str[i] != '-') //read the digits of the power until we reach the end or an operator
			   temp += str[i++];
		   *(*(p+currentIndex)+1) = atof(temp.c_str()); //add the power to the array
		   currentIndex++;
		   i++;
		}
		else if(str[i] == '+') //else if we hit an operator and not a ^ the power is set to 0
		{
		   *(*(p+currentIndex)+1) = 0.0;
		   currentIndex++;
		   i++;
		}
		else //the operator must be - so we set the power also to 0
		{
		   *(*(p+currentIndex)+1) = 0;
		   currentIndex++;
		} 
		if(i >= str.length() ) //its needed don't play with it again
		{
			break;
		}
		else
		{
		   str = str.substr(i,str.length());//remove what we already used from the string
		   i = 1;
		}
	}
	sort();
}

/**
*  This method sorts the array by comparing the powers.
*  I used selection sort as my algorithm since the size
*  of the array is usually not huge
*/
void Polynomial::sort()
{
	double max;
	int maxIndex;
	for(int i = rows-1; i > 0; i--)
	{
	   max = *(*(p)+1);
	   maxIndex = 0;
	   for(int j = 0; j <= i; j++)
	   {
	      if(max < *(*(p+j)+1))
		  {
		     max = *(*(p+j)+1);
			 maxIndex = j;
		  }
	   }
	   swap(i,maxIndex);
	}
}

/**
*   This function swaps the values of two indeces passed in. 
*   It swaps the power and the base
*/
void Polynomial::swap(int left,int right)
{
   double temp =  *(*(p+left)+0);
   *(*(p+left)+0) = *(*(p+right)+0); //swap bases
   *(*(p+right)+0) = temp;
    temp = *(*(p+left)+1);
   *(*(p+left)+1) = *(*(p+right)+1); //swap powers
   *(*(p+right)+1) = temp;
}

/**
*  This function is used by the overloaded + and -.
*  I use this function to calculate the space required
*  for a new dynamic array used to hold the result of the calculation
*/
int Polynomial::getRequiredSize(const Polynomial &obj) const
{
	int size = 0;
	int thisCounter = 0, objCounter = 0;
    while(thisCounter < rows && objCounter < obj.rows) //find size of new array
    {
      if(*(*(p+thisCounter)+1) == *(*(obj.p+objCounter)+1)) //powers are equal so we only add the bases
	  {
		  size++;
		  thisCounter++;
		  objCounter++;
	  }
	  else //the powers are not equal so we need to add 2 positions
	  {
		  size += 2;
		  thisCounter++;
		  objCounter++;
	  }
	}
    while(thisCounter < rows)
	{
	    size++;
	    thisCounter++;
    }
    while(objCounter < obj.rows)
    {
	    size++;
	    objCounter++;
	}//size required is calculated
	return size;
}

Polynomial& Polynomial::operator=(const Polynomial &obj)
{
	if(this == &obj)
		return *this;
	delete [] p;
	rows = obj.rows;
	cols = obj.cols;
	p = new double*[this->rows];
	for(int i = 0; i < rows; i++)
	   *((p)+i) = new double[2];
	for(int i = 0; i < rows; i++)
	{
	   *(*(p+i)+0) = *(*(obj.p+i)+0);
	   *(*(p+i)+1) = *(*(obj.p+i)+1);
	}
	return *this;
}

/**
*  Overloaded * operator which calculates what the product of multiplying 
*  two polynomials is. The product Polynomial object is then returned.
*/
Polynomial Polynomial::operator*(const Polynomial &obj)
{
	Polynomial *temp = new Polynomial;
	temp->rows = rows*obj.rows;
	temp->p = new double*[temp->rows];
	for(int i = 0; i < temp->rows; i++)
		*((temp->p)+i) = new double[2];
	int pos = 0;
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j< obj.rows ; j++)
		{
		   *(*(temp->p+pos)+0) = *(*(p+i)+0) * *(*(obj.p+j)+0);
		   *(*(temp->p+pos++)+1) = *(*(p+i)+1) + *(*(obj.p+j)+1);
		}
	}
	return *temp;
}

/**
*  Overloaded - operator which calculates the subtraction between two polynomials and returns
*  the result.
*/
Polynomial Polynomial::operator-(const Polynomial &obj)
{
	Polynomial *temp = new Polynomial;
	temp->rows = getRequiredSize(obj);
	temp->p = new double*[temp->rows];
	for(int i = 0; i < temp->rows ; i++)
		temp->p[i] = new double[2];
	int thisCounter = 0, objCounter = 0, tempCounter = 0;
	while(tempCounter < temp->rows && thisCounter < rows && objCounter < obj.rows)
	{
	   if(*(*(p+thisCounter)+1) == *(*(obj.p+objCounter)+1))
	   {
	       *(*(temp->p + tempCounter)+0) = *(*(p+thisCounter)+0) - *(*(obj.p + objCounter++)+0); //subtract the bases and store in new array
		   *(*(temp->p + tempCounter++)+1) = *(*(p+thisCounter++)+1);//add the power to the new array
	   }
	   else if(*(*(p+thisCounter)+1) < *(*(obj.p+objCounter)+1))//powers are not equal then add the smallest to new array and move that counter
	   {
		   *(*(temp->p + tempCounter)+0) = *(*(p+thisCounter)+0);
		   *(*(temp->p + tempCounter++)+1) = *(*(p+thisCounter++)+1);
	   }
	   else //add the smallest power to the new array
	   {
		   *(*(temp->p+tempCounter)+0) = *(*(obj.p+objCounter)+0);
		   *(*(temp->p+tempCounter++)+1) = *(*(obj.p+objCounter++)+1);
	   }
	}
    while(thisCounter < rows) //add the remaining powers to the array
    {
      *(*(temp->p + tempCounter)+0) = *(*(p+thisCounter)+0);
      *(*(temp->p + tempCounter++)+1) = *(*(p+thisCounter++)+1);
    }
	while(objCounter < obj.rows) // add the remaining powers to the array
    {
      *(*(temp->p+tempCounter)+0) = -1**(*(obj.p+objCounter)+0);
      *(*(temp->p+tempCounter++)+1) = *(*(obj.p+objCounter++)+1);
    }
	return *temp;
}

/**
*  Overloads the + operator. It calculates what the addition between two polynomials is and returns the 
*  resulting polynomial.
*/
Polynomial Polynomial::operator+(const Polynomial &obj)
{
   Polynomial *temp = new Polynomial;
   temp->rows = getRequiredSize(obj); //get required size
   temp->p = new double*[temp->rows]; 
   for(int i = 0; i < temp->rows; i++)
	   temp->p[i] = new double[2];
   int thisCounter = 0, objCounter = 0,tempCounter = 0;
   while(thisCounter < rows && objCounter < obj.rows)
   {
	   if(*(*(p+thisCounter)+1) == *(*(obj.p+objCounter)+1)) //compare the powers to check if addition if valid
	   {
		   *(*(temp->p + tempCounter)+0) = *(*(p+thisCounter)+0) + *(*(obj.p + objCounter++)+0); //add the bases and store in new array
		   *(*(temp->p + tempCounter++)+1) = *(*(p+thisCounter++)+1);//add the powers and store in new array
	   }
	   else 
	   if(*(*(p+thisCounter)+1) < *(*(obj.p+objCounter)+1))//powers are not equal then add the smallest to new array and move that counter
	   {
		   *(*(temp->p + tempCounter)+0) = *(*(p+thisCounter)+0);
		   *(*(temp->p + tempCounter++)+1) = *(*(p+thisCounter++)+1);
	   }
	   else
	   {
		   *(*(temp->p+tempCounter)+0) = *(*(obj.p+objCounter)+0);
		   *(*(temp->p+tempCounter++)+1) = *(*(obj.p+objCounter++)+1);
	   }
   }
   while(thisCounter < rows)
   {
      *(*(temp->p + tempCounter)+0) = *(*(p+thisCounter)+0);
      *(*(temp->p + tempCounter++)+1) = *(*(p+thisCounter++)+1);
   }
   while(objCounter < obj.rows)
   {
      *(*(temp->p+tempCounter)+0) = *(*(obj.p+objCounter)+0);
      *(*(temp->p+tempCounter++)+1) = *(*(obj.p+objCounter++)+1);
   }
   return *temp;
}

/**
*  This function prints out the Polynomials in starndard format with variables included
*  ex) 1+2x^2+3x^3
*/
void Polynomial::print()
{
   if(*(*(p+0)+1) == 0 )
      cout << **p;
   else
	   cout << **p << "X^" << *(*(p+0)+1);
   for(int i = 1; i < rows; i++)
   {
      if(*(*(p+i)+0) < 0)
		  cout <<setprecision(3)<< (*(*(p+i)+0)) << "X^" <<*(*(p+i)+1);
	  else
		  cout <<setprecision(3)<<"+"<< (*(*(p+i)+0)) << "X^" <<*(*(p+i)+1);
   }
}

/**
*   This function displays the main menu to the user.
*/
void displayMenu()
{
   cout << "Select one of the following options: \n"
	    << "1) Add polynomials\n"
		<< "2) Subtract polynomials\n"
		<< "3) Multiply polynomials\n"
		<< "4) Assignment\n"
		<< "5) Print polynomials\n"
		<< "6) Exit "
		<< endl;
}

/**
*  Main driver of the whole program. Here I ask the user for all input and initialize both of my polynomial objects with their corresponding
*  constructors and functions.
*  Here I control the program by asking the user what he wishes to do and then completing that action
*/
int main()
{
   string input;
   Polynomial poly1, poly2;
   bool running = true;
   int command, sizePoly1 = 0, sizePoly2 = 0;
   cout << "\t...Welcome to Polynomial Calculator...\n" << endl;
   cout << "\nEnter the polynomial with or without variables \nNo \"^\" is required for ^0...\nEx) 1+2^2+3^3+4^4 or 1x^2+2x^2...\n" << endl;
   cout << "Enter polynomial 1: ";
   cin >> input;
   poly1.setPolynomial(input);
   cout << "Enter polynomial 2: ";
   cin >> input;
   poly2.setPolynomial(input);
   do
   {
      displayMenu();
	  cin >> command;
	  switch(command)
	  {
	     case 1:
				  cout << "("; poly1.print(); cout << ") + (";poly2.print(); cout << ") = ";
				  (poly2 + poly1).print();
				  cout << endl;
			      break;
		 case 2:
				 cout << "(";poly1.print();cout << ") - (";poly2.print();cout << ") = ";
				 (poly1 - poly2).print();
				 cout << endl;
			     break;
		 case 3:
				 cout << "(";poly1.print();cout<<") * (";poly2.print();cout << ") = ";
				 (poly1*poly2).print();
				 cout << endl;
			     break;
		 case 4:
			    {
			      Polynomial temp = poly1;
				  poly1 = poly2;
				  poly2 = temp;
				  cout << "New Polynomial 1: "; poly1.print(); cout << endl;
				  cout << "New Polynomial 2: "; poly2.print(); cout << endl;
			      break;
			    }
		 case 5:
			      cout << "...Now printing polynomials...\n"
					   << "Polynomial 1: " ;
				  poly1.print();
				  cout << endl;
				  cout << "\nPolynomial 2: ";
				  poly2.print();
				  cout << endl;
				  break;
	     case 6:
			      running = false;
				  break;
		 default: 
			      cout << "Invalid input try again..." << endl;
				  break;
	  }
	  system("pause");
	  system("cls");
   }
   while(running);
   return 0;
}
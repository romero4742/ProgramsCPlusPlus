#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <climits>

using namespace std;

int initial_menu();
void display_menu();
void read_user(int **,int,int);
void deallocate(int **, int);
void print_matrix(int **, int, int);
void transpose(int **,int,int);
void add_matrices(int **,int**,int,int);
void sub_matrices(int **,int**,int,int);
void mult_matrices(int **, int, int,int**,int);
void read_file(int **, int, int, int**,int, int,ifstream &);
void fill_matrix(int **, int, int, ifstream &);

/**
* 		@author Victor Romero
*
*	This program performs mathematical operations on matrices.
*  
*  Driver of the whole program.  Here I deal with keeping the matrices variables and taking input on the commands
*  on what action the user wishes to perform.
*
*/
int main()
{
   int** matrix_one,** matrix_two, matrix_one_rows,matrix_one_cols,matrix_two_rows,matrix_two_cols,command;

   cout <<"\t ...Matrix Calculator...\n" << endl;
   command = initial_menu();
   if(command == 1)
   {
         cout <<"Enter the dimensions of array 1 <rows> <columns>";
         cin >> matrix_one_rows >> matrix_one_cols;
         cin.ignore(INT_MAX,'\n');
         matrix_one = new int*[matrix_one_rows];
         cout <<"Enter the dimensions of array 2 <rows> <columns>";
         cin >> matrix_two_rows >> matrix_two_cols;
         cin.ignore(INT_MAX,'\n');
         matrix_two = new int*[matrix_two_rows];
         cout << "Enter the values for first matrix" << endl;
         read_user(matrix_one,matrix_one_rows,matrix_one_cols);
         cout << "Enter the values for the second matrix" << endl;
         read_user(matrix_two, matrix_two_rows,matrix_two_cols);
   }
   else
   {
         cout << "Enter the name or path of file: ";
         string path;
         getline(cin,path,'\n');
         ifstream input;
         input.open(path.c_str());
         if(input.fail())
         {
               cout << "File " << path << " was not opened!!"<<endl;
               system("pause");
               return 1;
         }
         input >> matrix_one_rows >> matrix_one_cols >> matrix_two_rows >> matrix_two_cols;
         matrix_one = new int*[matrix_one_rows];
         matrix_two = new int*[matrix_two_rows];
         read_file(matrix_one, matrix_one_rows, matrix_one_cols, matrix_two, matrix_two_rows,matrix_two_cols,input);
   }
   bool running = true;
   do
   {
         display_menu();
         cin >> command;
         cin.ignore(INT_MAX,'\n');
         switch(command)
         {
            case 1: 
	       if(matrix_one_rows != matrix_two_rows || matrix_one_cols != matrix_two_cols)
	       {
	           cout << "Addition cannot be performed on these matrices...\n"
	                  << "Matrices must have the same size"
	                  <<  endl;
	       }
	       else
	             add_matrices(matrix_one, matrix_two, matrix_two_rows, matrix_two_cols);
	       system("pause");
	       break;
            case 2:
	       if(matrix_one_rows != matrix_two_rows || matrix_one_cols != matrix_two_cols)
	      {
	          cout << "Subtraction cannot be performed on these matrices...\n"
  	                 << "Matrices must have the same size"
		 <<  endl;
	      }
	      else
	            sub_matrices(matrix_one, matrix_two, matrix_two_rows, matrix_two_cols);
	      system("pause");
	      break;
            case 3:
	       if(matrix_one_cols != matrix_two_rows)
	       {
	          cout << "Multiplication cannot be performed on these matrices...\n"
		 << "The amount of columns in matrix one must be equal to the amount of rows in matrix 2"
		 << endl;
	       }
	       else
	             mult_matrices(matrix_one,matrix_one_rows,matrix_one_cols,matrix_two,matrix_two_cols);
  	       system("pause");
	       break;
            case 4:
	       cout << "Enter the matrix you wish to transpose: ";
	       cin >> command;
	       cin.ignore(INT_MAX,'\n');
	       if(command == 1)
	      {
	          cout <<"\t...Transpose of Matrix 1...\n"<<endl;
  	          transpose(matrix_one,matrix_one_rows,matrix_one_cols);
	      }
	      else
	            if(command == 2)
	            {
	                cout <<"\t...Transpose of Matrix 2...\n"<<endl;
	                transpose(matrix_two,matrix_two_rows,matrix_two_cols);
	            }
	            else
	                  cout << "Invalid matrix selection.. only matrices are 1 and 2..."<<endl;
                            system("pause");
	            break;
            case 5:
           	      cout << "\t...Matrix 1...\n" << endl;
	      print_matrix(matrix_one,matrix_one_rows,matrix_one_cols);
	      cout << "\n\t...Matrix 2...\n" << endl;
	      print_matrix(matrix_two,matrix_two_rows,matrix_two_cols);
	      system("pause");
	      break;
            case 6:
	       deallocate(matrix_one,matrix_one_rows);
  	       deallocate(matrix_two,matrix_two_rows);
	       running = false;
	       break;
         }
         system("cls");
   }
   while(running);
   return 0;
}

/**
 *  This function prints the options that the program can perform on the matrices.
 *  This is the main menu of the program where the user can select what he wishes to do with the matrices.
 */
void display_menu()
{
   cout << "\t...Enter the number of one of the following operations...\n\n"
          << "1) Add the matrices\n"
          << "2) subtract the matrices\n"
          << "3) multiply the matrices\n"
          << "4) transpose of a matrix\n"
          << "5) print matrices\n"
          << "6) exit program\n"
          << "command: ";
}

/**
*   This function prints out the multiplication of two matrices.  It performs the calculation and then it prints out 
*   the result in a matrix format. This method does not return a matrix it just prints out the result of the multiplication
*   in a format that seems like a matrix.
*/
void mult_matrices(int **matrix1,int matrix_one_rows,int matrix_one_cols,int **matrix2,int matrix_two_cols)
{
   int result = 0;
   for(int i = 0; i < matrix_one_rows; i++)
   {
      for(int j = 0; j < matrix_two_cols; j++)
      {
          for(int k = 0; k < matrix_one_cols; k++)
          {
              result += *(*(matrix1+i)+k) * *(*(matrix2+k)+j);
          }
          cout << result << "  ";
          result = 0;
      }
      cout << endl;
   }
}

/**
*  This function subtracts two matrices.  The size of the matrices has been checked 
*   in the main so the subtraction can be performed without checks.
*/
void sub_matrices(int ** matrix1, int **matrix2, int rows, int cols)
{
   for(int i = 0; i < rows; i++)
   {
      for(int j = 0; j < cols; j++)
      {
          cout <<*(*(matrix1+i)+j) - *(*(matrix2 + i)+j) << "   ";
      }
      cout << endl;
   }
}

/**
*  This function adds two matrices. The size of the matrices has already been checked in the main
*  so the addition can be performed without checks. The function prints the values as the addition
*  is solved.  It does not return a matrix.
*/
void add_matrices(int **matrix1,int **matrix2,int rows, int cols)
{
   for(int i = 0; i < rows; i++ )
   {
       for(int j = 0; j < cols; j++)
      {
         cout << (*(*(matrix1 + i)+j) + *(*(matrix2+i)+j)) << "   ";
      }
      cout << endl;
   }
}

/**
*   This function prints the tranpose of a passed in matrix.
*   It prints the transpose and does not return a matrix.
*/
void transpose(int **matrix,int rows, int cols)
{
   for(int i = 0; i < cols; i++)
   {
      for(int j = 0; j < rows; j++)
      {
         cout << *(*(matrix+j)+i) << "  ";
      }
      cout << endl;
   }
}

/**
*  This method prints a matrix which is passed in as a parameter.
*/
void print_matrix(int ** matrix, int rows, int cols)
{	
   for(int i = 0; i < rows; i++)
   {
      for(int j = 0; j < cols; j++)
      {
          cout <<*(*(matrix+i)+j)<< "   ";
      }
      cout << "\n"<<endl;
   }
}

/**
*  This function serves as a small driver for reading a file.  It displays a message
*  when the file is being read and when it has finished.  It uses a helper method
*  which reads the files into the matrices.
*
*	         ******************************
*                             * format for file should be: *
***********************************************************************************   
***********************************************************************************
*   size of the matrices is first line of file						*
*   values for both matrices are in second line					*
*   matrix values delimited by space						*
***********************************************************************************
*
*/
void read_file(int **matrix1, int matrix1_rows, int matrix1_cols, int **matrix2, int matrix2_rows, int matrix2_cols, ifstream &input_file)
{
   cout << "\nNow reading file..." <<endl;
   fill_matrix(matrix1, matrix1_rows,matrix1_cols,input_file);
   fill_matrix(matrix2, matrix2_rows,matrix2_cols,input_file);
   input_file.close();
   cout << "File was read succesfully!!" << endl;
}

/**
*  This function reads the values from a file and fills the matrices. 
*  This is a helper method used to read a matrix.
*/
void fill_matrix(int **matrix,int rows,int cols,ifstream &file)
{
   //dynamically allocate memory
   for(int i = 0; i < rows; i++)
   {
      matrix[i] = new int[cols];
   }
    //read file and assign values to array
    int temp;
    for(int i = 0; i < rows; i++)
    {
       for(int j = 0; j < cols; j++)
       {
           if(file.eof())
          {
              cout << "Illegal format of file check the amount of rows and columns!!" << endl;
              exit(1);
          }
          file >> temp;
         *(*(matrix + i) + j) = temp;
      }
   }
}

/**
*  This method takes in a matrix and proceeds to dynamically allocate memory
*  The values are read from the user
*/
void read_user(int **matrix,int rows,int cols)
{
   //dynamically allocate memory
   for(int i = 0; i < rows; i++)
   {
      matrix[i] = new int[cols];
   }
   //fill array
   for(int i = 0; i < rows; i++)
   {
      for(int j = 0; j < cols; j++)
      {
         cin >>  *(*(matrix+i) + j);
      }
   }
   cin.ignore(INT_MAX,'\n');
}


/**
 * Function asks the user to select whether they will enter the matrix
 * values directly or if they will be read from a file.
 */
int initial_menu()
{
   bool running = true;
   do
   {
      cout << "Enter <1> to read from the user\n"
             << "Enter <2> to read from a file" << endl;
      int option;
     cin >> option;
     cin.ignore(INT_MAX,'\n');
     switch(option)
     {
         case 1:
	    running = false;
  	    return 1;
  	    break;
         case 2:
	    running = false;
	    return 2;
	    break;
         default:
	    cout << "Invalid command try again!!" << endl;
      }
   }while(running);
}

/**
*  This method receives a matrix as a parameter and proceeds to free all of the
*  dynamically generated memory.
*/
void deallocate(int **matrix, int rows)
{
   for(int i = 0; i < rows; i++)
   {
      delete [] *(matrix+i);
   }
   delete [] matrix;
   matrix = 0;
}

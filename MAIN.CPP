#include <stdlib.h>
#include "EDITOR.H"
 
int main(int argc, char *argv[]) // count, values of command-line arguments
/* 
 
Pre:  Names of input and output files are given as command-line arguments.
Post: Reads an input file that contains lines (character strings),
performs simple editing operations on the lines, and
writes the edited version to the output file.
Uses: methods of class Editor
 
*/
{
   if (argc != 3) {
      cout << "Usage:\n\t edit  inputfile  outputfile" << endl;
      exit (1);
   }

   ifstream file_in(argv[1]);   //  Declare and open the input stream.
   if (file_in == 0) {
      cout << "Can't open input file " << argv[1] << endl;
      exit (1);
   }

   ofstream file_out(argv[2]);   //  Declare and open the output stream.
   if (file_out == 0) {
      cout << "Can't open output file " << argv[2] << endl;
      exit (1);
   }

   Editor buffer(&file_in, &file_out);
   while (buffer.get_command())
      buffer.run_command();
}

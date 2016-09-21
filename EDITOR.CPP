#include "../../C/UTILITY.H"
#include "EDITOR.H"
bool user_says_yes()
{
   if(cin.get()=='y')
	   return true;
   else
	   return false;
}

Error_code Editor::next_line()
{
   if (current->next == NULL) return fail;
   else {
      current = current->next;
      current_position++;
      return success;
   }
}
 
Error_code Editor::previous_line()
{
   if (current->back == NULL) return fail;
   else {
      current = current->back;
      current_position--;
      return success;
   }
}
 
Error_code Editor::goto_line()
{
   int line_number;
   cout << " Go to what line number? " << flush;
   cin >> line_number;
   if (line_number > size() - 1) return fail;
   else {
      set_position(line_number);
      return success;
   }
}
 
Error_code Editor::insert_line()
/* 
 
Post: A string entered by the user is inserted as a user-selected line number.
Uses: String and Editor methods and functions.
 
*/
{
   int line_number;
   cout << " Insert what line number? " << flush;
   cin  >> line_number;
   while (cin.get() != '\n');
   cout << " What is the new line to insert? " << flush;
   String to_insert = read_in(cin);
   return insert(line_number, to_insert);
}
 
Error_code Editor::substitute_line()
{
   int line_number;
   cout << " Substitute what line number? " << flush;
   cin >> line_number;
   while (cin.get() != '\n');
   cout << " What is the replacement line? " << flush;
   String to_insert = read_in(cin);
   return replace(line_number, to_insert);
}
 
Error_code Editor::change_line()
/* 
Pre:   The Editor is not empty.
Post: If a user-specified string appears in the
current line, it is replaced by a new user-selected string.
Else: an Error_code is returned.
Uses: String and Editor methods and functions.
*/
{
   Error_code result = success;
   cout << " What text segment do you want to replace? " << flush;
   String old_text = read_in(cin);
   cout << " What new text segment do you want to add in? " << flush;
   String new_text = read_in(cin);
 
   int the_index = strstr(current->entry, old_text);
   if (the_index == -1) result = fail;
   else {
      String new_line;
      strncpy(new_line, current->entry, the_index);
      strcat(new_line, new_text);
      const char *old_line = (current->entry).c_str();
      strcat(new_line, (String)
         (old_line + the_index + strlen(old_text.c_str())));
      current->entry = new_line;
   }
   return result;
}
 
void Editor::read_file()
/* 
Pre:  Either the Editor is empty or the user authorizes
the command.
Post: The contents of *infile are read to the Editor.
Any prior contents of the Editor are overwritten.
Uses: String and Editor methods and functions.
*/
{
   bool proceed = true;
   if (!empty()) {
      cout << "Buffer is not empty; the read will destroy it." << endl;
      cout << " OK to proceed? " << endl;
      if (proceed = user_says_yes()) clear();
   }

  int line_number = 0, terminal_char;
  while (proceed) {
      String in_string = read_in(*infile, terminal_char);
      if (terminal_char == EOF) {
         proceed = false;
         if (strlen(in_string.c_str()) > 0) insert(line_number , in_string);
      }
      else insert(line_number++, in_string);
   }
}
 
void Editor::write_file()
{
   while (previous_line() == success);
   do {
      *outfile << (current->entry).c_str() << endl;
   } while (next_line() == success);
}
 
void Editor::find_string()
/* 
Pre:   The Editor is not empty.
Post: The current line is advanced until either it contains a copy
of a user-selected string or it reaches the end of the Editor.
If the selected string is found, the corresponding line is
printed with the string highlighted.
Uses: String and Editor methods and functions.
*/
{
   int the_index;

   cout << "Enter string to search for:" << endl;
   String search_string = read_in(cin);
   while ((the_index = strstr(current->entry, search_string)) == -1)
      if (next_line() != success) break;
   if (the_index == -1) cout << "String was not found.";
   else {
      cout << (current->entry).c_str() << endl;
      for (int i = 0; i < the_index; i++)
         cout << " ";
      for (int j = 0; j < strlen(search_string.c_str()); j++)
         cout << "^";
   }
   cout << endl;
}
 
bool Editor::get_command()
/* 
Post: Sets member user_command; returns true 
unless the user's command is q.
Uses: C library function tolower.
*/
{
   if (current != NULL)
      cout << current_position << " : "
           << current->entry.c_str() << "\n??" << flush;
   else
      cout << "File is empty. \n??" << flush;
 

   cin >> user_command;    //  ignores white space and gets command
   user_command = tolower(user_command);
   while (cin.get() != '\n')
      ;                       //  ignore user's enter key
   if (user_command == 'q')
      return false;
   else
      return true;
}
 
void Editor::run_command()
/* 
Post: The command in user_command has been performed.
Uses: methods and auxiliary functions of the class Editor,
the class String, and the String processing functions.
*/
 
{
   String temp_string;

   switch (user_command) {

   case 'b':
      if (empty())
         cout << " Warning: empty buffer " << endl;
      else
         while (previous_line() == success)
            ;
      break;


   case 'c':
      if (empty())
         cout << " Warning: Empty file" << endl;
      else if (change_line() != success)
         cout << " Error: Substitution failed " << endl;
      break;


   case 'd':
      if (remove(current_position, temp_string) != success)
         cout << " Error: Deletion failed " << endl;
      break;


   case 'e':
      if (empty())
         cout << " Warning: empty buffer " << endl;
      else
         while (next_line() == success)
            ;
      break;


   case 'f':
      if (empty())
         cout << " Warning: Empty file" << endl;
      else
         find_string();
      break;


   case 'g':
      if (goto_line() != success)
         cout << " Warning: No such line" << endl;
      break;


 
   case '?':
   case 'h':
      cout << "Valid commands are: b(egin) c(hange) d(el) e(nd)" << endl
           << "f(ind) g(o) h(elp) i(nsert) l(ength) n(ext) p(rior) " << endl
           << "q(uit) r(ead) s(ubstitue) v(iew) w(rite) " << endl;
      break;
 
   case 'i':
      if (insert_line() != success)
         cout << " Error: Insertion failed " << endl;
      break;


   case 'l':
      cout << "There are " << size() << " lines in the file." << endl;
 
      if (!empty())
         cout << "Current line length is "
              << strlen((current->entry).c_str()) << endl;
      break;


   case 'n':
      if (next_line() != success)
         cout << " Warning: at end of buffer" << endl;
      break;


   case 'p':
      if (previous_line() != success)
         cout << " Warning: at start of buffer" << endl;
      break;


   case 'r':
      read_file();
      break;


   case 's':
      if (substitute_line() != success)
         cout << " Error: Substitution failed " << endl;
      break;

   case 'v':
      traverse(write);
      break;


   case 'w':
      if (empty())
         cout << " Warning: Empty file" << endl;
      else
         write_file();
      break;


   default :
      cout << "Press h or ? for help or enter a valid command: ";
   }
}
 
Editor::Editor(ifstream *file_in, ofstream *file_out)
/* 
 
Post: Initialize the Editor members infile and outfile 
with the parameters.
 
*/
{
   infile = file_in;
   outfile = file_out;
}

void write(String &s)
/* 

Post: The String parameter s is written to cout.
 
*/
{
   cout << s.c_str() << endl;
}
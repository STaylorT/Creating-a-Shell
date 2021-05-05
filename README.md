# Creating-a-Shell
Creating a shell in C

OVERVIEW:
Simpshis similar to existing shells such as csh, bash, ksh, and tcsh, in that it reads input a line ata time from the standard input file (terminal),  and produces output on the standard output file.However, it lacks much of the functionality found in other shells.  It also has a different commandsyntax

COMMANDS:

Simpshsupports the following commands.Each input line must begin with one of the following commmands.

•#anytext— The # command introduces a comment.simpshignores the rest of the line.The # must occur as the first token on the line; any # occurring other than the first token isa syntax error.

•cd  dirName— This command changessimpsh’s current directory todirName.  You donotneed to handlecdwith no parameters—unlike a shell likebash, which understands the absenceof parameters to “change to the home directory”.  (See thegetwd(3)andchdir(2)systemcalls.)   After  this  command,  any  program  thatsimpshstarts  will  run  withdirNameas  itsworking directory.  This command also updates the value of the variableCWD, see below.

•variable = value— Herevariableis any reasonable variable name (first character is a letter,others are letters or numbers), and thevalueis a single token.  The spaces around the = tokenare required.  The effect of this command is to store the value of the variable insidesimpshfor later substitution.  Certain special variables are maintained bysimpshitself, as describedbelow.

•lv— this command (“list variables”) causessimpshto print a list of all the variables thathave been assigned a value, including the predefined ones (see below).

•unset variable— the command causes the value of a user-specified variable to be forgotten.  Itis an error to try to unset any of the built-in variables.1

•!  cmd param* [infrom:  file0] [outto:  file1]This command tellssimpshto execute a program.The “!”  must be the first token on the command line.  Here,cmdis a token that specifiesthe filename of the program the user wants to execute.  It is followed by zero or more tokensspecifying parameters.  The optional token pairsinfrom: file0andoutto:file1indicate that thestandard input (respectively standard output) should be redirected fromfile0(respectively, tofile1).  Note that the colon is the final character in each token.  Eitherinfrom:oroutto:orboth may be omitted, and either may come first.  If present, the keyword-filename pair mustbe the last tokens on the line.simpshlooks forcmdin a list of directories indicated by thespecial variablePATH, whose value is a colon-separated list of directories.  By default,PATHhasthe value/bin:/usr/bin; the user can change that value by assigning a new value toPATH.However, ifcmdstarts with a “/” character, it is a full path name starting at the root of thefilesystem.  Similarly, ifcmdstarts with “./”, it is understood to be a path name starting inthe current directory.•quit— This command causessimpshto exit, with status 0.

Certain variables are already defined whensimpshstarts up.  Some can be changed by assignment,but none of them can be unset.PATHThis is a colon-separated list of directories to be searched for commands to be executed viathe!command.  Initially set to/bin:/usr/bin.CWDThis  is  maintained  bysimpshto  always  have  the  value  of  the  current  working  directory.Cannot be changed (except indirectly, via thecdcommand).PSThe prompt string.  This is printed before reading each line of input from the standard input.When a line is an assignment to a new variable (i.e., three tokens, of which the first is a validvariable name (sayFOO), the second is “=”, and the third is anything),simpshstores the name andvalue.Whensimpshencounters a variable name, preceded immediately by “$” (no space between$and  the  name)  either  as  a  token  or  a  substring  of  a  token,  it  should  replace  both  the$and  thevariable  name  with  the  corresponding  stored  definition.   It  is  a  syntax  error  if$occurs  before  astring that is not the name of a variable.

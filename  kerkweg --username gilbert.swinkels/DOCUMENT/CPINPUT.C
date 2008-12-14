/***********************************************************************
                                   cpinput.c
       void near nasty( int );
       void near process_arguments( int, int, char **, int );
************************************************************************/
#define  MAIN  0
#include "cpheader.h"

       void near nasty( int );
       void near process_arguments( int, int, char **, int );
/************************************************************************/

void near nasty( argc )
int argc;
{
if( argc < 2 )
   {
   (void)printf( "\ncp listfile [ outfile ] [\n" );
   (void)printf(
   "     /p:nn /w:nn /m:nn /r:nn /t:main /f:nnnn\n"
               );
   (void)printf(
   "     /l /n /s /q /d /c /h /x\n"
               );
   (void)printf(   "                        ]\n\n" );
   (void)printf(
   "     outfile            = prn\n" );
   (void)printf(
   "    p: page length      = %3d   [ 0, 50 -255 ]\n", defined_page_length
               );
   (void)printf(
   "     w: page width      = %3d   [ 80 - 255 ]\n", defined_page_width
               );
   (void)printf(
   "     m: left margin     = %2d    [ 0 - 30 ]\n", defined_left_margin
               );
   (void)printf(
   "     r: right margin     = %2d    [ 0 - 30 ]\n", defined_right_margin
               );
   (void)printf(
   "     t: target function  = %s\n", target
               );
   (void)printf(
   "     f: # of function calls = %4d    [ 2 - 5461 ]\n", MAX_functions
               );
   (void)printf(
   "     n: normal characters( ie not ibm character graphics )\n"
               );
   (void)printf(
   "     l  output library functions\n"
               );
   (void)printf(
   "     c  output file\'s 1st comment\n"
               );
   (void)printf(
   "     s  output statistics only\n"
               );
   (void)printf(
   "     d  show declarations and definitions\n"
               );
   (void)printf(
   "     o  show overlay information\n"
               );
   (void)printf(
   "     u  show unused sorted by filename\n"
               );
   (void)printf(
   "     q  show no messages\n"
               );
   (void)printf(
   "     h  show more help\n"
               );
   (void)printf(
   "     x  show tech info\n"
               );

   (void)printf( "\n" );
   exit( 0 );
   }
}
/**********************************************************************/
void near process_arguments( index, argc, argv, an_error )
int index, argc, an_error;
char **argv;
{
char c;
int i, tmp;

for( i = index; i < argc; ++i )
   {
   if( ( argv[ i ][ 0 ] == '/' ) || ( argv[ i ][ 0 ] == '-' ) )
      {
      c = (char)tolower( (int)argv[ i ][ 1 ] );
      switch( c )
         {
         case 'n':
            ibm_flag = ( ibm_flag )? false: true;
            break;
         case 'l':
            g_lib_flag = ( g_lib_flag )? false: true;
            break;
         case 'c':
            g_comment_flag = ( g_comment_flag )? false: true;
            break;
         case 'd':
            g_dec_def_flag = ( g_dec_def_flag )? false: true;
            break;
         case 's':
            stats_only = ( stats_only )? false: true;
            break;
         case 'q':
            g_quiet_flag = ( g_quiet_flag )? false: true;
            break;
         case 'o':
            g_ov_flag = true;
            break;
         case 'u':
            g_un_flag = true;
            break;
         case 'h':
            g_help_flag = true;
            break;
         case 'x':
            g_tech_flag = true;
            break;
         default:
          if( ( strlen( argv[ i ] ) > 3 ) && ( argv[ i ][ 2 ] == ':' ) )
              {
               tmp = atoi( &argv[ i ][ 3 ] );
               switch( c )
                  {
                  case 'p':
                   if( ( ( 50 < tmp ) && ( tmp < 256 ) ) || ( tmp == 0 ) )
                       defined_page_length = tmp;
                   break;
                  case 'm':
                     if( ( 0 <= tmp ) && ( tmp <= 30 ) )
                        defined_left_margin = tmp;
                     break;
                  case 'r':
                     if( ( 0 <= tmp ) && ( tmp <= 30 ) )
                        defined_right_margin = tmp;
                     break;
                  case 't':
                     (void)strcpy( target, &argv[ i ][ 3 ] );
                     target_flag = true;
                     break;
                  case 'w':
                     if( ( 79 < tmp ) && ( tmp < 256 ) )
                        defined_page_width = tmp;
                     break;
                  case 'f':
                     if( ( 1 < tmp ) && ( tmp < 5462 ) )
                        Max_functions = tmp;
                     break;
                  default:
                     (void)printf(
                         "\nUnknown argument character: %c, ignored!\n",
                           argv[ i ][ 1 ]
                              );
                     break;
                  }  /* end of switch on character after / or - */
               }     /* end of if :something */
            else
               (void)printf( "\nMissing : for argument %s, ignored!\n",
                             argv[ i ] );
            break;
         }           /* end of switch on character after / or - */
      }              /* end of if / or - */
   else
      (void)printf( "\nUnknown argument: %s, ignored!\n", argv[ i ] );
   }                 /* end of for loop on arguments */

if( g_tech_flag )
   {
   (void)printf( "\n" );
   (void)printf( "Notes: 1. Max recursive function displacement of %d.\n",
                 Max_Recursion
               );
   (void)printf(
"         2. Max # of unique function calls per defined function\n\
            for all defined functions is %d.\n",
   Max_functions );
   (void)printf( "         3. Max # of defined functions is %d.\n",
      Max_defined_functions );
   (void)printf( "\n" );
   (void)printf( "sizeof()\'s:\n" );
   (void)printf(
" function table = %u, contents = %u, data base = %u,\
 database = %u, lib = %u\n",
      sizeof( function_type ),
      sizeof( file_record_type ),
      sizeof( data_base_record_type ),
      sizeof( array_of_ptrs_to_records ),
      sizeof( sorted_called_list_ptrs )
               );
   (void)printf( "\n" );
   (void)printf(
   "The program will tend to show certain \'c\' functions as unused.\n" );
   (void)printf(
   "1. defined functions assigned to declared pointers to function names\n" );
   (void)printf(
   "   and executed as pointers to those function names won't be seen.\n" );
   (void)printf(
   "2. #if(s) controlling the generation of code especially with\n" );
   (void)printf(
   "   braces( { } ) in the conditional code section will especially\n" );
   (void)printf(
   "   screw up if there is an #else code part.  This program will work\n" );
   (void)printf(
   "   on both code parts of the conditional and most probably get out\n" );
   (void)printf(
   "   of sync with the braces. One might do a preprocessor pass compile\n" );
   (void)printf(
   "   and heave it\'s output files as input files at this program.\n" );
   (void)printf(
   "3. #define(s) that expand to functions and call functions will also\n" );
   (void)printf(
   "   be neglected.  The preprocessor may be used as stated above.\n" );
/******
   (void)printf(
   "\n" );
******/
   (void)printf( "\n" );
   }

if( g_help_flag )
   {
   (void)printf( "\n" );
   (void)printf(
   "The listfile argument is an ascii text file containing the list of\n"
               );
   (void)printf(
   "filenames to process, one filename per line (optional overlay number.)\n"
               );
   (void)printf(
   "The output file may be a device or a filename. If there is no\n"
               );
   (void)printf(
   "output filename, \'prn\' is assumed. Note that one may put \'con\'\n"
               );
   (void)printf(
   "here and view the output of the program before printing or saving\n"
               );
   (void)printf(
   "to a filename.\n"
               );
   (void)printf(
   "Also note that the output filename and the input filenames in the\n"
               );
   (void)printf(
   "listfile may be full pathnames with drives and or paths.\n"
               );
   (void)printf( "/ arguments accept the alternate - form.\n" );
   (void)printf( "For example: cp x y -s, cp /h, cp x -x /d -t:junk\n" );
   (void)printf( "arguments may be in upper or lower case.\n" );
   (void)printf( "Note that the target function is case sensitive\n" );
   (void)printf( "since it is a \'c\' function name.\n" );
   (void)printf( "\n" );
   }
if( an_error )
   {
   if( g_help_flag || g_tech_flag )
      exit( 0 );
   else
      (void)printf( "Oops..." );
   }
}
/***********************************************************************/


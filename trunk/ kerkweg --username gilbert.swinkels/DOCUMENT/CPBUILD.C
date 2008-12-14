/***************************************************************************
                                   cpbuild.c
static void near mark_as_static( function_type *, char*, int );
static int near  test_and_add( function_type *, char *, int );
static void near unget_chars( char );
static char near get_chars( FILE * );
static char near get_to_next_possible_token( FILE * );
static int near  is_legal_identifier_character( char );
       int near  build_the_data_base( char * );
***************************************************************************/

#define  MAIN  0
#include "cpheader.h"

       int near  build_the_data_base( char * );
static char near get_chars( FILE * );
static char near get_to_next_possible_token( FILE * );
static int near  is_legal_identifier_character( char );
static void near mark_as_static( function_type *, char*, int );
static int near  test_and_add( function_type *, char *, int );
static void near unget_chars( char );

/***************************************************************************/
static void near mark_as_static( ptr_to_function_list, 
                                 name_of_static_function, count
                               )
char *name_of_static_function;
function_type *ptr_to_function_list;
int count;
{
int i;

for( i = 0; i < count; ++i )
   {
   if(
      !strcmp( name_of_static_function, ptr_to_function_list->functions_name )
     )
      ptr_to_function_list->static_function = true;
   ++ptr_to_function_list;
   }
}
/***************************************************************************/
#define KEYS   7

static int near test_and_add( ptr_to_function_list, string, count )
function_type *ptr_to_function_list;
char *string;
int count;
{
int i, is_a_new_function_name;
static char *keywords[ KEYS ] =
   {  /* must catch do (void)printf, while(), else (void)... etc. ***/
   "do", "while", "if", "else", "for", "switch", "return"
   };

for( i = 0; ( i < KEYS ) && ( strcmp( string, keywords[ i ] ) != 0 ); ++i )
   ;
if( i < KEYS )
   is_a_new_function_name = false;     /* ie a reserved word match */
else                                   /* is a function name */
   {
   for( i = 0; i < count; ++i )
      {
      if( !strcmp( string, ptr_to_function_list->functions_name ) )
         {                       /* function name matches */
         if( !ptr_to_function_list->static_function )
            break;               /* and isn't static */
         else
            {
            if( !strcmp( ptr_to_function_list->its_filename,
                         file_record_array_ptr->source_filename
                      )
              )
               break;            /* only statics in same file match */
            }
         }
      ++ptr_to_function_list;
      }
   if( i == count )
      {                                /* new function name */
      is_a_new_function_name = true;   /* add function name to list */
      if( ( function_list_ptr->functions_name = strdup( string ) ) == NULL )
         {
         (void)fprintf( stderr, "Ran out of memory.\n" );
         exit( 1 );
         }
      function_list_ptr->static_function = false;
      function_list_ptr->its_filename =
         file_record_array_ptr->source_filename;
      function_list_ptr->is_referenced = 1;

      ++function_list_ptr;             /* point to next empty cell */
      ++count_of_functions;            /* increase current size */
      if( count_of_functions > Max_functions )
         {
         (void)fprintf( stderr, "Too many functions.\n" );
         exit( 1 );
         }
      }
   else                                /* string already in function list */
      {
      is_a_new_function_name = false;
      ptr_to_function_list->is_referenced++;
      }
   }
return is_a_new_function_name;
}
/***************************************************************************/
static void near unget_chars( c )
char c;
{
if( ( push_buffer_ptr - push_buffer ) < Max_unget_buffer )
   *push_buffer_ptr++ = c;
else
   {
   (void)fprintf( stderr, "\nProgram syntax error:" );
   (void)fprintf( stderr, " Too many pushed characters.\n" );
   exit( 1 );
   }
}
/***************************************************************************/
static char near get_chars( stream )
FILE * stream;
{
register char c;

if( push_buffer_ptr != push_buffer )
   c = *--push_buffer_ptr;
else
   {
   c = (char)fgetc( stream );
   if( c == EOF )
      c = Control_z;
   if( c == 0x0a )
      {
      file_record_array_ptr->line_count++;
      file_record_array_ptr->size++;           /* count the unseen <cr> */
      }
   file_record_array_ptr->size++;
   }
return c;
}
/***************************************************************************/
static char near get_to_next_possible_token( stream )
FILE *stream;
{
register char
   c;
char
   next_char_peek;
int
   done;

static int       /* the only apparent reason these are static is for speed */
   quotes_flag =           false,
   comment_flag =          false,
   escape_sequence_flag =  false,
   pound_sign_flag =       false,
   ascii_quote_flag =      false;
static int
   fp = 0;   /*****<<<<< */
static char *cp;

done = false;
do {
   c = get_chars( stream );
   if( c != Control_z )
      {   
      if( comment_flag )
         {
/**************************
   process /* comment sequence of characters
***************************/
         if( first_comment == true )
            {
            if( fp < ( Max_general_buffers - 2 ) )
               {
               if(
                  ( c != '\n' ) &&
                  ( strlen( cp ) < effective_width )
                 )
                  {
                  file_comment_buffer[ fp++ ] = c;
                  file_comment_buffer[ fp ] = '\0';
                  }
               else        /* c == \n or length >= width */
                  {
                  file_comment_buffer[ fp++ ] = '\n';
                  file_comment_buffer[ fp ] = '\0';
                  cp = (char *)&file_comment_buffer[ fp ];
                  if( c != '\n' )
                     {
                     file_comment_buffer[ fp++ ] = c;
                     file_comment_buffer[ fp ] = '\0';
                     }
                  }
               }
/*          else     /* 1st comment exceeds buffer */
            }        /* end of if first_comment == true */
         if( c == '*' )
            {
            next_char_peek = get_chars( stream );
            if( next_char_peek == '/' )          /* close comment */
               {
               comment_flag = false;
               unget_chars( ' ' );  /* comments are white space in 'c' */
               if( first_comment == true )
                  {
                  first_comment = completed;
                  fp = 0;
                  cp = (char *)&file_comment_buffer[ fp ];
                  }
               }
            else        /* next_char_peek != '/' ie close comment */
               unget_chars( (char)next_char_peek );
            }  /* end of if c == '*' */
         }
      else     /* not /* */
         {
/**************************
   process \sequence character, hoping \" \' \\ etc inside " or '
***************************/
         if( escape_sequence_flag )
            escape_sequence_flag = false;
         else     /* not /*, not \ */
            {
/**************************
   process " string sequence of characters
***************************/
            if( quotes_flag )
               {
               if( c == '\\' )                  /* check for \'\n' */
                  {
                  next_char_peek = get_chars( stream );
                  if( next_char_peek != '\n' )  /* so not \'\n' */
                     {
                     escape_sequence_flag = true;
                     unget_chars( (char)next_char_peek );
                     }
/*******          else                          /* \'\n' continuation */
                  }
               else                             /* not \ */
                  if( c == '\"' )
                     quotes_flag = false;
               }
            else     /* not ", not /*, not \ */
               {
/**************************
   process ' ascii character sequence
***************************/
               if( ascii_quote_flag )
                  {
                  if( c == '\\' )
                     escape_sequence_flag = true;
                  else
                     if( c == '\'' )
                        ascii_quote_flag = false;
                  }
               else  /* not ', not ", not /*, not \ */
                  {
/**************************
   process # sequence of characters, ie #if, #define, etc.
   define causes code sequencing problems it would seem!
***************************/
                  if( pound_sign_flag )
                     {
                     if( c == '/' )       /* comments override #defines etc */
                        {
                        next_char_peek = get_chars( stream );
                        if( next_char_peek == '*' )
                           comment_flag = true;
                        else
                           unget_chars( (char)next_char_peek );
                        }
                     else
                        {
                        if( c == '\n' )
                           pound_sign_flag = false;
                        else                          /* c != \n */
                           {
                           if( c == '\\' )  /* check for \'\n' continuation */
                              {   
                              next_char_peek = get_chars( stream );
                              if( next_char_peek != '\n' ) /* it aint \'\n' */
                                 unget_chars( (char)next_char_peek );
/*                            else              /* \'\n' means continue # */
                              }
                           }
                        }
                     }
                  else     /* not ', not #, not ", not /*, not \ */
                     {
/**************************
   process anything else
***************************/
                     done = false;     /* assume a ' or " or # or /* */
                     switch( c )
                        {
                        case '\"':
                           quotes_flag = true;
                           break;
                        case '\'':
                           ascii_quote_flag = true;
                           break;
                        case '#':
                           pound_sign_flag = true;
                           break;
                        case '/':
                           next_char_peek = get_chars( stream );
                           if( next_char_peek == '*' )
                              {
                              comment_flag = true;
                              if( first_comment == false )
                                 {           /* the 1st comment of the file */
                                 first_comment = true;
                                 fp = 0;
                                 cp = (char *)&file_comment_buffer[ fp ];
                                 }
                              }
                           else
                              {
                              unget_chars( (char)next_char_peek );
                              done = true;   
                              }
                           break;
                        default:       /* a worthy character to return */
                           done = true;   
                        }
                     }     /* end of else not ascii */
                  }        /* end of else not # */
               }           /* end of else not " */
            }              /* end of else not /* */
         }                 /* end of else not \ */
      }                    /* end of if c != Control_z */
   }
while( !done && ( c != Control_z ) );
if( c == Control_z )
   {
   ascii_quote_flag = false;
   pound_sign_flag = false;
   quotes_flag = false;
   escape_sequence_flag = false;
   comment_flag = false;
   fp = 0;
   }
return c;
}
/***************************************************************************/
static int near is_legal_identifier_character( c )
char c;
{
if(
   ( ( 'A' <= c ) && ( c <= 'Z' ) ) ||
   ( ( 'a' <= c ) && ( c <= 'z' ) ) ||
   ( ( '0' <= c ) && ( c <= '9' ) ) ||
   ( c == '_')
  )
   return true;
else
   return false;
}
/***************************************************************************/
#define  C_line_length  512
#define  C_identifier_length  80

int near build_the_data_base( the_filename )
char * the_filename;
{
static char fake_comment[ ] = "no room!";
int found_a_possible_function;
int brace_count, body_found;
int open_parenthesis, parenthesis_count;
int at_end_of_source_file;
int dummy_index, total_called_count;
int function_definition_flag, static_flag;
int analyze_buffer_flag = false;
char c;
char *function_name_buffer_ptr;
char function_name_buffer[ C_identifier_length ];
char look_ahead_buffer[ C_line_length + 1 ];
FILE *stream;
data_base_record_type *data_base_ptr, *starting_data_base_ptr;
function_type *starting_called_function_ptr;

if( !g_quiet_flag )
   {
   (void)printf( "Processing file: %-12s\n", the_filename );
   }
if( !( stream = fopen( the_filename, "r" ) ) )  /***** rt <<<<<<<<<< */
   {
   (void)printf( "Cant open %s\n", the_filename );
   return -1;
   }

push_buffer_ptr = push_buffer;         /* reset input character stack */
                                       /* add file name to data base */
if( !( file_record_array_ptr->source_filename = strdup( the_filename ) ) )
   {
   (void)printf( "Ran out of memory.\n" );
   exit( 1 );
   }

starting_called_function_ptr = function_list_ptr;
starting_data_base_ptr = data_base_array_ptr; /* mark start of defined list */

look_ahead_buffer[ 0 ] = '\0';

first_comment = false;
file_comment_buffer[ 0 ] = '\0';

file_record_array_ptr->line_count = 0;  /* clear it's variables */
file_record_array_ptr->size = 0l;

function_name_buffer_ptr = function_name_buffer;
function_name_buffer[ 0 ] = '\0';

static_flag = false;
found_a_possible_function = false;
open_parenthesis = false;
body_found = false;

brace_count = 0;
parenthesis_count = 0;

at_end_of_source_file = false;
while( !at_end_of_source_file )
   {
   c = get_to_next_possible_token( stream );
   switch( c )
      {
      case '{':
         ++brace_count;
         break;
      case '}':
         --brace_count;
         break;
      case Control_z:
         at_end_of_source_file = true;
         analyze_buffer_flag = true;
         break;
      case '(':
         if( !open_parenthesis )
            ++open_parenthesis;
         analyze_buffer_flag = true;
         break;
      case ' ':                  /* this is where we eat white space */
      case '\v':
      case '\b':
      case '\f':
      case '\t':
      case '\r':
      case '\n':
         do {
            c = get_to_next_possible_token( stream );
            }
         while(
               ( c == '\f' ) || ( c == ' ' ) || ( c == '\v' ) ||
               ( c == '\b' ) || ( c == '\t' ) || ( c == '\r' ) ||
               ( c == '\n' )
              );
         unget_chars( c ); /* put next non white character back */

         if( c != '(' )
            analyze_buffer_flag = true;
/***     else  /* c == '(' and next pass will find it */
         break;
      default:
         if( is_legal_identifier_character( c ) )
            {                          /* it's a good identifier character */
            *function_name_buffer_ptr++ = c;
            *function_name_buffer_ptr = '\0';
            }
         else                          /* it aint, so toss it */
            {
            if( static_flag && ( c == ';' ) )
               static_flag = false;
/*          if( c != '*' ) */
            analyze_buffer_flag = true;
            }
         break;
      }                    /* end of preliminary character parse */
/*****************
   start checking characters accumulated in function_name_buffer[]
******************/
   if( analyze_buffer_flag )
      {
      analyze_buffer_flag = false;
      if(
         function_name_buffer[ 0 ] &&        /* ie not null string */
         (                                   /* & not number */
          ( function_name_buffer[ 0 ] < '0' ) ||
          ( function_name_buffer[ 0 ] > '9' )
         )
        )
         found_a_possible_function = true;
      else                                   /* it aint an identifier */
         {                                   /* so erase buffer */
         function_name_buffer_ptr = function_name_buffer;
         function_name_buffer[ 0 ] = '\0';
         if( static_flag && ( c == ';' ) )
            static_flag = false;
         open_parenthesis = false;
         }
      }                       /* end of analyze_buffer_flag */
/*****************
   if function_name_buffer[] has legal function name, scan ahead
******************/
   if( found_a_possible_function )
      {
      found_a_possible_function = false;
      *function_name_buffer_ptr = '\0';   /* append nul char to end */
      if( !static_flag )                  /* don't retest if true */
         if( !strcmp( function_name_buffer, "static" ) )
            static_flag = true;
      if( open_parenthesis )
         {
         open_parenthesis = false;
         if( !brace_count )
            {                             /* ie outside any function body */
            parenthesis_count = 1;
            for( dummy_index = 0;
                 ( dummy_index < C_line_length ) && parenthesis_count;
                 ++dummy_index
               )
               {                          /* scan ahead for function() */
               c = get_to_next_possible_token( stream );
               if( c == Control_z )
                  break;            /* dummy_index not bumped */
               look_ahead_buffer[ dummy_index ] = c;
               look_ahead_buffer[ dummy_index + 1 ] = '\0';
               switch( c )
                  {
                  case '(':
                     ++parenthesis_count;
                     break;
                  case ')':
                     --parenthesis_count;
                     break;
                  }           /* dummy_index is bumped */
               }              /* end of for loop scanning for (...) */
            if( ( c == Control_z ) || ( !parenthesis_count ) )
               --dummy_index;
            function_definition_flag = false;
            for( ++dummy_index;
                 ( dummy_index < C_line_length ) && !function_definition_flag;
                 ++dummy_index
               )
               {                 /* what happens past (..) */
               c = get_to_next_possible_token( stream );
               if( c == Control_z )
                  break;            /* w/ function_definition_flag == false */
               look_ahead_buffer[ dummy_index ] = c;
               look_ahead_buffer[ dummy_index + 1 ] = '\0';
               switch( c )
                  {
                  case ' ':         /* this is where we eat white space */
                  case '\v':
                  case '\b':
                  case '\f':
                  case '\t':
                  case '\n':
                  case '\r':
                     break;
                  case '{':
                     ++body_found;
                     break;
                  case ';':
                  case ',':
                  case '(':            /* at (*)() type declaration */
                     if( !body_found )
                        {
                        function_definition_flag = true; /* declaration */
                        if( !g_quiet_flag )
                           {
                           if( g_dec_def_flag )
                              {
                              if( static_flag )
                                 (void)printf( " static" );
                              else
                                 (void)printf( "       " );
                              (void)printf( " declaration " );
                              (void)printf( "%s(%s\n",
                                            function_name_buffer,
                                            look_ahead_buffer );
                              }
                           }
                        }
                     break;
                  default:          /* any other non white character means */
                     function_definition_flag = completed;
                     if( !g_quiet_flag )
                        {
                        if( g_dec_def_flag )
                           {
                           if( static_flag )
                              (void)printf( "static " );
                           else
                              (void)printf( "       " );
                           (void)printf( "define " );
                           }   
                        }
                     break;
                  }           /* dummy_index is bumped */
               }              /* end of for loop parsing character after ) */
            body_found = false;
            if( function_definition_flag == false )
               {
               (void)printf( "\nSyntax error: " );
               (void)printf( "Function description.\n" );
               look_ahead_buffer[ dummy_index ] = '\0';
               (void)printf( "\n%s\n", look_ahead_buffer );
               exit( 1 );
               }
            while( dummy_index )
               {                       /* put all characters after ( back */
               unget_chars( look_ahead_buffer[ dummy_index - 1 ] );
               --dummy_index;
               }
            if( function_definition_flag == completed )
               {
               if( !g_quiet_flag )
                  {
                  if( g_dec_def_flag )
                     (void)printf( "%-40s\n", function_name_buffer );
                  }
/*******************
   this element can distinguish static functions
   in different files with the same name
 *******************/
               data_base_array_ptr->file_record_ptr = file_record_array_ptr;
               data_base_array_ptr->number_of_function_calls = 0;
               data_base_array_ptr->ptr_to_function_table = function_list_ptr;
               data_base_array_ptr->static_definition = static_flag;
               static_flag = false;

               if(
                  !( data_base_array_ptr->defined_function =
                     strdup( function_name_buffer )
                   )
                 )
                  {
                  (void)printf( "\nRan out of memory( for strdup() )." );
                  exit( 1 );
                  }
               data_base_array_ptr->number_of_references = 0;
               data_base_array_ptr->ptr_to_page_list = NULL;

               data_base_ptr = data_base_array_ptr; /* save current pointer */
               ++data_base_array_ptr;                 /* next entry */
               ++count_of_valid_records;
               if( count_of_valid_records > Max_defined_functions )
                  {
                  (void)printf( "\nToo many new functions\n" );
                  exit( 1 );
                  }
               }        /* end of function definition */
            static_flag = false;
            }
         else                    /* brace_count is not zero */
            {                    /* so inside a function */
            data_base_ptr->number_of_function_calls +=
               test_and_add( data_base_ptr->ptr_to_function_table,
                             function_name_buffer,
                             data_base_ptr->number_of_function_calls
                           );
            }
         look_ahead_buffer[ 0 ] = '\0';   /* reset tail buffer */
         static_flag = false;
         }                       /* end of parenthesis */
      function_name_buffer_ptr = function_name_buffer;   /* reset buffer */
      *function_name_buffer_ptr = '\0';
      }                    /* end of found_a_possible_function */
   }                       /* end of while !at_end_of_source_file */
(void)fclose( stream );
if( !g_quiet_flag )
   {
   (void)printf( "\n" );
   }

if(
   !( file_record_array_ptr->source_file_comment =
      strdup( file_comment_buffer )
    )
  )
    file_record_array_ptr->source_file_comment = fake_comment;

/***** mark called functions in list as static if in defined list *******/
total_called_count = 0;
data_base_ptr = starting_data_base_ptr;
while( data_base_ptr != data_base_array_ptr )
   {
   total_called_count += data_base_ptr->number_of_function_calls;
   ++data_base_ptr;
   }
data_base_ptr = starting_data_base_ptr;
while( data_base_ptr < data_base_array_ptr )
   {
   if( data_base_ptr->static_definition )
      mark_as_static( starting_called_function_ptr,
                      data_base_ptr->defined_function,
                      total_called_count
                    );
   ++data_base_ptr;
   }
++file_record_array_ptr;      /* next file name entry */
++count_of_source_files;
if( count_of_source_files >= Max_files )
   {
   (void)printf( "\nError: too many files to process.\n" );
   exit( 1 );
   }
return at_end_of_source_file;
}
/***************************************************************************/


/***************************************************************************
                                  cpfuncts.c
       void near build_box_parts( int );
       void near tab_to_left_margin( FILE * );
static void near stop( void );
static void near setpage( data_base_record_type * );
static int  near recursion_check( char *, int );
       void near check_for_new_page( void );
static void near draw_output_block( char *, char *, char *,
                                    char *, int, int, int );
       int near  doprint( int );
       void near scan_for_static_or_global( int *, int, char *, char * );
       int near  binary_search_sorted_data_base( char * );
  
 ***************************************************************************/

#define  MAIN  0
#include "cpheader.h"

static char
   top_line_of_box[ 37 ], bottom_line_of_box[ 37 ],
   wall, ibm_line, bottom_attach,
   upper_left_corner, lower_left_corner,
   upper_right_corner, lower_right_corner,
   left_attach, right_attach;

static char *recursion_filename, *test_filename;
static int static_recursion;

       int near  binary_search_sorted_data_base( char * );
       void near build_box_parts( int );
       void near check_for_new_page( void );
       int near  doprint( int );
       void near scan_for_static_or_global( int *, int, char *, char * );
       void near tab_to_left_margin( FILE * );

static void near draw_output_block( char *, char *, char *,
                                    char *, int, int, int );
static int near  recursion_check( char *, int );
static void near stop( void );
static void near setpage( data_base_record_type * );

/***************************************************************************/
void near build_box_parts( is_ibm )
int is_ibm;
{
int i;

if( is_ibm )
   {
   wall =               '\xb3';
   ibm_line =           '\xc4';
   bottom_attach =      '\xc2';
   upper_left_corner =  '\xda';
   lower_left_corner =  '\xc0';
   upper_right_corner = '\xbf';
   lower_right_corner = '\xd9';
   left_attach =        '\xb4';
   right_attach =       '\xc3';
   }
else
   {
   wall =               '|';
   ibm_line =           '-';
   bottom_attach =      '+';
   upper_left_corner =  '+';
   lower_left_corner =  '+';
   upper_right_corner = '+';
   lower_right_corner = '+';
   left_attach =        '+';
   right_attach =       '+';
   }

top_line_of_box[ 0 ] = upper_left_corner;   
bottom_line_of_box[ 0 ] = lower_left_corner;
for( i = 1; i <= 34; ++i )
   {
   top_line_of_box[ i ] = ibm_line;
   bottom_line_of_box[ i ] = ibm_line;
   }
top_line_of_box[ i ] = upper_right_corner;
bottom_line_of_box[ i ] = lower_right_corner;
top_line_of_box[ ++i ] = '\0';
bottom_line_of_box[ i ] = '\0';
}
/***************************************************************************/
void near tab_to_left_margin( output )
FILE *output;
{
register int i;

for( i = 0; i < defined_left_margin; ++i )
   (void)fputc( ' ', output );
}
/***************************************************************************/
static void near stop()
{
(void)printf( "hello" );
}
/***************************************************************************/
static void near setpage( data_base_ptr )
data_base_record_type *data_base_ptr;
{
linked_pages_list *page_list_ptr;

page_list_ptr = data_base_ptr->ptr_to_page_list;
if( page_list_ptr == NULL )
   {
   if(
      !( page_list_ptr =
         (linked_pages_list *)malloc( sizeof( linked_pages_list ) )
       )
     )
      {
      (void)fprintf( stderr, "Ran out of memory for page # list.\n" );
      exit( 1 );
      }

   data_base_ptr->ptr_to_page_list = page_list_ptr;
   }
else
   {
   while( page_list_ptr->next_page_ptr )
      page_list_ptr = page_list_ptr->next_page_ptr;

   if(
      !( page_list_ptr->next_page_ptr =
         (linked_pages_list *)malloc( sizeof( linked_pages_list ) )
       )
     )
      {
      (void)fprintf( stderr, "Ran out of memory for page # list.\n" );
      exit( 1 );
      }

   page_list_ptr = page_list_ptr->next_page_ptr;
   }
page_list_ptr->next_page_ptr = NULL;
page_list_ptr->on_this_page = page - 1;
}
/***************************************************************************/
static int near recursion_check( string, static_call )
char *string;
int static_call;
{
register char **recursion_array_ptr;

recursion_array_ptr = recursion_array;
if( static_recursion )
   {                             /* defined function is static */
   while(
         *recursion_array_ptr && /* not null */
                                 /* and different function names */
         ( strcmp( *recursion_array_ptr, string ) ||
                                 /* or same function names and */
                                 /* in different files */
           strcmp( test_filename, recursion_filename )
         )
        )
   ++recursion_array_ptr;
   }
else
   {                             /* defined function is not static */
   while(
         *recursion_array_ptr && /* not null & */
                                 /* and different function names */
         ( strcmp( *recursion_array_ptr, string ) ||
                                 /* or same function names and */
           static_call           /* called is static */
         )
        )
      ++recursion_array_ptr;
   }
return ( *recursion_array_ptr )? true: false;
}
/***************************************************************************/
void near check_for_new_page()
{
int i;

if( defined_page_length == 0 && line == 9999 )
   {
   (void)fprintf( output, "\n\n\n\n" );
   line = 0;
   }
else
   {
   if( defined_page_length != 0 )
      {
      if( line > ( defined_page_length - 5 ) )
         {
         (void)fprintf( output, "\f" );
         line = 0;
         }
      if( line == 0 )
         {
         top_of_form_done = true;
         tab_to_left_margin( output );
         (void)fprintf( output, "%s", title );
         for( i = strlen( title ); i < ( effective_width - 10 ); ++i )
            (void)fputc( ' ', output );
         (void)fprintf( output, "Page:%4d\n", page );
         tab_to_left_margin( output );
         for( i = 0; i < effective_width; ++i )
            (void)fputc( '_', output );
         (void)fprintf( output, "\n\n" );
         line = 3;
         ++page;
         }
      }
   }
}
/***************************************************************************/
static void near draw_output_block( lead_in_string,
                                    name_of_function,
                                    description,
                                    name_of_file,
                                    either_count,
                                    tail_flag,
                                    kill_flag
                                  )
char *lead_in_string,
   *name_of_function,
   *description,
   *name_of_file;
int either_count, tail_flag, kill_flag;
{
unsigned int string_length;
static char alternate_lead_in[ 140 ];

/******* 1st line ***********************************************************/
tab_to_left_margin( output );
(void)fprintf( output, "%s %s\n", lead_in_string, top_line_of_box );

/******* 2nd line ***********************************************************/
tab_to_left_margin( output );
string_length = strlen( lead_in_string );
if( string_length )        /******* ie not main or defined function box ***/
   {
   (void)strncpy( alternate_lead_in, lead_in_string, --string_length );
   alternate_lead_in[ string_length++ ] = '\0'; /* restore string_length */
   }
if( string_length )        /******* ie not main or defined function box ***/
   (void)fprintf( output, "%s%c%c%c%-33s %c\n",
                  alternate_lead_in,
/***  if( kill_flag )      /****** last line to this box ******************/
/***  else                 /****** line continues downwards ***************/
                  ( kill_flag )? lower_left_corner: right_attach,
                  ibm_line, left_attach, name_of_function, wall );
else                       /****** main or defined box starting ***********/
   (void)fprintf( output,     "%c%c%-33s %c\n",
                  ibm_line, left_attach, name_of_function, wall );

/******* 3rd line ***********************************************************/
tab_to_left_margin( output );
if( string_length-- )      /***** kill outside vertical line on last box ****/
   lead_in_string[ string_length++ ] = ( kill_flag )? (char)' ': wall;
(void)fprintf( output, "%s %c%-20s %8s%3d  %c\n",
               lead_in_string, wall, description,
               name_of_file, either_count, wall );

/******* 4th line ***********************************************************/
tab_to_left_margin( output );
bottom_line_of_box[ 2 ] =  /******** if defined box has calls ***********/
   ( tail_flag && either_count )? bottom_attach: ibm_line;
(void)fprintf( output, "%s %s\n", lead_in_string, bottom_line_of_box );

line += 4;
top_of_form_done = false;
}
/***************************************************************************/
static char library_string[] = { "(library)" };
static char usage_string[] =   { "Used=" };
static char funct_string[] =   { "Functs=" };

int near doprint( index )
int index;
{
int
   loop_counter,
   max_count,
   starting_index,
   found,
   return_value;
data_base_record_type *record_ptr;
function_type *f_list_ptr;

static int kill_flag = false;

starting_index = index;
record_ptr = array_of_ptrs_to_records[ starting_index ];

recursion_array[ recursion_depth ] = record_ptr->defined_function;
if( !recursion_depth )
   {
   recursion_filename = record_ptr->file_record_ptr->source_filename;
                        /* add function to list for recursion check */
   static_recursion = record_ptr->static_definition;
   }
check_for_new_page();
setpage( array_of_ptrs_to_records[ starting_index ] );

return_value = page - 1;               /* must be a relic! */
                                       /* start w/ target function */
draw_output_block( nesting_display_buffer,
                   record_ptr->defined_function,
                   ( record_ptr->file_record_ptr )->source_filename,
                   funct_string,
                   record_ptr->number_of_function_calls,
                   true,
                   kill_flag
                 );

++recursion_depth;
                           /****   mystic width = 4 *****/
(void)strcat( nesting_display_buffer, "   |" );
nesting_display_buffer[ strlen( nesting_display_buffer ) - 1 ] = wall;

max_count = record_ptr->number_of_function_calls;
for( loop_counter = 0, f_list_ptr = record_ptr->ptr_to_function_table;
     loop_counter < max_count;
     ++loop_counter, ++f_list_ptr
   )
   {
   kill_flag = ( loop_counter == ( max_count - 1 ) )? true: false;
   check_for_new_page();
                           /* is called function defined? */
   found = binary_search_sorted_data_base( f_list_ptr->functions_name );
   if( found >= 0 )
      {
      scan_for_static_or_global( &found,
                                 f_list_ptr->static_function,
                                 f_list_ptr->functions_name,
                                 f_list_ptr->its_filename
                               );

      }
   if( found >= 0 )        /* yes */
      {
      test_filename = f_list_ptr->its_filename;
      if( recursion_check( f_list_ptr->functions_name,
                           f_list_ptr->static_function )
        )
         {
/*         tab_to_left_margin( output );
/*         (void)fprintf( output, "%s\n", nesting_display_buffer ); */
         setpage( array_of_ptrs_to_records[ found ] );
/*         ++line; */
         top_of_form_done = false;
         draw_output_block( nesting_display_buffer,
                            f_list_ptr->functions_name,
                            "(recursive)",
                            "",
                            0,
                            false,
                            kill_flag
                          );
         }
      else        /* not recursive and found >= 0 */
         {
         if( array_of_ptrs_to_records[ found ]->number_of_references == 1 )
            {                       /* got a new function */
/*            tab_to_left_margin( output );
/*            (void)fprintf( output, "%s\n", nesting_display_buffer );
/*            ++line;
/*            top_of_form_done = false; */
            doprint( found );           /* used only once */
            }
         else
            {                       /* a previously defined function */
/*            tab_to_left_margin( output );
/*            (void)fprintf( output, "%s\n", nesting_display_buffer ); */
            setpage( array_of_ptrs_to_records[ found ] );
/*            ++line;
/*            top_of_form_done = false; */
            draw_output_block( nesting_display_buffer,
                               f_list_ptr->functions_name,
                               "(defined)",
                               usage_string,
                               f_list_ptr->is_referenced,
                               false,
                               kill_flag
                             ); 
            }
         }
      }
   else           /* found = -1 ie not defined means */
      {           /* a library function */
/*      tab_to_left_margin( output );
/*      (void)fprintf( output, "%s\n", nesting_display_buffer );
/*      ++line;
/*      top_of_form_done = false; */
      draw_output_block( nesting_display_buffer,
                         f_list_ptr->functions_name,
                         library_string,
                         usage_string,
                         f_list_ptr->is_referenced,
                         false,
                         kill_flag
                       );
      }
   }           /* end of loop on all called functions */

                           /* remove function f/ recursion list */
recursion_array[ recursion_depth ] = NULL;
                           /****   mystic width = 4 *****/
nesting_display_buffer[ strlen( nesting_display_buffer ) - 4 ] = '\0';
--recursion_depth;
return return_value;
}
/***************************************************************************/
void near scan_for_static_or_global(
                              index_ptr, is_static, function_name, file_name
                                   )
int *index_ptr, is_static;
char *function_name, *file_name;
{
int index;

index = *index_ptr;
if( index )
   while( index-- )
      if( strcmp( function_name,
                  array_of_ptrs_to_records[ index ]->defined_function )
        )
         {
         ++index;       /* exit at last matching defined function */
         break;
         }
do {
   if(
      ( !is_static && !array_of_ptrs_to_records[ index ]->static_definition
      ) ||
      ( is_static &&
        array_of_ptrs_to_records[ index ]->static_definition &&
        !strcmp( array_of_ptrs_to_records[ index ]->
                  file_record_ptr->source_filename,
                 file_name
               )
      )
     )
      break;
   }
while(
      ( ++index < count_of_functions ) &&
      !strcmp( function_name,
               array_of_ptrs_to_records[ index ]->defined_function
             )
     );
if(
   ( index >= count_of_functions ) ||
   strcmp( function_name, array_of_ptrs_to_records[ index ]->defined_function
         )
  )
   index = -1;
*index_ptr = index;
}
/***************************************************************************/
int near binary_search_sorted_data_base( key )
char *key;
{
int lo, hi, index;
int doesnt_match;

lo = 0;
hi = count_of_valid_records - 1;
index = ( hi - lo ) / 2;

while( true )
   {
   doesnt_match =
      strcmp( key, array_of_ptrs_to_records[ index ]->defined_function );
   if( !doesnt_match )        /* a match found at index */
      break;
   if( lo >= hi )             /* no match found */
      {
      index = -1;
      break;
      }
   if( doesnt_match < 0 )     /* key < choice so go downwards */
      hi = index - 1;
   else                       /* key > choice so go upwards */
      lo = index + 1;
   index = ( hi + lo ) / 2;   /* new choice */
   }
return index;
}
/***************************************************************************/


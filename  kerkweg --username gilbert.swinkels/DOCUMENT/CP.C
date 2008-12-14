/*********************************************************************
                                     cp.c
  
static void near bump_line_count( void );
static void near do_top_of_page( void );
static void near deallocate_arrays( void );
static void near allocate_arrays( void );
static void near initialize_globals( void );
static void near build_records_from_list( FILE * );
static void near sort_the_data_base_array( void );
static void near count_all_defined_references( void );
static void near show_function_relationships( void );
static void near show_line_and_byte_counts( void );
static void near show_sorted_function_list( void );
static void near show_page_references( void );
static void near show_unused_if_any( );
static void near show_library_functions( void );
static void near show_files_leading_comments( );
       int       main( int, char ** );

 ***************************************************************************/

#define  MAIN  1
#include "cpheader.h"
#include "time.h"

extern int  near binary_search_sorted_data_base( char * );
extern void near build_box_parts( int );
extern int  near build_the_data_base( char *, char * );
extern void near check_for_new_page( void );
extern int  near doprint( int );
extern void near nasty( int );
extern void near process_arguments( int, int, char **, int );
extern void near scan_for_static_or_global( int *, int, char *, char * );
extern void near tab_to_left_margin( FILE * );

static void near allocate_arrays( void );
static void near build_records_from_list( FILE * );
static void near bump_line_count( void );
static void near count_all_defined_references( void );
static void near deallocate_arrays( void );
static void near do_top_of_page( void );
static void near initialize_globals( void );
static void near show_files_leading_comments( void );
static void near show_function_relationships( void );
static void near show_library_functions( void );
static void near show_line_and_byte_counts( void );
static void near show_page_references( void );
static void near show_sorted_function_list( void );
static void near show_unused_if_any( void );
static void near sort_the_data_base_array( void );
       int       main( int, char ** );

/***************************************************************************/

static void near bump_line_count( )
{
top_of_form_done = false;
++line;
check_for_new_page();
tab_to_left_margin( output );
}
/***************************************************************************/
static void near do_top_of_page( )
{
if( !top_of_form_done )
   {
   top_of_form_done = true;
   line = 9999;
   check_for_new_page();
   tab_to_left_margin( output );
   }
}
/***************************************************************************/
static void near deallocate_arrays( )
{
if( function_list )
   free( function_list );
if( file_record_array )
   free( file_record_array );
if( data_base_array )
   free( data_base_array );
if( sorted_called_list_ptrs )
   free( sorted_called_list_ptrs );
if( array_of_ptrs_to_records )
   free( array_of_ptrs_to_records );
}
/***************************************************************************/

static void near allocate_arrays( )
{
unsigned long length;

length = (unsigned long)Max_functions * sizeof( function_type );
if( length > 65535 )
   {
   (void)printf( "too many called functions ( go to huge model code )\n" );
   exit( 1 );
   }
else
   if(
      !( function_list =
         (function_type *)malloc( (unsigned int)length )
       )
     )
      {
      (void)printf( "No room for function_list\n" );
      exit( 1 );
      }
   else
      {
      if( !g_quiet_flag && g_tech_flag )
         (void)printf( "function list = %lu bytes long\n", length );
      }

length = (unsigned long)Max_files * sizeof( file_record_type );
if( length > 65535 )
   {
   (void)printf( "too many files ( go to huge model code )\n" );
   exit( 1 );
   }
else
   if(
      !( file_record_array =
         (file_record_type *)malloc( (unsigned int)length )
       )
     )
      {
      (void)printf( "No room for file_record_array\n" );
      exit( 1 );
      }
   else
      {
      if( !g_quiet_flag && g_tech_flag )
         (void)printf( "file record array = %lu bytes long\n", length );
      }

length =
   (unsigned long)Max_defined_functions * sizeof( data_base_record_type );
if( length > 65535 )
   {
   (void)printf( "too many defined functions ( go to huge model code )\n" );
   exit( 1 );
   }
else
   if(
      !( data_base_array =
         (data_base_record_type *)malloc( (unsigned int)length )
       )
     )
      {
      (void)printf( "No room for data_base_array\n" );
      exit( 1 );
      }
   else
      {
      if( !g_quiet_flag && g_tech_flag )
         (void)printf( "data base array = %lu bytes long\n", length );
      }

length =
   (unsigned long)Max_defined_functions * sizeof( data_base_record_type * );
if( length > 65535 )
   {
   (void)printf(
            "too many defined functions pointers( go to huge model code )\n"
               );
   exit( 1 );
   }
else
   if(
      !( array_of_ptrs_to_records =
         (data_base_record_type **)malloc( (unsigned int)length )
       )
     )
      {
      (void)printf( "No room for *array_of_ptrs_to_records\n" );
      exit( 1 );
      }
   else
      {
      if( !g_quiet_flag && g_tech_flag )
         (void)printf( "array of ptrs to data base = %lu bytes long\n",
                        length );
      }

length = (unsigned long)Max_functions * sizeof( function_type * );
if( length > 65535 )
   {
   (void)printf(
      "too many called function ptrs ( go to huge model code )\n"
               );
   exit( 1 );
   }
else
   if(
      !( sorted_called_list_ptrs =
            (function_type **)malloc( (unsigned int)length )
       )
     )
      {
      (void)printf( "No room for ptr function_list\n" );
      exit( 1 );
      }
   else
      {
      if( !g_quiet_flag && g_tech_flag )
         (void)printf( "sorted called list ptrs = %lu bytes long\n", length );
      }
}
/***************************************************************************/

static void near initialize_globals( )
{
int i;
char *cp;

function_list_ptr = function_list;
data_base_array_ptr = data_base_array;
file_record_array_ptr = file_record_array;

for( i = 0; i < Max_Recursion; ++i )
   recursion_array[ i ] = NULL;
build_box_parts( ibm_flag );
effective_width =             /******** set global output width ***********/
   defined_page_width - defined_left_margin - defined_right_margin;
if( effective_width < 40 )
   {
   (void)printf( "\nThe page width is too narrow( needs > 40 )." );
   exit( 1 );
   }

cp = &title[ 0 ];    /* insert date and nice time into title */
(void)_strdate( cp );
title[ 8 ] = ' ';
cp = &title[ 10 ];
(void)_strtime( cp );

title[ 15 ] = ' ';   /* knock off seconds */
title[ 16 ] = ' ';   /* put am, pm here */
title[ 17 ] = 'm';
title[ 18 ] = ' ';

i = atoi( &title[ 10 ] );  /* f/ military to civilian time */
title[ 16 ] = ( i < 12 )? (char)'a': (char)'p';

if( i == 0 )
   i = 12;
if( i >= 13 )
   i -= 12;

(void)sprintf( &title[ 10 ], "%2d", i );
title[ 12 ] = ':';

if( title[ 10 ] == '0' )
   title[ 10 ] = ' ';
}
/***********************************************************************/
static void near build_records_from_list( stream )
FILE  *stream;
{
char input_list_filename[ 129 ], input_line[ 129 ], overlay_number[ 129 ];
int l;

while( !feof( stream ) )
   {
   input_list_filename[ 0 ] = '\0';
   input_line[ 0 ] = '\0';
   overlay_number[ 0 ] = '\0';
   fgets( input_line, 128, stream );   /* ends at \n or eof */

   if(
      ( l = strlen( input_line ) ) > 1    /* ie not nul string */
     )
      {
      if( input_line[ l - 1 ] == '\n' )
         input_line[ l - 1 ] = '\0';

      l = sscanf( input_line, " %s %s ",
                  input_list_filename, overlay_number
                );
      if( !g_quiet_flag && g_tech_flag )
         {
         (void)printf( "pathname = %s ", input_list_filename );
         if( l )
            (void)printf( "overlay # = %s ", overlay_number );
         }
      (void)build_the_data_base( input_list_filename, overlay_number );
      }
   }
}
/***************************************************************************/

static void near sort_the_data_base_array( )
{
int i, still_sorting_flag;

for( i = 0, data_base_array_ptr = data_base_array;
     i < count_of_valid_records;
     ++i
   )
   array_of_ptrs_to_records[ i ] = data_base_array_ptr++;

if( !g_quiet_flag )
   {
   (void)printf( "\n\nSorting the function list...\n" );
   (void)printf( " of %d functions\n", count_of_valid_records );
   }
still_sorting_flag = true;
while( still_sorting_flag )
   {
   still_sorting_flag = false;
   if( !g_quiet_flag )
      {
      (void)printf( "." );
      }
   for( i = 0; i < count_of_valid_records - 1; ++i )
      {
      if( strcmp( array_of_ptrs_to_records[ i ]->defined_function,
                  array_of_ptrs_to_records[ i + 1 ]->defined_function ) > 0 )
         {
         still_sorting_flag = true;
         data_base_array_ptr = array_of_ptrs_to_records[ i ];
         array_of_ptrs_to_records[ i ] = array_of_ptrs_to_records[ i + 1 ];
         array_of_ptrs_to_records[ i + 1 ] = data_base_array_ptr;
         }
      }
   }
}
/************************************************************************/

static void near count_all_defined_references()
{
register int count;
int found;
register function_type *f_list_ptr;

f_list_ptr = function_list;         /* the full list */

for( count = 0; count < count_of_functions; ++count )
   {
   found = binary_search_sorted_data_base( f_list_ptr->functions_name );
   if( found >= 0 )
      scan_for_static_or_global( &found,
                                 f_list_ptr->static_function,
                                 f_list_ptr->functions_name,
                                 f_list_ptr->its_filename
                               );
   if( found >= 0 )
      array_of_ptrs_to_records[ found ]->number_of_references +=
         f_list_ptr->is_referenced;
   ++f_list_ptr;        /* for all defined functions */
   }
if( !g_quiet_flag && g_dec_def_flag )
   (void)printf( "\n" );
}
/***************************************************************************/

static void near show_function_relationships( )
{
int found;
int record_index;

found = binary_search_sorted_data_base( target );/* w/o knowing filename */
                  /* note if static, will find random one if more than */
                  /* one with same name */
if( found >= 0 )
   {
   recursion_depth = 0;
   if( !g_quiet_flag )
      {
      (void)printf( "Checking for usage...\n" );
      }
   count_all_defined_references();
   nesting_display_buffer[ 0 ] = '\0';
   if( !g_quiet_flag )
      {
      (void)printf( "Starting the printout...\n" );
      }
   if( !target_flag )               /* main is only called once */
      array_of_ptrs_to_records[ found ]->number_of_references = 1;
   line = 0;
   if( !stats_only )
      {
      (void)doprint( found );       /* of target function */
      for( record_index = 0;
           record_index < count_of_valid_records;
           ++record_index
         )
         {
         (void)fprintf( output, "\n" );
         ++line;
         if( array_of_ptrs_to_records[ record_index ]->number_of_references >
             1
           )
            (void)doprint( record_index );
         }
      }
   }
else        /* cant find target */
   {
   (void)printf( "cant find %s, exitting\n", target );
   exit( 1 );
   }
}
/***************************************************************************/

static void near show_line_and_byte_counts( )
{
long int total_byte_count;
long int total_line_count;
int i;

file_record_array_ptr = file_record_array;

do_top_of_page();
(void)fprintf( output, "File statistics:\n" );
bump_line_count();
total_byte_count = 0l;
total_line_count = 0l;
for( i = 0; i < count_of_source_files; ++i )
   {
   (void)fprintf( output,
                  "%-40s - %8u lines, %12ld bytes\n",
                  file_record_array_ptr->source_filename,
                  file_record_array_ptr->line_count,
                  file_record_array_ptr->size
                );
   bump_line_count();

   total_byte_count += file_record_array_ptr->size;
   total_line_count += file_record_array_ptr->line_count;
   ++file_record_array_ptr;
   }
(void)fputc( '\n', output );
bump_line_count();
(void)fprintf( output, "Totals:\n" );
bump_line_count();
/********                       "%-40s - %8u lines, %12ld bytes\n", *******/
(void)fprintf( output, "%4d files%-30s - %8ld lines, %12ld bytes\n",
               count_of_source_files, " ", total_line_count, total_byte_count
             );
bump_line_count();
(void)fputc( '\n', output );
bump_line_count();
(void)fprintf( output,
               " %d defined functions found.\n", count_of_valid_records
             );
bump_line_count();
(void)fprintf( output, "Averages:\n" );
bump_line_count();
(void)fprintf( output,
               "%6d lines/file, %6d functions/file, %6d lines/function\n",
               (int)( total_line_count / count_of_source_files ),
               (int)( count_of_valid_records / count_of_source_files ),
               (int)( total_line_count / count_of_valid_records )
             );
}
/***************************************************************************/

static void near show_sorted_function_list( )
{
int i, record_index;
long reference_total = 0;

do_top_of_page();

(void)fprintf( output, "Function index:\n" );
bump_line_count();

if( g_ov_flag )
   (void)fprintf( output, "%-39s %-28s %s %s\n",
                  "function", "in file", "ov#", "refs" );
else
   (void)fprintf( output, "%-39s %-28s    %s\n",
                  "function", "in file", "refs" );

bump_line_count();

for( i = 0; i < effective_width; ++i )
   (void)fputc( '_', output );
(void)fprintf( output, "\n" );
bump_line_count();

for( record_index = 0;
     record_index < count_of_valid_records;
     ++record_index
   )
   {
   data_base_array_ptr = array_of_ptrs_to_records[ record_index ];
   if( data_base_array_ptr->number_of_references > 0 )
      {   
      if( g_ov_flag && data_base_array_ptr->overlay_number )
         (void)fprintf( output, "%-7s%-32s %-28s %3d %d\n",
                        ( data_base_array_ptr->static_definition )?
                        "static": "",
                        data_base_array_ptr->defined_function,
                  ( data_base_array_ptr->file_record_ptr )->source_filename,
                        data_base_array_ptr->overlay_number,
                        data_base_array_ptr->number_of_references
                      );
      else
         (void)fprintf( output, "%-7s%-32s %-28s     %d\n",
                        ( data_base_array_ptr->static_definition )?
                        "static": "",
                        data_base_array_ptr->defined_function,
                  ( data_base_array_ptr->file_record_ptr )->source_filename,
                        data_base_array_ptr->number_of_references
                      );
      reference_total += (long)data_base_array_ptr->number_of_references;
      bump_line_count();
      }
   }
(void)fprintf( output, "%-7s%-32s %-28s     %s\n",
               " ", " ", " ", "____"
             );
bump_line_count();
(void)fprintf( output, "%-7s%-32s %-28s     %ld\n",
               " ", " ", "total ", reference_total
             );
bump_line_count();
}
/***************************************************************************/

static void near show_page_references( )
{
int pmax;          /* max x ref columns */
int i, pcnt;
linked_pages_list *p;

if( !stats_only && ( defined_page_length > 0 ) )
   {
   pmax = (int)( effective_width - 7 - 32 - 2 ) / 5;
   do_top_of_page();
   (void)fprintf( output, "Function cross reference:\n" );
   bump_line_count();

   for( i = 0; i < count_of_valid_records; ++i )
      {
      data_base_array_ptr = array_of_ptrs_to_records[ i ];
      if( data_base_array_ptr->number_of_references > 0 )
         {
         (void)fprintf( output, "%-7s%-32s- ",
                        ( data_base_array_ptr->static_definition )?
                        "static": "",
                        data_base_array_ptr->defined_function );
         p = data_base_array_ptr->ptr_to_page_list;
         if( p )
            {
            pcnt = 0;
            while( p->next_page_ptr )
               {
               (void)fprintf( output, "%4d,", p->on_this_page );
               p = p->next_page_ptr;
               ++pcnt;
               if( pcnt >= pmax )
                  {
                  (void)fputc( '\n', output );
                  bump_line_count();
                  (void)fprintf( output, "%7s%32s  ", " ", " " );
                  pcnt = 0;
                  }
               }
            (void)fprintf( output, "%4d\n", p->on_this_page );
            }
         else
            (void)fprintf( output, "\n" );
         bump_line_count();
         }
      }
   }
}
/***************************************************************************/

static void near show_unused_if_any( )
{
int i, unused_count, unused_index, count, still_sorting_flag;
data_base_record_type **unused_list_ptr_ptr, *unused_list_ptr;

do_top_of_page();
(void)fprintf( output, "Un-used function list:\n" );
bump_line_count();

unused_count = 0;
for( i = 0; i < count_of_valid_records; ++i )
   {
   data_base_array_ptr = array_of_ptrs_to_records[ i ];
   if( !data_base_array_ptr->number_of_references )
      {
      ++unused_count;
      if( !g_un_flag )
         {
         (void)fprintf( output,
                        "%-7s%-32s- %-33s\n",
                        ( data_base_array_ptr->static_definition )?
                        "static": "",
                        data_base_array_ptr->defined_function,
                     ( data_base_array_ptr->file_record_ptr )->source_filename
                      );
         bump_line_count();
         }
      }
   }
if( g_un_flag )               /* show sorted */
   {
   if( unused_count )
      {
      if(
         !( array_of_unused_ptrs_to_records =
            (data_base_record_type **)malloc( (unsigned int)unused_count )
          )
        )
         (void)printf( "No room for *array_of_unused_ptrs_to_records\n" );
      else
         {
         unused_index = 0;
         for( i = 0; i < count_of_valid_records; ++i )
            {
            data_base_array_ptr = array_of_ptrs_to_records[ i ];
            if( !data_base_array_ptr->number_of_references )
               {                    /* first just collect them */
               array_of_unused_ptrs_to_records[ unused_index++ ] =
                  data_base_array_ptr;
               }
            }                 /* so now there are unused_index of them */
         unused_list_ptr_ptr = array_of_unused_ptrs_to_records;
         still_sorting_flag = true;
         if( unused_count > 1 )
            {
            while( still_sorting_flag )
               {
               still_sorting_flag = false;
               if( !g_quiet_flag && g_tech_flag )
                  (void)printf( ".%d   \r", count );
               for( count = 0; count < unused_count - 1; ++count )
                  {
                  if( strcmp( unused_list_ptr_ptr[ count ]->
                              file_record_ptr->source_filename,
                              unused_list_ptr_ptr[ count + 1 ]->
                              file_record_ptr->source_filename
                            ) > 0
                    )
                     {
                     still_sorting_flag = true;
                     unused_list_ptr = unused_list_ptr_ptr[ count ];
                     unused_list_ptr_ptr[ count ] =
                        unused_list_ptr_ptr[ count + 1 ];
                     unused_list_ptr_ptr[ count + 1 ] = unused_list_ptr;
                     }
                  }
               }
            }
         for( i = 0; i < unused_count; ++i )
            {
            (void)fprintf( output,
                           "%-7s%-32s- %-33s\n",
                           ( unused_list_ptr_ptr[ i ]->static_definition )?
                           "static": "",
                           unused_list_ptr_ptr[ i ]->defined_function,
               ( unused_list_ptr_ptr[ i ]->file_record_ptr )->source_filename
                         );
            bump_line_count();
            }
         }
      }
   }
if( !unused_count )
   {
   tab_to_left_margin( output );
   (void)fprintf( output, "No un-used functions in the list.\n" );
   bump_line_count();
   }
else
   {
   (void)fprintf( output, "%-7s%-39s- %d\n", "", "totals", unused_count );
   bump_line_count();
   }
}
/************************************************************************/

static void near show_library_functions( )
{
register int count;
int found, total, still_sorting_flag, x_count, final_count, final_call;
function_type **f_list_ptr_ptr, *f_list_ptr;

if( g_lib_flag )
   {
   if( !g_quiet_flag && g_tech_flag )
      (void)printf( "collecting library functions...\n" );
   do_top_of_page();
   (void)fprintf( output, "Library functions:\n" );
   bump_line_count();

   total = 0;
   f_list_ptr = function_list;
   for( count = 0; count < count_of_functions; ++count )
      {
      if( !f_list_ptr->static_function )
         {
         if(
            ( found =
              binary_search_sorted_data_base( f_list_ptr->functions_name )
            ) < 0
           )
            sorted_called_list_ptrs[ total++ ] = f_list_ptr;
         }
      ++f_list_ptr;        /* for all called functions */
      }

   if( !g_quiet_flag && g_tech_flag )
      (void)printf( "gathering identical library functions...\n" );
   final_count = total;    /* number of calls to be collected and sorted */
   f_list_ptr_ptr = sorted_called_list_ptrs;
   for( count = 0; count < ( total - 1 ); ++count )
      {
      for( x_count = count + 1; x_count < total; ++x_count )
         {
         if( ( f_list_ptr_ptr[ count ]->functions_name[ 0 ] != '\0' ) &&
             !strcmp( f_list_ptr_ptr[ count ]->functions_name,
                      f_list_ptr_ptr[ x_count ]->functions_name )
           )
            {
            f_list_ptr_ptr[ count ]->is_referenced +=
               f_list_ptr_ptr[ x_count ]->is_referenced;
            f_list_ptr_ptr[ x_count ]->functions_name[ 0 ] = '\0';
            --final_count;
            }
         }
      }

   if( !g_quiet_flag && g_tech_flag )
      {
      (void)printf( "\nSorting the library function calls...\n" );
      }

   f_list_ptr_ptr = sorted_called_list_ptrs;
   still_sorting_flag = true;
   while( still_sorting_flag )
      {
      still_sorting_flag = false;
      if( !g_quiet_flag && g_tech_flag )
         (void)printf( ".%d   \r", count );
      for( count = 0; count < total - 1; ++count )
         {
         if( strcmp( f_list_ptr_ptr[ count ]->functions_name,
                     f_list_ptr_ptr[ count + 1 ]->functions_name ) > 0 )
            {
            still_sorting_flag = true;
            f_list_ptr = f_list_ptr_ptr[ count ];
            f_list_ptr_ptr[ count ] = f_list_ptr_ptr[ count + 1 ];
            f_list_ptr_ptr[ count + 1 ] = f_list_ptr;
            }
         }
      }
   if( !g_quiet_flag && g_tech_flag )
      (void)printf( "\n" );

   (void)fprintf( output, "%-32s %-28s\n",
                  "library function", "calls" );
   bump_line_count();

   for( count = 0; count < effective_width; ++count )
      (void)fputc( '_', output );
   (void)fprintf( output, "\n" );
   bump_line_count();

   final_call = 0;
   f_list_ptr_ptr = sorted_called_list_ptrs;
   for( count = 0; count < total; ++count )
      {
      if( ( *f_list_ptr_ptr )->functions_name[ 0 ] != '\0' )
         {
         (void)fprintf( output, "%-32s %d\n",
                        ( *f_list_ptr_ptr )->functions_name,
                        ( *f_list_ptr_ptr )->is_referenced
                      );
         final_call += ( *f_list_ptr_ptr )->is_referenced;
         bump_line_count();
         }
      ++f_list_ptr_ptr;
      }
   (void)fprintf( output, "Totals:\n" );
   bump_line_count();
   (void)fprintf( output, "%6d %-25s %d calls.\n",
                  final_count, "library functions,", final_call
                );
   bump_line_count();
   }
}
/************************************************************************/

static void near show_files_leading_comments( )
{
int i;
char *cp;

if( g_comment_flag )
   {
   do_top_of_page();
   (void)fprintf( output, "File comments:\n" );
   bump_line_count();
   file_record_array_ptr = file_record_array;
   for( i = 0; i < count_of_source_files; ++i )
      {
      (void)fprintf( output, "%40s\n",
                     file_record_array_ptr->source_filename
                   );
      bump_line_count();
      cp = file_record_array_ptr->source_file_comment;
      while( *cp )
         {
         (void)fprintf( output, "%c", *cp );
         if( *++cp == '\n' )
            {
            bump_line_count();
            }
         }
      ++file_record_array_ptr;   
      do_top_of_page();          /* one page per comment at least */
      }
   }
}
/**********************************************************************/

int main( argc, argv )
char **argv;
int argc;
{
int   index, in_error = false, out_error = false;
FILE  *stream;

nasty( argc );

(void)printf( "\ncp - ver. 1.3,  (C)1987, 1988  Stewart A. Nutter\n" );
(void)printf( "    extended and corrected by  Ron Winter\n" );

index = 1;
if( !( stream = fopen( argv[ index ], "rt" ) ) )
   in_error = true;
else
   ++index;
if(
   ( argc > index ) &&
   (
    ( argv[ index ][ 0 ] != '/' ) && ( argv[ index ][ 0 ] != '-' )
   )
  )
   {
   output = fopen( argv[ 2 ], "w+" );     /******* wt+ <<<<<<<< ******/
   ++index;
   }
else
   output = fopen( "prn", "w+" );         /******** wt+ <<<<<< ********/

if( !output )
   out_error = true;

Max_functions = MAX_functions;
process_arguments( index, argc, argv, in_error || out_error );
if( in_error )
   {
   (void)printf( "\n can't open input list %s\n", argv[ 1 ] );
   exit( 1 );
   }
if( out_error )
   {
   (void)printf( "\n can't open output file, error %s\n", strerror( errno ) );
   exit( 1 );
   }
allocate_arrays( );
initialize_globals( );
(void)printf( "\n" );

build_records_from_list( stream );
sort_the_data_base_array( );
if( !g_quiet_flag )
   {
   (void)printf( "\n" );
   }
top_of_form_done = false;
show_function_relationships( );
show_page_references( );
show_line_and_byte_counts( );
show_sorted_function_list( );
show_unused_if_any( );
show_library_functions( );
show_files_leading_comments( );
deallocate_arrays( );

/************* done *****************/
(void)fprintf( output, "%c", 0x0c );   /* ff */

return false;     /* ok */
}
/********************************************************************/


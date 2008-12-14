/*********************************************************************
                                  cpheader.h
 *********************************************************************/

#include <malloc.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define  Max_unget_buffer        20000
#define  Max_general_buffers     3000
#define  MAX_functions           5000
/* #define  Max_functions           4000 */
#define  Max_defined_functions   1400
#define  Max_files               1400
#define  Max_Recursion           50

#define  false       0
#define  true        1
#define  completed   2

#define  Escape      0x1b
#define  Control_z   0x1a

/*********************************************************************/
typedef struct the_Pages
   {
   int               on_this_page;
   struct the_Pages  *next_page_ptr;
   }linked_pages_list;
/**********************************************************************/
typedef struct
   {
   char              *functions_name;
   char              *its_filename;
   int               is_referenced;
   int               static_function;
   }function_type;
/**********************************************************************/
typedef struct
   {
   char              *source_filename;
   char              *source_file_comment;
   unsigned int      line_count;
   long              size;
   }file_record_type;
/**********************************************************************/
typedef struct                   /* this is the main data base record */
   {
   file_record_type  *file_record_ptr;
   char              *defined_function;
   function_type     *ptr_to_function_table;
   int               number_of_function_calls;
   linked_pages_list *ptr_to_page_list;
   int               number_of_references;
   int               static_definition;
   int               overlay_number;
   }data_base_record_type;
/**********************************************************************/

#if MAIN != 0
/***********************************************************************/

function_type              /* 6 */
   **sorted_called_list_ptrs,
   *function_list,
   *function_list_ptr;
int
   Max_functions,
   count_of_functions = 0;
/********************************/
file_record_type           /* 14 */
   *file_record_array,
   *file_record_array_ptr;
int
   count_of_source_files = 0;
/********************************/
data_base_record_type      /* 20 */
   *data_base_array,
   *data_base_array_ptr,
   **array_of_unused_ptrs_to_records,
   **array_of_ptrs_to_records;
int
   count_of_valid_records = 0;
/********************************/

char *recursion_array[ Max_Recursion ];
int recursion_depth = 0;

char nesting_display_buffer[ Max_general_buffers ];

char target[ 40 ] = "main";
FILE *output = NULL;

char push_buffer[ Max_unget_buffer ] = { 0, 0, 0, 0 };
char *push_buffer_ptr;

char file_comment_buffer[ Max_general_buffers ];
int first_comment;

int effective_width;

int
   page = 1,
   line = 0,
   defined_page_width =    80,
   defined_page_length =   60,
   defined_left_margin =   1,
   defined_right_margin =  1,
   stats_only =      false,
   g_lib_flag =      false,
   g_comment_flag =  false,
   g_dec_def_flag =  false,
   g_help_flag =     false,
   ibm_flag =        true,
   g_quiet_flag =    false,
   g_tech_flag =     false,
   g_ov_flag =       false,
   g_un_flag =       false,
   target_flag =     false;
int top_of_form_done;
char title[] =
/*       mm/dd/yy0 hh:mm:ss0 */
      { "                    C PRINTER - (c) 1987, 1988 rev. 1.3" };

/********************************************************************/

#else
/*********************************************************************/

extern function_type
   **sorted_called_list_ptrs,
   *function_list,
   *function_list_ptr;
extern file_record_type
   *file_record_array,
   *file_record_array_ptr;
extern data_base_record_type
   *data_base_array,
   *data_base_array_ptr,
   **array_of_unused_ptrs_to_records,
   **array_of_ptrs_to_records;
extern char *recursion_array[ ];
extern int
   count_of_valid_records,
   Max_functions,
   count_of_functions,
   count_of_source_files;
extern int page, line, recursion_depth;
extern int first_comment;
extern char nesting_display_buffer[ ];
extern char top_bottom_line_of_box[ ];
extern FILE *output;
extern char push_buffer[ ];
extern char *push_buffer_ptr;
extern char file_comment_buffer[ ];
extern int defined_page_width;
extern int defined_page_length;
extern int defined_left_margin;
extern int defined_right_margin;
extern int effective_width;
extern char target[ ];
extern int
   stats_only,
   g_lib_flag,
   g_comment_flag,
   g_dec_def_flag,
   g_help_flag,
   ibm_flag,
   g_quiet_flag,
   g_tech_flag,
   g_ov_flag,
   g_un_flag,
   target_flag;
extern int top_of_form_done;
extern char title[];
/*********************************************************************/

#endif
/**********************************************************************/


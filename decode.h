#ifndef DECODE_H
#define DECODE_H
#include<string.h>

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname; 
    FILE *fptr_src_image;

    /* Destin Image info */
    char * dest_name;
    FILE * fptr_dest;
    char * out;

    char magic_string[2];

    int extn_size;
    char extn[10];
    int file_size;

} DecodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status decode_open_files(DecodeInfo *decInfo);

Status skip_bmp_header(FILE *fptr_src_image);

/* Encode a byte into LSB of image data array */
char decode_lsb_to_byte(char *image_buffer);

int decode_lsb_to_size(char *buffer);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

Status validate_magic_string(char * magic_string);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status Create_open_dest_file(DecodeInfo *decInfo);

Status decode_file_data_size(DecodeInfo *decInfo);

Status decode_file_data(DecodeInfo *decInfo);


#endif

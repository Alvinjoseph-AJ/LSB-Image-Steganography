#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /*
    1.check argv[2] is null?
    yes-> return e_failure
    2.check argv[2] does not have ".bmp" in it?
    yes -> return e_failure
    3.store argv[2] to encinfo ->src_imgage_fname
    4.validate secret file and store to encinfo
    5.check argv[4] != null
    yes ->
        check argv[4] has ".bmp"?
        yes-> store to eninfo
        no-> store "stego.bmp" to encinfo
    no-> store "stego.bmp" to eninfo
    6.return e_success

    */

    if (argv[2] == NULL || (strstr(argv[2], ".bmp") == NULL))
    {
        return e_failure;
    }

    encInfo->src_image_fname = argv[2];
    printf("Source image name: %s\n", encInfo->src_image_fname);

    if (argv[3] == NULL )
    {
        return e_failure;
    }
   // || (strstr(argv[3], ".txt") == NULL)
    encInfo->secret_fname = argv[3];
    printf("Secret file name: %s\n", encInfo->secret_fname);

    if (argv[4] != NULL)
    {

        if (strstr(argv[4], ".bmp") != NULL)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            (encInfo->stego_image_fname = "stego.bmp");
        }
        printf("Stego image name: %s\n", encInfo->stego_image_fname);
    }
    else
    {

        (encInfo->stego_image_fname = "stego.bmp");
        printf("Stego image name: %s\n", encInfo->stego_image_fname);
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    /*
    1.call open_files()
    2.call check_capacity()
    3.call copy_bmp_header()
    4.call encode_magic_string()
    */

    if (open_files(encInfo) == e_failure)
    {
        printf("INFO : OPEN FILE FUNCTION IS FAILED !!!!! \n\n");
        return e_failure;
    }
    printf("INFO : OPEN FILE FUNCTION IS SUCCESSFULLY COMPLETED\n\n");

    if (check_capacity(encInfo) == e_failure)
    {
        printf("INFO : CHECK CAPACITY FUCTION IS FAILED !!!!!\n\n");
        return e_failure;
    }
    printf("INFO : CHECK CAPACITY FUNCTION IS SUCCESSFULLY COMPLETED\n\n");

    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("INFO : COPY BMP HEADER FUNCTION IS FAILED !!!!!!\n\n");
        return e_failure;
    }
    printf("INFO : COPY BMP HEADER FUNCTION IS SUCCESSFULLY COMPLETED\n\n");

    if (encode_magic_string("#*", encInfo) == e_failure)
    {
        printf("INFO : ENCODE OF MAGIC_STRING FAILED !!!!!!\n\n");
        return e_failure;
    }
    printf("INFO : ENCODE OF MAGIC_STRING IS SUCCESSFULLY COMPLETED\n\n");

    // to calculate secret file extn size
    char * sec_ext = strstr(encInfo->secret_fname,".");
    encInfo->extn_size = strlen(sec_ext);
    if (encode_secret_file_extn_size(encInfo->extn_size,encInfo) == e_failure)
    {
        printf("INFO : ENCODE OF SECRET FILE EXTN SIZE FAILED !!!!!!\n\n");
        return e_failure;
    }
    printf("INFO : ENCODE OF SECRET FILE EXTN SIZE IS SUCCESSFULLY COMPLETED\n\n");
    if (encode_secret_file_extn(sec_ext,encInfo) == e_failure)
    {
        printf("INFO : ENCODE OF SECRET FILE EXTN FAILED !!!!!!\n\n");
        return e_failure;
    }
    printf("INFO : ENCODE OF SECRET FILE EXTN IS SUCCESSFULLY COMPLETED\n\n");
    fseek(encInfo->fptr_secret,0,SEEK_END);
    encInfo->size_secret_file = ftell(encInfo->fptr_secret);
    rewind(encInfo->fptr_secret);

    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        printf("INFO : ENCODE OF SECRET FILE SIZE FAILED !!!!!!\n\n");
        return e_failure;
    }
    printf("INFO : ENCODE OF SECRET FILE SIZE IS SUCCESSFULLY COMPLETED\n\n");
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("INFO : ENCODE OF SECRET FILE FAILED !!!!!!\n\n");
        return e_failure;
    }
    printf("INFO : ENCODE OF SECRET FILE IS SUCCESSFULLY COMPLETED\n\n");
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("INFO : ENCODE OF SECRET FILE FAILED !!!!!!\n\n");
        return e_failure;
    }
    printf("INFO : ENCODE OF SECRET FILE IS SUCCESSFULLY COMPLETED\n\n");

    printf("------!!!ENCODED!!!!-------\n\n");
    
}

Status check_capacity(EncodeInfo *encInfo)
{
    long int capacity;

    int magic_len = strlen("#*");
    int extn_size = 4;
    char *ext_add = strstr(encInfo->secret_fname, ".");
    int ext = strlen(ext_add);
    int sec_size = 4;
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    uint file = get_file_size(encInfo->fptr_secret);
    capacity = 54 + (8 * (magic_len + extn_size + ext + file));
    // printf("%d\n", file);
    // printf("%d\n",encInfo->image_capacity );
    // printf("%ld\n", capacity);
    if (encInfo->image_capacity >= capacity)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }

    /*
     magic str -> 2 byte
     extn size -> 4 bytes
     extn ->
     file size -> 4 bytes
     file data
     */
    // encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    // encInfo->size_secret_file= get_file_size(encInfo->fptr_secret);

    //
    //  int magic_extension = 4;
    //  int full_size = 54+(8 * (magic_len + magic_extension + encInfo->size_secret_file));
    //
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    uint ret = ftell(fptr);
    return ret;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char *str[54];
    rewind(fptr_src_image);

    fread(str, sizeof(char), 54, fptr_src_image);

    fwrite(str, sizeof(char), 54, fptr_dest_image);

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char str[8];
    int len = strlen(magic_string);
    int ret = 0;
    fseek(encInfo->fptr_src_image, 54, SEEK_SET);
    fseek(encInfo->fptr_stego_image, 54, SEEK_SET);
    for (int i = 0; i < len; i++)
    {
        fread(str, sizeof(char), 8, encInfo->fptr_src_image);
        ret = encode_byte_to_lsb(magic_string[i], str);
        fwrite(str, sizeof(char), 8, encInfo->fptr_stego_image);
    }
    if (ret == e_failure)
    {
        printf("Encode of magic string failed\n");
        return e_failure;
    }
    return e_success;
}
Status encode_byte_to_lsb(char  data, char *image_buffer)
{
    int pos = 7;
    for (int i = 0; i<8; i++)
    {
        unsigned char mask = 1<< (pos-i);
        unsigned char res = (data & mask) >> (pos -i);
        image_buffer[i] = image_buffer[i] & 0xFE;
        image_buffer[i] = image_buffer[i] | res;
    }
}

Status encode_secret_file_extn_size(int extn_len,EncodeInfo *encInfo)
{
    char arr[32];
    int ret = 0;
    fread(arr,sizeof(char),32,encInfo->fptr_src_image);
    ret = encode_size_to_lsb(extn_len,arr);
    fwrite(arr,sizeof(char),32,encInfo->fptr_stego_image);
    if (ret == e_failure)
    {
        printf("Encode of secret file extn size failed\n");
        return e_failure;
    }
    return e_success;

}

Status encode_size_to_lsb(int extn_size,char * image_buffer)
{
    int pos=31;
    for(int i=0;i<32;i++)
    {
        unsigned int mask = extn_size & (1 << (pos-i));
        mask = mask >> (pos-i);
        image_buffer[i] = image_buffer[i] & 0xFE;
        image_buffer[i] = image_buffer[i] | mask;
    }
    return e_success;

}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char str[8];
    int len = strlen(file_extn);
    int ret = 0;
    for (int i = 0; i < len; i++)
    {
        fread(str, sizeof(char), 8, encInfo->fptr_src_image);
        
        ret = encode_byte_to_lsb(file_extn[i], str);
        fwrite(str, sizeof(char), 8, encInfo->fptr_stego_image);
    }
    if (ret == e_failure)
    {
        printf("Encode of secret file extn failed\n");
        return e_failure;
    }
    return e_success;
    
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char arr[32];
    int ret = 0;
    fread(arr,sizeof(char),32,encInfo->fptr_src_image);
    ret = encode_size_to_lsb(file_size,arr);
    fwrite(arr,sizeof(char),32,encInfo->fptr_stego_image);
    if (ret == e_failure)
    {
        printf("Encode of secret file extn size failed\n");
        return e_failure;
    }
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char str[8];
    int ret = 0;
    char data;
    int len = encInfo->size_secret_file;
    for (int i = 0; i < len; i++)
    {
        fread(&data, sizeof(char), 1, encInfo->fptr_secret);
        fread(str, sizeof(char), 8, encInfo->fptr_src_image);
        ret = encode_byte_to_lsb(data, str);
        fwrite(str, sizeof(char), 8, encInfo->fptr_stego_image);
    }
    if (ret == e_failure)
    {
        printf("Encode of secret file failed\n");
        return e_failure;
    }
    return e_success;

}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    int ch;
    while((ch = fgetc(fptr_src))!= EOF)
    {
        fputc(ch,fptr_dest);
    }
    return e_success;   
}

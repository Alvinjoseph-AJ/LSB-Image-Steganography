#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>


Status decode_open_files(DecodeInfo*decInfo)
{
   
    // Stego Image file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (argv[2] == NULL || (strstr(argv[2], ".bmp") == NULL))
    {
        return e_failure;
    }

    decInfo->src_image_fname = argv[2];

    if (argv[3] != NULL)
    {
        
        decInfo->out=argv[3];
        
    }
    else
    {
       decInfo->out = "new";
    }

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (decode_open_files(decInfo) == e_failure)
    {
        printf("INFO : OPEN FILE FUNCTION IS FAILED !!!!! \n\n");
        return e_failure;
    }
    printf("INFO : OPEN FILE FUNCTION IS SUCCESSFULLY COMPLETED\n\n");

    if (skip_bmp_header(decInfo->fptr_src_image) == e_failure)
    {
        printf("INFO : SKIPPING OF BMP_HEADER FAILED !!!!! \n\n");
        return e_failure;
    }
    printf("INFO : SKIPPING OF BMP_HEADER IS SUCCESSFULLY COMPLETED\n\n");

    if (decode_magic_string(decInfo) == e_failure)
    {
        printf("INFO :DECODING OF MAGIC_STRING FAILED !!!!! \n\n");
        return e_failure;
    }
    printf("INFO : DECODING OF MAGIC_STRING IS SUCCESSFULLY COMPLETED\n\n");

    if (validate_magic_string(decInfo->magic_string) == e_failure)
    {
        printf("INFO :VALIDATION OF MAGIC_STRING FAILED !!!!! \n\n");
        return e_failure;
    }
    printf("INFO : VALIDATION OF MAGIC_STRING IS SUCCESSFULLY COMPLETED\n\n");

    if (decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("INFO :DECODING OF SECRET FILE EXTENSION SIZE  FAILED !!!!! \n\n");
        return e_failure;
    }
    printf("INFO : DECODING OF SECRET FILE EXTENSION SIZE IS SUCCESSFULLY COMPLETED : %d\n\n",decInfo->extn_size);

    if (decode_secret_file_extn(decInfo) == e_failure)
    {
        printf("INFO :DECODING OF SECRET FILE EXTENSION FAILED !!!!! \n\n");
        return e_failure;
    }
    printf("INFO : DECODING OF SECRET FILE EXTENSION IS SUCCESSFULLY COMPLETED : %s\n\n", decInfo->extn);

    if (Create_open_dest_file(decInfo) == e_failure)
    {
        printf("INFO :CREATION OF DEST FILE FAILED !!!!! \n\n");
        return e_failure;
    }
    printf("INFO : CREATION OF DEST FILE IS SUCCESSFULLY COMPLETED\n\n");

    if (decode_file_data_size(decInfo) == e_failure)
    {
        printf("INFO :DECODING OF FILE SIZE FAILED !!!!! \n\n");
        return e_failure;
    }
    printf("INFO : DECODING OF FILE SIZE  IS SUCCESSFULLY COMPLETED: %d bytes\n\n", decInfo->file_size);

    if (decode_file_data(decInfo) == e_failure)
    {
        printf("INFO :DECODING OF FILE FAILED !!!!! \n\n");
        return e_failure;
    }
    printf("INFO : DECODING OF FILE IS SUCCESSFULLY COMPLETED\n\n");

    printf("-----!!!DECODED!!!!-----\n\n");

    return e_success;



    //2.skip_bmp_header(FILE *fptr_src)
    //char lsb_byte(char*buffer)
    //3.decode_magic_string(DecodeInfo *decInfo)
    //4. validate magic string(decInfo->magic_string)
    //5.decode extn_size(DecodeInfo *decInfo)
    //6.decode extn(DecodeInfo *decInfo)
    //7.Create_open_dest_file(DecodeInfo *decInfo)
    //8.decode_file_size(DecodeInfo *decInfo)
    //9.decode_file_data(DecodeInfo *decInfo)
    //10.close files(2 fptr);
    //11.return success;

}

Status skip_bmp_header(FILE *fptr_src_image)
{
    if(fseek(fptr_src_image,54,SEEK_SET)!=0)
    {
        return e_failure;
    }
    return e_success;
}

char decode_lsb_to_byte(char *image_buffer)
{
    unsigned char ch = 0;
    for (int i = 0; i < 8; i++)
    {
        ch = ch << 1 | (image_buffer[i] & 1);

    }
    return ch; 
}

int decode_lsb_to_size(char *buffer)
{
    int size = 0;

    for (int i = 0; i < 32; i++)  
    {  
        size = (size << 1) | (buffer[i] & 1);  
    }
    return size;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
//     run a loop for two time
//     read 8 byte buffer
//     magic_string[i]=call lsb to byte(buffer);
    char buffer[8];
    for (int i = 0; i < 2; i++)
    {
        if(fread(buffer, 1, 8, decInfo->fptr_src_image)==0)
        return e_failure;
        decInfo->magic_string[i] = decode_lsb_to_byte(buffer);
    }
    return e_success;
}

Status validate_magic_string(char * magic_string)
{
    /*
    1.read magic string from user 
    2.compare magic string 
    return sucess or faliure
    */
   if (strcmp(magic_string, "#*") != 0)
    {
        return e_failure;
    }
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    // 1.read 32 byte buffer from src_image
    // decInfo->extn_size = call lsb_to_size(buffer)
    // return succes;

    char buffer[32];
    if(fread(buffer, 1, 32, decInfo->fptr_src_image)==0)
    return e_failure;
    decInfo->extn_size = decode_lsb_to_size(buffer);

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{

//     run a loop extn_size
//     {
//         1.read 8 byte buffer
//         char extn[i] = decode_lsb_to_byte(buffer);
//         add a null character
//     }
    int len = decInfo->extn_size;
    char buffer[8];
    for(int i=0;i<len;i++)
    {
        if(fread(buffer, 1, 8, decInfo->fptr_src_image)==0)
        return e_failure;
        decInfo->extn[i] = decode_lsb_to_byte(buffer);
    }
    decInfo->extn[decInfo->extn_size] = '\0';
    return e_success;
}
Status Create_open_dest_file(DecodeInfo *decInfo)
{
    static char output_name[50];  
    strcpy(output_name, decInfo->out);
    strcat(output_name, decInfo->extn);
    decInfo->dest_name = output_name;
    // 1.concate dest_img with extn;
    decInfo->fptr_dest = fopen(decInfo->dest_name, "w");
    // 2.open new name file write mode
    return e_success;
}

Status decode_file_data_size(DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer, 1, 32, decInfo->fptr_src_image);
    decInfo->file_size = decode_lsb_to_size(buffer);
    return e_success;
}

Status decode_file_data(DecodeInfo *decInfo)
{
    // decode_lsb_to_byte
    // fwrite to the file 
    int len = decInfo->file_size;
    char buffer[8];
    for(int i=0;i<len;i++)
    {
        fread(buffer, 1, 8, decInfo->fptr_src_image);
        char decoded_char = decode_lsb_to_byte(buffer);
        fwrite(&decoded_char, 1, 1, decInfo->fptr_dest); 
    }
    
    return e_success;

}
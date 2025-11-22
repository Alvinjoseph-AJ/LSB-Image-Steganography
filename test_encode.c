#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char* argv[])
{
    EncodeInfo encInfo;

    DecodeInfo decInfo;

    /*int ret = call check_operation_type(argv)
    check ret is e_encode?
    yes -- ##encode
            1.ret_arg = call read_and_validate_encode_args(argv,)
            2.check ret_arg is e_success
              yes -> //call do encoding
              no -> print error and stop
            3.


    check ret is e_decode?
    yes -- ##decode
    else
    unsorted and stop
    
    */

    int ret = check_operation_type(argv);

    if(ret==e_encode)
    {
        
        int ret_arg = read_and_validate_encode_args(argv,&encInfo);
        if(ret_arg == e_success)
        {
            do_encoding(&encInfo);
        }
        else
        {
            printf("ERROR: INVALID NO OF ARGUMENTS\n");
            return 0;
        }
    }
    if(ret==e_decode)
    {
        
        int ret_arg = read_and_validate_decode_args(argv,&decInfo);
        if(ret_arg == e_success)
        {
            {
            do_decoding(&decInfo);
            }
        }
        else
        {
            printf("ERROR: INVALID NO OF ARGUMENTS\n");
            return 0;
        }
    }
    // uint img_size;

    // // Fill with sample filenames
    // encInfo.src_image_fname = "beautiful.bmp";
    // encInfo.secret_fname = "secret.txt";
    // encInfo.stego_image_fname = "stego_img.bmp";

    // // Test open_files
    // if (open_files(&encInfo) == e_failure)
    // {
    // 	printf("ERROR: %s function failed\n", "open_files" );
    // 	return 1;
    // }
    // else
    // {
    // 	printf("SUCCESS: %s function completed\n", "open_files" );
    // }

    // // Test get_image_size_for_bmp
    // img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    // printf("INFO: Image size = %u\n", img_size);

    return 0;
}
/*
    OperationType check_operation_type(char *argv[])
    {
    1.check argv[1] is not null;
    yes->  *check argv[1] is "-e"?
             yes-> return e_encode
           *check argv[1] is "-d"?
             yes-> return e_decode
           * return e_unsupported
    }

    */
OperationType check_operation_type(char *argv[])
{
    if(argv[1]!=NULL)
    {
        if(strcmp(argv[1], "-e") == 0)
        {
            return e_encode;
        }
        else if(strcmp(argv[1], "-d") == 0)
        {
            return e_decode;
        }
        else
        {
            return e_unsupported;
        }
    }
    else
    {
        printf("ERROR: INVALID NO OF ARGUMENTS\nInfo: ./a.out -e beautiful.bmp secret.txt (encoding)\nInfo: ./a.out -d encoded_bmp_file.bmp(decoding)\n");
    }
    
}



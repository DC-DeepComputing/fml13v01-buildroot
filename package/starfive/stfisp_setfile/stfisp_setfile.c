#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>

#ifndef VERSION
#define VERSION "unknown"
#endif

#define OV4689_SETFILE "ov4689_stf_isp_fw.bin"
#define OV4689_DUMP_SETFILE "ov4689_stf_isp_fw_dump.bin"
#define SC2235_SETFILE "sc2235_stf_isp_fw.bin"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef unsigned int u32;

typedef struct {
    u32 addr;
    u32 val;
    u32 mask;
    u32 delay_ms;
} regval_t;

struct reg_table {
    const regval_t *regval;
    int regval_num;
};

// 0x11BB, 0 1 0 1 2 3 2 3, R Gr R Gr Gb B Gb B
static const regval_t isp_sc2235_reg_config_list[] = {
    {0x00000014, 0x00000008, 0, 0},
    // {0x00000018, 0x000011BB, 0, 0},
    {0x00000A1C, 0x00000030, 0, 0},
    // {0x0000001C, 0x00000000, 0, 0},
    // {0x00000020, 0x0437077F, 0, 0},
    // {0x00000A0C, 0x04380780, 0, 0},
    // {0x00000A80, 0xF9000000, 0, 0},
    // {0x00000A84, 0xF91FA400, 0, 0},
    // {0x00000A88, 0x00000780, 0, 0},
    {0x00000A8C, 0x00000010, 0, 0},
    {0x00000A90, 0x00000000, 0, 0},
    {0x00000AC4, 0x00000000, 0, 0},
    {0x00000E40, 0x0000004D, 0, 0},
    {0x00000E44, 0x00000096, 0, 0},
    {0x00000E48, 0x0000001D, 0, 0},
    {0x00000E4C, 0x000001DA, 0, 0},
    {0x00000E50, 0x000001B6, 0, 0},
    {0x00000E54, 0x00000070, 0, 0},
    {0x00000E58, 0x0000009D, 0, 0},
    {0x00000E5C, 0x0000017C, 0, 0},
    {0x00000E60, 0x000001E6, 0, 0},
    {0x00000010, 0x00000000, 0, 0},
    {0x00000A08, 0x10000022, 0xFFFFFFF, 0},
    {0x00000044, 0x00000000, 0, 0},
    {0x00000008, 0x00010005, 0, 0},
    {0x00000A00, 0x00120002, 0, 0},
    {0x00000A00, 0x00120000, 0, 0},
    {0x00000A00, 0x00120001, 0, 0},
    {0x00000008, 0x00010004, 0, 0},
    {0x00000000, 0x00000001, 0, 0},
};

// 0x11BB, 0 1 0 1 2 3 2 3, R Gr R Gr Gb B Gb B
static const regval_t isp_1080p_reg_config_list[] = {
    {0x00000014, 0x0000000D, 0, 0},
    // {0x00000018, 0x000011BB, 0, 0},
    {0x00000A1C, 0x00000032, 0, 0},
    // {0x0000001C, 0x00000000, 0, 0},
    // {0x00000020, 0x0437077F, 0, 0},
    // {0x00000A0C, 0x04380780, 0, 0},
    // {0x00000A80, 0xF9000000, 0, 0},
    // {0x00000A84, 0xF91FA400, 0, 0},
    // {0x00000A88, 0x00000780, 0, 0},
    {0x00000A8C, 0x00000000, 0, 0},
    {0x00000A90, 0x00000000, 0, 0},
    {0x00000E40, 0x0000004C, 0, 0},
    {0x00000E44, 0x00000097, 0, 0},
    {0x00000E48, 0x0000001D, 0, 0},
    {0x00000E4C, 0x000001D5, 0, 0},
    {0x00000E50, 0x000001AC, 0, 0},
    {0x00000E54, 0x00000080, 0, 0},
    {0x00000E58, 0x00000080, 0, 0},
    {0x00000E5C, 0x00000194, 0, 0},
    {0x00000E60, 0x000001EC, 0, 0},
    {0x00000280, 0x00000000, 0, 0},
    {0x00000284, 0x00000000, 0, 0},
    {0x00000288, 0x00000000, 0, 0},
    {0x0000028C, 0x00000000, 0, 0},
    {0x00000290, 0x00000000, 0, 0},
    {0x00000294, 0x00000000, 0, 0},
    {0x00000298, 0x00000000, 0, 0},
    {0x0000029C, 0x00000000, 0, 0},
    {0x000002A0, 0x00000000, 0, 0},
    {0x000002A4, 0x00000000, 0, 0},
    {0x000002A8, 0x00000000, 0, 0},
    {0x000002AC, 0x00000000, 0, 0},
    {0x000002B0, 0x00000000, 0, 0},
    {0x000002B4, 0x00000000, 0, 0},
    {0x000002B8, 0x00000000, 0, 0},
    {0x000002BC, 0x00000000, 0, 0},
    {0x000002C0, 0x00F000F0, 0, 0},
    {0x000002C4, 0x00F000F0, 0, 0},
    {0x000002C8, 0x00800080, 0, 0},
    {0x000002CC, 0x00800080, 0, 0},
    {0x000002D0, 0x00800080, 0, 0},
    {0x000002D4, 0x00800080, 0, 0},
    {0x000002D8, 0x00B000B0, 0, 0},
    {0x000002DC, 0x00B000B0, 0, 0},
    {0x00000E00, 0x24000000, 0, 0},
    {0x00000E04, 0x159500A5, 0, 0},
    {0x00000E08, 0x0F9900EE, 0, 0},
    {0x00000E0C, 0x0CE40127, 0, 0},
    {0x00000E10, 0x0B410157, 0, 0},
    {0x00000E14, 0x0A210181, 0, 0},
    {0x00000E18, 0x094B01A8, 0, 0},
    {0x00000E1C, 0x08A401CC, 0, 0},
    {0x00000E20, 0x081D01EE, 0, 0},
    {0x00000E24, 0x06B20263, 0, 0},
    {0x00000E28, 0x05D802C7, 0, 0},
    {0x00000E2C, 0x05420320, 0, 0},
    {0x00000E30, 0x04D30370, 0, 0},
    {0x00000E34, 0x047C03BB, 0, 0},
    {0x00000E38, 0x043703FF, 0, 0},
    {0x00000010, 0x00000080, 0, 0},
    {0x00000A08, 0x10000032, 0xFFFFFFF, 0},
    {0x00000A00, 0x00120002, 0, 0},
    {0x00000A00, 0x00120000, 0, 0},
    {0x00000A50, 0x00000002, 0, 0},
    {0x00000A00, 0x00120001, 0, 0},
    {0x00000008, 0x00010000, 0, 0},
    {0x00000008, 0x0002000A, 0, 0},
    {0x00000000, 0x00000001, 0, 0},
};

static const regval_t isp_dump_1080p_reg_config_list[] = {
	{0x00000014, 0x0000000D,0,0},
	// {0x00000018, 0x000011BB,0,0},
	{0x00000A1C, 0x00000032,0,0},
	// {0x0000001C, 0x00000000,0,0},
	// {0x00000020, 0x0437077F,0,0},
	// {0x00000A0C, 0x04380780,0,0},
	// {0x00000A80, 0xF9000000,0,0},
	// {0x00000A84, 0xF91FA400,0,0},
	{0x00000A88, 0x00000780,0,0},
	// {0x00000024, 0xFB000000, 0, 0},	// Buffer size = 3179520
	{0x00000028, 0x00030B80, 0, 0},
	{0x00000A8C, 0x00000000,0,0},
	{0x00000A90, 0x00000000,0,0},
	{0x00000E40, 0x0000004C,0,0},
	{0x00000E44, 0x00000097,0,0},
	{0x00000E48, 0x0000001D,0,0},
	{0x00000E4C, 0x000001D5,0,0},
	{0x00000E50, 0x000001AC,0,0},
	{0x00000E54, 0x00000080,0,0},
	{0x00000E58, 0x00000080,0,0},
	{0x00000E5C, 0x00000194,0,0},
	{0x00000E60, 0x000001EC,0,0},
	{0x00000280, 0x00000000,0,0},
	{0x00000284, 0x00000000,0,0},
	{0x00000288, 0x00000000,0,0},
	{0x0000028C, 0x00000000,0,0},
	{0x00000290, 0x00000000,0,0},
	{0x00000294, 0x00000000,0,0},
	{0x00000298, 0x00000000,0,0},
	{0x0000029C, 0x00000000,0,0},
	{0x000002A0, 0x00000000,0,0},
	{0x000002A4, 0x00000000,0,0},
	{0x000002A8, 0x00000000,0,0},
	{0x000002AC, 0x00000000,0,0},
	{0x000002B0, 0x00000000,0,0},
	{0x000002B4, 0x00000000,0,0},
	{0x000002B8, 0x00000000,0,0},
	{0x000002BC, 0x00000000,0,0},
	{0x000002C0, 0x00800080, 0, 0},
	{0x000002C4, 0x00800080, 0, 0},
	{0x000002C8, 0x00800080,0,0},
	{0x000002CC, 0x00800080,0,0},
	{0x000002D0, 0x00800080,0,0},
	{0x000002D4, 0x00800080,0,0},
	{0x000002D8, 0x00800080, 0, 0},
	{0x000002DC, 0x00800080, 0, 0},
	{0x00000E00, 0x24000000,0,0},
	{0x00000E04, 0x159500A5,0,0},
	{0x00000E08, 0x0F9900EE,0,0},
	{0x00000E0C, 0x0CE40127,0,0},
	{0x00000E10, 0x0B410157,0,0},
	{0x00000E14, 0x0A210181,0,0},
	{0x00000E18, 0x094B01A8,0,0},
	{0x00000E1C, 0x08A401CC,0,0},
	{0x00000E20, 0x081D01EE,0,0},
	{0x00000E24, 0x06B20263,0,0},
	{0x00000E28, 0x05D802C7,0,0},
	{0x00000E2C, 0x05420320,0,0},
	{0x00000E30, 0x04D30370,0,0},
	{0x00000E34, 0x047C03BB,0,0},
	{0x00000E38, 0x043703FF,0,0},
	{0x00000010, 0x00080080, 0, 0},
	{0x00000A08, 0x10000032,0xFFFFFFF,0},
	{0x00000A00, 0x00120002,0,0},
	{0x00000A00, 0x00120000,0,0},
	{0x00000A50, 0x00000002,0,0},
	{0x00000A00, 0x00120001,0,0},
	{0x00000008, 0x00010000,0,0},
	{0x00000008, 0x0002000A,0,0},
	{0x00000000, 0x00000001,0,0},
};

static const regval_t isp_imx219_reg_config_list[] = {
    {0x00000014, 0x0000000D, 0, 0},
    // {0x00000018, 0x000011BB, 0, 0},
    {0x00000A1C, 0x00000032, 0, 0},
    // {0x0000001C, 0x00000000, 0, 0},
    // {0x00000020, 0x0437077F, 0, 0},
    // {0x00000A0C, 0x04380780, 0, 0},
    // {0x00000A80, 0xF9000000, 0, 0},
    // {0x00000A84, 0xF91FA400, 0, 0},
    // {0x00000A88, 0x00000780, 0, 0},
    {0x00000A8C, 0x00000000, 0, 0},
    {0x00000A90, 0x00000000, 0, 0},
    {0x00000E40, 0x0000004C, 0, 0},
    {0x00000E44, 0x00000097, 0, 0},
    {0x00000E48, 0x0000001D, 0, 0},
    {0x00000E4C, 0x000001D5, 0, 0},
    {0x00000E50, 0x000001AC, 0, 0},
    {0x00000E54, 0x00000080, 0, 0},
    {0x00000E58, 0x00000080, 0, 0},
    {0x00000E5C, 0x00000194, 0, 0},
    {0x00000E60, 0x000001EC, 0, 0},
    {0x00000280, 0x00000000, 0, 0},
    {0x00000284, 0x00000000, 0, 0},
    {0x00000288, 0x00000000, 0, 0},
    {0x0000028C, 0x00000000, 0, 0},
    {0x00000290, 0x00000000, 0, 0},
    {0x00000294, 0x00000000, 0, 0},
    {0x00000298, 0x00000000, 0, 0},
    {0x0000029C, 0x00000000, 0, 0},
    {0x000002A0, 0x00000000, 0, 0},
    {0x000002A4, 0x00000000, 0, 0},
    {0x000002A8, 0x00000000, 0, 0},
    {0x000002AC, 0x00000000, 0, 0},
    {0x000002B0, 0x00000000, 0, 0},
    {0x000002B4, 0x00000000, 0, 0},
    {0x000002B8, 0x00000000, 0, 0},
    {0x000002BC, 0x00000000, 0, 0},
    {0x000002C0, 0x00F000F0, 0, 0},
    {0x000002C4, 0x00F000F0, 0, 0},
    {0x000002C8, 0x00800080, 0, 0},
    {0x000002CC, 0x00800080, 0, 0},
    {0x000002D0, 0x00800080, 0, 0},
    {0x000002D4, 0x00800080, 0, 0},
    {0x000002D8, 0x00B000B0, 0, 0},
    {0x000002DC, 0x00B000B0, 0, 0},
    {0x00000E00, 0x24000000, 0, 0},
    {0x00000E04, 0x159500A5, 0, 0},
    {0x00000E08, 0x0F9900EE, 0, 0},
    {0x00000E0C, 0x0CE40127, 0, 0},
    {0x00000E10, 0x0B410157, 0, 0},
    {0x00000E14, 0x0A210181, 0, 0},
    {0x00000E18, 0x094B01A8, 0, 0},
    {0x00000E1C, 0x08A401CC, 0, 0},
    {0x00000E20, 0x081D01EE, 0, 0},
    {0x00000E24, 0x06B20263, 0, 0},
    {0x00000E28, 0x05D802C7, 0, 0},
    {0x00000E2C, 0x05420320, 0, 0},
    {0x00000E30, 0x04D30370, 0, 0},
    {0x00000E34, 0x047C03BB, 0, 0},
    {0x00000E38, 0x043703FF, 0, 0},
    {0x00000010, 0x00000080, 0, 0},
    {0x00000A08, 0x10000032, 0xFFFFFFF, 0},
    {0x00000A00, 0x00120002, 0, 0},
    {0x00000A00, 0x00120000, 0, 0},
    {0x00000A50, 0x00000002, 0, 0},
    {0x00000A00, 0x00120001, 0, 0},
    {0x00000008, 0x00010000, 0, 0},
    {0x00000008, 0x0002000A, 0, 0},
    {0x00000000, 0x00000001, 0, 0},
};

const struct reg_table isp_1920_1080_settings[] = {
    {isp_1080p_reg_config_list,
    ARRAY_SIZE(isp_1080p_reg_config_list)},
};

const struct reg_table isp_dump_1920_1080_settings[] = {
    {isp_dump_1080p_reg_config_list,
    ARRAY_SIZE(isp_dump_1080p_reg_config_list)},
};

const struct reg_table isp_sc2235_settings[] = {
    {isp_sc2235_reg_config_list,
    ARRAY_SIZE(isp_sc2235_reg_config_list)},
};

/**
    print usage information
*/
static void usage(FILE* fp, int argc, char** argv)
{
    fprintf(fp,
        "Usage: %s [options]\n\n"
        "Options:\n"
        "-i | --input        Set input filename\n"
        "-o | --output       Set output filename\n"
        "-h | --help         Print this message\n"
        "-v | --version      Print version\n"
        "",
        argv[0]);
    }

static const char short_options [] = "hv";

static const struct option
long_options [] = {
    { "help",      no_argument,            NULL,        'h' },
    { "version",   no_argument,            NULL,        'v' },
    { 0, 0, 0, 0 }
};

int write_file (char * filename, unsigned char *image_buffer, int size)
{
    FILE * outfile;               /*  target file */

    if ((outfile = fopen(filename, "w+")) == NULL) {

        fprintf(stderr, "can't open %s\n", filename);

        return -1;

    }

    fwrite(image_buffer, size * sizeof(regval_t), 1, outfile);
    fwrite(&size, sizeof(int), 1, outfile);

    fclose(outfile);
    return 0 ;
}

int main(int argc, char **argv)
{
    int fd;

    for (;;) {
        int index, c = 0;

        c = getopt_long(argc, argv, short_options, long_options, &index);

        if (-1 == c)
            break;

        switch (c) {
            case 0: /* getopt_long() flag */
                break;

            case 'h':
                // print help
                usage(stdout, argc, argv);
                exit(EXIT_SUCCESS);

            case 'v':
                printf("Version: %s\n", VERSION);
                exit(EXIT_SUCCESS);
                break;

            default:
                usage(stderr, argc, argv);
                exit(EXIT_FAILURE);
        }
    }

    printf("isp_1920_1080_settings size: %d, sizeof(regval_t): %d, sizeof(int): %d\n",
		    isp_1920_1080_settings->regval_num, sizeof(regval_t), sizeof(int));
    write_file(OV4689_SETFILE, (unsigned char *)isp_1920_1080_settings->regval,
		    isp_1920_1080_settings->regval_num);

    printf("isp_dump_1920_1080_settings size: %d, sizeof(regval_t): %d, sizeof(int): %d\n",
		    isp_dump_1920_1080_settings->regval_num, sizeof(regval_t), sizeof(int));
    write_file(OV4689_DUMP_SETFILE, (unsigned char *)isp_dump_1920_1080_settings->regval,
		    isp_dump_1920_1080_settings->regval_num);

    printf("isp_sc2235_settings size: %d, sizeof(regval_t): %d\n",
		    isp_sc2235_settings->regval_num, sizeof(regval_t));
    write_file(SC2235_SETFILE, (unsigned char *)isp_sc2235_settings->regval,
		    isp_sc2235_settings->regval_num);
    return 0;
}

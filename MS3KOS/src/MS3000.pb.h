/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.0-dev at Tue Jul 24 23:55:12 2018. */

#ifndef PB_MS3000_PB_H_INCLUDED
#define PB_MS3000_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _MS3KG_Modes_Light_LightMode {
    MS3KG_Modes_Light_LightMode_RAINBOW = 0,
    MS3KG_Modes_Light_LightMode_NORMAL = 1,
    MS3KG_Modes_Light_LightMode_SCANNER_RGB = 2,
    MS3KG_Modes_Light_LightMode_SCANNER_BW = 3
} MS3KG_Modes_Light_LightMode;
#define _MS3KG_Modes_Light_LightMode_MIN MS3KG_Modes_Light_LightMode_RAINBOW
#define _MS3KG_Modes_Light_LightMode_MAX MS3KG_Modes_Light_LightMode_SCANNER_BW
#define _MS3KG_Modes_Light_LightMode_ARRAYSIZE ((MS3KG_Modes_Light_LightMode)(MS3KG_Modes_Light_LightMode_SCANNER_BW+1))

/* Struct definitions */
typedef struct _MS3KG_Modes_Arpi {
    pb_callback_t name;
/* @@protoc_insertion_point(struct:MS3KG_Modes_Arpi) */
} MS3KG_Modes_Arpi;

typedef struct _MS3KG_Modes_MagicPOV {
    pb_callback_t name;
/* @@protoc_insertion_point(struct:MS3KG_Modes_MagicPOV) */
} MS3KG_Modes_MagicPOV;

typedef struct _MS3KG_Modes_MagicPOV_User {
    pb_callback_t image;
/* @@protoc_insertion_point(struct:MS3KG_Modes_MagicPOV_User) */
} MS3KG_Modes_MagicPOV_User;

typedef struct _RGB {
    int32_t R;
    int32_t G;
    int32_t B;
/* @@protoc_insertion_point(struct:RGB) */
} RGB;

typedef struct _MS3KG_Modes_Light {
    pb_callback_t name;
    MS3KG_Modes_Light_LightMode subMode;
    RGB color;
/* @@protoc_insertion_point(struct:MS3KG_Modes_Light) */
} MS3KG_Modes_Light;

typedef struct _MS3KG_Modes {
    pb_callback_t current;
    MS3KG_Modes_Light light;
/* @@protoc_insertion_point(struct:MS3KG_Modes) */
} MS3KG_Modes;

typedef struct _MS3KG {
    MS3KG_Modes modes;
/* @@protoc_insertion_point(struct:MS3KG) */
} MS3KG;

/* Default values for struct fields */
extern const int32_t RGB_R_default;
extern const int32_t RGB_G_default;
extern const int32_t RGB_B_default;

/* Initializer values for message structs */
#define RGB_init_default                         {0, 0, 0}
#define MS3KG_init_default                       {MS3KG_Modes_init_default}
#define MS3KG_Modes_init_default                 {{{NULL}, NULL}, MS3KG_Modes_Light_init_default}
#define MS3KG_Modes_MagicPOV_init_default        {{{NULL}, NULL}}
#define MS3KG_Modes_MagicPOV_User_init_default   {{{NULL}, NULL}}
#define MS3KG_Modes_Light_init_default           {{{NULL}, NULL}, _MS3KG_Modes_Light_LightMode_MIN, RGB_init_default}
#define MS3KG_Modes_Arpi_init_default            {{{NULL}, NULL}}
#define RGB_init_zero                            {0, 0, 0}
#define MS3KG_init_zero                          {MS3KG_Modes_init_zero}
#define MS3KG_Modes_init_zero                    {{{NULL}, NULL}, MS3KG_Modes_Light_init_zero}
#define MS3KG_Modes_MagicPOV_init_zero           {{{NULL}, NULL}}
#define MS3KG_Modes_MagicPOV_User_init_zero      {{{NULL}, NULL}}
#define MS3KG_Modes_Light_init_zero              {{{NULL}, NULL}, _MS3KG_Modes_Light_LightMode_MIN, RGB_init_zero}
#define MS3KG_Modes_Arpi_init_zero               {{{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define MS3KG_Modes_Arpi_name_tag                1
#define MS3KG_Modes_MagicPOV_name_tag            1
#define MS3KG_Modes_MagicPOV_User_image_tag      2
#define RGB_R_tag                                1
#define RGB_G_tag                                2
#define RGB_B_tag                                3
#define MS3KG_Modes_Light_name_tag               1
#define MS3KG_Modes_Light_subMode_tag            2
#define MS3KG_Modes_Light_color_tag              3
#define MS3KG_Modes_current_tag                  1
#define MS3KG_Modes_light_tag                    2
#define MS3KG_modes_tag                          1

/* Struct field encoding specification for nanopb */
extern const pb_field_t RGB_fields[4];
extern const pb_field_t MS3KG_fields[2];
extern const pb_field_t MS3KG_Modes_fields[3];
extern const pb_field_t MS3KG_Modes_MagicPOV_fields[2];
extern const pb_field_t MS3KG_Modes_MagicPOV_User_fields[2];
extern const pb_field_t MS3KG_Modes_Light_fields[4];
extern const pb_field_t MS3KG_Modes_Arpi_fields[2];

/* Maximum encoded size of messages (where known) */
#define RGB_size                                 33
#define MS3KG_size                               (6 + MS3KG_Modes_size)
/* MS3KG_Modes_size depends on runtime parameters */
/* MS3KG_Modes_MagicPOV_size depends on runtime parameters */
/* MS3KG_Modes_MagicPOV_User_size depends on runtime parameters */
/* MS3KG_Modes_Light_size depends on runtime parameters */
/* MS3KG_Modes_Arpi_size depends on runtime parameters */

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define MS3000_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif

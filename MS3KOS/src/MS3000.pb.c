/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.2 at Tue Sep  6 22:28:32 2022. */

#include "MS3000.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

const int32_t RGB_R_default = 255;
const int32_t RGB_G_default = 121;
const int32_t RGB_B_default = 0;
const int32_t MIDI_OCTAVE_o_default = 5;
const int32_t MIDI_INTERVALS_v_default = 0;
const MS3KG_App_Light_Mode MS3KG_App_Light_mode_default = MS3KG_App_Light_Mode_RAINBOW;
const MS3KG_App_Magnet_Mode MS3KG_App_Magnet_mode_default = MS3KG_App_Magnet_Mode_BARS;
const MS3KG_App_System_Mode MS3KG_App_System_mode_default = MS3KG_App_System_Mode_VERSION;
const MS3KG_App_Beat_Mode MS3KG_App_Beat_mode_default = MS3KG_App_Beat_Mode_SIDE;
const int32_t MS3KG_App_Beat_sensitivity_default = 2;
const MS3KG_App_Countdown_Mode MS3KG_App_Countdown_mode_default = MS3KG_App_Countdown_Mode_DOWN_1M;
const MS3KG_App_Arpi_Mode MS3KG_App_Arpi_mode_default = MS3KG_App_Arpi_Mode_ARP8;


const pb_field_t RGB_fields[4] = {
    PB_FIELD(  1, INT32   , REQUIRED, STATIC  , FIRST, RGB, R, R, &RGB_R_default),
    PB_FIELD(  2, INT32   , REQUIRED, STATIC  , OTHER, RGB, G, R, &RGB_G_default),
    PB_FIELD(  3, INT32   , REQUIRED, STATIC  , OTHER, RGB, B, G, &RGB_B_default),
    PB_LAST_FIELD
};

const pb_field_t MIDI_OCTAVE_fields[2] = {
    PB_FIELD(  1, INT32   , REQUIRED, STATIC  , FIRST, MIDI_OCTAVE, o, o, &MIDI_OCTAVE_o_default),
    PB_LAST_FIELD
};

const pb_field_t MIDI_INTERVALS_fields[2] = {
    PB_FIELD(  1, INT32   , REQUIRED, STATIC  , FIRST, MIDI_INTERVALS, v, v, &MIDI_INTERVALS_v_default),
    PB_LAST_FIELD
};

const pb_field_t MIDI_STEP_fields[4] = {
    PB_FIELD(  1, MESSAGE , REQUIRED, STATIC  , FIRST, MIDI_STEP, interval, interval, &MIDI_INTERVALS_fields),
    PB_FIELD(  2, MESSAGE , REQUIRED, STATIC  , OTHER, MIDI_STEP, octave, interval, &MIDI_OCTAVE_fields),
    PB_FIELD(  3, INT32   , REQUIRED, STATIC  , OTHER, MIDI_STEP, mode, octave, 0),
    PB_LAST_FIELD
};

const pb_field_t MIDI_SEQUENCE_fields[2] = {
    PB_FIELD(  1, MESSAGE , REPEATED, STATIC  , FIRST, MIDI_SEQUENCE, steps, steps, &MIDI_STEP_fields),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_fields[2] = {
    PB_FIELD(  1, MESSAGE , OPTIONAL, STATIC  , FIRST, MS3KG, apps, apps, &MS3KG_App_fields),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_SysPref_fields[1] = {
    PB_LAST_FIELD
};

const pb_field_t MS3KG_SysPref_AP_INFO_fields[3] = {
    PB_FIELD(  1, STRING  , OPTIONAL, CALLBACK, FIRST, MS3KG_SysPref_AP_INFO, networkName, networkName, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, CALLBACK, OTHER, MS3KG_SysPref_AP_INFO, password, networkName, 0),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_App_fields[12] = {
    PB_FIELD(  1, UENUM   , OPTIONAL, STATIC  , FIRST, MS3KG_App, current, current, 0),
    PB_FIELD(  2, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App, shake, current, &MS3KG_App_Shake_fields),
    PB_FIELD(  3, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App, light, shake, &MS3KG_App_Light_fields),
    PB_FIELD(  4, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App, magnet, light, &MS3KG_App_Magnet_fields),
    PB_FIELD(  5, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App, system, magnet, &MS3KG_App_System_fields),
    PB_FIELD(  6, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App, remote, system, &MS3KG_App_Remote_fields),
    PB_FIELD(  7, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App, beat, remote, &MS3KG_App_Beat_fields),
    PB_FIELD(  8, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App, countdown, beat, &MS3KG_App_Countdown_fields),
    PB_FIELD(  9, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App, arpi, countdown, &MS3KG_App_Arpi_fields),
    PB_FIELD( 10, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App, sequi, arpi, &MS3KG_App_Sequi_fields),
    PB_FIELD( 11, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App, updater, sequi, &MS3KG_App_Updater_fields),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_App_Shake_fields[4] = {
    PB_FIELD(  1, STRING  , OPTIONAL, STATIC  , FIRST, MS3KG_App_Shake, image, image, 0),
    PB_FIELD(  2, INT32   , OPTIONAL, STATIC  , OTHER, MS3KG_App_Shake, colorIndex, image, 0),
    PB_FIELD(  3, INT32   , OPTIONAL, STATIC  , OTHER, MS3KG_App_Shake, bounce, colorIndex, 0),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_App_Light_fields[5] = {
    PB_FIELD(  1, UENUM   , OPTIONAL, STATIC  , FIRST, MS3KG_App_Light, mode, mode, &MS3KG_App_Light_mode_default),
    PB_FIELD(  2, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App_Light, color, mode, &RGB_fields),
    PB_FIELD(  3, INT32   , OPTIONAL, STATIC  , OTHER, MS3KG_App_Light, colorIndex, color, 0),
    PB_FIELD(  4, INT32   , OPTIONAL, STATIC  , OTHER, MS3KG_App_Light, triggerSpeed, colorIndex, 0),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_App_Magnet_fields[2] = {
    PB_FIELD(  1, UENUM   , OPTIONAL, STATIC  , FIRST, MS3KG_App_Magnet, mode, mode, &MS3KG_App_Magnet_mode_default),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_App_System_fields[2] = {
    PB_FIELD(  1, UENUM   , OPTIONAL, STATIC  , FIRST, MS3KG_App_System, mode, mode, &MS3KG_App_System_mode_default),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_App_Remote_fields[1] = {
    PB_LAST_FIELD
};

const pb_field_t MS3KG_App_Beat_fields[4] = {
    PB_FIELD(  1, UENUM   , OPTIONAL, STATIC  , FIRST, MS3KG_App_Beat, mode, mode, &MS3KG_App_Beat_mode_default),
    PB_FIELD(  2, INT32   , OPTIONAL, STATIC  , OTHER, MS3KG_App_Beat, sensitivity, mode, &MS3KG_App_Beat_sensitivity_default),
    PB_FIELD(  3, MESSAGE , OPTIONAL, STATIC  , OTHER, MS3KG_App_Beat, color, sensitivity, &RGB_fields),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_App_Countdown_fields[2] = {
    PB_FIELD(  1, UENUM   , OPTIONAL, STATIC  , FIRST, MS3KG_App_Countdown, mode, mode, &MS3KG_App_Countdown_mode_default),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_App_Arpi_fields[2] = {
    PB_FIELD(  1, UENUM   , OPTIONAL, STATIC  , FIRST, MS3KG_App_Arpi, mode, mode, &MS3KG_App_Arpi_mode_default),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_App_Sequi_fields[2] = {
    PB_FIELD(  1, MESSAGE , OPTIONAL, STATIC  , FIRST, MS3KG_App_Sequi, sequence, sequence, &MIDI_SEQUENCE_fields),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_App_Updater_fields[2] = {
    PB_FIELD(  1, INT32   , OPTIONAL, STATIC  , FIRST, MS3KG_App_Updater, shouldReset, shouldReset, 0),
    PB_LAST_FIELD
};









/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(MIDI_STEP, interval) < 65536 && pb_membersize(MIDI_STEP, octave) < 65536 && pb_membersize(MIDI_SEQUENCE, steps[0]) < 65536 && pb_membersize(MS3KG, apps) < 65536 && pb_membersize(MS3KG_App, shake) < 65536 && pb_membersize(MS3KG_App, light) < 65536 && pb_membersize(MS3KG_App, magnet) < 65536 && pb_membersize(MS3KG_App, system) < 65536 && pb_membersize(MS3KG_App, remote) < 65536 && pb_membersize(MS3KG_App, beat) < 65536 && pb_membersize(MS3KG_App, countdown) < 65536 && pb_membersize(MS3KG_App, arpi) < 65536 && pb_membersize(MS3KG_App, sequi) < 65536 && pb_membersize(MS3KG_App, updater) < 65536 && pb_membersize(MS3KG_App_Light, color) < 65536 && pb_membersize(MS3KG_App_Beat, color) < 65536 && pb_membersize(MS3KG_App_Sequi, sequence) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_RGB_MIDI_OCTAVE_MIDI_INTERVALS_MIDI_STEP_MIDI_SEQUENCE_MS3KG_MS3KG_SysPref_MS3KG_SysPref_AP_INFO_MS3KG_App_MS3KG_App_Shake_MS3KG_App_Light_MS3KG_App_Magnet_MS3KG_App_System_MS3KG_App_Remote_MS3KG_App_Beat_MS3KG_App_Countdown_MS3KG_App_Arpi_MS3KG_App_Sequi_MS3KG_App_Updater)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(MIDI_STEP, interval) < 256 && pb_membersize(MIDI_STEP, octave) < 256 && pb_membersize(MIDI_SEQUENCE, steps[0]) < 256 && pb_membersize(MS3KG, apps) < 256 && pb_membersize(MS3KG_App, shake) < 256 && pb_membersize(MS3KG_App, light) < 256 && pb_membersize(MS3KG_App, magnet) < 256 && pb_membersize(MS3KG_App, system) < 256 && pb_membersize(MS3KG_App, remote) < 256 && pb_membersize(MS3KG_App, beat) < 256 && pb_membersize(MS3KG_App, countdown) < 256 && pb_membersize(MS3KG_App, arpi) < 256 && pb_membersize(MS3KG_App, sequi) < 256 && pb_membersize(MS3KG_App, updater) < 256 && pb_membersize(MS3KG_App_Light, color) < 256 && pb_membersize(MS3KG_App_Beat, color) < 256 && pb_membersize(MS3KG_App_Sequi, sequence) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_RGB_MIDI_OCTAVE_MIDI_INTERVALS_MIDI_STEP_MIDI_SEQUENCE_MS3KG_MS3KG_SysPref_MS3KG_SysPref_AP_INFO_MS3KG_App_MS3KG_App_Shake_MS3KG_App_Light_MS3KG_App_Magnet_MS3KG_App_System_MS3KG_App_Remote_MS3KG_App_Beat_MS3KG_App_Countdown_MS3KG_App_Arpi_MS3KG_App_Sequi_MS3KG_App_Updater)
#endif


/* @@protoc_insertion_point(eof) */
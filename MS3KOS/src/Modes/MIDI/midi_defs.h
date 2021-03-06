 /**
 * \defgroup MIDI_Interface Constants for MIDI v1.0
 * Constants for MIDI v1.0.
 * \{
 */

#ifndef MIDI_DEFS_H
#define MIDI_DEFS_H

#define MIDI_CHANNELS			16
#define MIDI_GM_DRUM_CHANNEL	(10-1)

// messages
#define MIDI_NOTE_OFF			0x80
#define MIDI_NOTE_ON			0x90
#define MIDI_AFTERTOUCH			0xA0
#define MIDI_CONTROLLER			0xB0
#define MIDI_PROGRAM_CHANGE		0xC0
#define MIDI_CHANNEL_PRESSURE	0xD0
#define MIDI_PITCH_WHEEL		0xE0
#define MIDI_SYSEX_BEGIN		0xF0
#define MIDI_QUARTER_FRAME_MSG	0xF1
#define MIDI_SONG_POSITION_PTR	0xF2
#define MIDI_SONG_SELECT		0xF3
#define MIDI_TUNE_REQUEST		0xF6
#define MIDI_SYSEX_END			0xF7
#define MIDI_CLOCK				0xF8
#define MIDI_START				0xFA
#define MIDI_CONTINUE			0xFB
#define MIDI_STOP				0xFC
#define MIDI_ACTIVE_SENSE		0xFE
#define MIDI_RESET				0xFF

// misc
#define MIDI_SYSTEM_COMMON		0xF0
#define MIDI_SYSEX				0xF0
#define MIDI_SYSTEM_REAL_TIME	0xF8
#define MIDI_CHANNEL_MASK		0x0F

// controllers
#define MIDI_BANK_SELECT				0
#define MIDI_MODULATION_WHEEL			1
#define MIDI_BREATH_CONTROLLER			2
#define MIDI_FOOT_PEDAL					4
#define MIDI_PORTAMENTO_TIME			5
#define MIDI_DATA_ENTRY					6
#define MIDI_VOLUME						7
#define MIDI_BALANCE					8
#define MIDI_PAN_POSITION				10
#define MIDI_EXPRESSION					11
#define MIDI_EFFECT_CONTROL_1			12
#define MIDI_EFFECT_CONTROL_2			13
#define MIDI_GENERAL_PURPOSE_SLIDER_1	16
#define MIDI_GENERAL_PURPOSE_SLIDER_2	17
#define MIDI_GENERAL_PURPOSE_SLIDER_3	18
#define MIDI_GENERAL_PURPOSE_SLIDER_4	19
#define MIDI_BANK_SELECT_FINE			32
#define MIDI_MODULATION_WHEEL_FINE		33
#define MIDI_BREATH_CONTROLLER_FINE		34
#define MIDI_FOOT_PEDAL_FINE			36
#define MIDI_PORTAMENTO_TIME_FINE		37
#define MIDI_DATA_ENTRY_FINE			38
#define MIDI_VOLUME_FINE				39
#define MIDI_BALANCE_FINE				40
#define MIDI_PAN_POSITION_FINE			42
#define MIDI_EXPRESSION_FINE			43
#define MIDI_EFFECT_CONTROL_1_FINE		44
#define MIDI_EFFECT_CONTROL_2_FINE		45
#define MIDI_HOLD_PEDAL					64
#define MIDI_PORTAMENTO					65
#define MIDI_SUSTENUTO_PEDAL			66
#define MIDI_SOFT_PEDAL					67
#define MIDI_LEGATO_PEDAL				68
#define MIDI_HOLD_2_PEDAL				69
#define MIDI_SOUND_VARIATION			70
#define MIDI_SOUND_TIMBRE				71
#define MIDI_SOUND_RELEASE_TIME			72
#define MIDI_SOUND_ATTACK_TIME			73
#define MIDI_SOUND_BRIGHTNESS			74
#define MIDI_SOUND_CONTROL_6			75
#define MIDI_SOUND_CONTROL_7			76
#define MIDI_SOUND_CONTROL_8			77
#define MIDI_SOUND_CONTROL_9			78
#define MIDI_SOUND_CONTROL_10			79
#define MIDI_GENERAL_PURPOSE_BUTTON_1	80
#define MIDI_GENERAL_PURPOSE_BUTTON_2	81
#define MIDI_GENERAL_PURPOSE_BUTTON_3	82
#define MIDI_GENERAL_PURPOSE_BUTTON_4	83
#define MIDI_EFFECTS_LEVEL				91
#define MIDI_TREMULO_LEVEL				92
#define MIDI_CHORUS_LEVEL				93
#define MIDI_CELESTE_LEVEL				94
#define MIDI_PHASER_LEVEL				95
#define MIDI_DATA_BUTTON_INCREMENT		96
#define MIDI_DATA_BUTTON_DECREMENT		97
#define MIDI_NON_REGISTERED_PARAMETER	98
#define MIDI_NON_REGISTERED_PARAMETER_FINE	99
#define MIDI_REGISTERED_PARAMETER		100
#define MIDI_REGISTERED_PARAMETER_FINE	101
#define MIDI_ALL_SOUND_OFF				120
#define MIDI_ALL_CONTROLLERS_OFF		121
#define MIDI_LOCAL_KEYBOARD				122
#define MIDI_ALL_NOTES_OFF				123
#define MIDI_OMNI_MODE_OFF				124
#define MIDI_OMNI_MODE_ON				125
#define MIDI_MONO_OPERATION				126
#define MIDI_POLY_OPERATION				127

// Note lengths
#define WHOLE 64
#define HALF 32
#define QUARTER 16
#define EIGHTH 8
#define SIXTEENTH 4
#define BEAT1 0
#define BEAT2 16
#define BEAT3 32
#define BEAT4 48

#endif							/* MIDI_DEFS_H */

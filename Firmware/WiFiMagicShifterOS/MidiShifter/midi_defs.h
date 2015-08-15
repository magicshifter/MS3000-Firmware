 /**
 * \defgroup MIDI_Interface Constants for MIDI v1.0
 * Constants for MIDI v1.0.
 * \{
 */

#define MIDI_CHANNELS					16		/**< Number of channels per port/cable. */
#define MIDI_GM_DRUM_CHANNEL			(10-1)	/**< Channel number for GM drums. */

/**
 * \defgroup MIDI_Commands MIDI Commands
 * MIDI command codes.
 * \{
 */

#define MIDI_CMD_NOTE_OFF				0x80	/**< note off */
#define MIDI_CMD_NOTE_ON				0x90	/**< note on */
#define MIDI_CMD_NOTE_PRESSURE			0xa0	/**< key pressure */
#define MIDI_CMD_CONTROL				0xb0	/**< control change */
#define MIDI_CMD_PGM_CHANGE				0xc0	/**< program change */
#define MIDI_CMD_CHANNEL_PRESSURE		0xd0	/**< channel pressure */
#define MIDI_CMD_BENDER					0xe0	/**< pitch bender */
#define MIDI_CMD_COMMON_SYSEX			0xf0	/**< sysex (system exclusive) begin */
#define MIDI_CMD_COMMON_MTC_QUARTER		0xf1	/**< MTC quarter frame */
#define MIDI_CMD_COMMON_SONG_POS		0xf2	/**< song position */
#define MIDI_CMD_COMMON_SONG_SELECT		0xf3	/**< song select */
#define MIDI_CMD_COMMON_TUNE_REQUEST	0xf6	/**< tune request */
#define MIDI_CMD_COMMON_SYSEX_END		0xf7	/**< end of sysex */
#define MIDI_CMD_COMMON_CLOCK			0xf8	/**< clock */
#define MIDI_CMD_COMMON_START			0xfa	/**< start */
#define MIDI_CMD_COMMON_CONTINUE		0xfb	/**< continue */
#define MIDI_CMD_COMMON_STOP			0xfc	/**< stop */
#define MIDI_CMD_COMMON_SENSING			0xfe	/**< active sensing */
#define MIDI_CMD_COMMON_RESET			0xff	/**< reset */
 

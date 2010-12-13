/**
 * === MIDI Utility ===
 *  Unit Name:  midi_def
 *  Author:     Lincoln Yu
 *  Purpose:    Midi function encapsulation
 * == History ==        
 *  2006-01-14: Created.
 * == Comments ==
 *
 */

#ifndef _midi_def_H
#define _midi_def_H

#include "../all/predef.h"

_QUL_BEGIN

enum MidiInstrument {
	/* Piano */

	Acoustic_Grand_Piano = 0,
	Bright_Acoustic_Piano,
	Electric_Grand_Piano,
	Honky_tonk_Piano,
	Rhodes_Piano,
	Chorused_Piano,
	Harpsichord,
	Clavinet,

	/* Chromatic Percussion */

	Celesta,
	Glockenspiel,
	Music_Box,
	Vibraphone,
	Marimba,
	Xylophone,
	Tubular_Bells,
	Dulcimer,

	/* Organ */

	Hammond_Organ,
	Percussive_Organ,
	Rock_Organ,
	Church_Organ,
	Reed_Organ,
	Accordian,
	Harmonica,
	Tango_Accordian,

	/* Guitar */

	Acoustic_Guitar_nylon,
	Acoustic_Guitar_steel,
	Electric_Guitar_jazz,
	Electric_Guitar_clean,
	Electric_Guitar_muted,
	Overdriven_Guitar,
	Distortion_Guitar,
	Guitar_Harmonics,

	/* Bass */

	Acoustic_Bass,
	Electric_Bass_finger,
	Electric_Bass_pick,
	Fretless_Bass,
	Slap_Bass_1,
	Slap_Bass_2,
	Synth_Bass_1,
	Synth_Bass_2,

	/* Strings */

	Violin,
	Viola,
	Cello,
	Contrabass,
	Tremolo_Strings,
	Pizzicato_Strings,
	Orchestral_Harp,
	Timpani,

	/* Ensemble */

	String_Ensemble_1,
	String_Ensemble_2,
	Synth_Strings_1,
	Synth_Strings_2,
	Choir_Aahs,
	Voice_Oohs,
	Synth_Voice,
	Orchestra_Hit,

	/* Brass */

	Trumpet,
	Trombone,
	Tuba,
	Muted_Trumpet,
	French_Horn,
	Brass_Section,
	Synth_Brass_1,
	Synth_Brass_2,

	/* Reed */

	Soprano_Sax,
	Alto_Sax,
	Tenor_Sax,
	Baritone_Sax,
	Oboe,
	English_Horn,
	Bassoon,
	Clarinet,

	/* Pipe */

	Piccolo,
	Flute,
	Recorder,
	Pan_Flute,
	Bottle_Blow,
	Shakuhachi,
	Whistle,
	Ocarina,

	/* Synth Lead */

	Lead_1_square,
	Lead_2_sawtooth,
	Lead_3_caliope_lead,
	Lead_4_chiff_lead,
	Lead_5_charang,
	Lead_6_voice,
	Lead_7_fifths,
	Lead_8_brass_plus_lead,

	/* Synth Pad */

	Pad_1_new_age,
	Pad_2_warm,
	Pad_3_polysynth,
	Pad_4_choir,
	Pad_5_bowed,
	Pad_6_metallic,
	Pad_7_halo,
	Pad_8_sweep,

	/* Synth Effects */

	FX_1_rain,
	FX_2_soundtrack,
	FX_3_crystal,
	FX_4_atmosphere,
	FX_5_brightness,
	FX_6_goblins,
	FX_7_echoes,
	FX_8_sci_fi,

	/* Ethnic */

	Sitar,
	Banjo,
	Shamisen,
	Koto,
	Kalimba,
	Bagpipe,
	Fiddle,
	Shanai,

	/* Percussive */
	Tinkle_Bell,
	Agogo,
	Steel_Drums,
	Woodblock,
	Taiko_Drum,
	Melodic_Tom,
	Synth_Drum,
	Reverse_Cymbal,

	/* Sound Effects */

	Guitar_Fret_Noise,
	Breath_Noise,
	Seashore,
	Bird_Tweet,
	Telephone_Ring,
	Helicopter,
	Applause,
	Gunshot
	};	// enum MidiInstrument

_QUL_END
	
#endif	// ifndef _midi_def_H

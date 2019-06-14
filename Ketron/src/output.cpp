/*
 * output.cpp
 *
 * Created: 16.03.2019 21:48:56
 *  Author: Tomek
 */ 
#include <avr/pgmspace.h>
#include <main.h>
#include <output.h>
#include <stdint-gcc.h>
#include <Arduino.h>
#include <TFT.h>
#include <OSFS.h>
#include <EEPROM.h>

extern uint8_t PIN_BTN0;
extern uint8_t PIN_BTN1;
extern uint8_t PIN_BTN2;
extern uint8_t PIN_BTN3;
extern uint8_t PIN_POT0;
extern uint8_t PIN_POT1;
extern uint8_t PIN_LED_GRN;
extern uint8_t PIN_LED_RED;
extern uint8_t bankNumbers[5];
extern inputStates lastdata;
extern inputStates newdata;
extern TFT tft;
extern uint8_t family_sounds[NUM_OF_FAMILIES];

uint16_t OSFS::startOfEEPROM = 1;
uint16_t OSFS::endOfEEPROM = 1024;

static const char *presetNames[NUM_OF_PRESETS] = {"Preset A","Preset B","Preset C","Preset D","Preset E","Preset F"};
static char lcdline[16];
static char modes_right[7][8] =
	{
		{"Sound"},{"Family"},{"Instr."},{"Sound"},{"Sound"}
	};
static char modes_left[6][12] =
	{
		{"Sound Left"},{"Sound Left"},{"Sound Left"},{"Family Left"},{"Instr. Left"}
	};
/*
 * sounds.cpp
 *
 * Created: 19.03.2019 21:39:16
 *  Author: Tomek
 */ 

PROGMEM const char bank_a[][16] =
{
	{"GrandPiano"},
	{"RockPiano"},
	{"Electric"},
	{"Honky-tonk"},
	{"El.Piano1"},
	{"El.Piano2"},
	{"Harpsichord"},
	{"Clavi"},
	{"Celesta"},
	{"Glockenspiel"},
	{"MusicBox"},
	{"Vibraphone"},
	{"Marimba"},
	{"Xylophone"},
	{"Tubularbells"},
	{"Dulcimer"},
	{"Rotary"},
	{"Percussive"},
	{"RockOrgan"},
	{"Church"},
	{"Theatre"},
	{"Musette"},
	{"Harmonica"},
	{"Bandoneon"},
	{"Nylon"},
	{"Steel"},
	{"Jazz"},
	{"Clean"},
	{"Muted"},
	{"Overdrive"},
	{"Distorted"},
	{"Harmonics"},
	{"Acoustic"},
	{"Finger"},
	{"Picked"},
	{"Fretless"},
	{"SlapBass"},
	{"SlapBass"},
	{"Synbas"},
	{"Synbas"},
	{"Violin"},
	{"Viola"},
	{"Cello"},
	{"Contrabass"},
	{"Tremolo"},
	{"Pizzicato"},
	{"Harp"},
	{"Timpani"},
	{"StringsEns"},
	{"SlowStrings"},
	{"SynStrings"},
	{"SynStrings"},
	{"ChoirAah"},
	{"VoiceOoh"},
	{"SynthVoice"},
	{"Hits"},
	{"Trumpet"},
	{"Trombone"},
	{"Tuba"},
	{"MutedTpt"},
	{"FrenchHorn"},
	{"BrassSect."},
	{"SynBrass"},
	{"SynBrass"},
	{"Altosoft"},
	{"Contralto"},
	{"TenorSax"},
	{"BaritonSax"},
	{"Oboe"},
	{"Engl.Horn"},
	{"Bassoon"},
	{"Clarinet"},
	{"Piccolo"},
	{"Flute"},
	{"Recorder"},
	{"PanFlute"},
	{"Bottle"},
	{"Shakuhashi"},
	{"Whistle"},
	{"Ocarina"},
	{"Square"},
	{"Sawtooth"},
	{"Calliope"},
	{"Chiff"},
	{"Charang"},
	{"Voice"},
	{"Fifth"},
	{"Bass&Lead"},
	{"Fantasy"},
	{"Warm"},
	{"NewAge"},
	{"Space"},
	{"Soft"},
	{"Metal"},
	{"Halo"},
	{"Sweep"},
	{"IceRain"},
	{"Soundtrack"},
	{"Crystal"},
	{"Atmosphere"},
	{"Brightness"},
	{"Goblin"},
	{"Echodrops"},
	{"Startheme"},
	{"Sitar"},
	{"Banjo"},
	{"Shamisen"},
	{"Koto"},
	{"Kalimba"},
	{"Bagpipe"},
	{"Fiddle"},
	{"Shanai"},
	{"Tinklebell"},
	{"Agogo"},
	{"Steeldrum"},
	{"Woodblock"},
	{"Taiko"},
	{"Melotom"},
	{"Syntom"},
	{"Reverse"},
	{"FretNoise"},
	{"Breath"},
	{"Seashore"},
	{"Bird"},
	{"Telephone"},
	{"Helicopter"},
	{"Applause"},
	{"Gunshot"}
};

PROGMEM const char bank_b[][16] =
{
	{"Jingle"},
	{"DarkGrand"},
	{"House"},
	{"Suitcase"},
	{"Mark"},
	{"FunkDx"},
	{"FmTine"},
	{"DXPiano"},
	{"ToyPad"},
	{"Orch.Bell"},
	{"Mallet"},
	{"ShortVibe"},
	{"Karimba"},
	{"Etnowood"},
	{"Metallophone"},
	{"Windchimes"},
	{"RotorFa"},
	{"JazzOrgan"},
	{"Drawbar"},
	{"Positive"},
	{"FrenchMusette"},
	{"Francaise"},
	{"Cassotto"},
	{"Tango"},
	{"Classic"},
	{"Folk"},
	{"FunkStrat"},
	{"60\'Stopped"},
	{"Mutedvelo"},
	{"Vintage"},
	{"12Strings"},
	{"Solidbody"},
	{"WarmBass"},
	{"FolkBass"},
	{"MuteBass"},
	{"DeepBass"},
	{"Precision"},
	{"ResoBass"},
	{"SawBass"},
	{"HouseBass"},
	{"Philarmonic"},
	{"Quartet"},
	{"GlockString"},
	{"OctaStrings1"},
	{"OctaStrings2"},
	{"Marcato"},
	{"String&Fall"},
	{"SlowArcos"},
	{"BowedStrings"},
	{"DarkStrings"},
	{"Dah"},
	{"ClassicChoir"},
	{"Men_Doo"},
	{"Girl_Doo"},
	{"Laah"},
	{"Voices"},
	{"SwingTpt"},
	{"Cornet"},
	{"Euphonium"},
	{"MilesTpt"},
	{"Flughorn"},
	{"StereoBrass"},
	{"GrowBras"},
	{"Octabrass"},
	{"Soprano"},
	{"CircusSax"},
	{"LoungeTenor"},
	{"GrowlSax"},
	{"MeloTrumpet"},
	{"HornSwell"},
	{"Trumpets"},
	{"SoftTrombones"},
	{"LatinFlute"},
	{"PopFlute"},
	{"Andes"},
	{"VoiceFlute"},
	{"Analogs"},
	{"Flanger"},
	{"Brightpad"},
	{"Wiring"},
	{"Wow"},
	{"Polysynth"},
	{"Trance"},
	{"Wave"},
	{"RaveBass"},
	{"ModularBass"},
	{"PercussiveBass"},
	{"WildBass"},
	{"2ndPerc"},
	{"3thPerc"},
	{"PowerFa"},
	{"Tonebar"},
	{"DarkB3"},
	{"JimmyOrg."},
	{"Lower8-4"},
	{"Jazzy"},
	{"PopStrat"},
	{"Teleca"},
	{"Strato"},
	{"Texas"},
	{"Western"},
	{"Fingerpick"},
	{"5thStops"},
	{"Whaguitar"},
	{"Hawaian"},
	{"Mandolin"},
	{"Django"},
	{"ClassicFx"},
	{"Pedalsteel"},
	{"Rock&Roll"},
	{"Reed8"},
	{"Fisa8"},
	{"Gategroup"},
	{"Amazing"},
	{"Housesynth"},
	{"Resonance"},
	{"KeyLead"},
	{"JunoSynth"},
	{"ObxPoly"},
	{"JXBrass"},
	{"Alarm&Sirene"},
	{"Jet"},
	{"Pfif&Jew_Harp"},
	{"Rain"},
	{"Vocals"},
	{"Thunder"},
	{"Train"},
	{"Sweep"}
};

PROGMEM const char bank_c[][16] =
{
	{"Piano&Strings"},
	{"Grand&Pad"},
	{"Octapiano"},
	{"Electropad"},
	{"Msk"},
	{"StagePno"},
	{"Rodes"},
	{"Elopiano"},
	{"FMLayer"},
	{"ChineseBell"},
	{"Stereochrom"},
	{"BrightVibes"},
	{"ToyBox"},
	{"Mbira"},
	{"XMasBell"},
	{"FMMarimba"},
	{"PopOrgan"},
	{"Gospel"},
	{"PowerSlow"},
	{"Principale"},
	{"DryOrgan"},
	{"Alpen"},
	{"Bellow"},
	{"Steirisch"},
	{"DynaFolk"},
	{"CountryFx"},
	{"Humbuck"},
	{"StratoFx"},
	{"Ac.Stereo"},
	{"Acid"},
	{"Plucked"},
	{"Flamenco"},
	{"AcousticBass"},
	{"Bass&Guit1"},
	{"Oberbas1"},
	{"RetroBass"},
	{"Fusion"},
	{"Oberbas2"},
	{"Pedalbass"},
	{"Bass&Guit2"},
	{"SlowViolin"},
	{"ElectricViolin"},
	{"Chamber"},
	{"Tzivago"},
	{"Staccato"},
	{"StringFall"},
	{"Harp&Orchestra"},
	{"Symphony"},
	{"FullEnsemble"},
	{"Digistring"},
	{"DiscoStrings"},
	{"CountryViolin"},
	{"Tuuh"},
	{"OohGirls"},
	{"AahGirls"},
	{"Choir"},
	{"Tbones&Cornet"},
	{"SlideTrombone"},
	{"DixieTpt"},
	{"OpenHorn"},
	{"BrassFallOff"},
	{"HornPad"},
	{"MultiBrass"},
	{"BrassCool"},
	{"Altosax"},
	{"JazzAlto"},
	{"AltoStraight"},
	{"RockSax"},
	{"Superhorn"},
	{"TptSlideUp"},
	{"DryTrumpet"},
	{"TrumpetSection"},
	{"OpenTrombones"},
	{"IrishFlute"},
	{"StaccatoFlute"},
	{"AsianFlute"},
	{"Fife"},
	{"EthnicFlute"},
	{"StaccatoWhistle"},
	{"Pipe"},
	{"SoloDance"},
	{"Juicy"},
	{"Drama"},
	{"Odissey"},
	{"Minimug"},
	{"Attack"},
	{"Phantom"},
	{"Soloist"},
	{"Warmness"},
	{"Explorer"},
	{"Choral"},
	{"Cyber"},
	{"Magic"},
	{"Glockpad"},
	{"Stream"},
	{"DigiPro"},
	{"UsPad"},
	{"BottledAir"},
	{"Bellvox"},
	{"DigiPad"},
	{"Syntax"},
	{"BrightAnalog"},
	{"AuraPad"},
	{"AcousticDream"},
	{"Tres"},
	{"Banjo5String"},
	{"Futozao"},
	{"Hackbrett"},
	{"ParaguayanHarp"},
	{"ShortMandolin"},
	{"HarpGlissando"},
	{"Bouzuki"},
	{"Tinkles"},
	{"Triangle"},
	{"Tambora"},
	{"Claves"},
	{"Ac.Tom"},
	{"Brushtom"},
	{"Snare"},
	{"Crash"},
	{"Scratch1"},
	{"Scratch2"},
	{"Scratch3"},
	{"Scratch4"},
	{"Scratch5"},
	{"FxSlap"},
	{"Laser"},
	{"Wind"}
};

PROGMEM const char bank_d[][16] =
{
	{"LayeredPiano"},
	{"LatinPiano"},
	{"FmThin"},
	{"RdxOperator"},
	{"TwinElectro"},
	{"StageMix"},
	{"HugeFM"},
	{"Digitine"},
	{"Glasshouse"},
	{"Fantasyland"},
	{"ItalianPositive"},
	{"Principal&Octv"},
	{"FullOrgan"},
	{"ChorusB3"},
	{"HotOrgan"},
	{"16-8-5"},
	{"PipeOctave"},
	{"Tonewheel"},
	{"MovieOrgan"},
	{"FastDrawbars"},
	{"CarouselOrg"},
	{"Diatonic"},
	{"BluesHarp"},
	{"Conservatory"},
	{"ClassicBright"},
	{"ThinFolk"},
	{"JazzOctave"},
	{"MutedWha"},
	{"RockStrat"},
	{"StrPower"},
	{"ValveLead"},
	{"SmoothElectro"},
	{"Strat&Folk"},
	{"OpenCountry"},
	{"GoldenSteel"},
	{"CntryDetune"},
	{"Acoust.Picking"},
	{"TwinFolk"},
	{"Gipsy"},
	{"Brazilian"},
	{"Rockabilly"},
	{"FunkBass"},
	{"Maxipick"},
	{"OohMix"},
	{"DuhGroup"},
	{"DahQuartet"},
	{"LahTrio"},
	{"Vocal_Praise"},
	{"CinemaStrings"},
	{"Score"},
	{"AahMix"},
	{"Humming"},
	{"LahChoir"},
	{"LahMelloChoir"},
	{"ChurchChoir"},
	{"NightVocals"},
	{"Mmh"},
	{"Boom&Uap"},
	{"HornCrescendo"},
	{"EmperorBrass"},
	{"DryTenor"},
	{"SaxClub"},
	{"SwingTenor"},
	{"Baritone"},
	{"GlennSax"},
	{"BoogieSax"},
	{"Octasax"},
	{"CountieSax"},
	{"SaxSection"},
	{"Saxes"},
	{"BillySax"},
	{"PopSax"},
	{"Lute"},
	{"Mandola"},
	{"LatinHarp"},
	{"Portuguese"},
	{"Fingered&Slide"},
	{"FingeredGtr"},
	{"FingerSlap"},
	{"Planet"},
	{"RockSynth"},
	{"Chuck"},
	{"SmoothLead"},
	{"SynthPlug"},
	{"Subbass"},
	{"Synvoice"},
	{"DeepJazz"},
	{"MemorySynth"},
	{"Evolution"},
	{"Sideral"},
	{"NorthWire"},
	{"Blowing"},
	{"Aural"},
	{"Velvet"},
	{"Etnovox"},
	{"Whisper"},
	{"Evopad"},
	{"Plucking"},
	{"MalletArea"},
	{"80’sPad"},
	{"FlutyPad"},
	{"NylonTheme"},
	{"BottledChoir"},
	{"BrassHits"},
	{"SouthernHarp"},
	{"UngarianZither"},
	{"AlpenChoir"},
	{"MalletWorld"},
	{"Country&Western"},
	{"StereoFolk"},
	{"AlpenMusette"},
	{"JazzAccordion"},
	{"BrightMusette"},
	{"335Jazz"},
	{"Smooth_Bass"},
	{"FunkyMuted"},
	{"CountryGtr"},
	{"Nylon_slide"},
	{"PowerGuit"},
	{"CleanChorus"},
	{"TremoloMark"},
	{"PhaseElectro"},
	{"OrganOverdrive"},
	{"Metal"},
	{"Distorted"},
	{"Crunch"},
	{"JazzCombo"},
	{"Echolead"}
};

PROGMEM const char drumsets[][16] =
{
	{"Standard1(SD)"},
	{"Standard2(SD)"},
	{"Standard1"},
	{"Standard2"},
	{"Room"},
	{"FolkSD*"},
	{"Acoustic*"},
	{"Jazz*"},
	{"Studio"},
	{"Folk"},
	{"Acoustic"},
	{"Jazz"},
	{"Rock1"},
	{"Rock2"},
	{"Disco1"},
	{"Disco2"},
	{"House*"},
	{"Techno*"},
	{"Techno"},
	{"House1"},
	{"House2"},
	{"Rap*"},
	{"HipHop1"},
	{"Heavy_Pop*"},
	{"Heavy_Pop"},
	{"Electro"},
	{"HipHop2"},
	{"Swing*"},
	{"Brush1*"},
	{"Brush2"},
	{"Swing"},
	{"Brush1"},
	{"Orchestra"},
	{"Latin*"},
	{"LightPop"},
	{"Industrial"},
	{"Dance1"},
	{"Dance2"},
	{"Street"},
	{"R&B"},
	{"Contemporary"},
	{"Analog1"},
	{"Analog2"},
	{"NewSoul"},
	{"Garage"},
	{"Vocal1"},
	{"Vocal2"}
};

// sound families
PROGMEM const uint8_t piano[35][2] = {
	{0,1},
	{0,3},
	{1,2},
	{0,4},
	{10,5},
	{0,5},
	{0,6},
	{1,6},
	{1,5},
	{0,8},
	{0,2},
	{10,3},
	{1,1},
	{10,1},
	{1,3},
	{1,4},
	{1,7},
	{1,8},
	{10,4},
	{0,7},
	{10,2},
	{11,1},
	{11,2},
	{10,6},
	{10,7},
	{10,8},
	{11,3},
	{11,4},
	{11,5},
	{11,6},
	{11,7},
	{10,9},
	{11,8},
	{11,121},
	{11,122}	
};

PROGMEM const uint8_t chromatic[][2] = {
	{0,12},
	{0,13},
	{0,14},
	{0,9},
	{0,10},
	{0,11},
	{0,15},
	{1,13},
	{1,9},
	{0,113},
	{1,10},
	{1,11},
	{1,12},
	{1,14},
	{1,15},
	{1,16},
	{10,10},
	{10,11},
	{10,12},
	{10,13},
	{10,14},
	{10,15},
	{10,16},
	{11,9},
	{11,10}
	
};

PROGMEM const uint8_t organ[][2] = {
	{10,19},
	{0,18},
	{0,19},
	{0,21},
	{1,20},
	{1,91},
	{1,96},
	{1,19},
	{1,17},
	{0,20},
	{1,92},
	{1,93},
	{1,94},
	{1,18},
	{10,17},
	{10,18},
	{0,17},
	{10,20},
	{10,21},
	{11,11},
	{11,12},
	{11,13},
	{11,14},
	{11,15},
	{11,16},
	{11,17},
	{11,18},
	{11,19},
	{11,20},
	{11,21},
	{1,95},
	{1,89},
	{1,90},
	{11,87},
	{11,123}
	
};

PROGMEM const uint8_t accordion[][2] = {
	{0,22},
	{0,23},
	{0,24},
	{1,21},
	{1,22},
	{1,23},
	{1,24},
	{10,22},
	{10,23},
	{10,24},
	{11,22},
	{11,23},
	{11,24},
	{11,111},
	{11,112},
	{11,113},
	{1,111},
	{1,112}
	
};

PROGMEM const uint8_t guitar[][2] = {
	{0,25},
	{0,26},
	{0,27},
	{0,28},
	{0,30},
	{1,27},
	{1,26},
	{1,99},
	{1,30},
	{1,32},
	
	{1,25},
	{10,29},
	{1,105},
	{1,107},
	{0,31},
	{11,26},
	{11,30},
	{1,110},
	{1,102},
	{0,29},
	
	{1,28},
	{1,29},
	{1,31},
	{1,97},
	{1,98},
	{1,100},
	{1,101},
	{1,103},
	{1,104},
	{1,108},
	
	{1,109},
	{10,25},
	{10,26},
	{10,27},
	{10,28},
	{10,30},
	{10,31},
	{10,32},
	{11,25},
	{11,27},
	
	{11,28},
	{11,29},
	{11,31},
	{11,32},
	{11,33},
	{11,34},
	{11,35},
	{11,36},
	{11,37},
	{11,38},
	
	{11,39},
	{11,40},
	{11,41},
	{11,114},
	{11,116},
	{11,117},
	{11,118},
	{11,119},
	{11,120},
	{0,32},
	
	{11,110},
	{11,124},
	{11,125},
	{11,126},
	{11,127},
	{11,77},
	{11,78},
	{11,76}
	
};

PROGMEM const uint8_t strings_choir[][2] = {
	{0,49},
	{1,49},
	{0,41},
	{0,42},
	{0,47},
	{1,47},
	{0,53},
	{0,54},
	{0,55},
	{1,53},
	
	{1,55},
	{1,51},
	{0,46},
	{0,43},
	{0,50},
	{10,41},
	{0,44},
	{0,51},
	{1,54},
	{1,52},
	
	{1,56},
	{10,53},
	{10,54},
	{10,55},
	{0,52},
	{0,45},
	{0,48},
	{1,41},
	{1,42},
	{1,43},
	
	{10,56},
	{11,51},
	{11,52},
	{11,53},
	{11,54},
	{11,55},
	{1,44},
	{1,45},
	{1,46},
	{1,48},
	
	{1,50},
	{10,42},
	{11,56},
	{11,57},
	{11,44},
	{11,45},
	{11,46},
	{11,47},
	{10,43},
	{10,44},
	
	{10,45},
	{10,46},
	{10,47},
	{10,48},
	{10,49},
	{10,50},
	{10,51},
	{10,52},
	{11,49},
	{11,48},
	
	{11,107},
	{11,58},
	{11,86},
	{0,56}
};

PROGMEM const uint8_t brass[][2] = {
	{0,62},
	{0,57},
	{0,58},
	{0,60},
	{0,63},
	{1,62},
	{1,57},
	{10,60},
	{1,60},
	{10,59},
	
	{1,61},
	{1,70},
	{1,71},
	{1,72},
	{0,64},
	{1,63},
	{1,59},
	{10,61},
	{10,70},
	{10,63},
	
	{0,59},
	{0,61},
	{1,58},
	{1,64},
	{1,69},
	{10,57},
	{10,58},
	{10,62},
	{10,64},
	{10,69},
	
	{10,71},
	{10,72},
	{10,73},
	{11,59},
	{11,60},
	{11,104}
	
};

PROGMEM const uint8_t sax_flute[][2] = {
	{1,67},
	{0,66},
	{0,68},
	{0,65},
	{10,66},
	{0,72},
	{0,74},
	{0,76},
	{1,76},
	{0,79},
	
	{1,73},
	{1,74},
	{0,67},
	{0,69},
	{10,65},
	{1,65},
	{1,66},
	{1,68},
	{1,75},
	{10,74},
	
	{0,73},
	{0,75},
	{0,77},
	{0,78},
	{0,70},
	{0,71},
	{10,67},
	{10,68},
	{11,61},
	{11,62},
	
	{0,80},
	{10,75},
	{10,76},
	{10,77},
	{10,78},
	{10,79},
	{11,63},
	{11,64},
	{11,65},
	{11,66},
	
	{11,67},
	{11,68},
	{10,80},
	{11,73},
	{11,69},
	{11,70},
	{11,71},
	{11,72}
	
};

PROGMEM const uint8_t pad[][2] = {
	{0,89},
	{0,90},
	{0,91},
	{0,92},
	{0,93},
	{0,94},
	{0,95},
	{0,96},
	{0,97},
	{0,98},
	
	{0,99},
	{0,100},
	{0,101},
	{0,102},
	{0,103},
	{0,104},
	{1,77},
	{1,78},
	{1,79},
	{10,89},
	
	{10,90},
	{10,91},
	{10,92},
	{10,93},
	{10,94},
	{10,95},
	{10,96},
	{10,97},
	{10,98},
	{10,99},
	
	{10,100},
	{10,101},
	{10,103},
	{10,104},
	{11,80},
	{11,89},
	{11,90},
	{11,91},
	{11,92},
	{11,93},
	
	{11,94},
	{11,95},
	{11,96},
	{11,97},
	{11,98},
	{11,99},
	{11,100},
	{11,102},
	{11,103},
	{11,101},
	
	{10,102},
	{11,50}
};

PROGMEM const uint8_t synth[][2] = {
	{1,83},
	{1,113},
	{1,114},
	{1,115},
	{1,117},
	{1,118},
	{1,119},
	{10,81},
	{1,80},
	{1,81},
	
	{1,82},
	{1,116},
	{1,120},
	{1,84},
	{10,82},
	{0,85},
	{10,84},
	{10,85},
	{10,86},
	{11,84},
	
	{0,81},
	{0,82},
	{0,83},
	{0,84},
	{10,83},
	{0,86},
	{0,87},
	{0,88},
	{10,87},
	{10,88},
	
	{11,81},
	{11,82},
	{11,83},
	{11,88},
	{11,128}
};

PROGMEM const uint8_t ethnic[][2] = {
	{0,106},
	{0,111},
	{0,110},
	{10,108},
	{0,105},
	{1,106},
	{10,109},
	{0,108},
	{0,115},
	{10,107},
	
	{10,106},
	{10,110},
	{0,109},
	{0,107},
	{0,112},
	{10,111},
	{0,16},
	{10,105},
	{10,112},
	{11,74},
	
	{11,105},
	{11,106},
	{11,108},
	{11,109},
	{11,75},
	{0,114},
	{0,116},
	{0,117},
	{0,118},
	{0,119},
	
	{10,113},
	{10,114},
	{10,115},
	{10,116},
	{10,117},
	{10,106},
	{10,111},
	{10,110},
	{0,108}
};

PROGMEM const uint8_t bass_sfx[][2] = {
	{0,32},
	{0,33},
	{0,34},
	{0,35},
	{0,36},
	{0,38},
	{1,35},
	{1,32},
	{1,86},
	{1,39},
	
	{10,32},
	{10,33},
	{10,34},
	{1,37},
	{1,38},
	{0,37},
	{0,39},
	{1,33},
	{1,34},
	{1,36},
	
	{1,84},
	{1,85},
	{1,87},
	{10,35},
	{10,36},
	{10,37},
	{10,38},
	{10,39},
	{11,41},
	{11,42},
	
	{11,78},
	{11,84},
	{11,114},
	{1,120},
	{1,121},
	{1,122},
	{1,123},
	{1,124},
	{1,125},
	{1,126},
	
	{1,127},
	{0,122},
	{0,123},
	{0,124},
	{0,125},
	{0,126},
	{0,127},
	{10,120},
	{10,121},
	{10,122},
	
	{10,123},
	{10,124},
	{10,125},
	{10,126},
	{10,127},
	{0,120},
	{0,121}
};

PROGMEM const uint8_t drums[] = {1,2,3,4,8,9,10,11,12,13,14,15,17,18,23,24,25,26,27,28,29,30,31,33,34,35,36,41,42,43,44,45,49,50,51,52,53,54,55,56,58,59,60,61,62,63,64};


static const char *family_names[NUM_OF_FAMILIES] = {
	"Piano","Chromatic","Organ","Accordion",
	"Guitar","Strings/Choir","Brass","Sax/Flute",
	"PAD","Synth","Ethnic","Bass/SFX","Drums"	
};

void updateLEDs(inputStates * data)
{
	digitalWrite(PIN_LED_GRN, !data->button[0]);
	digitalWrite(PIN_LED_RED, !data->button[1]);	
	
}

void showSplitPoint(byte data){
	sprintf(lcdline,"%d\n",data);
	tft.fillRect(0,40,ST7735_TFTHEIGHT,20,ST7735_MAGENTA);
	tft.text(lcdline,0,40);
}

void showScreenPart(uint8_t * family,uint8_t * program,uint8_t * bank,uint8_t mod,uint8_t hand,bool midiThru){
	showCurrentMode(mod,hand,midiThru);
	showFamilyName(family,hand,mod);
	showInstrumentName(bank,program,hand,mod);
}

void showFamilyName(uint8_t *ind,uint8_t hand,uint8_t currentMode){
	switch(hand){
		case RIGHT: if(currentMode == FAMILY_SELECT_RIGHT)
						tft.fillRect(0,20,ST7735_TFTHEIGHT,20,ST7735_BLUE);
					else
						tft.fillRect(0,20,ST7735_TFTHEIGHT,20,ST7735_MAGENTA);
					tft.text(family_names[ind[RIGHT]],0,20);
					break;
		case LEFT:	if(currentMode == FAMILY_SELECT_LEFT)
						tft.fillRect(0,80,ST7735_TFTHEIGHT,20,ST7735_BLUE);
					else
						tft.fillRect(0,80,ST7735_TFTHEIGHT,20,ST7735_MAGENTA);
					tft.text(family_names[ind[LEFT]],0,80);
					break;
	}		
		
}

static void getInstrumentName(uint8_t *bank,uint8_t *prog,uint8_t currentHand){
	uint8_t i = 0;
	switch(bank[currentHand]){
		case BANKA:	
			for(i = 0; i < 16; i++)
				lcdline[i] = pgm_read_byte(bank_a[prog[currentHand]] + i);
			break;
		case BANKB: 
			for(i = 0; i < 16; i++)
				lcdline[i] = pgm_read_byte(bank_b[prog[currentHand]] + i);
			break;
		case BANKC: 
			for(i = 0; i < 16; i++)
				lcdline[i] = pgm_read_byte(bank_c[prog[currentHand]] + i);
				break;
		case BANKD: 
			for(i = 0; i < 16; i++)
				lcdline[i] = pgm_read_byte(bank_d[prog[currentHand]] + i);
				break;
		case BANKE: 
			for(i = 0; i < 16; i++)
				lcdline[i] = pgm_read_byte(drumsets[prog[currentHand]] + i);
	}
}

void showInstrumentName(uint8_t *bank,uint8_t *prog,uint8_t hand,uint8_t currentMode){			
	getInstrumentName(bank,prog,hand);
	switch(hand){
		case RIGHT: if(currentMode == SOUND_SELECT_RIGHT)
						tft.fillRect(0,40,ST7735_TFTHEIGHT,20,ST7735_BLUE);
					else
						tft.fillRect(0,40,ST7735_TFTHEIGHT,20,ST7735_MAGENTA);
					tft.text(lcdline,0,40);
					break;
		case LEFT:	if(currentMode == SOUND_SELECT_LEFT)
						tft.fillRect(0,100,ST7735_TFTHEIGHT,20,ST7735_BLUE);
					else
						tft.fillRect(0,100,ST7735_TFTHEIGHT,20,ST7735_MAGENTA);
					tft.text(lcdline,0,100);
					break;
	}	
	
}

void showCurrentMode(uint8_t currentMode,uint8_t hand,bool midiThru){	
	switch(hand){
		case RIGHT:	tft.fillRect(0,0,ST7735_TFTHEIGHT,20,ST7735_RED);
					tft.text(modes_right[currentMode],0,0);
					tft.stroke(ST7735_YELLOW);
					if(midiThru == true)
						tft.text("Off",100,0);
					else
						tft.text("On",100,0);
					tft.stroke(0,0,0);
					break;
		case LEFT:	tft.fillRect(0,60,ST7735_TFTHEIGHT,20,ST7735_RED);
					tft.text(modes_left[currentMode],0,60);
					break;					
		}	
	
}

uint8_t getNextMode(uint8_t mod,uint8_t split){
	switch(mod){
		case NONE_SELECT: return FAMILY_SELECT_RIGHT;
		case FAMILY_SELECT_RIGHT: return SOUND_SELECT_RIGHT;		
	}
	if(split == true)
		switch(mod){
			case SOUND_SELECT_RIGHT: return FAMILY_SELECT_LEFT;
			case FAMILY_SELECT_LEFT: return SOUND_SELECT_LEFT;
			case SOUND_SELECT_LEFT: return NONE_SELECT;
		}		
	
	if(mod == SOUND_SELECT_RIGHT)
		return NONE_SELECT;			
		
	return NONE_SELECT;
}

void updateLCD(inputStates * data){
	char name[32];
	uint8_t familyIndex = data->pot[0];
	uint8_t programIndex = data->pot[1];
	
	tft.background(255,0,255);
	itoa(familyIndex,name,10);
	tft.text(name,0,0);
	itoa(programIndex,name,10);
	tft.text(name,20,0);
}

void getBankProgram(uint8_t *family,uint8_t *index,uint8_t *currentBank, uint8_t *currentProgram){
	uint8_t hand;
	for(hand = 0; hand < 2; hand++){				
		switch(family[hand]){
			case PIANO:	currentBank[hand] = pgm_read_byte(piano[index[hand]] + 0);
						currentProgram[hand] = pgm_read_byte(piano[index[hand]] + 1);
						break;
			case CHROMATIC:	currentBank[hand] = pgm_read_byte(chromatic[index[hand]] + 0);
							currentProgram[hand] = pgm_read_byte(chromatic[index[hand]] + 1);
							break;
			case ORGAN:	currentBank[hand] = pgm_read_byte(organ[index[hand]] + 0);
						currentProgram[hand] = pgm_read_byte(organ[index[hand]] + 1);
						break;
			case ACCORDION:	currentBank[hand] = pgm_read_byte(accordion[index[hand]] + 0);
						currentProgram[hand] = pgm_read_byte(accordion[index[hand]] + 1);
						break;		
			case GUITAR:	currentBank[hand] = pgm_read_byte(guitar[index[hand]] + 0);
						currentProgram[hand] = pgm_read_byte(guitar[index[hand]] + 1);
						break;
			case STRINGS_CHOIR:	currentBank[hand] = pgm_read_byte(strings_choir[index[hand]] + 0);
						currentProgram[hand] = pgm_read_byte(strings_choir[index[hand]] + 1);
						break;
			case BRASS:	currentBank[hand] = pgm_read_byte(brass[index[hand]] + 0);
						currentProgram[hand] = pgm_read_byte(brass[index[hand]] + 1);
						break;
			case SAX_FLUTE:	currentBank[hand] = pgm_read_byte(sax_flute[index[hand]] + 0);
						currentProgram[hand] = pgm_read_byte(sax_flute[index[hand]] + 1);
						break;
			case PAD:	currentBank[hand] = pgm_read_byte(pad[index[hand]] + 0);
						currentProgram[hand] = pgm_read_byte(pad[index[hand]] + 1);
						break;
			case SYNTH:	currentBank[hand] = pgm_read_byte(synth[index[hand]] + 0);
						currentProgram[hand] = pgm_read_byte(synth[index[hand]] + 1);
						break;
			case ETHNIC:currentBank[hand] = pgm_read_byte(ethnic[index[hand]] + 0);
						currentProgram[hand] = pgm_read_byte(ethnic[index[hand]] + 1);
						break;
			case BASS_SFX:	currentBank[hand] = pgm_read_byte(bass_sfx[index[hand]] + 0);
						currentProgram[hand] = pgm_read_byte(bass_sfx[index[hand]] + 1);
						break;
			case DRUMS:	currentBank[hand] = BANKE;
						currentProgram[hand] = pgm_read_byte(drums + index[hand]);
						break;
		}
	
		currentProgram[hand] -= 1;
	}
	
}

bool savePreset(uint8_t number,uint8_t * family,uint8_t *familyIndex,bool split,uint8_t splitNote){
	uint8_t data[6];
	OSFS::result res;
	if(number >= NUM_OF_PRESETS)
		return false;
		
	data[0] = family[LEFT];
	data[1] = familyIndex[LEFT];
	data[2] = family[RIGHT];
	data[3] = familyIndex[RIGHT];
	data[4] = (uint8_t)split;
	data[5] = splitNote;
	
	
	res = OSFS::newFile(presetNames[number],data,sizeof(data),true);
	
	return res == OSFS::result::NO_ERROR;
}

bool readPreset(uint8_t number,uint8_t * family,uint8_t *familyIndex,bool *split,byte *splitNote){
	uint8_t data[6];
	OSFS::result res;
	if(number >= NUM_OF_PRESETS)
		return false;
	tft.text(presetNames[number],0,0);
	res = OSFS::getFile(presetNames[number],data);	
	if(res != OSFS::result::NO_ERROR)
		return false;
	family[LEFT] = data[0];
	familyIndex[LEFT] = data[1];
	family[RIGHT] = data[2];
	familyIndex[RIGHT] = data[3];
	*split = data[4];
	*splitNote = data[5];
	return true;
}


void OSFS::readNBytes(uint16_t address, unsigned int num, byte* output) {
	for (uint16_t i = address; i < address + num; i++) {
		*output = EEPROM.read(i);
		output++;
	}
}
void OSFS::writeNBytes(uint16_t address, unsigned int num, const byte* input) {
	for (uint16_t i = address; i < address + num; i++) {
		EEPROM.update(i, *input);
		input++;
	}
}

void showPresetList(uint8_t selected,uint8_t but){
	uint8_t offset = 0;
	uint8_t index = 0;
	switch(but){
		case NONE:
			offset = selected * 20;
			tft.text("Preset A",0,0);
			tft.text("Preset B",0,20);
			tft.text("Preset C",0,40);
			tft.text("Preset D",0,60);
			tft.text("Preset E",0,80);
			tft.text("Preset F",0,100);
			break;
		case BUTTON0:
			offset = 20 * (selected + 1);
			index = selected + 1;
			break;
		case BUTTON2:
			offset = 20 * (selected - 1);
			index = selected - 1;
			break;
	}
	// delete previous selection
	if(but != NONE){
		tft.fillRect(0,offset,ST7735_TFTHEIGHT,20,ST7735_MAGENTA);
		tft.text(presetNames[index],0,offset);
	}
	
	tft.fillRect(0,20 * selected,ST7735_TFTHEIGHT,20,ST7735_RED);
	tft.text(presetNames[selected],0,20 * selected);
		
}
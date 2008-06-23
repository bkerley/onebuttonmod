/*
 these structs are used for mikmod - can't include the whole header because
 it conflicts with Cocoa.h
*/
typedef char CHAR;
typedef signed char     SBYTE;      /* 1 byte, signed */
typedef unsigned char   UBYTE;      /* 1 byte, unsigned */
typedef signed short    SWORD;      /* 2 bytes, signed */
typedef unsigned short  UWORD;      /* 2 bytes, unsigned */
typedef signed long     SLONG;      /* 4 bytes, signed */
typedef unsigned long   ULONG;      /* 4 bytes, unsigned */

typedef struct SAMPLE {
	SWORD  panning;     /* panning (0-255 or PAN_SURROUND) */
	ULONG  speed;       /* Base playing speed/frequency of note */
	UBYTE  volume;      /* volume 0-64 */
	UWORD  inflags;		/* sample format on disk */
	UWORD  flags;       /* sample format in memory */
	ULONG  length;      /* length of sample (in samples!) */
	ULONG  loopstart;   /* repeat position (relative to start, in samples) */
	ULONG  loopend;     /* repeat end */
	ULONG  susbegin;    /* sustain loop begin (in samples) \  Not Supported */
	ULONG  susend;      /* sustain loop end                /      Yet! */
	
	/* Variables used by the module player only! (ignored for sound effects) */
	UBYTE  globvol;     /* global volume */
	UBYTE  vibflags;    /* autovibrato flag stuffs */
	UBYTE  vibtype;     /* Vibratos moved from INSTRUMENT to SAMPLE */
	UBYTE  vibsweep;
	UBYTE  vibdepth;
	UBYTE  vibrate;
	CHAR*  samplename;  /* name of the sample */
	
	/* Values used internally only */
	UWORD  avibpos;     /* autovibrato pos [player use] */
	UBYTE  divfactor;   /* for sample scaling, maintains proper period slides */
	ULONG  seekpos;     /* seek position in file */
	SWORD  handle;      /* sample handle used by individual drivers */
} SAMPLE;

/* Instrument note count */
#define INSTNOTES 120

/* Envelope point */
typedef struct ENVPT {
	SWORD pos;
	SWORD val;
} ENVPT;

/* Envelope point count */
#define ENVPOINTS 32

/* Instrument structure */
typedef struct INSTRUMENT {
	CHAR* insname;
	
	UBYTE flags;
	UWORD samplenumber[INSTNOTES];
	UBYTE samplenote[INSTNOTES];
	
	UBYTE nnatype;
	UBYTE dca;              /* duplicate check action */
	UBYTE dct;              /* duplicate check type */
	UBYTE globvol;
	UWORD volfade;
	SWORD panning;          /* instrument-based panning var */
	
	UBYTE pitpansep;        /* pitch pan separation (0 to 255) */
	UBYTE pitpancenter;     /* pitch pan center (0 to 119) */
	UBYTE rvolvar;          /* random volume varations (0 - 100%) */
	UBYTE rpanvar;          /* random panning varations (0 - 100%) */
	
	/* volume envelope */
	UBYTE volflg;           /* bit 0: on 1: sustain 2: loop */
	UBYTE volpts;
	UBYTE volsusbeg;
	UBYTE volsusend;
	UBYTE volbeg;
	UBYTE volend;
	ENVPT volenv[ENVPOINTS];
	/* panning envelope */
	UBYTE panflg;           /* bit 0: on 1: sustain 2: loop */
	UBYTE panpts;
	UBYTE pansusbeg;
	UBYTE pansusend;
	UBYTE panbeg;
	UBYTE panend;
	ENVPT panenv[ENVPOINTS];
	/* pitch envelope */
	UBYTE pitflg;           /* bit 0: on 1: sustain 2: loop */
	UBYTE pitpts;
	UBYTE pitsusbeg;
	UBYTE pitsusend;
	UBYTE pitbeg;
	UBYTE pitend;
	ENVPT pitenv[ENVPOINTS];
} INSTRUMENT;

struct MP_CONTROL;
struct MP_VOICE;

/*
	Module definition
 */

/* maximum master channels supported */
#define UF_MAXCHAN	64

/* Module flags */
#define UF_XMPERIODS	0x0001 /* XM periods / finetuning */
#define UF_LINEAR		0x0002 /* LINEAR periods (UF_XMPERIODS must be set) */
#define UF_INST			0x0004 /* Instruments are used */
#define UF_NNA			0x0008 /* IT: NNA used, set numvoices rather
than numchn */
#define UF_S3MSLIDES	0x0010 /* uses old S3M volume slides */
#define UF_BGSLIDES		0x0020 /* continue volume slides in the background */
#define UF_HIGHBPM		0x0040 /* MED: can use >255 bpm */
#define UF_NOWRAP		0x0080 /* XM-type (i.e. illogical) pattern break
semantics */
#define UF_ARPMEM		0x0100 /* IT: need arpeggio memory */
#define UF_FT2QUIRKS	0x0200 /* emulate some FT2 replay quirks */
#define UF_PANNING		0x0400 /* module uses panning effects or have
non-tracker default initial panning */

typedef struct MODULE {
	/* general module information */
		CHAR*       songname;    /* name of the song */
		CHAR*       modtype;     /* string type of module loaded */
		CHAR*       comment;     /* module comments */
		
		UWORD       flags;       /* See module flags above */
		UBYTE       numchn;      /* number of module channels */
		UBYTE       numvoices;   /* max # voices used for full NNA playback */
		UWORD       numpos;      /* number of positions in this song */
		UWORD       numpat;      /* number of patterns in this song */
		UWORD       numins;      /* number of instruments */
		UWORD       numsmp;      /* number of samples */
		struct  INSTRUMENT* instruments; /* all instruments */
		struct  SAMPLE*     samples;     /* all samples */
		UBYTE       realchn;     /* real number of channels used */
		UBYTE       totalchn;    /* total number of channels used (incl NNAs) */
		
		/* playback settings */
		UWORD       reppos;      /* restart position */
		UBYTE       initspeed;   /* initial song speed */
		UWORD       inittempo;   /* initial song tempo */
		UBYTE       initvolume;  /* initial global volume (0 - 128) */
		UWORD       panning[UF_MAXCHAN]; /* panning positions */
		UBYTE       chanvol[UF_MAXCHAN]; /* channel positions */
		UWORD       bpm;         /* current beats-per-minute speed */
		UWORD       sngspd;      /* current song speed */
		SWORD       volume;      /* song volume (0-128) (or user volume) */
		
		int        extspd;      /* extended speed flag (default enabled) */
		int        panflag;     /* panning flag (default enabled) */
		int        wrap;        /* wrap module ? (default disabled) */
		int        loop;		 /* allow module to loop ? (default enabled) */
		int        fadeout;	 /* volume fade out during last pattern */
		
		UWORD       patpos;      /* current row number */
		SWORD       sngpos;      /* current song position */
		ULONG       sngtime;     /* current song time in 2^-10 seconds */
		
		SWORD       relspd;      /* relative speed factor */
		
		/* internal module representation */
		UWORD       numtrk;      /* number of tracks */
		UBYTE**     tracks;      /* array of numtrk pointers to tracks */
		UWORD*      patterns;    /* array of Patterns */
		UWORD*      pattrows;    /* array of number of rows for each pattern */
		UWORD*      positions;   /* all positions */
		
		BOOL        forbid;      /* if true, no player update! */
		UWORD       numrow;      /* number of rows on current pattern */
		UWORD       vbtick;      /* tick counter (counts from 0 to sngspd) */
		UWORD       sngremainder;/* used for song time computation */
			
			struct MP_CONTROL*  control;     /* Effects Channel info (size pf->numchn) */
			struct MP_VOICE*    voice;       /* Audio Voice information (size md_numchn) */
			
			UBYTE       globalslide; /* global volume slide rate */
			UBYTE       pat_repcrazy;/* module has just looped to position -1 */
				UWORD       patbrk;      /* position where to start a new pattern */
				UBYTE       patdly;      /* patterndelay counter (command memory) */
				UBYTE       patdly2;     /* patterndelay counter (real one) */
				SWORD       posjmp;      /* flag to indicate a jump is needed... */
				UWORD		bpmlimit;	 /* threshold to detect bpm or speed values */
} MODULE;


/* This structure is used to query current playing voices status */
typedef struct VOICEINFO {
		INSTRUMENT* i;            /* Current channel instrument */
		SAMPLE*     s;            /* Current channel sample */
		SWORD       panning;      /* panning position */
		SBYTE       volume;       /* channel's "global" volume (0..64) */
		UWORD       period;       /* period to play the sample at */
		UBYTE       kick;         /* if true = sample has been restarted */
} VOICEINFO;

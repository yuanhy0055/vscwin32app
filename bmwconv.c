/*
 *  BMWCONV v0.10 - BMW Audio File Converter
 *
 *  Converts BMW audio files (BR3/BR4/BR5) to/from regular audio files (MP4/MP3/WMA)
 *  
 *  Written quickly by Justin Newman <180ring@gmail.com>
 *
 *  No license, it's public domain (feel free to do what you want with it)
 *
 *  This code should convert the above-mentioned audio files to/from each other. I
 *  wrote and built this code under Cygwin on Windows XP. It should build/run on
 *  most Windows releases with or without Cygwin. It should also build/run on most
 *  Linux and Mac distros/releases. I did a quick test on Debian and it worked fine.
 *
 *  To build, go to the directory with this code and type:
 *
 *    gcc bmwconv.c -o bmwconv
 *
 *  If gcc does not exist, you may need to install it. If you run into errors
 *  compiling, you may need to fix them.
 *
 *  This code has not been fully tested, but it should work. I tested this code with
 *  my 2013 BMW 550i and 2010 BMW 535i. It converted to/from without any issues. The
 *  export capability does not exist on my 2006 BMW 330i.
 *
 *  If you need to process a lot of files, multiple directories, etc, just write a 
 *  small script that  passes the directory and name of each file to this code. It is
 *  very easy to do.
 *
 *  WARNING: Use at your own risk. Only operate on backup files in a safe directory.
 *  This code will overwrite files with the same name.
 *
 */

#include <stdio.h>
#include <string.h>

#if defined(_MSC_VER)
#define strcasecmp _stricmp
#endif

void usage() {
	printf("bmwconv v0.10 - converts bmw's audio files (br3/br4/br5) to/from regular audio files (mp4/mp3/wma)\n");
	printf("written quickly by justin newman <180ring@gmail.com> - no license, it's public domain\n");
	printf("\n");
	printf("usage: bmwconv [options] <input.ext> [output.ext]\n");
	printf("\n");
	printf("options:\n");
	printf("  -h | --help     shows this help screen\n");
	printf("\n");
	printf("mapping:\n");
	printf("  br3 = mp4\n");
	printf("  br4 = mp3\n");
	printf("  br5 = wma\n");
	printf("\n");
	printf("example: bmwconv depeche_mode_enjoy_the_silence.BR4\n");
        printf("  (converts to depeche_mode_enjoy_the_silence.mp3)\n");
	printf("\n");
	printf("note: use correct input ext or output ext will be wrong\n");
}

char *swapext(char *ext, char *buf, int maxlen) {

	if (!buf) {
		return NULL;
	}

	buf[0] = '\0';

	if (!ext) {
		return buf;
	}

	if (!strcasecmp(ext, "BR3")) {
		strncpy(buf, "mp4", maxlen);
	} else if (!strcasecmp(ext, "BR4")) {
		strncpy(buf, "mp3", maxlen);
	} else if (!strcasecmp(ext, "BR5")) {
		strncpy(buf, "wma", maxlen);
	} else if (!strcasecmp(ext, "mp4")) {
		strncpy(buf, "BR3", maxlen);
	} else if (!strcasecmp(ext, "mp3")) {
		strncpy(buf, "BR4", maxlen);
	} else if (!strcasecmp(ext, "wma")) {
		strncpy(buf, "BR5", maxlen);
	}

	return buf;
}

int main(int argc, char **argv) {

	FILE *fpi, *fpo;
	char input[256] = "\0";
	char output[256] = "\0";
	char ext[256] = "\0";
	char buf[1024] = "\0";
	char *p;
	int i, l, c;

	for (i = 1; i < argc; i++) {
		if (*argv[i] != '-' && *argv[i] != '/') {
			break;
		}
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			usage();
			return 0;
		} else {
			printf("error: invalid option specified\n");
			return -1;
		}
	}

	if (i >= argc) {
		printf("error: missing input filename\n");
		return -1;
	}

	strncpy(input, argv[1], sizeof(input));
	strncpy(output, (argc > 2) ? argv[2] : input, sizeof(output));

	if (argc < 3) {
		if ((p = strrchr(output, '.')) != NULL) {
			*p++ = '\0';
		}				
		swapext(p, ext, sizeof(ext));
		if (strlen(ext) < 1) {
			printf("error: cannot figure file type from input ext\n");
			return -1;
		}
		strncat(output, ".", sizeof(output) - strlen(output));
		strncat(output, ext, sizeof(output) - strlen(output));
	}

	if ((fpi = fopen(input, "rb")) == NULL) {
		printf("error: could not open input \"%s\"\n", input);
		return -1;
	}

	unlink(output);
	if ((fpo = fopen(output, "wb")) == NULL) {
		printf("error: could not open output \"%s\"\n", output);
		return -1;
	}

	while (!feof(fpi)) {
		l = fread(buf, 1, sizeof(buf), fpi);
		if (l < 1) {
			break;
		}
		for (i = 0; i < l; i++) {
			c = ~(buf[i] & 0xff) & 0xff;
			buf[i] = c;
		}
		fwrite(buf, 1, l, fpo);
	}

	fflush(fpo);
	fclose(fpo);

	fclose(fpi);

	return 0;
}

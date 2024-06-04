#include <libavformat/avformat.h>
#include <stdint.h>
#include <stdio.h>

#define INBUF_SIZE 4096

/* For now, read sample MP4 file and print header information */
int main(int argc, char **argv) {
	__label__ close;
	const char *filename = "videos/input.mp4";
	AVFormatContext *s = NULL;
	int ret = avformat_open_input(&s, filename, NULL, NULL);
	/* AVFormat library - decoding media documentation:
	 * https://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#details */
	if (ret < 0) {
		printf("Open failed. Does file %s exist?\n", filename);
		exit(1);
	}
	printf("Filename: %s\n"   \
				 "Start time: %ld\n" \
				 "Duration: %ld\n"  \
				 "Bit rate: %ld\n"  \
				 "Codec Whitelist: %s\n",
					s->url, 
					s->start_time,
					s->duration,
					s->bit_rate,
					s->codec_whitelist);
close:
	avformat_close_input(&s);
	return ret;
}

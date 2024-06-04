#include <libavformat/avformat.h>
#include <stdint.h>
#include <stdio.h>

#define INBUF_SIZE 4096

/* For now, read sample MP4 file and print header information */
int main(int argc, char **argv) {
	__label__ close_packet, close;
	const char *filename = "videos/input.mp4";
	AVFormatContext *s = NULL;
	int ret = avformat_open_input(&s, filename, NULL, NULL);
	/* AVFormat library - decoding media documentation:
	 * https://ffmpeg.org/doxygen/trunk/group__lavf__decoding.html#details */
	if (ret < 0) {
		printf("Open failed. Does file %s exist?\n", filename);
		exit(1);
	}
	/* Certain details are not present in headers */
	ret = avformat_find_stream_info(s, NULL);
	if (ret < 0) {
		printf("Finding stream failed. Check %s health\n", filename);
		goto close;
	}
	printf("Filename: %s\n"   \
				 "Start time: %ld\n" \
				 "Duration: %ld\n"  \
				 "Bit rate: %ld\n",
					s->url, 
					s->start_time,
					s->duration,
					s->bit_rate);
	/* Read each frame in the video */
	AVPacket *pkt = av_packet_alloc();
	if (pkt == NULL) {
		printf("Packet allocation failed.");
		ret = -1;
		goto close;
	}
	do {
		int done = av_read_frame(s, pkt);
		if (done < 0) {
			break;
		}
		av_packet_unref(pkt);
	} while (ret >= 0);

	close_packet:
		av_packet_free(&pkt);
	close:
		avformat_close_input(&s);
		return ret;
}

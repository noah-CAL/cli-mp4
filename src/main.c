#include <libavcodec/avcodec.h>
#include <libavcodec/codec.h>
#include <libavformat/avformat.h>
#include <stdint.h>
#include <stdio.h>

#define INBUF_SIZE 4096

static int process_packet(AVPacket*);

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
		ret = process_packet(pkt);
		if (ret) {
			goto close_packet;
		}
		av_packet_unref(pkt);
	} while (ret >= 0);

	close_packet:
		av_packet_free(&pkt);
	close:
		avformat_close_input(&s);
		return ret;
}

/* Decodes and processes each packet. Returns non-zero value on error. */
int process_packet(AVPacket* pkt) {
	__label__ free_context;
	const AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_MPEG4);
	if (codec == NULL) {
		printf("Error while finding AV Codec\n");
		return 1;
	}
	int ret = 0;
	/* 1. Allocate Codec context */
	AVCodecContext *avctx = avcodec_alloc_context3(codec);
	if (avctx == NULL) {
		printf("Error while allocating AV Codec context\n");
		ret = 1;
		goto free_context;
	}
	/* 2. Open the codec context to allow for decoding 
	 * TODO: make the context initialization occur once per MP4
	 * instead of once-per-packet */
	ret = avcodec_open2(avctx, codec, NULL);
	if (ret) {
		printf("Error while opening codec context\n");
		goto free_context;
	}
	/* 3. Decode packet with MP4 context 
	 * TODO: Warning: The input buffer, avpkt->data must be 
	 * AV_INPUT_BUFFER_PADDING_SIZE larger than the actual read bytes because some 
	 * optimized bitstream readers read 32 or 64 bits at once and could read over 
	 * the end. How do you determine the read bytes? */
	ret = avcodec_send_packet(avctx, pkt);
	switch(ret) {
		case 0:
			/* no decoding errors */
			printf("Packet decoding successful.\n");  // TODO: remove statement
			break;
		case AVERROR(EAGAIN): 
			printf("Not implemented yet.\n");
			goto free_context;
		case AVERROR(EINVAL): 
			printf("Codec has not been opened.\n");
			goto free_context;
		case AVERROR(ENOMEM):
			printf("Failure to add packet to queue.\n");
			goto free_context;
		default:
			printf("Error decoding packet.\n");
			goto free_context;
	}
	free_context:
		avcodec_free_context(&avctx);
		return ret;
}

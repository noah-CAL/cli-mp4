/* Test inclusion of all FFmpeg libraries */
#include <libavcodec/codec.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavutil/avassert.h>
#include <libavutil/tx.h>
#include <libpostproc/postprocess.h>
#include <libswresample/swresample.h>
#include <libswscale/version.h>
#include <stdio.h>

int main() {
	printf("Hello world!\n");
	printf("All libraries loaded\n");
	return 0;
}

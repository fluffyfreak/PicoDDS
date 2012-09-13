#include "PicoDDS.h"

int main(int argc, char *argv[])
{
	using namespace PicoDDS;

	DDSImage ddsIn;
	const char filename[] = "../../../data/lena.dds";
	size_t sizeLoaded = ddsLoad(filename, ddsIn);

	return 0;
}
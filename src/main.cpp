#include "PicoDDS.h"

int main(int argc, char *argv[])
{
	using namespace PicoDDS;

	DDSImage dxt1;
	size_t sizeLoaded = DDSLoad("../../../data/lena.dds", dxt1);

	DDSImage dxt5;
	sizeLoaded = DDSLoad("../../../data/bowfin_DXT5_xGxR.dds", dxt5);

	DDSImage ati2;
	sizeLoaded = DDSLoad("../../../data/bowfin_ATI2.dds", ati2);

	return 0;
}
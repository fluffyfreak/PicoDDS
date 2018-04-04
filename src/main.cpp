#include "PicoDDS.h"
#include <cstdio>
#include <cassert>

void LoadTextureByName(const char *pName)
{
	assert(pName != nullptr);
	printf("------------------------\n");
	PicoDDS::DDSImage dds;
	const size_t sizeLoaded = DDSLoad(pName, dds);
	if (dds.IsLoaded() && (sizeLoaded>0)) {
		printf("Size loaded: %zu\n", sizeLoaded);
		printf("Storage Size: %d\n", dds.CalculateStorageSize());
		printf("Number Images: %d\n", dds.GetNumImages());
	} else {
		printf("Not loaded: %s\n", pName);
	}
}

int main(int argc, char *argv[])
{
	LoadTextureByName("../../../data/lena.dds");
	LoadTextureByName("../../../data/bowfin_norm_PNG_DXT5_xGxR_1.dds");
	LoadTextureByName("bad data");

	return 0;
}
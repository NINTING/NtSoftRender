#include"NtUtility.h"
#include <ctime>

#include <cstdlib>



void UtilityInit()
{
	// load support for the JPG and PNG image formats
	int flags = IMG_INIT_JPG | IMG_INIT_PNG ;
	int initted = IMG_Init(flags);
	if ((initted&flags) != flags) {
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
		// handle error
	}
}






#pragma pack(push,1)
struct TGA_Header {
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;
	char  imagedescriptor;
};
#pragma pack(pop)

bool NtUtility::load_rle_data(std::ifstream &in, Tex2D_UC *img) {

	unsigned long pixelcount = img->GetWidth()*img->GetHeight();
	

	unsigned long currentpixel = 0; 
	unsigned long currentbyte = 0;
	int bytespp = img->GetBytesPerPixel();
	unsigned char *data = img->GetBuffer();  
	NtColor colorbuffer;
	unsigned char bgra[4] = { 0 };
	do {
		unsigned char chunkheader = 0;
		chunkheader = in.get();
		if (!in.good()) {
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
		if (chunkheader < 128) {
			chunkheader++;
			for (int i = 0; i < chunkheader; i++) {
				in.read((char *)bgra, bytespp);
				if (!in.good()) {
					std::cerr << "an error occured while reading the header\n";
					return false;
				}
				for (int i = 0; i < bytespp; i++) {
					data[currentbyte++] = bgra[i];
				}
				
				currentpixel++;
				/*
				if (bytespp == RGBA){

					data[currentpixel++] = (bgra[0] << 24) | (bgra[1] << 16) | (bgra[2] << 8) | bgra[3];
					}
				else if (bytespp == RGB)
					data[currentpixel++] = (bgra[2] << 24) | (bgra[1] << 16) | (bgra[0] << 8) | bgra[3];
				*/


				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
		else {
			chunkheader -= 127;
			in.read((char *)bgra, bytespp);
			if (!in.good()) {
				std::cerr << "an error occured while reading the header\n";
				return false;
			}
			for (int i = 0; i < chunkheader; i++) {
				for (int i = 0; i < bytespp; i++) {
					/*
					if (i == 3) {
						data[currentbyte++] = bgra[3];
						break;
					}
					*/
					data[currentbyte++] = bgra[i];
				}
				currentpixel++;

				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
	} while (currentpixel < pixelcount);
	return true;
}

Tex2D_UC NtUtility::Read_file(const std::string& filename)
{
	// load sample.tga into image
	SDL_Surface *image;
	//SDL_RWops *rwop;
	//rwop = SDL_RWFromFile(filename.c_str(), "rb");
	image = IMG_Load(filename.c_str());
	
	Tex2D_UC ret;
	
	if (!image) {
		printf("IMG_LoadTGA_RW: %s\n", IMG_GetError());
		// handle error
	}
	else {
		int bpp = image->format->BytesPerPixel;
		size_t size = image->w*image->h*bpp;
		SDL_PixelFormat * fmt = image->format;
		ret = Tex2D_UC(image->w, image->h);
		Uint8* s = (Uint8*)image->pixels;
		unsigned char* dest = ret.GetBuffer();
		//for (int i = 0, p = 0; i < size; i += bpp, p += 4)
		//{
			//memcpy(dest + p, s + i, bpp);
		//}
		
		if (fmt->palette != nullptr)
		{
			for (int i = 0, p = 0; i < size; i += bpp, p += 4)
			{
				int index = ((Uint8 *)image->pixels)[i];
				SDL_Color color = fmt->palette->colors[index];
				
			}
		}
		else {
			SDL_LockSurface(image);

			for (int i = 0, p = 0; i < size; i += bpp, p += 4)
			{


				Uint32* np = (Uint32*)(s + i);
				Uint32 pixel = *np, temp;
				

				Uint8 r, g, b, a;
				SDL_GetRGBA(pixel, fmt, &r, &g, &b, &a);
				ret.Set(p, r);
				ret.Set(p + 1, g);
				ret.Set(p + 2, b);
				ret.Set(p + 3, a);

			}
			SDL_UnlockSurface(image);
		}
		
		/*
		Uint32* s = (Uint32*)image->pixels;
		SDL_LockSurface(image);
		for (int i = 0, p = 0; i < image->w*image->h; i ++, p += 4)
		{
			SDL_PixelFormat * fmt = image->format;

			//Uint32* np = s[i];
			Uint32 pixel = s[i], temp;
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixel, fmt, &r, &g, &b, &a);
			ret.Set(p, r);
			ret.Set(p + 1, g);
			ret.Set(p + 2, b);
			ret.Set(p + 3, a);

		}
		SDL_UnlockSurface(image);
		return ret;
		
*/
	}
	return ret;
}

bool NtUtility::Read_Tga_file(std::string filename,Tex2D_4F* ret)
{
	Tex2D_UC tmp;
	Read_Tga_file(filename, &tmp);
	*ret = RGBAImageToFloat4Image(tmp);
	return true;
}

bool NtUtility::Read_Tga_file(std::string filename, Tex2D_UC*img)
{
	{
		img->CleanBuffer();
		//Uint32* data = *(img->GetPointToBuffer32());


		std::ifstream in;
		in.open(filename.c_str(), std::ios::binary);
		if (!in.is_open()) {
			std::cerr << "can't open file " << filename << "\n";
			in.close();
			return false;
		}
		TGA_Header header;
		in.read((char *)&header, sizeof(header));
		if (!in.good()) {
			in.close();
			std::cerr << "an error occured while reading the header\n";
			return false;
		}

		img->SetWidth(header.width);
		img->SetHeight(header.height);
		//img->SetBytesPerPixel(header.bitsperpixel >> 3);
		int bytespp = header.bitsperpixel>>3;
		int width = img->GetWidth();
		int height = img->GetHeight();
		unsigned long nbytes = bytespp * width*height;
		unsigned char * data = img->CreateBuffer_(nbytes);

		if (width <= 0 || height <= 0 || (bytespp != GRAYSCALE && bytespp != RGB && bytespp != RGBA)) {
			in.close();
			std::cerr << "bad bpp (or width/height) value\n";
			return false;
		}
	


		if (3 == header.datatypecode || 2 == header.datatypecode) {
			in.read((char *)data, nbytes);
			if (!in.good()) {
				in.close();
				std::cerr << "an error occured while reading the data\n";
				return false;
			}
		}
		else if (10 == header.datatypecode || 11 == header.datatypecode) {
			if (!load_rle_data(in, img)) {
				in.close();
				std::cerr << "an error occured while reading the data\n";
				return false;
			}
		}
		else {
			in.close();
			std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
			return false;
		}
		if (!(header.imagedescriptor & 0x20)) {
			//
			//img->flip_vertically();
		}
		if (header.imagedescriptor & 0x10) {
		//	img->flip_horizontally();
		}
		std::cerr << width << "x" << height << "/" << bytespp * 8 << "\n";
		in.close();
		return true;

	}
}

NtVector4 NtUtility::RandomVec()
{
	srand((int)time(0));
	return NtVector4(rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX));
}
float NtUtility::RandomF(float l, float r)
{
	srand((int)time(0));
	return (rand() / double(RAND_MAX)*(r - l) + l);
}

void NtUtility::RandomOffsetVec(NtVector4*offset)
{
	offset[0] = NtVector4(1.f,1.f,1.f,0.f);
	offset[1] = NtVector4(-1.f, -1.f, -1.f, 0.f);

	offset[2] = NtVector4(-1.f, 1.f, 1.f, 0.f);
	offset[3] = NtVector4(1.f, -1.f, -1.f, 0.f);

	offset[4] = NtVector4(1.f, 1.f, -1.f, 0.f);
	offset[5] = NtVector4(-1.f, -1.f, 1.f, 0.f);

	offset[6] = NtVector4(-1.f, 1.f, -1.f, 0.f);
	offset[7] = NtVector4(1.f, -1.f, 1.f, 0.f);

	offset[8] = NtVector4(-1.f, 0.f, 0.f, 0.f);
	offset[9] = NtVector4(1.f, 1.f, 1.f, 0.f);

	offset[10] = NtVector4(0.f, 1.f, 0.f, 0.f);
	offset[11] = NtVector4(0.f, 1.f, 0.f, 0.f);

	offset[12] = NtVector4(0.f, 0.f, -1.f, 0.f);
	offset[13] = NtVector4(0.f, 0.f, 1.f, 0.f);
	for (int i = 0; i < 14; i++)
	{
		float s = RandomF(0.25, 1.f);
		 offset[i].normalize();
		offset[i] *= s;
	}


}
void NtUtility::RandomNormalImage(int w, int h,Tex2D_3F**out)
{
	if (*out != nullptr)
		delete []*out;
	*out = new Tex2D_3F(w,h);
	for (int j = 0; j <h; j++) {
		for (int i = 0; i < w; i++)
		{
			NtVector3 tmp = NtVector4To3(NtUtility::RandomVec());
			tmp.normalize();
			(*out)->Set(i, j, tmp);
		}
	}


}
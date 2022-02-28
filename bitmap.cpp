#include <iostream>
#include <fstream>


#pragma pack(push, 1)

struct bitmap_header {
	unsigned short file_type{0x4D42};
	unsigned int file_size{0};
	unsigned short reserved1{0};
	unsigned short reserved2{0};
	unsigned int offset_data{14 + 12};
};

struct bitmap_info_header {
	unsigned int header_size{12};
	unsigned short width{0};
	unsigned short height{0};
	unsigned short planes{1};
	unsigned short bitperpixel{24};
};

struct bitmap {
private:
	bitmap_header header;
	bitmap_info_header info_header;

	unsigned char *pixels;

public:
	bitmap(unsigned int w, unsigned int h) {
		info_header.width = w; info_header.height = h;
		header.file_size = w * h * 3 + 14 + 12;
		pixels = new unsigned char[w * h * 3];
	}

	bitmap(char* filname) {
		FILE *file;
		file = fopen(filname, "rb");

		fread(&header, 14, 1, file);
		fread(&info_header, 12, 1, file);
		pixels = new unsigned char[info_header.width * info_header.height * 3];
		fread(pixels, info_header.width * info_header.height * 3, 1, file);

		fclose(file);
	}

	unsigned int width() {return info_header.width;};
	unsigned int height() {return info_header.height;};

	void set(unsigned int x, unsigned int y, unsigned int color) {
		y = info_header.height - y;

		unsigned char r,g,b;

		r = color & 0xff; g = color << 8 & 0xff; b = color << 16 & 0xff;

		pixels[(x + y * info_header.width) * 3 + 0] = b;
		pixels[(x + y * info_header.width) * 3 + 1] = g;
		pixels[(x + y * info_header.width) * 3 + 2] = r;
	}

	void set(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b) {
		y = info_header.height - y;
		
		pixels[(x + y * info_header.width) * 3 + 0] = b;
		pixels[(x + y * info_header.width) * 3 + 1] = g;
		pixels[(x + y * info_header.width) * 3 + 2] = r;
	}

	unsigned int get(unsigned int x, unsigned int y) {
		y = info_header.height - y;

		unsigned int color = pixels[(x + y * info_header.width) * 3 + 2] | pixels[(x + y * info_header.width) * 3 + 1] << 8 | pixels[(x + y * info_header.width) * 3 + 0] << 16;
		return color;
	}

	unsigned char red(unsigned int x, unsigned int y) {
		y = info_header.height - y;

		return pixels[(x + y * info_header.width) * 3 + 2];
	}

	unsigned char green(unsigned int x, unsigned int y) {
		y = info_header.height - y;

		return pixels[(x + y * info_header.width) * 3 + 1];
	}

	unsigned char blue(unsigned int x, unsigned int y) {
		y = info_header.height - y;

		return pixels[(x + y * info_header.width) * 3 + 0];
	}

	void save(char* filename) {
		FILE *file;
		file = fopen(filename, "wb");

		fwrite(&header, 14, 1, file);
		fwrite(&info_header, 12, 1, file);
		fwrite(pixels, info_header.width * info_header.height * 4, 1, file);

		fclose(file);
	}
};

#pragma pack(pop)
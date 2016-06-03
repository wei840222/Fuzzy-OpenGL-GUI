// g_bmp.h
//
// Created by Gary Ho, ma_hty@hotmail.com, 2005
//

#ifndef G_BMP_H
#define G_BMP_H

class GBmp
{
public:
	int w, h;
	unsigned char *rgb;

	GBmp();
	~GBmp();

	void load(const char *spath);
	void load(int width, int height);
	void load(int width, int height, unsigned char *rgb_data);
	void save(const char *spath);
	void flip_vectical();

private:
	void rb_swap();
};

#endif
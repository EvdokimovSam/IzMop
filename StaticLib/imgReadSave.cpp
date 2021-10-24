/**
* \file ImageIO.cpp
* \brief Позволяет считать BMP-изображение
* \details Файл библиотеки, считывающий с файла и записывающий в файл BMP-изображение
* \date 18.02.2021
* \authors 
*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "framework.h"
typedef unsigned char BYTE;

img* scan_img(char* filename)
{
	FILE* f = fopen(filename, "rb");

	// Проверка на наличие файла "*.bmp" при включенном режиме отладки (#define _DEBUG)
#ifdef _DEBUG
	if (f == NULL)
	{
		std::cout << filename << " not found" << std::endl;
		return NULL;
	}
#endif

	header h;
	fread(&h, sizeof(header), 1, f);

	rgb col[256];
	for (int i = 0; i < 256; i++) fread(col + i, sizeof(rgb), 1, f);

	// Растровая развертка

	unsigned int padded = h.width;
	if (h.width % 4) padded = h.width + (4 - (h.width % 4));
	BYTE* data = (BYTE*)calloc(sizeof(BYTE), h.width * h.height);

	// Буфер для считывание и пропуска доп. байтов
	BYTE* trash_buf = nullptr;
	if(padded > h.width)
	{
		trash_buf = (BYTE*)calloc(sizeof(BYTE), padded - h.width);
	}
	
	// Считывание, игнорируя доп. байты
	for (int i = 0; i < h.height; i++)
	{
		BYTE* buf = (data + i * h.width);
		fread(buf, sizeof(BYTE), h.width, f);
		if (padded > h.width)
		{
			fread(trash_buf, sizeof(BYTE), padded - h.width, f);
		}
	}
	free(trash_buf);
	
	// Создание выходного изображения
	img* image = (img*)calloc(1, sizeof(img));
	image->x_size = h.width;
	image->y_size = h.height;

	image->pix = (BYTE*)calloc(image->x_size * image->y_size, sizeof(BYTE));
	image->pix = data;
	image->meta = h;

	fclose(f);

	return image;
}

void export_img(img* m, char* name)
{
	FILE* f = fopen(name, "wb");
	header h = m->meta;
	rgb p[256];

	// Генерация палитры
	for (int i = 0; i < 256; i++) p[i] = { (BYTE)i,(BYTE)i,(BYTE)i,(BYTE)0 };

	// Расчет длины с учетом доп. байтов
	unsigned int padded = h.width;
	if (h.width % 4) padded = h.width + (4 - (h.width % 4));

	BYTE* data = (BYTE*) calloc(padded * h.height, sizeof(char));

	// Перенесение пикселей в массив pixels с учетом доп. байтов (они добавляются)
	for (int i = 0; i < h.height; i++)
	{
		for (int pix = 0; pix < h.width; pix++)
		{
			*(data + i * h.width + pix + (i)*(padded - h.width)) = *(m->pix + i * h.width + pix);
		}
	}

	// Определение дополнительных данных об изображении
	h.sizeImage = padded * h.height;
	h.bmpSize = padded * h.height + h.offBits;
	
	fwrite(&h, sizeof(h), 1, f);
	fwrite(p, sizeof(rgb), 256, f);
	fwrite(data, sizeof(BYTE), padded * m->y_size, f);

	fclose(f);
}
/**
* \file StaticLib.cpp
* \brief Файл библиотеки, в котором приведена реализация функций работы с изображениями.
* \details Основная задача этого модуля - это изменение структур, хранящих данные об изображении,
* с помощью функций поворота, переворота, масштабирования, изменения яркости изображений.
* \author 
* \date 17.02.2021
*/
#include "framework.h"
#include <sstream>
typedef unsigned char BYTE;

char get_pixel(img* im, int x, int y)
{
	return *(im->pix + y * im->x_size + x);
}

void flip(img* im, char dir)
{

	if (dir == 'H' || dir=='h')
	{
		// Меняет местами j сверху с j-м снизу элементом, проходя до середины столбца. Применяется ко всем столбцам
		for (int i = 0; i < im->x_size; i++)
		{
			for (int j = 0; j < im->y_size/2; j++) {
				char t = *(im->pix + j * im->x_size + i);
				*(im->pix + j * im->x_size + i) = get_pixel(im, i, im->y_size - j - 1);
				*(im->pix + (im->y_size - j - 1) * im->x_size + i) = t;
			}
		}
	}
	else if(dir == 'V' || dir == 'v')
	{
		for (int i = 0; i < im->x_size/2; i++)
		{
			for (int j = 0; j < im->y_size; j++)
			{
				char t = *(im->pix + j * im->x_size + i);
				*(im->pix + j * im->x_size + i) = get_pixel(im, im->x_size-i-1, j);
				*(im->pix + j * im->x_size + (im->x_size - i - 1)) = t;
			}
		}
	}
}

void upscale(img* im, int v, int h)
{

	img modified(im->x_size * h, im->y_size * v);
	modified.pix = new BYTE[modified.x_size * modified.y_size];
	modified.meta = im->meta;
	modified.meta.height = im->y_size * v;
	modified.meta.width = im->x_size*h;

	modified.meta.sizeImage *= v * h;
	modified.meta.bmpSize = modified.meta.sizeImage + modified.meta.offBits;
	for (int i = 0; i < im->x_size; i++)
	{
		for (int j = 0; j < im->y_size; j++)
		{
			for (int fill_h = 0; fill_h < h; fill_h++)
			{
				for (int fill_v = 0; fill_v < v; fill_v++)
				{

					*((&modified)->pix + (j * v + fill_v) * modified.x_size + (i * h + fill_h)) = get_pixel(im, i, j);
				}
			}
		}
	}
	// Очистка данных старой картинки
	free(im->pix);

	// Запись результата
	*im = modified;
}

void downscale(img* im, int v, int h)
{
	flip(im, 'h');
	// Проверка на кратность
	if (im->x_size % h == 0 && im->y_size % v == 0)
	{
		img modified(im->x_size / h, im->y_size / v);
		modified.pix = new BYTE[modified.x_size * modified.y_size];
		modified.meta = im->meta;
		modified.meta.height = im->y_size / v;
		modified.meta.width = im->x_size / h;
		modified.meta.sizeImage /= (v * h);
		modified.meta.bmpSize = modified.meta.sizeImage + modified.meta.offBits;
		for (int i = 0; i < im->x_size; i += h)
		{
			for (int j = 0; j < im->y_size; j += v)
			{
				*((&modified)->pix + (j / v) * modified.x_size + (i / h)) = get_pixel(im, i, j);
			}
		}
		flip(&modified, 'h');
		// Очистка данных старой картинки
		free(im->pix);

		// Запись результата
		*im = modified;
	}
}

void rotate(img* im, int angle)
{
	angle = angle % 360;

	if (angle == 180 || angle == 270)
	{
		flip(im, 'v');
		flip(im, 'h');
		angle -= 180;
	}

	// Повернуть ещё на 90 градусов
	if (angle == 90)
	{
		img modified(im->y_size, im->x_size);
		modified.pix = new BYTE[modified.x_size * modified.y_size];
		modified.meta = im->meta;
		modified.meta.height = im->x_size;
		modified.meta.width = im->y_size;

		for (int i = 0; i < im->x_size; i++)
			for (int j = 0; j < im->y_size; j++) {
				BYTE* addr = (modified.pix + (im->x_size - 1 - i) * modified.x_size + j);
				*addr = get_pixel(im, i, j);
			}


		// Очистка данных изображения
		free(im->pix);

		// Запись результата
		*im = modified;
	}
}

void bright(img* im, int f)
{

	for (int i = 0; i < im->x_size; i++)
	{
		for (int j = 0; j < im->y_size; j++)
		{
			if (f > 0) *(im->pix + j * im->x_size + i) = ((BYTE) get_pixel(im, i, j) << f);
			else if (f < 0) *(im->pix + j * im->x_size + i) = ((BYTE)get_pixel(im, i, j) >> -f);
		}
	}
}

int* nums(char* stroka)
{
	char* stroka_ptr = stroka;
	int* arr = new int[100];
	for (int i = 0; i < 100; i++)
	{
		arr[i] = 0;
	}
	int* p = arr;
	bool flag = true;
	int i = 0;
	for (; flag; ++i) {
		sscanf_s(stroka_ptr, "%d", &(arr[i]));
		while(*stroka_ptr != ',' && *stroka_ptr) stroka_ptr++;
		if (!*stroka_ptr) flag = false;
		stroka_ptr++;
	}
	return arr;
}

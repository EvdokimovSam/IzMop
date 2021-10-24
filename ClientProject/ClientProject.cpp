/**
* \file ClientProject.cpp
* \brief Основной файл, который содержит реализацию возможностей библиотеки
* \details Файл с методом main(),
* в котором производится работа с файлом команд и библиотекой для считывания, преобразования и сохранения изображений
* \date 14.02.2021
* \authors 
*/

#define _CRT_SECURE_NO_WARNINGS ///< Для возможности использования функций fopen и т.д.
#define _DEBUG                  ///< Режим отладки
#include <iostream>
#include "framework.h"
using namespace std;

/**
* \brief Применяет преобразование command для изображений или изображения
* \details Выполняет определённую команду для нужных изображений
* \param f Указатель на структуру FILE, необходимый для считывания файла command.txt
* \param command Указатель на строку, содержащую команду
* \param channels Указатель на начало массива номеров каналов, для которых нужно применить команду
* \param kanal_cnt Суммарное кол-во каналов 
* \param images Указатель на совокупность указателей на изображения
*/
void run_command(char* command, FILE* f, int* channels, int kanal_cnt, img** images);

/**
* \brief Основной метод
* \details Реализует процесс считывания файла с командами, преобразования и сохранения изображения с помощью библиотечных и других функций
*/
int main()
{
    FILE* f = fopen("command.txt", "r");

    // Проверка на наличие файла "command.txt" при включенном режиме отладки (#define _DEBUG)
#ifdef _DEBUG 
    if (f == NULL)
    {
        cout << "command.txt not found." << endl;
        return 1;
    }
#endif
    
    int commands_cnt = -1;
    int channels_cnt = -1; ///< Сколько всего было запрошено к обработке каналов

    fscanf(f, "%d %d\n", &channels_cnt, &commands_cnt);

    img** images; ///< Указатель на указатели на считанные изображения

    // Считывание всех спектральных каналов
    images = (img**)calloc(channels_cnt, sizeof(img*));
    images = (img**)calloc(channels_cnt, sizeof(img*));
    for (int i = 1; i <= channels_cnt; i++)
    {

        // Формирование имени файла
        char* name = (char*)calloc(32, 1);
        int first = i / 10;
        int second = i % 10;

        snprintf(name, 32, "input%d%d.bmp", first, second);

        // Считывание очередного файла
        images[i - 1] = scan_img(name);

        free(name);
    }

    for (int i = 0; i < commands_cnt; i++)
    {
        char channels_raw[256], command[16];
        // Считывание строк вида <Список_спектральных_каналов> : <Команда> : <Параметры_команды>
        fscanf(f, "%[0-9,] : ", channels_raw);
        fscanf(f, "%[A-Za-z] : ", command);

        int* channels = nums(channels_raw);
        
        // Применение команды к нужным каналам
        run_command(command, f, channels, channels_cnt, images);

    }

    // Сохранение всех спектральных каналов
    for (int i = 1; i <= channels_cnt; i++)
    {

        // Формирование имени файла
        char* name = (char*)calloc(32, 1);
        int first = i / 10;
        int second = i % 10;

        snprintf(name, 32, "output%d%d.bmp", first, second);

        // Сохранение очередного файла
        export_img(images[i - 1], name);


        free(name);
    }
    
    // Очистка памяти
    for (int i = 0; i < channels_cnt; i++)
    {
        free(images[i]->pix);
        free(images[i]);
    }
    free(images);
}

void run_command(char* command, FILE* f, int* channels, int kanal_cnt, img** images)
{
    // Если запрашивается перевернуть изображение
    if (!strcmp(command, "flip"))
    {
        char param;
        fscanf(f, "%c\n", &param);

        for (int i = 0; channels[i] > 0 && i < kanal_cnt && channels[0] != 0 || channels[0] == 0 && i < kanal_cnt; i++)
        {
            // Обработка каналов по отдельности
            int kanal = channels[0] == 0 ? i + 1 : channels[i];

            flip(images[kanal - 1], param);
        }
    }
    // Если запрашивается повернуть изображение
    else if (!strcmp(command, "rotate"))
    {
        int param;
        fscanf(f, "%d\n", &param);

        for (int i = 0; i < kanal_cnt; i++)
        {
            // Обработка каналов по отдельности
            int kanal = channels[0] == 0 ? i + 1 : channels[i];


            rotate(images[kanal - 1], param);

        }
    }
    // Если запрашивается отмасштабировать изображение
    else if (!strcmp(command, "upscale"))
    {
        int v, h;
        fscanf(f, "%d %d\n", &v, &h);

        for (int i = 0; i < kanal_cnt; i++)
        {
            // Обработка каналов по отдельности
            int kanal = channels[0] == 0 ? i + 1 : channels[i];

            upscale(images[kanal - 1], v, h);
        }
    }
    // Если запрашивается отмасштабировать изображение
    else if (!strcmp(command, "downscale"))
    {
        int v, h;
        fscanf(f, "%d %d\n", &v, &h);

        for (int i = 0; i < kanal_cnt; i++)
        {
            // Обработка каналов по отдельности
            int kanal = channels[0] == 0 ? i + 1 : channels[i];


            downscale(images[kanal - 1], v, h);
        }
    }
    // Если запрашивается изменить яркость изображения
    else if (!strcmp(command, "bright"))
    {
        int param;
        fscanf(f, "%d\n", &param);

        for (int i = 0; i < channels[i] > 0 && i < kanal_cnt && channels[0] != 0 || channels[0] == 0 && i < kanal_cnt; i++)
        {
            // Обработка каналов по отдельности
            int kanal = channels[0] == 0 ? i + 1 : channels[i];

            img* to = images[kanal - 1];

            bright(to, param);
        }
    }

}

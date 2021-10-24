/**
* \file ClientProject.cpp
* \brief �������� ����, ������� �������� ���������� ������������ ����������
* \details ���� � ������� main(),
* � ������� ������������ ������ � ������ ������ � ����������� ��� ����������, �������������� � ���������� �����������
* \date 14.02.2021
* \authors 
*/

#define _CRT_SECURE_NO_WARNINGS ///< ��� ����������� ������������� ������� fopen � �.�.
#define _DEBUG                  ///< ����� �������
#include <iostream>
#include "framework.h"
using namespace std;

/**
* \brief ��������� �������������� command ��� ����������� ��� �����������
* \details ��������� ����������� ������� ��� ������ �����������
* \param f ��������� �� ��������� FILE, ����������� ��� ���������� ����� command.txt
* \param command ��������� �� ������, ���������� �������
* \param channels ��������� �� ������ ������� ������� �������, ��� ������� ����� ��������� �������
* \param kanal_cnt ��������� ���-�� ������� 
* \param images ��������� �� ������������ ���������� �� �����������
*/
void run_command(char* command, FILE* f, int* channels, int kanal_cnt, img** images);

/**
* \brief �������� �����
* \details ��������� ������� ���������� ����� � ���������, �������������� � ���������� ����������� � ������� ������������ � ������ �������
*/
int main()
{
    FILE* f = fopen("command.txt", "r");

    // �������� �� ������� ����� "command.txt" ��� ���������� ������ ������� (#define _DEBUG)
#ifdef _DEBUG 
    if (f == NULL)
    {
        cout << "command.txt not found." << endl;
        return 1;
    }
#endif
    
    int commands_cnt = -1;
    int channels_cnt = -1; ///< ������� ����� ���� ��������� � ��������� �������

    fscanf(f, "%d %d\n", &channels_cnt, &commands_cnt);

    img** images; ///< ��������� �� ��������� �� ��������� �����������

    // ���������� ���� ������������ �������
    images = (img**)calloc(channels_cnt, sizeof(img*));
    images = (img**)calloc(channels_cnt, sizeof(img*));
    for (int i = 1; i <= channels_cnt; i++)
    {

        // ������������ ����� �����
        char* name = (char*)calloc(32, 1);
        int first = i / 10;
        int second = i % 10;

        snprintf(name, 32, "input%d%d.bmp", first, second);

        // ���������� ���������� �����
        images[i - 1] = scan_img(name);

        free(name);
    }

    for (int i = 0; i < commands_cnt; i++)
    {
        char channels_raw[256], command[16];
        // ���������� ����� ���� <������_������������_�������> : <�������> : <���������_�������>
        fscanf(f, "%[0-9,] : ", channels_raw);
        fscanf(f, "%[A-Za-z] : ", command);

        int* channels = nums(channels_raw);
        
        // ���������� ������� � ������ �������
        run_command(command, f, channels, channels_cnt, images);

    }

    // ���������� ���� ������������ �������
    for (int i = 1; i <= channels_cnt; i++)
    {

        // ������������ ����� �����
        char* name = (char*)calloc(32, 1);
        int first = i / 10;
        int second = i % 10;

        snprintf(name, 32, "output%d%d.bmp", first, second);

        // ���������� ���������� �����
        export_img(images[i - 1], name);


        free(name);
    }
    
    // ������� ������
    for (int i = 0; i < channels_cnt; i++)
    {
        free(images[i]->pix);
        free(images[i]);
    }
    free(images);
}

void run_command(char* command, FILE* f, int* channels, int kanal_cnt, img** images)
{
    // ���� ������������� ����������� �����������
    if (!strcmp(command, "flip"))
    {
        char param;
        fscanf(f, "%c\n", &param);

        for (int i = 0; channels[i] > 0 && i < kanal_cnt && channels[0] != 0 || channels[0] == 0 && i < kanal_cnt; i++)
        {
            // ��������� ������� �� �����������
            int kanal = channels[0] == 0 ? i + 1 : channels[i];

            flip(images[kanal - 1], param);
        }
    }
    // ���� ������������� ��������� �����������
    else if (!strcmp(command, "rotate"))
    {
        int param;
        fscanf(f, "%d\n", &param);

        for (int i = 0; i < kanal_cnt; i++)
        {
            // ��������� ������� �� �����������
            int kanal = channels[0] == 0 ? i + 1 : channels[i];


            rotate(images[kanal - 1], param);

        }
    }
    // ���� ������������� ���������������� �����������
    else if (!strcmp(command, "upscale"))
    {
        int v, h;
        fscanf(f, "%d %d\n", &v, &h);

        for (int i = 0; i < kanal_cnt; i++)
        {
            // ��������� ������� �� �����������
            int kanal = channels[0] == 0 ? i + 1 : channels[i];

            upscale(images[kanal - 1], v, h);
        }
    }
    // ���� ������������� ���������������� �����������
    else if (!strcmp(command, "downscale"))
    {
        int v, h;
        fscanf(f, "%d %d\n", &v, &h);

        for (int i = 0; i < kanal_cnt; i++)
        {
            // ��������� ������� �� �����������
            int kanal = channels[0] == 0 ? i + 1 : channels[i];


            downscale(images[kanal - 1], v, h);
        }
    }
    // ���� ������������� �������� ������� �����������
    else if (!strcmp(command, "bright"))
    {
        int param;
        fscanf(f, "%d\n", &param);

        for (int i = 0; i < channels[i] > 0 && i < kanal_cnt && channels[0] != 0 || channels[0] == 0 && i < kanal_cnt; i++)
        {
            // ��������� ������� �� �����������
            int kanal = channels[0] == 0 ? i + 1 : channels[i];

            img* to = images[kanal - 1];

            bright(to, param);
        }
    }

}

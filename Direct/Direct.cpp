// winApi.cpp: определяет точку входа для консольного приложения.
//

#include <fstream>
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <windows.h>
#include <string.h>
#include <WindowsX.h>

using namespace std;

const LPCWSTR ADRESS = (L"C:\\Projects");
const LPCWSTR MASK = (L"\\*");
const LPCWSTR SLASH = (L"\\");

wstring sAdress = ADRESS;
wstring Mask = MASK;
wstring FullAdress = sAdress + Mask;
wstring Slash = SLASH;
LPCWSTR adress = FullAdress.c_str();

int sizeSubCatolog(LPCWSTR Adress, LPCWSTR catalogAdressAdd)
{
	wstring CatalogAdress = catalogAdressAdd;

	WIN32_FIND_DATAW wsd;

	HANDLE const hSize = FindFirstFileW(Adress, &wsd);
	setlocale(LC_ALL, "");
	LPCWSTR catalogName1;
	LPCWSTR catalogNameAdd1;
	int sizeElement1 = 0;
	int sizeCatalog1 = 0;

	if (INVALID_HANDLE_VALUE != hSize)
	{
		do
		{
			if (!((wsd.cFileName[0] == (L'.')) || (wsd.cFileName == (L"..")))) // '..' -родительская папка, '.' - сама папка
			{
				sizeElement1 = (wsd.nFileSizeHigh * MAXDWORD) + wsd.nFileSizeLow;

				sizeCatalog1 += sizeElement1;

				if ((wsd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0) //если каталог то нужно идти глубже
				{
					wstring strFullCatalogAdress = CatalogAdress + Slash + &wsd.cFileName[0] + Mask;
					wstring strCatalogAdressRec = CatalogAdress + Slash + &wsd.cFileName[0];
					catalogName1 = strFullCatalogAdress.c_str();
					catalogNameAdd1 = strCatalogAdressRec.c_str();
					sizeCatalog1 += sizeSubCatolog(catalogName1, catalogNameAdd1);
				}
			}
		} while (NULL != FindNextFileW(hSize, &wsd));

		FindClose(hSize);
	}
	else
		cout << " Каталог не найден" << Adress << endl;

	return sizeCatalog1;
}

int main()
{
	WIN32_FIND_DATAW wfd;
	HANDLE const hFind = FindFirstFileW(adress, &wfd);
	setlocale(LC_ALL, "");
	LPCWSTR catalogName;
	LPCWSTR catalogNameAdd;
	int sizeElement = 0;
	int sizeCatalog = 0;
	int sizeCatalogAdd = 0;

	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (!((wfd.cFileName[0] == (L'.')) || (wfd.cFileName == (L".."))))
			{
				cout << "Имя элемента: ";
				wcout << &wfd.cFileName[0]<< " ";

				sizeElement = (wfd.nFileSizeHigh * MAXDWORD) + wfd.nFileSizeLow; //считамем размер элемента

				sizeCatalog += sizeElement;

				if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0) //если текущий элемент является каталогом то идем глубже
				{
					wstring strAl = sAdress + Slash + &wfd.cFileName[0] + Mask;
					wstring strBt = sAdress + Slash + &wfd.cFileName[0];
					catalogName = strAl.c_str();
					catalogNameAdd = strBt.c_str();
					sizeCatalogAdd = sizeSubCatolog(catalogName, catalogNameAdd);
					sizeCatalog += sizeCatalogAdd;
					cout << " Размер каталога: " << sizeCatalogAdd <<" bytes"<< endl;
					sizeCatalogAdd = 0;
				}
				else
				{
					cout << "Размер файла: ";
					wcout << sizeElement << " bytes" << endl;
				}
			}
		} while (NULL != FindNextFileW(hFind, &wfd));

		FindClose(hFind);
	}
	else
		cout << "Каталог не найден!" << endl;

	wcout << ADRESS;
	cout << " Размер каталога: " << sizeCatalog << " bytes" << endl;
	getchar();
	return 0;
}
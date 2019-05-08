#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <vld.h>
#include <windows.h>



#define _CRT_SECURE_NO_WARNINGS //zapewnia wsparcie dla niektorych funkcji jak np fopen.
enum gracz { pierwszy, drugi };

bool zaladowanie(int tablica1[10][10], int grywalna[10][10], char* nazwaPlikuGracza) {
	char* nazwaPliku = nazwaPlikuGracza;
	FILE *plik = fopen(nazwaPliku, "r");

	if (plik != NULL)
	{
		size_t rozmiarSlowa = 20;
		char * obecneSlowoWPliku = malloc(sizeof(char) * rozmiarSlowa);

		for (int i = 0; i < 10; i++) {
			if (fgets(obecneSlowoWPliku, rozmiarSlowa, plik) != NULL) // czytam linie z pliku
			{
				obecneSlowoWPliku[strlen(obecneSlowoWPliku) - 1] = '\0';
				for (int j = 0; j < 10; j++) {
					tablica1[i][j] = obecneSlowoWPliku[j] - '0';
					grywalna[i][j] = 7;
				}
			}
		}
		free(obecneSlowoWPliku);
		fclose(plik);
		return true;
	}
	else
	{
		perror(nazwaPliku); // komunikat o tym dlaczego plik sie nie otworzyl
		printf("BLAD - nie otworzono pliku !!");
		return false;
	}

}

void wyswietlMojeTablice(int tablica1[10][10]) {
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++) {
			printf("%d", tablica1[i][j]);
		}
		printf("\n");
	}
}

bool sprawdzTablice(int tablica[10][10]) {
	//need to be upgrade ! 
	int licznik = 0; // w orginalnej tablicy powinno byc 35 jedynek
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++) {
			if (tablica[i][j] == 1) licznik++;
		}

	}
	if (licznik == 35) return true;
	else return false;
}

void przepisz(int tablica1[10][10], int tablica2[10][10]) {
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++) {
			tablica2[i][j] = tablica1[i][j];
		}

	}
}

bool koniecGry(int warunek1[10][10], int warunek2[10][10]) {
	if (sprawdzTablice(warunek1) ) return true;
	if (sprawdzTablice(warunek2)) return true;
	else return false;
}

void wyswietlTabliceDoStrzalu(int mapa[10][10], int x, int y, bool graPierwszy, bool byloTrafione) {
	
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (graPierwszy) SetConsoleTextAttribute(hStdOut, (FOREGROUND_RED | BACKGROUND_BLUE));
	else SetConsoleTextAttribute(hStdOut, (FOREGROUND_RED | BACKGROUND_GREEN));
	system("@cls||clear");

	if (byloTrafione) printf("Bylo TRAFIONE !!! \n");
	else {
		if (graPierwszy) printf("Pudlo gracza 2 ! :D \n");
		else printf("Pudlo gracza 1 ! :D \n");
	}
	int aktualneX = x;
	int aktualneY = y;
	for (int j = 0; j < 10; j++)
		printf("%d", j);
	printf("\n");
	for (int j = 0; j < 10; j++)
		printf("-");

	for (int i = 0; i < 10; i++)
	{
		printf("\n");
		for (int j = 0; j < 10; j++) {

			if ((aktualneX == j) && aktualneY == i)
				printf("S");
			else  printf("%d", mapa[i][j]);
			if ((j == 9)) printf("- %d", i);
		}
	}
	printf("\n");
	printf("\n Kliknij w WSAD oraz zatwierdz enterem aby sie przemiescic \n");
	if (graPierwszy) printf("Obecnie gra gracz 1 \n");
	else printf("Obecnie gra gracz 2 \n");
}

// zwraca 1 dla przycisku 'q'
// zwraca 0 w innym przypadku
int wybierzstrzal(int mapa[10][10], int* x, int* y, bool graPierwszy, bool byloTrafione) {

	int aktualneX = *x;
	int aktualneY = *y;
	bool bylEnter = false;
	bool powinienemOdswiezyc = true;
	while (bylEnter == false) {

		if (powinienemOdswiezyc) {
			
			wyswietlTabliceDoStrzalu(mapa, aktualneX, aktualneY, graPierwszy, byloTrafione);
		}

		char wcisnietyPrzycisk;
		scanf(" %c", &wcisnietyPrzycisk);


		switch (wcisnietyPrzycisk) {
		case 'w':
			if (aktualneY > 0) {
				aktualneY--;
				powinienemOdswiezyc = true;
			}
			else powinienemOdswiezyc = false;
			break;
		case 's':
			if (aktualneY < 9) {
				aktualneY++;
				powinienemOdswiezyc = true;
			}
			else powinienemOdswiezyc = false;
			break;
		case 'a':
			if (aktualneX > 0) {
				aktualneX--;
				powinienemOdswiezyc = true;
			}
			else powinienemOdswiezyc = false;
			break;
		case 'd':
			if (aktualneX < 9)
			{
				aktualneX++;
				powinienemOdswiezyc = true;
			}
			else powinienemOdswiezyc = false;
			break;
			//p jest rowne enterowi/ strzalowi
		case 'p':
			if (mapa[aktualneY][aktualneX] == 7) {
				*x = aktualneX;
				*y = aktualneY;
				bylEnter = true;
			}
			powinienemOdswiezyc = false;
			break;
		case 'q':
			return 1;
			break;
		default:
			powinienemOdswiezyc = false;
			break;
		}
	}
	return 0;
}

bool sprawdzStrzal(int mapa[10][10], int x, int y) {
	if (mapa[y][x] == 1) return true;
	else return false;
}

int main(int argc, char *argv[]) {


	int orginalna_gracz1[10][10];
	int orginalna_gracz2[10][10];
	int grywalna_gracz1[10][10];
	int grywalna_gracz2[10][10];
	enum gracz ktoryTeraz = pierwszy;
	bool byloTrafione = false;
	char* nazwaPliku1;
	char* nazwaPliku2;

	if (argc == 1)
		printf("\n Nie podano dodatkowych argumentow ! \n Wpisz  << nazwa_programu -help >> aby ujrzec pomoc");
	else if (argc == 3)
	{
		nazwaPliku1 = argv[1];
		nazwaPliku2 = argv[2];


		if (zaladowanie(orginalna_gracz1, grywalna_gracz1, nazwaPliku1)
			&& zaladowanie(orginalna_gracz2, grywalna_gracz2, nazwaPliku2))
		{

			//wyswietlMojeTablice(orginalna_gracz1);
			//printf("\n");
			//wyswietlMojeTablice(orginalna_gracz2);

			if (sprawdzTablice(orginalna_gracz1) && sprawdzTablice(orginalna_gracz2)) {
				while (koniecGry(grywalna_gracz1, grywalna_gracz2) != true) {

					int x = 0;
					int y = 0;
					switch (ktoryTeraz)
					{
					case pierwszy:
						if (wybierzstrzal(grywalna_gracz1, &x, &y, true, byloTrafione) == 0);
						else return 0;
						if (sprawdzStrzal(orginalna_gracz2, x, y) == true)
						{
							grywalna_gracz1[y][x] = 1;
							byloTrafione = true;
						}
						else
						{
							grywalna_gracz1[y][x] = 0;
							ktoryTeraz = drugi;
							byloTrafione = false;
							x = y = 0;
						}
						break;
					case drugi:
						if (wybierzstrzal(grywalna_gracz2, &x, &y, false, byloTrafione) == 0);
						else return 0;
						if (sprawdzStrzal(orginalna_gracz1, x, y) == true)
						{
							grywalna_gracz2[y][x] = 1;
							byloTrafione = true;
						}
						else
						{
							grywalna_gracz2[y][x] = 0;
							ktoryTeraz = pierwszy;
							byloTrafione = false;
							x = y = 0;
						}
						break;
					}

				}
			}
			else printf("Masz zle mapy !");

		}
		else printf("NIE ZALADOWANO PLIKU !");
	}
	else if (argv[1] == "-help")
	{
		printf("\n Poprawne wywolanie programu to : \n nazwa_programu nazwa_pliku_1 nazwa_pliku_2");
	}
	_CrtDumpMemoryLeaks();
	return 0;
}
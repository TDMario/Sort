// Catch Error of scanf
#define _CRT_SECURE_NO_WARNINGS

// Umlaute definieren
#define ue (unsigned char)129
#define oe (unsigned char)148
#define ae (unsigned char)132

// Bibliothek importieren
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

// Struktur PersonData erstellen: Mario Forrer
typedef struct PersonData {
	char Vorname[40];
	char Nachname[40];
	int Jahrgang;
} struData;

// Struktur Person erstellen: Adrian Cerdeira
typedef struct Person {
	struData* pData;
	struct Person* pNext;
	struct Person* pPrev;
} struPerson;

//Create Funktion erstellen: Mario Forrer
struPerson* Create(const int Anzahl) {
	srand((unsigned)time(NULL));
	struPerson *pStart = NULL;
	struPerson *pCurrent = NULL;
	struPerson *pPrevious = NULL;

	for (int i = 0; i < Anzahl; i++) {
		//Befüllen der Daten
		pCurrent = (struPerson *)malloc(sizeof(struPerson));
		pCurrent->pData = (struData *)malloc(sizeof(struData));

		pCurrent->pData->Vorname[0] = 'A' + (rand() % 26);
		pCurrent->pData->Vorname[1] = '\0';
		pCurrent->pData->Nachname[0] = 'A' + (rand() % 26);
		pCurrent->pData->Nachname[1] = '\0';
		pCurrent->pData->Jahrgang = 1900 + rand() % 118;

		//Aufbauen der Kette
		if (pPrevious == NULL) {
			pStart = pCurrent;
		}
		else {
			pCurrent->pPrev = pPrevious;
			pCurrent->pPrev->pNext = pCurrent;
		}
		pPrevious = pCurrent;
		bool isLastElement = (i + 1) == Anzahl;
		if (isLastElement) {
			pCurrent->pNext = NULL;
			pStart->pPrev = NULL;
		}
	}

	return pStart;
}

//searchElement Funktion erstellen: Adrian Cerdeira
struPerson* searchElement(struPerson *pStart, char lastName[], char firstName[]) {
	// Filtern, ob das gewünschte Element in der Liste mindestens einmal vorkommt.
	struPerson* pSearch = pStart;

	while (pSearch != NULL) {
		bool isFirstNameAndLastName = pSearch->pData->Nachname[0] == lastName[0] && pSearch->pData->Vorname[0] == firstName[0];
		if (isFirstNameAndLastName) {
			return pSearch;
		}
		pSearch = pSearch->pNext;
	}
	return NULL;
}

//deleteElement Funktion erstellen: Adrian Cerdeira und Mario Forrer
struPerson* deleteElement(struPerson *pStart, struPerson *pSearchElement) {
	struPerson* pCurrent = pStart;
	struPerson* pDeleteElement = NULL;

	//Fall 1: Gewünschtes Element zum Löschen steht an der Spitze
	if (pStart == pSearchElement) {
		pDeleteElement = pStart;
		free(pDeleteElement->pData);
		pStart = pStart->pNext;
		pStart->pPrev = NULL;
		free(pDeleteElement);
		return pStart;
	}

	//Fall 2: Gewünschtes Element zum Löschen steht in der Mitte
	while (pCurrent->pNext != NULL) {
		if (pCurrent == pSearchElement) {
			pDeleteElement = pCurrent;
			free(pDeleteElement->pData);
			pCurrent = pCurrent->pPrev;
			pCurrent->pNext = pDeleteElement->pNext;
			pCurrent->pNext->pPrev = pCurrent;
			free(pDeleteElement);
			return pStart;
		}
		pCurrent = pCurrent->pNext;
	}

	//Fall 3: Gewünschtes Element zum Löschen steht am Ende
	pDeleteElement = pCurrent;
	free(pDeleteElement->pData);
	pCurrent = pCurrent->pPrev;
	pCurrent->pNext = NULL;
	free(pDeleteElement);
	return pStart;
}

//deleteElementPrep Funktion erstellen: Mario Forrer
struPerson* deleteElementPrep(struPerson *pStart) {
	struPerson *pSearchElement = NULL;
	char inputFirstName[40];
	char inputLastName[40];
	int count = 0;

	// TODO: With get_s falls möglich
	printf("Name:\n");
	scanf("%s", &inputLastName[0]);
	printf("Vorname:\n");
	scanf("%s", &inputFirstName[0]);

	// Jeglichste Elemente mit den Entsprechenden Werten werden gelöscht. Die Anzahl wird gezählt und am Ende ausgegeben.
	// Bei keinem gefundenen Element wird eine Meldung ausgegeben.
	pSearchElement = searchElement(pStart, inputLastName, inputFirstName);
	if (pSearchElement != NULL) {

		while (pSearchElement != NULL) {
			pStart = deleteElement(pStart, pSearchElement);
			count++;
			pSearchElement = searchElement(pStart, inputLastName, inputFirstName);
		}
		printf("%i Elemente wurden gel%cscht\n", count, oe);
	}
	else {
		printf("Element wurde nicht gefunden oder konnte nicht gel%cscht werden\n", oe);
	}

	return pStart;
}

// Counter-Funktion erstellen: Adrian Cerdeira
int countElements(struPerson* pStart) {
	int counter = 0;
	struPerson* pTemp = pStart;

	// Jedes Element wird durchgegangen und gezählt.
	while (pTemp != NULL) {
		counter++;
		pTemp = pTemp->pNext;
	};
	return counter;
}

// putDataToConsole Funktion erstellen: Adrian Cerdeira
void putDataToConsole(struPerson* pOut, int elementNumber) {
	struPerson *pCurrent = pOut;

	//Ausgabe eines bestimmten Elementes
	printf("\n ---- \nElement:%i\n", elementNumber);
	printf("Name: %c\n", pCurrent->pData->Nachname[0]);
	printf("Vorname: %c\n", pCurrent->pData->Vorname[0]);
	printf("Jahrgang: %i\n", pCurrent->pData->Jahrgang);
}

//Output Funktion erstellen: Mario Forrer
void Output(struPerson* pStart) {
	int  amountElements = 0;
	printf("Wie viele Elemente m%cchten Sie ausgeben? (0 = Alle)\n", oe);
	scanf("%i", &amountElements);

	//Ausgabe der Elemente. Falls die gewünschte Zahl höher ist als die Maximale anzahl, wird eine Meldung ausgegben.
	if (pStart != NULL) {
		if (amountElements != 0) {
			int existingElements = countElements(pStart);
			if (amountElements <= existingElements) {
				int i = 1;
				for (struPerson* pOut = pStart; i <= amountElements || pOut == NULL; pOut = pOut->pNext) {
					putDataToConsole(pOut, i);
					i++;
				}
			}
			else {
				printf("Es k%cnnen maximal %i Elemente ausgegeben werden\n", oe, existingElements);
			}
		}
		else {
			int i = 1;
			for (struPerson* pOut = pStart; pOut != NULL; pOut = pOut->pNext) {
				putDataToConsole(pOut, i);
				i++;
			}
		}
	}
	else {
		printf("Keine Elemente vorhanden\n");
	}

}

//Bubblesort Funktion erstellen: Adrian Cerdeira
struPerson* BubbleSort(struPerson* pStart) {
	struPerson* ipStart = NULL, *jpStart = NULL, *pSortedList = NULL;
	struData* pTemp = NULL;

	for (ipStart = pStart; ipStart->pNext != NULL; ipStart = ipStart->pNext)
	{
		for (jpStart = ipStart->pNext; jpStart != NULL; jpStart = jpStart->pNext) {
			if (ipStart->pData->Nachname[0] > jpStart->pData->Nachname[0]) {
				pTemp = ipStart->pData;
				ipStart->pData = jpStart->pData;
				jpStart->pData = pTemp;
			}
			if (ipStart->pData->Nachname[0] == jpStart->pData->Nachname[0]) {
				if (ipStart->pData->Vorname[0] > jpStart->pData->Vorname[0]) {
					pTemp = ipStart->pData;
					ipStart->pData = jpStart->pData;
					jpStart->pData = pTemp;
				}
			}
		}
	}

	printf("Liste wurde per BubbleSort sortiert\n");
	return pSortedList = pStart;
}

//START QUICKSORT
//Quicksort funktioniert NICHT, mit ausnahme von 10 Elementen. Der Code steht trotzdem
//für eine Inspektion auskommentiert im Programm. Es soll nicht unbedingt bewertet werden,
//jedoch wäre ich über eine Rückmeldung für den Fall, dass der Fehler gefunden wurde
//sehr erfreut.

////(QUICKSORT) compareEP (Compare Element Position) Funktion erstellen: Mario Forrer
//bool compareEP(struPerson* pL, struPerson* pH) {
//	struPerson* pLower = pL;
//	struPerson* pHigher = pH;
//	while (pLower != NULL) {
//		pLower = pLower->pNext;
//		if (pLower == pHigher) {
//			return true;
//		}
//	}
//	return false;
//}
//
////(QUICKSORT) compareElements Funktion erstellen: Mario Forrer
//bool compareElements(struData* fI, struData* sI) {
//	struData* first = fI;
//	struData* second = sI;
//		int compareLN = strcmp(first->Nachname, second->Nachname);
//		if (0 > compareLN) return true;
//		else if (0 == compareLN) {
//			int compareLP = strcmp(first->Vorname, second->Vorname);
//			if (0 > compareLN) return true;
//		}
//		return false;
//
//}
//
////(QUICKSORT) Partition Funktion erstellen: Mario Forrer
//struPerson* Partition(struPerson* pStart, struPerson* pLow, struPerson* pHigh) {
//	struPerson* pPivot = pLow;
//	struPerson* pLeft = pLow->pNext;
//	struPerson* pRight = pHigh;
//
//	while (compareEP(pLeft, pRight)) {
//		while (compareElements(pLeft->pData, pPivot->pData)) pLeft = pLeft->pNext;
//		while (compareElements(pPivot->pData, pRight->pData)) pRight = pRight->pPrev;
//		if (compareEP(pLeft, pRight)) {
//			struData* pTemp1 = pLeft->pData;
//			pLeft->pData = pRight->pData;
//			pRight->pData = pTemp1;
//		}
//	}
//
//		struData* pTemp2 = pRight->pData;
//		pRight->pData = pPivot->pData;
//		pPivot->pData = pTemp2;
//	
//	return pRight;
//}
//
////(QUICKSORT) QuickSort Funktion erstellen: Mario Forrer
//void QuickSort(struPerson* pStart, struPerson* pLow, struPerson* pHigh) {
//	if (compareEP(pLow, pHigh)) {
//		struPerson* pivot = Partition(pStart, pLow, pHigh);
//		QuickSort(pStart, pLow, pivot->pPrev);
//		QuickSort(pStart, pivot->pNext, pHigh);
//	}
//}
//
////(QUICKSORT) QuickSortPrep Funktion erstellen: Mario Forrer 
//struPerson* QuickSortPrep(struPerson* pStart) {
//	//	int max = countElements(pStart) - 1;
//	struPerson* pLast = pStart;
//	struPerson* pTemp = pStart;
//	while (pTemp != NULL) {
//		pLast = pTemp;
//		pTemp = pTemp->pNext;
//	};
//
//	QuickSort(pStart, pStart, pLast);
//
//	return pStart;
//}
// END QUICKSORT

// createList-Funktion erstellen: Adrian Cerdeira
struPerson* createList(struPerson *pStart) {
	int amount = 0;

	printf("Wie viele Elemente m%cchten Sie erstellen?\n", oe);
	scanf("%i", &amount);
	pStart = Create(amount);
	printf("Liste wurde erstellt\n", oe);

	// Um Buffer zu leeren
	fseek(stdin, 0, SEEK_END);

	return pStart;
}
// sortPrep Funktion erstellen: Mario Forrer
struPerson* sortPrep(struPerson *pStart) {
	char input;
	printf("Bitte w%chlen Sie Ihr gew%cnschtes Sortierverfahren aus. Verf%cgbare Sortierverfahren: Quicksort(q),Bubblesort(s)\n", ae, ue, ue);
	input = getchar();

	// Um Buffer zu leeren
	fseek(stdin, 0, SEEK_END);

	switch (input) {
	case 'q':
	case 'Q':
//		pStart = QuickSortPrep(pStart);
		printf("QuickSort ist zur Zeit nicht verwendbar.\n");
		break;
	case 's':
	case 'S':
		pStart = BubbleSort(pStart);
		break;
	default:
		printf("Ihre Eingabe ist nicht g%cltig. Bitte versuchen sie es erneut\n", ue);
		break;
	}

	return pStart;
}

// deleteList Funktion erstellen: Mario Forrer
struPerson* deleteList(struPerson *pStart) {
	struPerson* pDelete = pStart;
	pStart = NULL;

	while (pDelete->pNext != NULL) {
		pDelete = pDelete->pNext;
		free(pDelete->pPrev->pData);
		free(pDelete->pPrev);
	}

	free(pDelete->pData);
	free(pDelete);

	printf("Liste gel%cscht\n", oe);
	return pStart;
}

// Main-Funktion erstellen: Mario Forrer und Adrian Cerdeira
int main() {
	//Die Variablen in der Main Funktion wurden auf ein Minimales von 2 dezimiert. Alles andere findet in Übergangsfunktionen statt.
	//So ist die Main funktion übersichtlich und klein. Das Switch-Case bleibt in der Main Funktion, um eine einfache Übersicht zu gewährleisten.
	struPerson* pStart = NULL;
	char input;

	while (true) {
		if (pStart != NULL) {
			// Um Buffer zu leeren
			fseek(stdin, 0, SEEK_END);

			// TODO: Restliche verlangte Funktionen einbauen
			printf("Was m%cchten Sie tun?: Liste sortieren(s), Liste l%cschen(d), Element l%cschen (e), Ausgeben(a), Programm beenden(x), Console leeren(r)\n", oe, oe, oe);
			input = getchar();

			// Um Buffer zu leeren
			fseek(stdin, 0, SEEK_END);

			switch (input)
			{
			case 's':
			case 'S':
				pStart = sortPrep(pStart);
				break;
			case 'd':
			case 'D':
				pStart = deleteList(pStart);
				break;
			case 'a':
			case 'A':
				Output(pStart);
				break;
			case 'e':
			case 'E':
				pStart = deleteElementPrep(pStart);
				break;
			case 'r':
			case 'R':
				system("@cls||clear");
				break;
			case 'x':
			case 'X':
				exit(0);
				return 0;
				break;
			default:
				printf("Die Eingabe ist Ung%cltig\n", ue);
				break;
			}
		}
		else {
			pStart = createList(pStart);
		}
	}
}

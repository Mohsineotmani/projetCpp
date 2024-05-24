#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <ctime>
#include "Evenement.h"
using namespace std ;

int main() {
    int choix;
    Evenement evs;
    int end_prog = 0;
    int year = getCurrentYear();
    int month = getCurrentMonth();
    int day = 0;
    int n_of_event = 0;

    while (end_prog == 0) {
        system("cls");
        cout << endl << "Calendrier pour " << year << " : " << endl;
        for (int month = 1; month <= 12; month++) {
            print_month(year, month);
        }
        cout << "\n\nEntrez une date au format(jj mm aaaa): ";
        cin >> day >> month >> year;

        cout << endl << "Evenements pour cette date: " << endl;
        if (search_date(day, month, year) != -1) {
            printEventsForDate(day, month, year);
        } else {
            cout << endl << "Il n'y a pas d'evenements pour cette date." << endl;
        }
        choix = menu();

        switch (choix) {
            case 1: {
                int times;
                string eventes;
                cout << "Combien d'evenements voulez-vous ajouter: ";
                cin >> times;
                cin.ignore();

                for (int i = 0; i < times; i++) {
                    cout << endl << "Add event " << i + 1 << ": " << endl;
                    getline(cin, eventes);
                    evs.addEvenement(eventes);
                }
                evs.setnumEvenements(times);
                evs.writeEventsToFile(day, month, year);
                break;
            }
            case 2: {
                int index, many;
                cout << endl << "Combien d'evenements voulez-vous supprimer: ";
                cin >> many;
                for (int i = 0; i < many; i++) {
                    cout << endl << "Entrez le numéro de l'evenement que vous souhaitez supprime: ";
                    cin >> index;
                    deleteLineFromFile(search_date(day, month, year) + index + 1);
                }
                deletenumber_of_eventes(search_date(day, month, year), many);
                break;
            }
            case 3: {
                // Duplication d'événements
                int newDay, newMonth, newYear;
                cout << "Entrez la nouvelle date pour la duplication de l'evenement (jour mois annee): ";
                cin >> newDay >> newMonth >> newYear;
                evs.duplicateEvent(newDay, newMonth, newYear);
                break;
            }
            case 4: {
                end_prog = 1;
                break;
            }
        }

        if (choix != 3 && choix != 4) {
            cout << " \n \n ** Entrez 0 pour continuer\n Entrez 1 pour fermer l'application : ** " ;
            cin >> end_prog;
        }
        system("cls");
    }

    return 0;
}

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <ctime>

using namespace std;

class Evenement {
private:
    int anne, jour, mois;
    string evenements[10];
    int numEvenements;

public:
    Evenement();
    void setAtts(int a, int j, int m);
    void setnumEvenements(int nevs);
    void addEvenement(string newEvent);
    int getAnne();
    int getJour();
    int getMonth();
    int getnumEvenements();
    string* getEvenements();
    void writeEventsToFile(int jour, int mois, int anne);
    void duplicateEvent(int newJour, int newMois, int newAnne);
    ~Evenement();
};

int search_date(int jour, int mois, int anne);

Evenement::Evenement() {
    anne = 0;
    jour = 0;
    mois = 0;
    numEvenements = 0;
}

void Evenement::setAtts(int a, int j, int m) {
    anne = a;
    jour = j;
    mois = m;
}

void Evenement::setnumEvenements(int Nouveau) {
    numEvenements = Nouveau;
}

void Evenement::addEvenement(string newEvent) {
    evenements[numEvenements] = newEvent;
    numEvenements++;
}

int Evenement::getAnne() {
    return anne;
}

int Evenement::getJour() {
    return jour;
}

int Evenement::getMonth() {
    return mois;
}

int Evenement::getnumEvenements() {
    return numEvenements;
}

string* Evenement::getEvenements() {
    return evenements;
}

void Evenement::writeEventsToFile(int jour, int mois, int anne) {
    int line_number = search_date(jour, mois, anne);
    if (line_number == -1) {
        ofstream myfile;
        myfile.open("evenements.txt", ios::app);
        if (myfile.is_open()) {
            myfile << jour << " " << mois << " " << anne << "\n";
            myfile << getnumEvenements() << "\n";
            for (int i = 0; i < getnumEvenements(); i++) {
                myfile << evenements[i] << "\n";
            }
            myfile.close();
            cout << "L'evenement a ete ajoute.\n";
        } else {
            cout << "Impossible d'ouvrir le fichier.\n";
        }
    } else {
        int nmevs;
        ifstream infile("evenements.txt");
        if (!infile) {
            cout << "\nImpossible d'ouvrir le fichier.\n";
            return;
        }

        string line;
        for (int i = 1; i <= line_number + 1; i++) {
            getline(infile, line);
            if (i == line_number + 1) {
                nmevs = stoi(line);
            }
        }

        infile.close();

        int newvalue = nmevs + getnumEvenements();
        string filename = "evenements.txt";
        ofstream outfile("temp.txt");
        if (!outfile) {
            cout << "\nImpossible d'ouvrir le fichier.\n";
            return;
        }

        int currentLine = 1;
        infile.open("evenements.txt");
        while (getline(infile, line)) {
            if (currentLine == line_number + 1) {
                line = to_string(newvalue);
            }
            if (currentLine == line_number + 2 + nmevs) {
                for (int i = 0; i < getnumEvenements(); i++) {
                    outfile << evenements[i] << endl;
                }
            }
            outfile << line << endl;
            currentLine++;
        }

        infile.close();
        outfile.close();

        remove(filename.c_str());
        rename("temp.txt", filename.c_str());
    }
}

void Evenement::duplicateEvent(int newJour, int newMois, int newAnne) {
    Evenement newEvent;
    newEvent.setAtts(newAnne, newJour, newMois);
    newEvent.setnumEvenements(numEvenements);

    for (int i = 0; i < numEvenements; i++) {
        newEvent.addEvenement(evenements[i]);
    }

    newEvent.writeEventsToFile(newJour, newMois, newAnne);
}

Evenement::~Evenement() {}

int search_date(int jour, int mois, int anne) {
    ifstream file("evenements.txt");
    string line;
    int line_number = 0;
    while (getline(file, line)) {
        line_number++;
        int d, m, y;
        if (sscanf(line.c_str(), "%d %d %d", &d, &m, &y) == 3 && d == jour && m == mois && y == anne) {
            file.close();
            return line_number;
        }
    }
    file.close();
    return -1;
}

int getCurrentYear() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    return localTime->tm_year + 1900;
}

int getCurrentMonth() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    return localTime->tm_mon + 1;
}

void printLinesFromFile(int start_line, int num_lines) {
    string line;
    ifstream infile("evenements.txt");
    if (!infile) {
        	cout << "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n";
			cout << "X     Impossible d'ouvrir le fichier.     X" << endl;
			cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
        return;
    }

    int current_line_number = 1;
    while (current_line_number < start_line) {
        getline(infile, line);
        current_line_number++;
    }
    int i = 1;

    while (getline(infile, line) && num_lines > 0) {
        cout << i << "/ " << line << endl;
        i++;
        num_lines--;
    }

    infile.close();
}

void printEventsForDate(int day, int month, int year) {
    string line;
    int num_events;

    ifstream infile("evenements.txt");
    if (!infile) {
        	cout << "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n";
			cout << "X     Impossible d'ouvrir le fichier.     X" << endl;
			cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
        return;
    }

    bool date_found = false;
    int current_line_number = 0;
    while (getline(infile, line)) {
        current_line_number++;

        stringstream ss(line);
        int d, m, y;
        ss >> d >> m >> y;
        if (d == day && m == month && y == year) {
            date_found = true;

            getline(infile, line);
            num_events = stoi(line);
            break;
        }
    }

    infile.close();

    if (!date_found) {
        cout << " **Aucun evenement trouve pour la date demandee.**" << endl;
        return;
    }

    int start_line = current_line_number + 2;
    printLinesFromFile(start_line, num_events);
}

void deleteLineFromFile(int lineToDelete) {
    ifstream infile("evenements.txt");
    if (!infile) {
        cout << "X     Impossible d'ouvrir le fichier.     X" << endl;
        return;
    }

    ofstream outfile("temp.txt");
    if (!outfile) {
        cout << "Impossible de créer le fichier temporaire" << endl;
        infile.close();
        return;
    }

    string line;
    int lineNum = 0;
    while (getline(infile, line)) {
        lineNum++;
        if (lineNum != lineToDelete) {
            outfile << line << endl;
        }
    }
    cout << "L'evenement a ete supprime avec succes." << endl;

    infile.close();
    outfile.close();

    remove("evenements.txt");
    rename("temp.txt", "evenements.txt");
}

void deletenumber_of_eventes(int line_number, int many) {
    int nmevs;
    ifstream infile("evenements.txt");
    if (!infile) {
            	cout << "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n";
			cout << "X     Impossible d'ouvrir le fichier.     X" << endl;
			cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
        return;
    }

    string line;
    for (int i = 1; i <= line_number + 1; i++) {
        getline(infile, line);
        if (i == line_number + 1) {
            nmevs = stoi(line);
        }
    }

    infile.close();

    int newvalue = nmevs - many;

    string filename = "evenements.txt";
    ifstream infile2(filename);
    ofstream outfile("temp.txt");

    int current_line_number = 1;
    while (getline(infile2, line)) {
        if (current_line_number == line_number + 1) {
            outfile << newvalue << endl;
        } else {
            outfile << line << endl;
        }
        current_line_number++;
    }

    infile2.close();
    outfile.close();

    remove("evenements.txt");
    rename("temp.txt", "evenements.txt");
}

int DAYS_IN_MONTH[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

const string MONTH_NAMES[] = { "January", "February", "March", "April", "May", "June",
                            "July", "August", "September", "October", "November", "December" };
const string DAY_NAMES[] = { "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };

int leap(int year) {
    if (year % 400 == 0) return 1;
    if (year % 100 == 0) return 0;
    if (year % 4 == 0) return 1;
    return 0;
}

int leap2(int year, int month) {
    if (month == 2) return leap(year);
    return 0;
}

int days(int year, int month) {
    return DAYS_IN_MONTH[month - 1] + leap2(year, month);
}

int start_day(int year, int month) {
    if (month < 3) {
        month += 12;
        year--;
    }
    return (1 + 2 * month + (3 * (month + 1)) / 5 + year + (year / 4) - (year / 100) + (year / 400) + 1) % 7;
}

void print_month(int year, int month) {
    printf("       %s %d\n", MONTH_NAMES[month - 1].c_str(), year);
    for (int i = 0; i < 7; i++)
        printf(" %s", DAY_NAMES[i].c_str());
    printf("\n");

    int days_in_month = days(year, month);
    int current = start_day(year, month);

    for (int i = 0; i < current; i++) printf("   ");

    for (int i = 1; i <= days_in_month; i++) {
        printf(" %2d", i);
        if (++current > 6) {
            current = 0;
            printf("\n");
        }
    }
    if (current) printf("\n");
}
//////////////////////////////////


int menu() {
    int choice;
    cout << "\n\n";
    cout << "Please select a choice:" << endl;
    cout << " 1- Ajouter un evenement au calendrier." << endl;
    cout << " 2- Supprimer un evenement au calendrier." << endl;
    cout << " 3- Duplication d'un evenement." << endl;
    cout << " 4- Exit" << endl;
    cout << " 5- Continuer" << endl;

    cout << "Choice: ";
    cin >> choice;
    return choice;
}

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

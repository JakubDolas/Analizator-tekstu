#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <set>

using namespace std;

// Funkcja wczytujaca plik
void wczytaj_plik(string& tekst) {
    string nazwa_pliku;
    ifstream plik;

    while (true) {
        cout << "Podaj nazwe pliku: ";
        cin >> nazwa_pliku;

        plik.open(nazwa_pliku);

        if (plik.is_open()) {
            break;
        } else {
            cout << "Nie udalo sie otworzyc pliku. Sproboj ponownie." << endl;
            plik.clear();
        }
    }

    string linia;
    while (getline(plik, linia)) {
        tekst += linia + '\n';
    }

    plik.close();
}

// Funkcja ktora wyswietla plik
void wyswietl_plik(string& tekst){
    cout << tekst;
}

// Wyszukiwanie wzorca
void wyszukajKMP(string& tekst, string& wzorzec) {
    int dlugosc_tekstu = tekst.size();
    int dlugosc_wzorca = wzorzec.size();

    int* tab = new int[dlugosc_wzorca];

    int i = 0; // Indeks dla tekstu
    int j = 0; // Indeks dla wzorca
    int ilosc_wystapien = 0;
    int numer_linii = 1; 
    int numer_znaku_w_linii = 0; 

    while (i < dlugosc_tekstu) {
        if (wzorzec[j] == tekst[i] || wzorzec[j] == ' ') {
            j++;
            i++;
            numer_znaku_w_linii++;
        }
        if (j == dlugosc_wzorca) {
            ilosc_wystapien++;
            cout << "Znaleziono w linii " << numer_linii << ", znak " << numer_znaku_w_linii - j + 1
                 << ": " << tekst.substr(i - j, j) << endl;
            j = tab[j - 1];
        } else if (i < dlugosc_tekstu && (wzorzec[j] != tekst[i] && wzorzec[j] != ' ')) {
            if (j != 0) {
                j = tab[j - 1];
            } else {
                i++;
                numer_znaku_w_linii++;
            }
        }

        if (i < dlugosc_tekstu && tekst[i] == '\n') {
            numer_linii++;
            numer_znaku_w_linii = 0; 
            i++; 
        }
    }
    cout << endl;
    cout << "Ilosc wystapien frazy ' " << wzorzec << " ' : " << ilosc_wystapien << endl;

    delete[] tab;
}


// Funkcja obliczająca długość najdłuższego wspólnego podciągu
int lcsLength(const string& str1, const string& str2) {
    int dlugosc1 = str1.length();
    int dlugosc2 = str2.length();

    int** dp = new int*[dlugosc1 + 1];
    for (int i = 0; i <= dlugosc1; ++i) {
        dp[i] = new int[dlugosc2 + 1];
    }

    for (int i = 0; i <= dlugosc1; ++i) {
        for (int j = 0; j <= dlugosc2; ++j) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    int result = dp[dlugosc1][dlugosc2];
    
    for (int i = 0; i <= dlugosc1; ++i) {
        delete[] dp[i];
    }
    delete[] dp;

    return result;
}

// Funkcja znajdująca najbardziej podobne słowo do zadanego wzorca
string znajdzNajbardziejPodobneSlowo(const string& wzorzec, const string* slowa, int ilosc_slow) {
    int maxPodobienstwo = 0;
    string najbardziejPodobneSlowo;

    for (int i = 0; i < ilosc_slow; ++i) {
        int dlugoscLCS = lcsLength(wzorzec, slowa[i]);

        if (dlugoscLCS > maxPodobienstwo) {
            maxPodobienstwo = dlugoscLCS;
            najbardziejPodobneSlowo = slowa[i];
        }
    }

    return najbardziejPodobneSlowo;
}

// Funkcja obsługująca wyszukiwanie frazy w tekście przy użyciu słownika
void Slownik(string& tekst, const string& wzorzec) {
    string slowo2 = wzorzec;
    const int MAX_SLOW = 1000; 
    string* slowa = new string[MAX_SLOW];
    int ilosc_slow = 0;
    int dlugosc_tekstu = tekst.size();

    string slowo;
    for (int i = 0; i < dlugosc_tekstu; ++i) {
        char znak = tekst[i];
        if ((znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak <= 'Z') || (znak >= '0' && znak <= '9')) {
            slowo += tolower(znak);
        } else if (slowo.size() > 0) {
            slowa[ilosc_slow++] = slowo;
            slowo = "";

            if (ilosc_slow == MAX_SLOW) {
                //cout << "Osiagnieto maksymalna liczbe slow" << endl;
                break;
            }
        }
    }

    string najbardziejPodobne = znajdzNajbardziejPodobneSlowo(wzorzec, slowa, ilosc_slow);

    
    if (slowo.size() > 0) {
        slowa[ilosc_slow++] = slowo;
    }

    if (ilosc_slow > 0) {
        cout << "Czy chodzilo Ci o: " << najbardziejPodobne << "? (T/N): ";
        char odpowiedz;
        cin >> odpowiedz;

        if (odpowiedz == 'T' || odpowiedz == 't') {
            wyszukajKMP(tekst, najbardziejPodobne);
        } else {
            wyszukajKMP(tekst, slowo2);
        }
    
        //Badanie roznicy ilosci znakow
        int dlugosc;
        if(najbardziejPodobne.length() > wzorzec.length()){
            dlugosc = najbardziejPodobne.length();
        } else {
            dlugosc = wzorzec.length();
        }
        int licznik = 0;
        for (int i = 0; i < dlugosc; i++) {
            if (najbardziejPodobne[i] != wzorzec[i]) {
                licznik++;
            }
        }
            cout << "Roznica pomiedzy slowem: " << wzorzec << ", a " << najbardziejPodobne << " to: " << licznik << endl;
    } else {
        cout << "Nie znaleziono podobnych słów." << endl;
    }

    delete[] slowa;
}


// Funkcja obliczająca statystyki tekstu
void obliczanie_statystyk(string& tekst) {
    const int max_rozmiar_slowa = 50;
    char slowo[max_rozmiar_slowa];
    int dlugosc_tekstu = tekst.size();
    int indeks_slowa = 0;

    int max_czestosc = 0;
    string najczestsze_slowo;

    int min_czestosc = INT_MAX;  
    string najrzadsze_slowo = "";

    set<string>unikalne_slowa;  // zbiór do przechowywania unikalnych słów

    for (int i = 0; i < dlugosc_tekstu; ++i) {
        char znak = tekst[i];

        if ((znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak <= 'Z') || (znak >= '0' && znak <= '9')) {
            slowo[indeks_slowa++] = tolower(znak);
        } else if (indeks_slowa > 0) {
            slowo[indeks_slowa] = '\0';
            string current_word = slowo;

            // Dodaj słowo do zbioru unikalnych słów
            if (unikalne_slowa.find(current_word) == unikalne_slowa.end()) {
                unikalne_slowa.insert(current_word);

                int count = 0;
                for (int j = 0; j < dlugosc_tekstu; ++j) {
                    char znak2 = tekst[j];

                    if ((znak2 >= 'a' && znak2 <= 'z') || (znak2 >= 'A' && znak2 <= 'Z') || (znak2 >= '0' && znak2 <= '9')) {
                        char slowo2[max_rozmiar_slowa];
                        int indeks_slowa2 = 0;

                        while ((znak2 >= 'a' && znak2 <= 'z') || (znak2 >= 'A' && znak2 <= 'Z') || (znak2 >= '0' && znak2 <= '9')) {
                            slowo2[indeks_slowa2++] = tolower(znak2);
                            znak2 = tekst[++j];
                        }

                        slowo2[indeks_slowa2] = '\0';

                        if (strcmp(slowo, slowo2) == 0) {
                            count++;
                        }
                    }
                }

                if (count > max_czestosc) {
                    max_czestosc = count;
                    najczestsze_slowo = current_word;
                } else if(count == max_czestosc){
                    najczestsze_slowo += ", " + current_word;
                }

                if (count < min_czestosc) {
                    min_czestosc = count;
                    najrzadsze_slowo = current_word;
                } else if (count == min_czestosc) {
                    najrzadsze_slowo += ", " + current_word;
                }
            }

            indeks_slowa = 0;
        }
    }

    if (najczestsze_slowo.length() > 0) {
        cout << "Najczesciej wystepujace slowo: " << najczestsze_slowo << " (wystapilo " << max_czestosc << " razy)" << endl;
    } else {
        cout << "Brak najczesztszych slow" << endl;
    }
    if (najrzadsze_slowo.length() > 0) {
        cout << "Najrzadziej wystepujace slowo: " << najrzadsze_slowo << " (wystapilo " << min_czestosc << " razy)" << endl;
    } else {
        cout << "Brak najrzadszych slow" << endl;
    }
}

// Funkcja sprawdzająca ilość wystąpień danego znaku w tekście
void iloscWystapienZnaku(string tekst, char znak){
    int dlugosc_tekstu = tekst.size();
    int count = 0;

    for(int i = 0; i < dlugosc_tekstu; i++){
        char znak2 = tekst[i];
        if(znak == znak2){
            count++;
        }
    }
    cout << "Znak ' " << znak << " ' wystepuje " << count << " razy w tekscie" << endl;
}

int main() {
    string tekst;
    wczytaj_plik(tekst);

    int opcja;
    while(1){
        cout << endl;
        cout << "1 ---> Wyszukaj fraze" << endl;
        cout << "2 ---> Obliczanie statystyk tekstu" << endl;
        cout << "3 ---> Wyswietl tekst" << endl;
        cout << "4 ---> Sprawdz ile razy dany znak wystepuje w tekscie" << endl;
        cout << "5 ---> Zakoncz dzialanie programu" << endl << endl;
        cout << "Twoj wybor: ";
        cin >> opcja;
        cout << endl;

        if (opcja == 1) {
            string wzorzec;
            cout << "Podaj wzorzec: ";
            cin.ignore();  // Ignorowanie ewentualnych znaków w buforze
            getline(cin, wzorzec);
            Slownik(tekst, wzorzec);
        } else if (opcja == 2) {
            obliczanie_statystyk(tekst);
        } else if (opcja == 3){
            wyswietl_plik(tekst);
        } else if (opcja == 4){
            char znak;
            cout << "Podaj znak: ";
            cin >> znak;
            iloscWystapienZnaku(tekst, znak);
        }else if (opcja == 5) {
            cout << "Zakonczono program" << endl;
            exit(1);
        } else {
            cout << "Niepoprawny znak" << endl;
        }
    }

    return 0;
}

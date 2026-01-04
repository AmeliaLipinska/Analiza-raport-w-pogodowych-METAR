#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>

using namespace std;

void Powitanie()
{
    cout << "==============================" << endl;
    cout << "Witaj w programie odczytujacym METAR" << endl;
    cout << "Autor: Amelia Lipinska" << endl;
    cout << "Indeks: 203721" << endl;
    cout << "==============================" << endl;
}

void pokazMenu() {
    cout << "Menu programu:" << endl;
    cout << "o - Odczytaj dane z pliku i zapisz do innego pliku" << endl;
    cout << "q - Wyjdź z programu" << endl;
    cout << "Wybierz opcje: ";
}

struct Zjawisko {
    const char* skrot;
    const char* opis;
};

const Zjawisko zjawiska[] = {
    {" -", "lekki"},
    {" +", "silny"},
    {" VC", "w poblizu"},
    {" MI", "niska"},
    {" BC","platy"},
    {"CAVOK", "brak"},
    {"DR", "niska zamiec,"},
    {"BL", "zawieja,"},
    {"SH", "opady przelotne,"},
    {"TS", "burza,"},
    {"SG", "snieg ziarnisty,"},
    {"IC", "słupki lodowe,"},
    {"PE", "deszcz lodowy,"},
    {"GR", "grad,"},
    {"DU", "pyl,"},
    {"SA", "piasek,"},
    {"RA", "deszcz,"},
    {"SN", "śnieg,"},
    {"BR", "zamglenie,"},
    {"FG", "mgła,"},
    {"TS", "burza"},
    {"DZ", "mżawka,"},
    {"HZ", "zmętnienie,"},
    {"SQ", "nawalnica,"},
    {"FC", "tornado,"},
    {"FU", "dymy,"},

};

const int liczbaZjawisk = 25;

void przetworzZjawiska(const char* linia, char* wynik) {
    int dlugosc = 0;
    while (linia[dlugosc] != '\0') dlugosc++;

    wynik[0] = '\0';
    bool pierwszeZjaw = true;

    for (int j = 0; j < liczbaZjawisk; j++) {
        const char* skrot = zjawiska[j].skrot;
        int s = 0;

        for (int i = 0; i < dlugosc; i++) {
            s = 0;
            while (skrot[s] != '\0' && linia[i + s] == skrot[s]) {
                s++;
            }

            if (skrot[s] == '\0') {
                if (!pierwszeZjaw) {
                    int n = 0;
                    while (wynik[n] != '\0') n++;
                    wynik[n++] = ' ';
                    wynik[n] = '\0';
                }

                const char* opis = zjawiska[j].opis;
                int k = 0, l = 0;
                while (wynik[k] != '\0') k++;
                while (opis[l] != '\0') {
                    wynik[k++] = opis[l++];
                }
                wynik[k] = '\0';

                pierwszeZjaw = false;
                break;
            }
        }
    }

}

struct Zachmurzenie {
    const char* kod;
    const char* opis;
};

const Zachmurzenie zachmurzenia[] = {
    {"SKC", "bezchmurnie"},
    {"CLR", "czyste niebo"},
    {"FEW", "male zachmurzenie (1-2/8)"},
    {"SCT", "cześciowe zachmurzenie (3-4/8)"},
    {"BKN", "duze zachmurzenie (5-7/8)"},
    {"OVC", "calkowite zachmurzenie (8/8)"}
};


void odczytajZachmurzenie(const char linia[], int dlugosc, char wynikiZachmurzenia[][4], char wynikiPodstawChmur[][4], int& liczbaZachmurzen) {
    const char* kodyZachmurzenia[] = { "SCT", "FEW", "BKN", "OVC", "SKC" };
    int liczbaKodow = 5;

    liczbaZachmurzen = 0; 

    for (int i = 0; i < dlugosc - 6; i++) {
        for (int j = 0; j < liczbaKodow; j++) {
            if (linia[i] == kodyZachmurzenia[j][0] && linia[i + 1] == kodyZachmurzenia[j][1] && linia[i + 2] == kodyZachmurzenia[j][2]) {
                
                for (int k = 0; k < 3; k++) {
                    wynikiZachmurzenia[liczbaZachmurzen][k] = linia[i + k];
                }
                wynikiZachmurzenia[liczbaZachmurzen][3] = '\0'; 

                
                wynikiPodstawChmur[liczbaZachmurzen][0] = linia[i + 3];
                wynikiPodstawChmur[liczbaZachmurzen][1] = linia[i + 4];
                wynikiPodstawChmur[liczbaZachmurzen][2] = linia[i + 5];
                wynikiPodstawChmur[liczbaZachmurzen][3] = '\0';

                liczbaZachmurzen++; 
                break;
            }
        }
    }
}


const char* rozszyfrujZachmurzenie(const char* kod) {
    for (int i = 0; i < sizeof(zachmurzenia) / sizeof(Zachmurzenie); i++) {
        if (strcmp(kod, zachmurzenia[i].kod) == 0) {
            return zachmurzenia[i].opis;
        }
    }
    return "";
}


void odczytajDanePlikuZapisz()
{

    fstream wejsciowyPlik("Metar_Gdansk.txt", ios::in);

    if (wejsciowyPlik.good())
    {
        cout << endl;
        cout << "Uzyskano dostep do pliku!" << endl;

        ofstream wyjsciowyPlik("pogoda_gd.txt");

        if (!wyjsciowyPlik) {
            cout << "Blad: Nie mozna otworzyc pliku pogoda_gd.txt do zapisu." << endl;
            return;
        }

        cout << endl;
        cout << "Dane pogodowe z pliku Metar_Gdansk.txt:" << endl;

        char linia[256];
        int licznikWywietlonych = 0;

        while (wejsciowyPlik.getline(linia, 256))
        {
            int dlugosc = 0;
            while (linia[dlugosc] != '\0' && dlugosc < 256) {
                dlugosc++;
            }

            char dzien[4] = { 0 }, czas[6] = { 0 }, kierunek[4] = { 0 }, predkosc[4] = { 0 };
            char temperatura[7] = { 0 }, cisnienie[8] = { 0 };
            char widocznosc[7] = { 0 };

            for (int i = 0; i < 2; i++) dzien[i] = linia[5 + i];
            for (int i = 0; i < 4; i++) czas[i] = linia[7 + i];
            for (int i = 0; i < 3; i++) kierunek[i] = linia[13 + i];
            for (int i = 0; i < 2; i++) predkosc[i] = linia[16 + i];

            char zakresKierunku[7] = { 0 };
            bool zmiennyWiatr = false;
            int indexZakresu = -1;
            int kierunek1 = -1, kierunek2 = -1;

            for (int i = 0; i < dlugosc - 6; i++) {
               
                if (isdigit(linia[i]) && isdigit(linia[i + 1]) && isdigit(linia[i + 2]) && linia[i + 3] == 'V' && isdigit(linia[i + 4]) && isdigit(linia[i + 5])) {
                    zmiennyWiatr = true;
                    
                    for (int j = 0; j < 7; j++) {
                        zakresKierunku[j] = linia[i + j];
                    }

                    kierunek1 = (zakresKierunku[0] - '0') * 100 + (zakresKierunku[1] - '0') * 10 + (zakresKierunku[2] - '0');
                    kierunek2 = (zakresKierunku[4] - '0') * 100 + (zakresKierunku[5] - '0') * 10 + (zakresKierunku[6] - '0');
                    break;
                }

            }

            bool znalezionoWidocznosc = false;
            for (int i = 0; i < dlugosc - 5; i++) {
               
                if (linia[i] == 'C' && linia[i + 1] == 'A' && linia[i + 2] == 'V' && linia[i + 3] == 'O' && linia[i + 4] == 'K') {
                    
                    widocznosc[0] = '>';
                    widocznosc[1] = '=';
                    widocznosc[2] = '1';
                    widocznosc[3] = '0';
                    widocznosc[4] = '\0'; 
                    znalezionoWidocznosc = true;
                    break; 
                }

               
                if (linia[i] == ' ' && isdigit(linia[i + 1]) && isdigit(linia[i + 2]) && isdigit(linia[i + 3]) && isdigit(linia[i + 4]) && linia[i + 5] == ' ') {
                    
                    for (int j = 0; j < 4; j++) {
                        widocznosc[j] = linia[i + 1 + j];
                    }
                    widocznosc[4] = '\0'; 
                    znalezionoWidocznosc = true;
                    break;
                }
            }

            char zjawiskaWynik[256];
            przetworzZjawiska(linia, zjawiskaWynik);

            char zachmurzenia[10][4] = { 0 };
            char podstawyChmur[10][4] = { 0 };
            char zachmurzenie[4] = { 0 };
            char podstawaChmur[4] = { 0 };
            int liczbaZachmurzen = 0;

            odczytajZachmurzenie(linia, dlugosc, zachmurzenia, podstawyChmur, liczbaZachmurzen);

            const char* opisZachmurzenia = rozszyfrujZachmurzenie(zachmurzenie);

            int indexTemp = -1; 
            int indexRos = -1;

            for (int i = 0; i < dlugosc; i++) {
                if (linia[i] == '/') { 
                    indexTemp = i - 3;
                    indexRos = i + 1;
                    break;
                }
            }
            if (indexTemp >= 0)
            {
                for (int i = 0; i < 3; i++) temperatura[i] = linia[indexTemp + i];
            }
            if (indexRos >= 0)
            {
                for (int i = 0; i < 3; i++) temperatura[i + 3] = linia[indexRos + i];
            }

            int indexCis = -1;

            for (int i = dlugosc; i > 0; i--)
            {
                if (linia[i] == 'Q')
                {
                    indexCis = i + 1; 
                }
            }
            if (indexCis >= 0)
            {
                for (int i = 0; i < 4; i++) cisnienie[i] = linia[indexCis + i];
            }

            wyjsciowyPlik << "Dzien wykonania pomiaru:" << dzien << endl;
            wyjsciowyPlik << "Czas UTC: " << czas[0] << czas[1] << ":" << czas[2] << czas[3] << endl;
            if (kierunek[0] == 'V' && kierunek[1] == 'R' && kierunek[2] == 'B') {
                wyjsciowyPlik << "Wiatr: zmienny, predkosc " << predkosc << " wezlow" << endl;
            }
            else {
                wyjsciowyPlik << "Wiatr: z kierunku " << kierunek << " stopni, predkosc " << predkosc << " wezlow" << endl;
            }

            if (linia[18] == 'G') {
                char poryw[3] = { 0 };
                poryw[0] = linia[19];
                poryw[1] = linia[20];
                wyjsciowyPlik << " w porywach do " << poryw << " wezlow" << endl;
            }
            if (zmiennyWiatr) {
                wyjsciowyPlik << "Wiatr zmienny: skrajne wartości kierunku wiatru od " << kierunek1 << " do " << kierunek2 << " " << endl;
            }
            if (znalezionoWidocznosc) {
                wyjsciowyPlik << "Widocznosc: " << widocznosc << " metrow" << endl;
            }

            wyjsciowyPlik << "Zjawiska pogodowe: " << zjawiskaWynik << endl;


            for (int i = 0; i < liczbaZachmurzen; i++) {
                const char* opisZachmurzenia = rozszyfrujZachmurzenie(zachmurzenia[i]);
                wyjsciowyPlik << "Zachmurzenie: " << opisZachmurzenia << endl;

                if (podstawyChmur[i][0] != '\0') {
                    wyjsciowyPlik << "Wysokosc podstawy chmur: " << podstawyChmur[i] << "00 stop" << endl;
                }
                else {
                    wyjsciowyPlik << endl;
                }
            }

            if (temperatura[0] == 'M') {
                wyjsciowyPlik << "Temperatura: -" << temperatura[1] << temperatura[2] << " stopni C" << endl;
            }
            else
            {
                wyjsciowyPlik << "Temperatura: " << temperatura[1] << temperatura[2] << " stopni C" << endl;
            }
            if (temperatura[3] == 'M') {
                wyjsciowyPlik << "Temperatura punktu rosy: -" << temperatura[4] << temperatura[5] << " stopni C" << endl;
            }
            else
            {
                wyjsciowyPlik << "Temperatura punktu rosy: " << temperatura[3] << temperatura[4] << " stopni C" << endl;
            }
            wyjsciowyPlik << "Cisnienie atmosferyczne: " << cisnienie << " hPa" << endl;
            wyjsciowyPlik << endl;

            if (licznikWywietlonych < 3) {

                cout << "Dzien wykonania pomiaru:" << dzien << endl;
                cout << "Czas UTC: " << czas[0] << czas[1] << ":" << czas[2] << czas[3] << endl;
                if (kierunek[0] == 'V' && kierunek[1] == 'R' && kierunek[2] == 'B') {
                    cout << "Wiatr: zmienny, predkosc " << predkosc << " wezlow" << endl;
                }
                else {
                    cout << "Wiatr: z kierunku " << kierunek << " stopni, predkosc " << predkosc << " wezlow" << endl;
                }

                if (linia[18] == 'G') {
                    char poryw[3] = { 0 };
                    poryw[0] = linia[19];
                    poryw[1] = linia[20];
                    cout << " w porywach do " << poryw << " wezlow" << endl;
                }
                if (zmiennyWiatr) {
                    cout << "Wiatr zmienny: skrajne wartości kierunku wiatru od " << kierunek1 << " do " << kierunek2 << " " << endl;
                }
                if (znalezionoWidocznosc) {
                    cout << "Widocznosc: " << widocznosc << " metrow" << endl;
                }

                cout << "Zjawiska pogodowe: " << zjawiskaWynik << endl;

                for (int i = 0; i < liczbaZachmurzen; i++) {
                    const char* opisZachmurzenia = rozszyfrujZachmurzenie(zachmurzenia[i]);

                    cout << "Zachmurzenie: " << opisZachmurzenia << endl;
                    if (podstawyChmur[i][0] != '\0') {
                        cout << "Wysokosc podstawy chmur: " << podstawyChmur[i] << "00 stop" << endl;
                    }
                }


                if (temperatura[0] == 'M') {
                    cout << "Temperatura: -" << temperatura[1] << temperatura[2] << " stopni C" << endl;
                }
                else
                {
                    cout << "Temperatura: " << temperatura[1] << temperatura[2] << " stopni C" << endl;
                }
                if (temperatura[3] == 'M') {
                    cout << "Temperatura punktu rosy: -" << temperatura[4] << temperatura[5] << " stopni C" << endl;
                }
                else
                {
                    cout << "Temperatura punktu rosy: " << temperatura[3] << temperatura[4] << " stopni C" << endl;
                }
                cout << "Cisnienie atmosferyczne: " << cisnienie << " hPa" << endl;
                cout << endl;

                licznikWywietlonych++;
            }

        }

        cout << endl;
        cout << "Dane zostaly zapisane do pliku pogoda_gd.txt" << endl;

        wyjsciowyPlik.close();

    }
    else
    {
        cout << "Dostep do pliku zostal zabroniony!" << endl;
    }

    wejsciowyPlik.close();
}

int main()
{
    Powitanie();

    char wybor_menu;
    do {
        pokazMenu();
        cin >> wybor_menu;

        switch (wybor_menu)
        {
        case 'o':
        {
            odczytajDanePlikuZapisz();
            break;
        }
        case 'q':
        {
            cout << "Zakonczyles program." << endl;
            break;
        }
        default:
        {
            cout << "Nieprawidlowa opcja. Sprobuj ponownie." << endl;
        }
        }
    } while (wybor_menu != 'q');
}

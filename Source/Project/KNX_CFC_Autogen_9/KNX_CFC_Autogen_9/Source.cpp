#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <chrono>
#include "mdhl_Tools.h"
#include "mdhl_Knx_v9.h"




using namespace std;


int const iMax = 1000;
string sFilPath, sPath2;      //Adresse til mappe hvor exe fil er lagret
string sSource;     //Variabel som velger filnavn p� fil som skal leses
string sInput[iMax];        //Input verdier, hver linje i hver variabel i arrayet
bool bUsed[iMax];       //Viser om Inputen faktisk er brukt, eller om det er tomt. For � ung� � ta inn tomme linjer
string sInfo = "STLV80_563\tSTLV80_A_563";
string sGVL = "STLV80_563"; //�nsket navn p� GVL
string sAdresseFormat = "STLV80_A_563";
string sRom[iMax];      //Navn p� rom
string sRomtype563[iMax];     //Romtype brukt til romstyring
string sRomtypeKnx[iMax];     //Romtype brukt i KNX
string sKommentar[iMax];      //Adressen til VAV-en


void OpenFile()
{
    //sSource = "1.txt";        //Variabel som velger filnavn p� fil som skal leses
    cout << "Filnavn: ";      //Sp�r etter filnavn
    cin >> sSource;       //Tar inn filnavn og lagrer p� sSource

    sFilPath = (sPath2 + sSource);       //Den fulle adressen til filen som skal leses fra
    cout << sFilPath << "\n";                   //test



    ifstream fInput(sFilPath + ".txt");      //�pner fil som skal leses fra
    //fInput.open(sFilPath);        //test
    if (!fInput.is_open())       //Sjekker om filen ble �pnet
    {
        cout << "Cant open file\n\tHusk .txt, Ikke run program i 'rare' mapper (mtp. adresse)\n";     //Melder ifra om feil hvis fil ikke ble �pnet
        //Sleep(10000);
        OpenFile();
        return;
    }

    cout << "File found\n";

    fInput.close();     //Lukker fil som vi leste fra
}
void Read() //Sp�r etter navn p� fil som skal leses, og leser deretter inn hver linje og lagrer dem i egne variabler.
{
    ifstream fInput(sFilPath + ".txt");      //�pner fil som skal leses fra
    /*cout << "Skriv inn GVL navn\n\teksempel: STLV80_563\n\tGVL navn: ";      //Sp�r etter GVL navn eks. STLV80_360
    cin >> sGVL;       //Tar inn GVL Navn
    cout << "Skriv inn adresseformat\n\teksempel: STLV80_A_563_[Rom]\t IKKE ta med Rom\n\tAdresseformat: ";      //Sp�r etter adresse format eks. STLV80_563.STLV80_A_563_
    cin >> sAdresseFormat;       //Tar inn adresseformat */
    //Sleep(2000);

    getline(fInput, sInfo);     //Leser inn f�rste linje hvor info skal v�re


    for (int i = 0; i < iMax; i++)        //G�r gjennom hver variabel i arrayet
    {
        int j = (i - 1);        //Variabel for � sjekke om forgje linje er lik neste (is�fall avsluttes lese delen av programmet)
        getline(fInput, sInput[i]);     //Leser inn en linje og lagrer den i sInput[for loop var]
        if (sInput[i] == sInput[j] || sInput[i] == "")      //Sjekker om linje er lik den forgje, bryter is�fall ut av loopen og avslutter lese delen av programmet
        {
            bUsed[i] = false;
            break;
        }
        else
            bUsed[i] = true;        //markerer Input som brukt (for � ung� � prossesere og skrive ut mer verdier enn vi trenger)
    }
    fInput.close();     //Lukker fil som vi leste fra
}

void Prossesering() //Bruker Input variablene og deler dem opp i sepparate variabler for ventil navn og serie nr. Bytter ogs� + til -
{
    size_t pos;
    int iSize = 0;

    for (int i = 0; i < 2; i++)
    {
        while (sInfo.substr(0, 1) == " " || sInfo.substr(0, 1) == "\t")        //Fjerner evt. mellomrom/tabs som er f�r verdiene
        {
            iSize = ((sInfo.size()) - 1);
            sInfo = (sInfo.substr(1, iSize));
        }

        if (sInfo.find(" ") < sInfo.find("\t"))
            pos = sInfo.find(" ");
        else
            pos = sInfo.find("\t");
        if (i == 0)
        {
            iSize = ((sInfo.size()) - pos - 1);     //Bestemmer st�relsen p� stringet vi lagrer verdien i (posisjon p� understrek - 1(understreken))
            sGVL = (sInfo.substr(0, pos));       //Lagrer verdi i Tilh�rende variabel
            sInfo = sInfo.substr(pos + 1, iSize);       //Fjerner verdien fra input variabelen
        }
        else
        {
            iSize = ((sInfo.size()) - pos - 1);     //Bestemmer st�relsen p� stringet vi lagrer verdien i (posisjon p� understrek - 1(understreken))
            sAdresseFormat = (sInfo.substr(0, pos));       //Lagrer verdi i Tilh�rende variabel
            sInfo = sInfo.substr(pos + 1, iSize);       //Fjerner verdien fra input variabelen
        }
    }


    for (int i = 0; i < iMax; i++)      //G�r igjennom alle variabler
    {
        if (bUsed[i])       //Bryter ut av loop hvis variabelen ikke er bruke
        {
            iSize = 0;


            for (int j = 0; j < 4; j++)      //G�r gjennom og deler opp etter Rom, kommentar, romtype
            {
                while (sInput[i].substr(0, 1) == "\t")        //Fjerner evt. mellomrom/tabs som er f�r verdiene
                {
                    iSize = ((sInput[i].size()) - 1);
                    sInput[i] = (sInput[i].substr(1, iSize));
                }


                pos = sInput[i].find("\t");

                if (pos < 1)        //Melder om feil og avbryter hvis mellomrom(f�r verdier) ikke ble fjernet
                {
                    cout << "\n\nError: line 128.\n\n";
                    abort();
                }
                else
                {
                    iSize = ((sInput[i].size()) - pos - 1);     //Bestemmer st�relsen p� stringet vi lagrer verdien i (posisjon p� mellomrom - 1(mellomromet))
                    switch (j)
                    {
                    case 0:     //F�rste verdi (Navn)

                        //pos = sInput[i].find("\t");     //Finner neste understrek posisjonen i stringet hvor verdien vi leser inn ender
                        iSize = ((sInput[i].size()) - pos - 1);     //Bestemmer st�relsen p� stringet vi lagrer verdien i (posisjon p� understrek - 1(understreken))
                        sRom[i] = (sInput[i].substr(0, pos));       //Lagrer verdi i Tilh�rende variabel
                        sInput[i] = sInput[i].substr(pos + 1, iSize);       //Fjerner verdien fra input variabelen
                        break;

                    case 1:     //Andre verdi (Romtype)

                        //pos = sInput[i].find("\t");     //Finner neste understrek posisjonen i stringet hvor verdien vi leser inn ender
                        iSize = ((sInput[i].size()) - pos - 1);     //Bestemmer st�relsen p� stringet vi lagrer verdien i (posisjon p� understrek - 1(understreken))
                        sRomtype563[i] = (sInput[i].substr(0, pos));       //Lagrer verdi i Tilh�rende variabel
                        sInput[i] = sInput[i].substr(pos + 1, iSize);       //Fjerner verdien fra input variabelen
                        break;

                    case 2:     //Andre verdi (Romtype) KNX

                        //pos = sInput[i].find("\t");     //Finner neste understrek posisjonen i stringet hvor verdien vi leser inn ender
                        iSize = ((sInput[i].size()) - pos - 1);     //Bestemmer st�relsen p� stringet vi lagrer verdien i (posisjon p� understrek - 1(understreken))
                        sRomtypeKnx[i] = (sInput[i].substr(0, pos));       //Lagrer verdi i Tilh�rende variabel
                        sInput[i] = sInput[i].substr(pos + 1, iSize);       //Fjerner verdien fra input variabelen
                        break;

                    case 3:     //Tredje verdi (Kommentar)

                        //pos = sInput[i].find("\t");     //Finner neste understrek posisjonen i stringet hvor verdien vi leser inn ender
                        iSize = ((sInput[i].size()) - pos - 1);     //Bestemmer st�relsen p� stringet vi lagrer verdien i (posisjon p� understrek - 1(understreken))
                        sKommentar[i] = (sInput[i].substr(0, pos));       //Lagrer verdi i Tilh�rende variabel
                        sInput[i] = sInput[i].substr(pos + 1, iSize);       //Fjerner verdien fra input variabelen
                        break;

                    default:
                        std::cout << "Error Rom " << sRom[i] << " Not used";
                        break;
                    }
                }

            }

            //Skriver ut navn i console slik at du kan se feil f�r du g�r til txt fil
            cout << "\n\t" << sRom[i] << "\t" << sRomtype563[i] << "\t" << sRomtypeKnx[i] << "\t" << sKommentar[i] << "\n\n";
        }
        else
            break;
    }
}


int main() //Starter med � lokalisere seg selv, og f�r derfor sin egen plassering som brukes i Read og Write.
{
    WCHAR wcPath[MAX_PATH];     //Lager Path variabel
    GetModuleFileNameW(NULL, wcPath, MAX_PATH);     //Lagrer sin egen lokasjon i path variabelen
    wstring wsPath(&wcPath[0]);     //convert to wstring
    string sPath(wsPath.begin(), wsPath.end());     //convert to string.
    size_t pos = sPath.find("KNX_CFC_Autogen_9.exe");     //Lokaliserer hvor i pathen selve exe filen er
    sPath2 = sPath.substr(0, pos);      //Lager en ny variabel hvor det lagres en ny variabel hvor det lagres lokasjonen til mappen som exe filen er lagret i (path - exe fil)
    //cout << sPath2 << "\n";       test

    OpenFile();

    auto tStart = std::chrono::high_resolution_clock::now();

    Read();
    Prossesering();
    WriteXML_KNX(sPath2, bUsed, sGVL, sRom, sRomtype563, sRomtypeKnx, sKommentar, sAdresseFormat, iMax);

    auto tStopp = std::chrono::high_resolution_clock::now();
    auto tRuntime = std::chrono::duration_cast<std::chrono::milliseconds>(tStopp - tStart);

    std::ifstream inFile(sPath2 + "/AutGenImport.xml");

    std::cout << "Done" << endl << "Generated " << std::count(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>(), '\n') << " lines in " << tRuntime.count() << " milliseconds" << endl;


    Sleep(30000);

    return 0;
}
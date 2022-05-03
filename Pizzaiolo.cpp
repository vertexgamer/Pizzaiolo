#include <windows.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <ostream>
#include <vector>
#include <filesystem>
#include <tlhelp32.h>
#include <tchar.h>
using namespace std;
namespace fs = std::filesystem;

string eseguibile;
string nomeeseguibile;
string argomenti;
string argomentioriginale;
string cartella;
int attivi = 0;
int operai;
string percorso;
int i;
int loop = 0;
string cartellaoutput;
int z;
string spazifix = "\"\" ";
string inizio;
string iniziocartellaoutput;

int main()
{
    while (1)
    {
        if (loop == 0)
        {
            cout << "Percorso dell eseguibile / Executable path" << endl;  //input path of the .exe
            getline(std::cin, eseguibile);
        }
        cout << "Scrivi gli argomenti ma sostituisci con il nome \"perin\" e \"perout\" il percorso di input e output / Write the arguments but replace the input and output path with the word \"perin\" and \"perout\"" << endl; //the user write the desired arguments but replace the input and output path with "percorso"
        if (loop == 1) {
            cin.ignore();
        }
        getline(std::cin, argomenti);
        cout << "Trascina la cartella con i files (se il percorso contiene spazi, dovrai aggiungere \" alla fine di \"perout\" + l'estensione del file) / Drag the folder containing the files(if the path contain spaces, add \" after \"perout\" + the file extension)" << endl; //the user drag the folder which contains the files to be encoded
        getline(std::cin, cartella);
        cout << "Desideri che l'output vada su una cartella specifica? Se no, lascia il campo vuoto. (se il percorso ha spazi, leggi sopra) / Do you desire a specific output folder? If not, leave blank. (if the path have spaces, read above)" << endl;
        getline(std::cin, cartellaoutput);
        cout << "Inserisci quante istanze saranno attive simultaneamente (USA UN VALORE MAGGIORE DI 0) / Insert the number of instances allowed at the same time (THE VALUE MUST BE GREATER THAN 0)" << endl;
        cin >> operai;   //number of simultaneus instances
        argomentioriginale = argomenti;

        i = eseguibile.find_last_of("\\");          //the executable name is extracted from the path given before
        nomeeseguibile = eseguibile.substr(i + 1);

        string inizio(cartella, 0, 1);
        string iniziocartellaoutput(cartellaoutput, 0, 1);

        if (inizio == "\"") {
            cartella.erase(0, 1);
            cartella.pop_back();
        }
        if (iniziocartellaoutput == "\"")
        {
            cartellaoutput.pop_back();
        }
        for (const auto& entry : fs::recursive_directory_iterator(cartella)) {    //it gets the path of the files inside the folder
            percorso = entry.path().string();
            //if (inizio == "\"") {
                percorso = "\"" + percorso + "\"";
            //}

            while (argomenti.find("perin") != string::npos)
                argomenti.replace(argomenti.find("perin"), 5, percorso);    //it replace "percorso" with the file path

            percorso = percorso.substr(0, percorso.find(".", 0));   //remove everyithing after "." included

            if (cartellaoutput.empty())
            {

                while (argomenti.find("perout") != string::npos)
                    argomenti.replace(argomenti.find("perout"), 6, percorso);
            }
            else
            {
                z = percorso.find_last_of("\\");     
                percorso = percorso.substr(z + 1);
                percorso = cartellaoutput + "\\" + percorso;
                while (argomenti.find("perout") != string::npos)
                    argomenti.replace(argomenti.find("perout"), 6, percorso);
            }
           
            cout << "start " + spazifix + eseguibile + " " + argomenti << endl;      //it cout the result for debugging purposes, in the final version it will start the process
            system(("start " + spazifix + eseguibile + " " + argomenti).c_str());
            argomenti = argomentioriginale;
            attivi++;

            for (; attivi == operai;) {
                Sleep(2000);
                string cmd = "tasklist /FI \"IMAGENAME eq " + nomeeseguibile + "\" | find /C \"" + nomeeseguibile + "\" > tmp.txt";
                system(cmd.c_str());
                ifstream in("tmp.txt");
                in >> attivi;
            }


        }
        loop = 1;
    }
        
    
}

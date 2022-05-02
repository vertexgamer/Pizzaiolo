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

int main()
{
    while (1)
    {
        if (loop == 0)
        {
            cout << "Path dell eseguibile senza spazi / Executable path without spaces" << endl;  //input path of the .exe
            getline(std::cin, eseguibile);
        }
        cout << "Scrivi gli argomenti ma sostituisci con il nome \"perin\" e \"perout\" il percorso di input e output / Write the arguments but replace the input and output path with the word \"perin\" and \"perout\"" << endl; //the user write the desired arguments but replace the input and output path with "percorso"
        if (loop == 1) {
            cin.ignore();
        }
        getline(std::cin, argomenti);
        cout << "Trascina la cartella con i files (il path non deve avere spazi) / Drag the folder containing the files (the path mustn't have spaces)" << endl; //the user drag the folder which contains the files to be encoded
        getline(std::cin, cartella);
        cout << "Inserisci quante istanze saranno attive simultaneamente (USA UN VALORE MAGGIORE DI 0) / Insert the number of instances allowed at the same time (THE VALUE MUST BE GREATER THAN 0)" << endl;
        cin >> operai;   //number of simultaneus instances
        argomentioriginale = argomenti;

        i = eseguibile.find_last_of("\\");          //the executable name is extracted from the path given before
        nomeeseguibile = eseguibile.substr(i + 1);

        for (const auto& entry : fs::directory_iterator(cartella)) {    //it gets the path of the files inside the folder
            percorso = entry.path().string();

            while (argomenti.find("perin") != string::npos)
                argomenti.replace(argomenti.find("perin"), 5, percorso);    //it replace "percorso" with the file path

            percorso = percorso.substr(0, percorso.find(".", 0));

            while (argomenti.find("perout") != string::npos)
                argomenti.replace(argomenti.find("perout"), 6, percorso);

            cout << "start " + eseguibile + " " + argomenti << endl;      //it cout the result for debugging purposes, in the final version it will start the process
            system(("start " + eseguibile + " " + argomenti).c_str());
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

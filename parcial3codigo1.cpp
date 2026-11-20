#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
using namespace std;

const int MAX_ALUMNOS = 100, MAX_GRUPOS = 50, MAX_MAESTROS = 50;

struct Alumno {
    string nombre, matricula;
    float calif[3];
    int grupo;
};

struct Maestro {
    string nombre, materia;
};

struct Grupo {
    string nombre, carrera;
};

int LeerEntero() {
    int n;
    while (!(cin >> n)) {
        cout << "Ingresa un numero valido: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return n;
}

float LeerCalificacion() {
    float n;
    while (!(cin >> n) || n < 0 || n > 10) {
        cout << "Ingresa una calificacion entre 0 y 10: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return n;
}

float CalcularPromedio(Alumno a) {
    return (a.calif[0] + a.calif[1] + a.calif[2]) / 3;
}

void CapturarGrupos(Grupo grupos[], int& totalGrupos) {
    int cantidad;
    cout << "Cuantos grupos deseas registrar? ";
    cantidad = LeerEntero();

    for (int i = 0; i < cantidad && totalGrupos < MAX_GRUPOS; i++) {
        cout << "Nombre del grupo: ";
        getline(cin, grupos[totalGrupos].nombre);

        cout << "Carrera: ";
        getline(cin, grupos[totalGrupos].carrera);

        totalGrupos++;
    }
}

void CapturarMaestros(Maestro maestros[], int& totalMaestros) {
    int cantidad;
    cout << "Cuantos maestros deseas registrar? ";
    cantidad = LeerEntero();

    for (int i = 0; i < cantidad && totalMaestros < MAX_MAESTROS; i++) {
        cout << "Nombre del maestro: ";
        getline(cin, maestros[totalMaestros].nombre);

        cout << "Materia: ";
        getline(cin, maestros[totalMaestros].materia);

        totalMaestros++;
    }
}

void CapturarAlumnos(Alumno alumnos[], int& total,
    Grupo grupos[], int totalGrupos) {
    int cantidad;

    cout << "Cuantos alumnos deseas registrar? ";
    cantidad = LeerEntero();

    for (int i = 0; i < cantidad && total < MAX_ALUMNOS; i++) {
        cout << "Nombre del alumno: ";
        getline(cin, alumnos[total].nombre);

        cout << "Matricula: ";
        getline(cin, alumnos[total].matricula);

        for (int j = 0; j < 3; j++) {
            cout << "Calificacion " << j + 1 << ": ";
            alumnos[total].calif[j] = LeerCalificacion();
        }

        cout << "Grupos disponibles:" << endl;
        for (int j = 0; j < totalGrupos; j++)
            cout << j + 1 << ". " << grupos[j].nombre << endl;

        int grupo;
        do {
            cout << "Selecciona el grupo: ";
            grupo = LeerEntero();
        } while (grupo < 1 || grupo > totalGrupos);

        alumnos[total].grupo = grupo - 1;
        total++;
    }
}

void MostrarReportes(Alumno alumnos[], int total,
    Grupo grupos[], int totalGrupos) {
    if (total == 0) {
        cout << "No hay alumnos registrados." << endl;
        return;
    }

    cout << "\nPromedio de cada alumno" << endl;
    for (int i = 0; i < total; i++) {
        cout << alumnos[i].nombre << ": "
            << fixed << setprecision(2)
            << CalcularPromedio(alumnos[i]) << endl;
    }

    cout << "\nMejor promedio por grupo" << endl;
    for (int g = 0; g < totalGrupos; g++) {
        float mejor = -1;
        string nombre;

        for (int i = 0; i < total; i++) {
            if (alumnos[i].grupo == g &&
                CalcularPromedio(alumnos[i]) > mejor) {
                mejor = CalcularPromedio(alumnos[i]);
                nombre = alumnos[i].nombre;
            }
        }

        cout << grupos[g].nombre << ": ";
        if (mejor == -1)
            cout << "Sin alumnos" << endl;
        else
            cout << nombre << " (" << mejor << ")" << endl;
    }

    cout << "\nAlumnos por carrera" << endl;
    for (int g = 0; g < totalGrupos; g++) {
        cout << grupos[g].carrera << ":" << endl;
        for (int i = 0; i < total; i++) {
            if (alumnos[i].grupo == g)
                cout << "- " << alumnos[i].nombre << endl;
        }
    }
}

void GuardarDatos(Alumno alumnos[], int total,
    Grupo grupos[], int totalGrupos,
    Maestro maestros[], int totalMaestros) {
    ofstream archivo("datos.txt");

    for (int i = 0; i < totalGrupos; i++)
        archivo << "G|" << grupos[i].nombre << "|"
        << grupos[i].carrera << endl;

    for (int i = 0; i < totalMaestros; i++)
        archivo << "M|" << maestros[i].nombre << "|"
        << maestros[i].materia << endl;

    for (int i = 0; i < total; i++)
        archivo << "A|" << alumnos[i].nombre << "|"
        << alumnos[i].matricula << "|"
        << alumnos[i].calif[0] << "|"
        << alumnos[i].calif[1] << "|"
        << alumnos[i].calif[2] << "|"
        << alumnos[i].grupo << endl;

    archivo.close();
    cout << "Datos guardados correctamente." << endl;
}

void CargarDatos(Alumno alumnos[], int& total,
    Grupo grupos[], int& totalGrupos,
    Maestro maestros[], int& totalMaestros) {
    ifstream archivo("datos.txt");

    if (!archivo) {
        cout << "No hay datos anteriores. Iniciando vacio." << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string tipo, dato;

        getline(ss, tipo, '|');

        if (tipo == "G" && totalGrupos < MAX_GRUPOS) {
            getline(ss, grupos[totalGrupos].nombre, '|');
            getline(ss, grupos[totalGrupos].carrera);
            totalGrupos++;
        }
        else if (tipo == "M" && totalMaestros < MAX_MAESTROS) {
            getline(ss, maestros[totalMaestros].nombre, '|');
            getline(ss, maestros[totalMaestros].materia);
            totalMaestros++;
        }
        else if (tipo == "A" && total < MAX_ALUMNOS) {
            getline(ss, alumnos[total].nombre, '|');
            getline(ss, alumnos[total].matricula, '|');

            ss >> alumnos[total].calif[0];
            ss.ignore(1, '|');
            ss >> alumnos[total].calif[1];
            ss.ignore(1, '|');
            ss >> alumnos[total].calif[2];
            ss.ignore(1, '|');
            ss >> alumnos[total].grupo;

            if (!ss.fail() && alumnos[total].grupo >= 0 &&
                alumnos[total].grupo < totalGrupos)
                total++;
        }
    }

    archivo.close();
}

void GenerarReporte(Alumno alumnos[], int total) {
    if (total == 0) {
        cout << "No hay alumnos para generar el reporte." << endl;
        return;
    }

    Alumno ordenados[MAX_ALUMNOS];

    for (int i = 0; i < total; i++)
        ordenados[i] = alumnos[i];

    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (CalcularPromedio(ordenados[j]) <
                CalcularPromedio(ordenados[j + 1])) {
                Alumno aux = ordenados[j];
                ordenados[j] = ordenados[j + 1];
                ordenados[j + 1] = aux;
            }
        }
    }

    ofstream archivo("reporte.txt");
    archivo << "Reporte de alumnos por promedio\n\n";

    for (int i = 0; i < total; i++) {
        archivo << i + 1 << ". " << ordenados[i].nombre
            << " | Matricula: " << ordenados[i].matricula
            << " | Promedio: " << fixed << setprecision(2)
            << CalcularPromedio(ordenados[i]) << endl;
    }

    archivo.close();
    cout << "Reporte generado en reporte.txt." << endl;
}

void MostrarMenu() {
    cout << "\nMenu principal" << endl;
    cout << "1. Registrar alumnos" << endl;
    cout << "2. Registrar grupos y carreras" << endl;
    cout << "3. Registrar maestros" << endl;
    cout << "4. Mostrar reportes" << endl;
    cout << "5. Guardar datos" << endl;
    cout << "6. Generar reporte de texto" << endl;
    cout << "7. Salir" << endl;
    cout << "Selecciona una opcion: ";
}

int main() {
    Alumno alumnos[MAX_ALUMNOS];
    Grupo grupos[MAX_GRUPOS];
    Maestro maestros[MAX_MAESTROS];

    int total = 0, totalGrupos = 0, totalMaestros = 0;
    int opcion;

    CargarDatos(alumnos, total, grupos, totalGrupos,
        maestros, totalMaestros);

    do {
        MostrarMenu();
        opcion = LeerEntero();

        switch (opcion) {
        case 1:
            if (totalGrupos == 0) {
                cout << "No hay grupos registrados." << endl;
                cout << "Primero selecciona la opcion 2 para registrar un grupo." << endl;
            }
            else {
                CapturarAlumnos(alumnos, total, grupos, totalGrupos);
            }
            break;

        case 2:
            CapturarGrupos(grupos, totalGrupos);
            break;

        case 3:
            if (totalGrupos == 0) {
                cout << "No hay grupos registrados." << endl;
                cout << "Primero selecciona la opcion 2 para registrar un grupo." << endl;
            }
            else {
                CapturarMaestros(maestros, totalMaestros);
            }
            break;

        case 4:
            MostrarReportes(alumnos, total, grupos, totalGrupos);
            break;

        case 5:
            GuardarDatos(alumnos, total, grupos, totalGrupos,
                maestros, totalMaestros);
            break;

        case 6:
            GenerarReporte(alumnos, total);
            break;

        case 7:
            GuardarDatos(alumnos, total, grupos, totalGrupos,
                maestros, totalMaestros);
            cout << "Saliendo del programa." << endl;
            break;

        default:
            cout << "Opcion no valida." << endl;
        }

    } while (opcion != 7);

    return 0;
}
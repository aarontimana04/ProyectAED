#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
using namespace std;


// Nodo para listas enlazadas
template<typename E>
struct Nodo {
    E data;
    Nodo* next;
    Nodo(const E& valor, Nodo* sig = nullptr) {
        data = valor;
        next = sig;
    }
};

// Lista enlazada para almacenar datos
template<typename E>
class LLista {
private:
    Nodo<E>* head;
    Nodo<E>* tail;
    int tam;

public:
    LLista() {
        head = tail = nullptr;
        tam = 0;
    }

    ~LLista() {
        limpiar();
    }

    void insertarFinal(const E& valor) {
        Nodo<E>* nuevoNodo = new Nodo<E>(valor);
        if (head == nullptr) {
            head = tail = nuevoNodo;
        } else {
            tail->next = nuevoNodo;
            tail = nuevoNodo;
        }
        tam++;
    }

    void limpiar() {
        while (head != nullptr) {
            Nodo<E>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        tam = 0;
    }

    E obtener(int index) const {
        if (index < 0 || index >= tam || head == nullptr) {
            return E();
        }
        Nodo<E>* actual = head;
        for (int i = 0; i < index && actual != nullptr; i++) {
            actual = actual->next;
        }
        if (actual == nullptr) {
            return E();
        }
        return actual->data;
    }

    int longitud() const {
        return tam;
    }

    void mostrar() const {
        Nodo<E>* actual = head;
        int contador = 0;
        while (actual != nullptr && contador < 10) {
            cout << actual->data << " ";
            actual = actual->next;
            contador++;
        }
        if (tam > 10) cout << "... (+" << (tam-10) << " más)";
        cout << endl;
    }
};

// Estructura para almacenar datos categóricos con frecuencia 
// pasar a clase
struct DatoCategorial {
    string categoria;
    int frecuencia;
    
    DatoCategorial() : categoria(""), frecuencia(0) {}
    DatoCategorial(string cat, int freq) : categoria(cat), frecuencia(freq) {}
};

// Estructura para almacenar una fila del CSV
struct FilaCSV { // representa a una fila del dataset
    LLista<string> campos;
    
    FilaCSV() {}
    
    FilaCSV(const FilaCSV& other) {
        for (int i = 0; i < other.campos.longitud(); i++) {
            campos.insertarFinal(other.campos.obtener(i));
        }
    }
    
    // Operador de asignación
    FilaCSV& operator=(const FilaCSV& other) {
        if (this != &other) {
            campos.limpiar();
            for (int i = 0; i < other.campos.longitud(); i++) {
                campos.insertarFinal(other.campos.obtener(i));
            }
        }
        return *this;
    }
};

// manejo de archivox
class ManejadorCSV { // representa al conjunto de filas del dataset (todo el dataset)
private:
    LLista<FilaCSV> datos;
    LLista<string> encabezados;
    string rutaArchivo;
    bool archivoLeido;

    string limpiarCampo(const string& campo) {
        string resultado = campo;
        
        // Eliminar espacios al inicio
        while (!resultado.empty() && (resultado[0] == ' ' || resultado[0] == '\t' || resultado[0] == '\r')) {
            resultado = resultado.substr(1);
        }
        
        // Eliminar espacios al final
        while (!resultado.empty() && (resultado[resultado.length()-1] == ' ' || 
               resultado[resultado.length()-1] == '\t' || resultado[resultado.length()-1] == '\r')) {
            resultado = resultado.substr(0, resultado.length()-1);
        }
        
        // Eliminar comillas si las hay
        if (!resultado.empty() && resultado[0] == '"' && resultado[resultado.length()-1] == '"') {
            resultado = resultado.substr(1, resultado.length()-2);
        }
        
        return resultado;
        // estetica xd
    }

public:
    ManejadorCSV() : archivoLeido(false) {}
    
    bool cargarArchivoCSV(const string& ruta) {
        rutaArchivo = ruta;
        datos.limpiar();
        encabezados.limpiar();
        
        ifstream archivo(ruta.c_str()); // ifstream recibe un argumento (ruta) y lo convierte a una cadena para la lectura
        if (!archivo.is_open()) {
            //archivo.is_open !== null
            cout << "Error: No se pudo abrir el archivo: " << ruta << endl;
            return false;
        }
        
        string linea;
        bool primeraLinea = true;
        int filaActual = 0;
        
        while (getline(archivo, linea) && filaActual < 1000) {
            if (linea.empty()) continue; // Saltar líneas vacías
            
            stringstream ss(linea); // ["true", "false", "true"] 
            string campo;
            FilaCSV fila;
            
            while (getline(ss, campo, ',')) {
                campo = limpiarCampo(campo);
                
                if (primeraLinea) {
                    encabezados.insertarFinal(campo);
                } else {
                    fila.campos.insertarFinal(campo);
                }
            }
            
            if (!primeraLinea && fila.campos.longitud() > 0) {
                datos.insertarFinal(fila);
            }
            primeraLinea = false;
            filaActual++;
        }
        
        archivo.close();
        archivoLeido = true;
        
        cout << "Archivo cargado exitosamente!" << endl;
        cout << "Filas leídas: " << datos.longitud() << endl;
        cout << "Columnas detectadas: " << encabezados.longitud() << endl;
        
        return true;
    }
    
    void mostrarEncabezados() {
        cout << "\nColumnas disponibles:" << endl;
        for (int i = 0; i < encabezados.longitud(); i++) {
            cout << i + 1 << ". " << encabezados.obtener(i) << endl;
        }
    }
    
    void mostrarVistaPrevia() {
        if (!archivoLeido) {
            cout << "Primero debe cargar un archivo CSV." << endl;
            return;
        }
        
        cout << "\n-- VISTA PREVIA DEL DATASET --" << endl;
        
        // Mostrar encabezados
        cout << "Encabezados: ";
        for (int i = 0; i < encabezados.longitud(); i++) {
            cout << encabezados.obtener(i);
            if (i < encabezados.longitud() - 1) cout << " | ";
        }
        cout << endl;
        
        // Mostrar primeras 5 filas
        cout << "\nPrimeras 5 filas:" << endl;
        int filasAMostrar = min(5, datos.longitud());
        for (int i = 0; i < filasAMostrar; i++) {
            FilaCSV fila = datos.obtener(i);
            int numCampos = min(fila.campos.longitud(), encabezados.longitud());
            for (int j = 0; j < numCampos; j++) {
                string valor = fila.campos.obtener(j);
                cout << valor;
                if (j < numCampos - 1) cout << " | ";
            }
            cout << endl;
        }
    }
    
    LLista<string> obtenerColumna(int indiceColumna) {
        LLista<string> columna;
        
        if (indiceColumna < 0 || indiceColumna >= encabezados.longitud()) {
            cout << "Índice de columna inválido." << endl;
            return columna;
        }
        
        for (int i = 0; i < datos.longitud(); i++) {
            FilaCSV fila = datos.obtener(i);
            if (indiceColumna < fila.campos.longitud()) {
                string valor = fila.campos.obtener(indiceColumna);
                if (!valor.empty()) {
                    columna.insertarFinal(valor);
                }
            }
        }
        
        return columna;
    }
    
    LLista<double> obtenerColumnaNumerica(int indiceColumna) {
        LLista<double> columna;
        
        if (indiceColumna < 0 || indiceColumna >= encabezados.longitud()) {
            cout << "Índice de columna inválido." << endl;
            return columna;
        }
        
        for (int i = 0; i < datos.longitud(); i++) {
            FilaCSV fila = datos.obtener(i);
            if (indiceColumna < fila.campos.longitud()) {
                string valor = fila.campos.obtener(indiceColumna);
                if (!valor.empty()) {
                    // Convertir comas decimales a puntos para países con formato europeo
                    for (int j = 0; j < valor.length(); j++) {
                        if (valor[j] == ',') {
                            valor[j] = '.';
                        }
                    }
                    
                    // Verificar si es un número válido
                    bool esNumero = true;
                    bool tienePunto = false;
                    int inicioNumero = 0;
                    
                    // Permitir signo negativo al inicio
                    if (!valor.empty() && valor[0] == '-') {
                        inicioNumero = 1;
                    }
                    
                    for (int j = inicioNumero; j < valor.length() && esNumero; j++) {
                        if (valor[j] >= '0' && valor[j] <= '9') {
                            continue;
                        } else if (valor[j] == '.' && !tienePunto) {
                            tienePunto = true;
                        } else {
                            esNumero = false;
                        }
                    }
                    
                    if (esNumero && valor.length() > inicioNumero) {
                        try {
                            double num = 0;
                            stringstream ss(valor);
                            ss >> num;
                            if (!ss.fail()) {
                                columna.insertarFinal(num);
                            }
                        } catch (...) {
                            // Ignorar valores que no se puedan convertir
                        }
                    }
                }
            }
        }
        
        return columna;
    }
    
    string obtenerNombreColumna(int indice) {
        if (indice >= 0 && indice < encabezados.longitud()) {
            return encabezados.obtener(indice);
        }
        return "";
    }
    
    bool estaArchivoLeido() { return archivoLeido; }
    int numeroColumnas() { return encabezados.longitud(); }
    int numeroFilas() { return datos.longitud(); }
    
private:
    int min(int a, int b) { return (a < b) ? a : b; }
};


class AnalizadorEstadistico {
private:
    ManejadorCSV csv;

public:
    // Métodos para técnicas univariadas - Variables categóricas
    void calcularFrecuenciasAbsolutasRelativas() { // asegura que se haya cargado algún .csv, caso contrario, retornamos
        if (!csv.estaArchivoLeido()) {
            cout << "Primero debe cargar un archivo CSV." << endl;
            return;
        }
        
        cout << "\n=== FRECUENCIAS ABSOLUTAS Y RELATIVAS ===" << endl;
        csv.mostrarEncabezados();
        
        int columna;
        cout << "Seleccione el número de columna a analizar: ";
        cin >> columna;
        columna--; // Convertir a índice base 0
        
        LLista<string> datosColumna = csv.obtenerColumna(columna);
        if (datosColumna.longitud() == 0) {
            cout << "No se encontraron datos en la columna seleccionada." << endl;
            return;
        }
        
        cout << "\nAnalizando columna: " << csv.obtenerNombreColumna(columna) << endl;
        cout << "Total de datos: " << datosColumna.longitud() << endl;
        
        // Contar frecuencias
        LLista<DatoCategorial> frecuencias;
        
        for(int i = 0; i < datosColumna.longitud(); i++) {
            string categoriaActual = datosColumna.obtener(i);
            bool encontrado = false;
            
            // Buscar si ya existe la categoría
            for(int j = 0; j < frecuencias.longitud(); j++) {
                DatoCategorial dato = frecuencias.obtener(j);
                if(dato.categoria == categoriaActual) {
                    encontrado = true;
                    break;
                }
            }
            
            if(!encontrado) {
                // Contar cuántas veces aparece esta categoría
                int freq = 0;
                for(int k = 0; k < datosColumna.longitud(); k++) {
                    if(datosColumna.obtener(k) == categoriaActual) {
                        freq++;
                    }
                }
                frecuencias.insertarFinal(DatoCategorial(categoriaActual, freq));
            }
        }
        
        // Mostrar resultados
        cout << "\n" << string(70, '=') << endl;
        cout << "Categoría\t\tFrecuencia Absoluta\tFrecuencia Relativa\t%" << endl;
        cout << string(70, '=') << endl;
        
        for(int i = 0; i < frecuencias.longitud(); i++) {
            DatoCategorial dato = frecuencias.obtener(i);
            double frecRelativa = (double)dato.frecuencia / datosColumna.longitud();
            double porcentaje = frecRelativa * 100;
            cout << dato.categoria << "\t\t\t" << dato.frecuencia << "\t\t\t" 
                 << frecRelativa << "\t\t" << porcentaje << "%" << endl;
        }
        cout << string(70, '=') << endl;
    }
    
    void calcularPorcentajeValoresFaltantes() {
        if (!csv.estaArchivoLeido()) {
            cout << "Primero debe cargar un archivo CSV." << endl;
            return;
        }
        
        cout << "\n=== PORCENTAJE DE VALORES FALTANTES ===" << endl;
        csv.mostrarEncabezados();
        
        int columna;
        cout << "Seleccione el número de columna a analizar: ";
        cin >> columna;
        columna--; // Convertir a índice base 0
        
        int totalFilas = csv.numeroFilas();
        LLista<string> datosColumna = csv.obtenerColumna(columna);
        int datosReales = datosColumna.longitud();
        int valoresFaltantes = totalFilas - datosReales;
        
        double porcentajeFaltantes = (totalFilas > 0) ? (double)valoresFaltantes / totalFilas * 100 : 0;
        
        cout << "\nAnálisis de columna: " << csv.obtenerNombreColumna(columna) << endl;
        cout << string(50, '=') << endl;
        cout << "Total de filas en el archivo: " << totalFilas << endl;
        cout << "Datos reales (no vacíos): " << datosReales << endl;
        cout << "Valores faltantes: " << valoresFaltantes << endl;
        cout << "Porcentaje de valores faltantes: " << porcentajeFaltantes << "%" << endl;
        cout << string(50, '=') << endl;
    }
    
    // Métodos para técnicas univariadas - Variables numéricas
    void calcularRangoIntercuartilico() {
        if (!csv.estaArchivoLeido()) {
            cout << "Primero debe cargar un archivo CSV." << endl;
            return;
        }
        
        cout << "\n=== RANGO INTERCUARTÍLICO (IQR) ===" << endl;
        csv.mostrarEncabezados();
        
        int columna;
        cout << "Seleccione el número de columna numérica a analizar: ";
        cin >> columna;
        columna--; // Convertir a índice base 0
        
        LLista<double> datosNumericos = csv.obtenerColumnaNumerica(columna);
        if (datosNumericos.longitud() == 0) {
            cout << "No se encontraron datos numéricos en la columna seleccionada." << endl;
            return;
        }
        
        int n = datosNumericos.longitud();
        double* arreglo = new double[n];
        
        // Copiar datos a arreglo para ordenamiento
        for(int i = 0; i < n; i++) {
            arreglo[i] = datosNumericos.obtener(i);
        }
        
        // Ordenar datos usando bubble sort
        for(int i = 0; i < n-1; i++) {
            for(int j = 0; j < n-i-1; j++) {
                if(arreglo[j] > arreglo[j+1]) {
                    double temp = arreglo[j];
                    arreglo[j] = arreglo[j+1];
                    arreglo[j+1] = temp;
                }
            }
        }
        
        // Calcular cuartiles usando interpolación
        double Q1, Q2, Q3;
        
        double pos1 = (n - 1) * 0.25;
        double pos2 = (n - 1) * 0.5;
        double pos3 = (n - 1) * 0.75;
        
        // Q1
        int base1 = (int)pos1;
        double frac1 = pos1 - base1;
        if (base1 + 1 < n) {
            Q1 = arreglo[base1] + frac1 * (arreglo[base1 + 1] - arreglo[base1]);
        } else {
            Q1 = arreglo[base1];
        }
        
        // Q2 (Mediana)
        int base2 = (int)pos2;
        double frac2 = pos2 - base2;
        if (base2 + 1 < n) {
            Q2 = arreglo[base2] + frac2 * (arreglo[base2 + 1] - arreglo[base2]);
        } else {
            Q2 = arreglo[base2];
        }
        
        // Q3
        int base3 = (int)pos3;
        double frac3 = pos3 - base3;
        if (base3 + 1 < n) {
            Q3 = arreglo[base3] + frac3 * (arreglo[base3 + 1] - arreglo[base3]);
        } else {
            Q3 = arreglo[base3];
        }
        
        double IQR = Q3 - Q1;
        
        cout << "\nAnálisis de columna: " << csv.obtenerNombreColumna(columna) << endl;
        cout << "Total de datos numéricos: " << n << endl;
        cout << string(50, '=') << endl;
        cout << "Valor mínimo: " << arreglo[0] << endl;
        cout << "Q1 (Cuartil 1): " << Q1 << endl;
        cout << "Q2 (Mediana): " << Q2 << endl;
        cout << "Q3 (Cuartil 3): " << Q3 << endl;
        cout << "Valor máximo: " << arreglo[n-1] << endl;
        cout << "IQR (Rango Intercuartílico): " << IQR << endl;
        cout << string(50, '=') << endl;
        
        delete[] arreglo;
    }
    
    void calcularCoeficienteVariacion() {
        if (!csv.estaArchivoLeido()) {
            cout << "Primero debe cargar un archivo CSV." << endl;
            return;
        }
        
        cout << "\n -- COEFICIENTE DE VARIACIÓN --" << endl;
        csv.mostrarEncabezados();
        
        int columna;
        cout << "Seleccione el número de columna numérica a analizar: ";
        cin >> columna;
        columna--; // Convertir a índice base 0
        
        LLista<double> datosNumericos = csv.obtenerColumnaNumerica(columna);
        if (datosNumericos.longitud() == 0) {
            cout << "No se encontraron datos numéricos en la columna seleccionada." << endl;
            return;
        }
        
        int n = datosNumericos.longitud();
        if (n < 2) {
            cout << "Se necesitan al menos 2 datos para calcular el coeficiente de variación." << endl;
            return;
        }
        
        double suma = 0;
        
        // Calcular media
        for(int i = 0; i < n; i++) {
            suma += datosNumericos.obtener(i);
        }
        double media = suma / n;
        
        if (media == 0) {
            cout << "No se puede calcular el coeficiente de variación cuando la media es 0." << endl;
            return;
        }
        
        // Calcular desviación estándar
        double sumaCuadrados = 0;
        for(int i = 0; i < n; i++) {
            double valor = datosNumericos.obtener(i);
            sumaCuadrados += (valor - media) * (valor - media);
        }
        
        double varianza = sumaCuadrados / (n - 1);
        double desviacion = sqrt(varianza);
        
        // Calcular coeficiente de variación
        double CV = (desviacion / media) * 100;
        
        cout << "\nAnálisis de columna: " << csv.obtenerNombreColumna(columna) << endl;
        cout << "Total de datos numéricos: " << n << endl;
        cout << string(50, '=') << endl;
        cout << "Media: " << media << endl;
        cout << "Varianza: " << varianza << endl;
        cout << "Desviación estándar: " << desviacion << endl;
        cout << "Coeficiente de Variación (CV): " << CV << "%" << endl;
        cout << string(50, '=') << endl;
        
        if (CV < 15) {
            cout << "Interpretación: Variabilidad BAJA (CV < 15%)" << endl;
        } else if (CV < 30) {
            cout << "Interpretación: Variabilidad MODERADA (15% ≤ CV < 30%)" << endl;
        } else {
            cout << "Interpretación: Variabilidad ALTA (CV ≥ 30%)" << endl;
        }
    }
    
    // Método mejorado para cargar archivo con sugerencias de ruta
    void cargarArchivo() {
        string ruta;
        cout << " ----- CARGAR DATAEET CSV ------" << endl;
        
        cout << "\nEjemplos de rutas comunes:" << endl;
        cout << "Windows: C:\\Users\\[NombreUsuario]\\Downloads\\archivo.csv" << endl;
        cout << "Ejemplo: C:\\Users\\Juan\\Downloads\\datos.csv" << endl;
        cout << "\nMac/Linux: /Users/[NombreUsuario]/Downloads/archivo.csv" << endl;
        cout << "Ejemplo: /Users/juan/Downloads/datos.csv" << endl;
        
        cout << "\n" << string(50, '-') << endl;
        cout << "Ingrese la ruta completa del archivo CSV:" << endl;
        cout << "Ruta: ";
        cin.ignore(); // Limpiar buffer
        getline(cin, ruta);
        
        cout << "\nCargando archivo..." << endl;
        cout << "Ruta especificada: " << ruta << endl;
        
        if (csv.cargarArchivoCSV(ruta)) {
            cout << "\n" << string(50, '=') << endl;
            cout << "¡ARCHIVO CARGADO EXITOSAMENTE!" << endl;
            cout << string(50, '=') << endl;
            csv.mostrarVistaPrevia();
            
            cout << "\nEl archivo está listo para análisis." << endl;
            cout << "Puede proceder con las técnicas estadísticas." << endl;
        } else {
            cout << "\n" << string(50, '!') << endl;
            cout << "ERROR AL CARGAR EL ARCHIVO" << endl;
            cout << string(50, '!') << endl;
            cout << "\nPosibles causas del error:" << endl;
            cout << "1. La ruta del archivo es incorrecta" << endl;
            cout << "2. El archivo no existe en la ubicación especificada" << endl;
            cout << "3. No tiene permisos para leer el archivo" << endl;
            cout << "4. El archivo no es un CSV válido" << endl;
        }
    }
    
    void mostrarInformacionArchivo() {
        if (!csv.estaArchivoLeido()) {
            cout << "\n¡ATENCIÓN!" << endl;
            cout << "No hay ningún archivo CSV cargado." << endl;
            cout << "Primero debe cargar un archivo desde el menú." << endl;
            return;
        }
        
        cout << " --- INFORMACIÓN DEL DATASET --- " << endl;
        cout << "Total de filas de datos: " << csv.numeroFilas() << endl;
        cout << "Total de columnas: " << csv.numeroColumnas() << endl;
        cout << "Estado: Archivo cargado correctamente" << endl;
        
        cout << "\n" << string(50, '-') << endl;
        cout << "COLUMNAS DISPONIBLES PARA ANÁLISIS:" << endl;
        cout << string(50, '-') << endl;
        csv.mostrarEncabezados();
    }
    
    void menuArchivo() {
        int opcion;
        do {
            cout << " ---- GESTIÓN DE ARCHIVOS CSV ----" << endl;
            
            if (csv.estaArchivoLeido()) {
                cout << "Estado actual: ✓ Archivo cargado" << endl;
            } else {
                cout << "Estado actual: ✗ Sin archivo" << endl;
            }
            
            cout << "===============================================" << endl;
            cout << "1. Cargar nuevo archivo CSV" << endl;
            cout << "2. Mostrar vista previa del archivo actual" << endl;
            cout << "3. Mostrar información detallada del archivo" << endl;
            cout << "0. Volver al menú principal" << endl;
            cout << "===============================================" << endl;
            cout << "Seleccione una opción: ";
            cin >> opcion;
            
            switch(opcion) {
                case 1:
                    cargarArchivo();
                    break;
                case 2:
                    if (csv.estaArchivoLeido()) {
                        csv.mostrarVistaPrevia();
                    } else {
                        cout << "\nNo hay archivo cargado." << endl;
                        cout << "Primero debe cargar un archivo CSV." << endl;
                    }
                    break;
                case 3:
                    mostrarInformacionArchivo();
                    break;
                case 0:
                    cout << "Volviendo al menú principal..." << endl;
                    break;
                default:
                    cout << "Opción inválida. Intente nuevamente." << endl;
            }
        } while(opcion != 0);
    }
    
    void menuVariablesNumericas() {
        int opcion;
        do {
            cout << "      ANÁLISIS DE VARIABLES NUMÉRICAS" << endl;
            cout << "1. Rango intercuartílico (IQR)" << endl;
            cout << "2. Coeficiente de variación" << endl;
            cout << "0. Volver al menú anterior" << endl;
            cout << "===============================================" << endl;
            cout << "Seleccione una opción: ";
            cin >> opcion;
            
            switch(opcion) {
                case 1:
                    calcularRangoIntercuartilico();
                    break;
                case 2:
                    calcularCoeficienteVariacion();
                    break;
                case 0:
                    cout << "Volviendo al menú anterior..." << endl;
                    break;
                default:
                    cout << "Opción inválida. Por favor intente nuevamente." << endl;
            }
        } while(opcion != 0);
    }
    
    void menuVariablesCategoricas() {
        int opcion;
        do {
            cout << "      ANÁLISIS DE VARIABLES CATEGÓRICAS" << endl;
            cout << "1. Frecuencias absolutas y relativas" << endl;
            cout << "2. Porcentaje de valores faltantes" << endl;
            cout << "0. Volver al menú anterior" << endl;
            cout << "===============================================" << endl;
            cout << "Seleccione una opción: ";
            cin >> opcion;
            
            switch(opcion) {
                case 1:
                    calcularFrecuenciasAbsolutasRelativas();
                    break;
                case 2:
                    calcularPorcentajeValoresFaltantes();
                    break;
                case 0:
                    cout << "Volviendo al menú anterior..." << endl;
                    break;
                default:
                    cout << "Opción inválida. Por favor intente nuevamente." << endl;
            }
        } while(opcion != 0);
    }
    
    // Estructura para tabla de contingencia
    struct CeldaTabla {
        string categoria1;
        string categoria2;
        int frecuencia;
        double frecuenciaEsperada;
        
        CeldaTabla() : categoria1(""), categoria2(""), frecuencia(0), frecuenciaEsperada(0.0) {}
        CeldaTabla(string cat1, string cat2, int freq) 
            : categoria1(cat1), categoria2(cat2), frecuencia(freq), frecuenciaEsperada(0.0) {}
    };
    
    void calcularTablaContingencia() {
        if (!csv.estaArchivoLeido()) {
            cout << "Primero debe cargar un archivo CSV." << endl;
            return;
        }
        
        cout << "\n=== TABLA DE CONTINGENCIA (CATEGÓRICA vs CATEGÓRICA) ===" << endl;
        csv.mostrarEncabezados();
        
        int columna1, columna2;
        cout << "\nSeleccione la primera variable categórica (número de columna): ";
        cin >> columna1;
        columna1--; // Convertir a índice base 0
        
        cout << "Seleccione la segunda variable categórica (número de columna): ";
        cin >> columna2;
        columna2--; // Convertir a índice base 0
        
        if (columna1 == columna2) {
            cout << "Las columnas deben ser diferentes." << endl;
            return;
        }
        
        LLista<string> datos1 = csv.obtenerColumna(columna1);
        LLista<string> datos2 = csv.obtenerColumna(columna2);
        
        if (datos1.longitud() == 0 || datos2.longitud() == 0) {
            cout << "No se encontraron datos en las columnas seleccionadas." << endl;
            return;
        }
        
        // Obtener categorías únicas para cada variable
        LLista<string> categorias1, categorias2;
        
        // Categorías de la primera variable
        for (int i = 0; i < datos1.longitud(); i++) {
            string cat = datos1.obtener(i);
            bool existe = false;
            for (int j = 0; j < categorias1.longitud(); j++) {
                if (categorias1.obtener(j) == cat) {
                    existe = true;
                    break;
                }
            }
            if (!existe) {
                categorias1.insertarFinal(cat);
            }
        }
        
        // Categorías de la segunda variable
        for (int i = 0; i < datos2.longitud(); i++) {
            string cat = datos2.obtener(i);
            bool existe = false;
            for (int j = 0; j < categorias2.longitud(); j++) {
                if (categorias2.obtener(j) == cat) {
                    existe = true;
                    break;
                }
            }
            if (!existe) {
                categorias2.insertarFinal(cat);
            }
        }
        
        // Crear tabla de contingencia
        LLista<CeldaTabla> tabla;
        int totalObservaciones = min(datos1.longitud(), datos2.longitud());
        
        // Inicializar todas las combinaciones con frecuencia 0
        for (int i = 0; i < categorias1.longitud(); i++) {
            for (int j = 0; j < categorias2.longitud(); j++) {
                tabla.insertarFinal(CeldaTabla(categorias1.obtener(i), categorias2.obtener(j), 0));
            }
        }
        
        // Contar frecuencias
        for (int i = 0; i < totalObservaciones; i++) {
            string cat1 = datos1.obtener(i);
            string cat2 = datos2.obtener(i);
            
            // Buscar la celda correspondiente y aumentar frecuencia
            for (int j = 0; j < tabla.longitud(); j++) {
                CeldaTabla celda = tabla.obtener(j);
                if (celda.categoria1 == cat1 && celda.categoria2 == cat2) {
                    // Necesitamos actualizar la frecuencia (limitación de la implementación actual)
                    // Por simplicidad, recontamos todo
                    break;
                }
            }
        }
        
        // Recontar todas las frecuencias (método más directo)
        for (int idx = 0; idx < tabla.longitud(); idx++) {
            CeldaTabla celda = tabla.obtener(idx);
            int frecuencia = 0;
            
            for (int i = 0; i < totalObservaciones; i++) {
                if (datos1.obtener(i) == celda.categoria1 && datos2.obtener(i) == celda.categoria2) {
                    frecuencia++;
                }
            }
            
            // Crear nueva celda con frecuencia actualizada
            CeldaTabla nuevaCelda(celda.categoria1, celda.categoria2, frecuencia);
            
            // Aquí tendríamos que reemplazar en la lista, pero por simplicidad mostramos directamente
        }
        
        // Mostrar tabla de contingencia
        cout << "\nTabla de Contingencia:" << endl;
        cout << "Variable 1: " << csv.obtenerNombreColumna(columna1) << endl;
        cout << "Variable 2: " << csv.obtenerNombreColumna(columna2) << endl;
        cout << "Total de observaciones: " << totalObservaciones << endl;
        
        cout << "\n" << string(80, '=') << endl;
        cout << "Variable 1 \\ Variable 2\t";
        for (int j = 0; j < categorias2.longitud(); j++) {
            cout << categorias2.obtener(j) << "\t";
        }
        cout << "Total" << endl;
        cout << string(80, '=') << endl;
        
        // Mostrar filas de la tabla
        for (int i = 0; i < categorias1.longitud(); i++) {
            cout << categorias1.obtener(i) << "\t\t\t";
            int totalFila = 0;
            
            for (int j = 0; j < categorias2.longitud(); j++) {
                int frecuencia = 0;
                
                // Buscar frecuencia para esta combinación
                for (int k = 0; k < totalObservaciones; k++) {
                    if (datos1.obtener(k) == categorias1.obtener(i) && 
                        datos2.obtener(k) == categorias2.obtener(j)) {
                        frecuencia++;
                    }
                }
                
                cout << frecuencia << "\t";
                totalFila += frecuencia;
            }
            cout << totalFila << endl;
        }
        
        // Mostrar totales de columnas
        cout << "Total\t\t\t";
        int granTotal = 0;
        for (int j = 0; j < categorias2.longitud(); j++) {
            int totalColumna = 0;
            for (int i = 0; i < categorias1.longitud(); i++) {
                for (int k = 0; k < totalObservaciones; k++) {
                    if (datos1.obtener(k) == categorias1.obtener(i) && 
                        datos2.obtener(k) == categorias2.obtener(j)) {
                        totalColumna++;
                    }
                }
            }
            cout << totalColumna << "\t";
            granTotal += totalColumna;
        }
        cout << granTotal << endl;
        cout << string(80, '=') << endl;
    }
    
    void calcularChiCuadrado() {
        if (!csv.estaArchivoLeido()) {
            cout << "Primero debe cargar un archivo CSV." << endl;
            return;
        }
        
        cout << "\n -- PRUEBA CHI-CUADRADO DE INDEPENDENCIA -- " << endl;
        csv.mostrarEncabezados();
        
        int columna1, columna2;
        cout << "\nSeleccione la primera variable categórica (número de columna): ";
        cin >> columna1;
        columna1--; // Convertir a índice base 0
        
        cout << "Seleccione la segunda variable categórica (número de columna): ";
        cin >> columna2;
        columna2--; // Convertir a índice base 0
        
        if (columna1 == columna2) {
            cout << "Las columnas deben ser diferentes." << endl;
            return;
        }
        
        LLista<string> datos1 = csv.obtenerColumna(columna1);
        LLista<string> datos2 = csv.obtenerColumna(columna2);
        
        if (datos1.longitud() == 0 || datos2.longitud() == 0) {
            cout << "No se encontraron datos en las columnas seleccionadas." << endl;
            return;
        }
        
        // Obtener categorías únicas
        LLista<string> categorias1, categorias2;
        
        for (int i = 0; i < datos1.longitud(); i++) {
            string cat = datos1.obtener(i);
            bool existe = false;
            for (int j = 0; j < categorias1.longitud(); j++) {
                if (categorias1.obtener(j) == cat) {
                    existe = true;
                    break;
                }
            }
            if (!existe) {
                categorias1.insertarFinal(cat);
            }
        }
        
        for (int i = 0; i < datos2.longitud(); i++) {
            string cat = datos2.obtener(i);
            bool existe = false;
            for (int j = 0; j < categorias2.longitud(); j++) {
                if (categorias2.obtener(j) == cat) {
                    existe = true;
                    break;
                }
            }
            if (!existe) {
                categorias2.insertarFinal(cat);
            }
        }
        
        int filas = categorias1.longitud();
        int columnas = categorias2.longitud();
        int totalObservaciones = min(datos1.longitud(), datos2.longitud());
        
        // Crear matrices para frecuencias observadas y esperadas
        int** observadas = new int*[filas];
        double** esperadas = new double*[filas];
        for (int i = 0; i < filas; i++) {
            observadas[i] = new int[columnas];
            esperadas[i] = new double[columnas];
            for (int j = 0; j < columnas; j++) {
                observadas[i][j] = 0;
                esperadas[i][j] = 0.0;
            }
        }
        
        // Calcular frecuencias observadas
        for (int k = 0; k < totalObservaciones; k++) {
            string cat1 = datos1.obtener(k);
            string cat2 = datos2.obtener(k);
            
            int indiceFila = -1, indiceColumna = -1;
            
            for (int i = 0; i < filas; i++) {
                if (categorias1.obtener(i) == cat1) {
                    indiceFila = i;
                    break;
                }
            }
            
            for (int j = 0; j < columnas; j++) {
                if (categorias2.obtener(j) == cat2) {
                    indiceColumna = j;
                    break;
                }
            }
            
            if (indiceFila >= 0 && indiceColumna >= 0) {
                observadas[indiceFila][indiceColumna]++;
            }
        }
        
        // Calcular totales marginales
        int* totalesFilas = new int[filas];
        int* totalesColumnas = new int[columnas];
        
        for (int i = 0; i < filas; i++) {
            totalesFilas[i] = 0;
            for (int j = 0; j < columnas; j++) {
                totalesFilas[i] += observadas[i][j];
            }
        }
        
        for (int j = 0; j < columnas; j++) {
            totalesColumnas[j] = 0;
            for (int i = 0; i < filas; i++) {
                totalesColumnas[j] += observadas[i][j];
            }
        }
        
        // Calcular frecuencias esperadas
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                esperadas[i][j] = (double)(totalesFilas[i] * totalesColumnas[j]) / totalObservaciones;
            }
        }
        
        // Calcular estadístico Chi-cuadrado
        double chiCuadrado = 0.0;
        bool validoParaChi = true;
        
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                if (esperadas[i][j] < 5) {
                    validoParaChi = false;
                }
                if (esperadas[i][j] > 0) {
                    double diferencia = observadas[i][j] - esperadas[i][j];
                    chiCuadrado += (diferencia * diferencia) / esperadas[i][j];
                }
            }
        }
        
        // Grados de libertad
        int gradosLibertad = (filas - 1) * (columnas - 1);
        
        // Mostrar resultados
        cout << "\nAnálisis Chi-Cuadrado:" << endl;
        cout << "Variable 1: " << csv.obtenerNombreColumna(columna1) << endl;
        cout << "Variable 2: " << csv.obtenerNombreColumna(columna2) << endl;
        cout << string(60, '=') << endl;
        
        cout << "\nTabla de Frecuencias Observadas:" << endl;
        cout << "Variable 1 \\ Variable 2\t";
        for (int j = 0; j < columnas; j++) {
            cout << categorias2.obtener(j) << "\t";
        }
        cout << endl;
        
        for (int i = 0; i < filas; i++) {
            cout << categorias1.obtener(i) << "\t\t";
            for (int j = 0; j < columnas; j++) {
                cout << observadas[i][j] << "\t";
            }
            cout << endl;
        }
        
        cout << "\nTabla de Frecuencias Esperadas:" << endl;
        cout << "Variable 1 \\ Variable 2\t";
        for (int j = 0; j < columnas; j++) {
            cout << categorias2.obtener(j) << "\t";
        }
        cout << endl;
        
        for (int i = 0; i < filas; i++) {
            cout << categorias1.obtener(i) << "\t\t";
            for (int j = 0; j < columnas; j++) {
                cout << esperadas[i][j] << "\t";
            }
            cout << endl;
        }
        
        cout << "\n" << string(60, '=') << endl;
        cout << "Estadístico Chi-cuadrado: " << chiCuadrado << endl;
        cout << "Grados de libertad: " << gradosLibertad << endl;
        cout << "Total de observaciones: " << totalObservaciones << endl;
        
        if (!validoParaChi) {
            cout << "\nADVERTENCIA: Algunas frecuencias esperadas son menores a 5." << endl;
            cout << "Los resultados del test Chi-cuadrado pueden no ser confiables." << endl;
        }
        
        cout << "\nInterpretación:" << endl;
        cout << "- Valores grandes de Chi-cuadrado sugieren dependencia entre variables" << endl;
        cout << "- Compare con tabla de distribución Chi-cuadrado con " << gradosLibertad << " gl" << endl;
        cout << "- Valores críticos comunes (α=0.05):" << endl;
        if (gradosLibertad == 1) cout << "  * 1 gl: 3.841" << endl;
        if (gradosLibertad == 2) cout << "  * 2 gl: 5.991" << endl;
        if (gradosLibertad == 3) cout << "  * 3 gl: 7.815" << endl;
        if (gradosLibertad == 4) cout << "  * 4 gl: 9.488" << endl;
        
        // Liberar memoria
        for (int i = 0; i < filas; i++) {
            delete[] observadas[i];
            delete[] esperadas[i];
        }
        delete[] observadas;
        delete[] esperadas;
        delete[] totalesFilas;
        delete[] totalesColumnas;
    }
    
    void menuVariablesBivariadas() {
        int opcion;
        do {
            cout << "      ANÁLISIS BIVARIADO (CATEGÓRICAS)" << endl;
            cout << "1. Tabla de contingencia" << endl;
            cout << "2. Prueba Chi-cuadrado de independencia" << endl;
            cout << "0. Volver al menú anterior" << endl;
            cout << "===============================================" << endl;
            cout << "Seleccione una opción: ";
            cin >> opcion;
            
            switch(opcion) {
                case 1:
                    calcularTablaContingencia();
                    break;
                case 2:
                    calcularChiCuadrado();
                    break;
                case 0:
                    cout << "Volviendo al menú anterior..." << endl;
                    break;
                default:
                    cout << "Opción inválida. Por favor intente nuevamente." << endl;
            }
        } while(opcion != 0);
    }
    
    void menuPrincipal() {
        int opcion;
        do {
            cout << "     HERRAMIENTA ESTADÍSTICA PARA DATOS CSV" << endl;
            
            if (csv.estaArchivoLeido()) {
                cout << "Estado: ✓ Archivo CSV cargado (" << csv.numeroFilas() << " filas, " 
                     << csv.numeroColumnas() << " columnas)" << endl;
            } else {
                cout << "Estado: ✗ Sin archivo cargado" << endl;
            }
            
            cout << "===============================================" << endl;
            cout << "1. Gestión de archivos CSV" << endl;
            cout << "2. Análisis univariado - Variables categóricas" << endl;
            cout << "3. Análisis univariado - Variables numéricas" << endl;
            cout << "4. Análisis bivariado - Categóricas vs Categóricas" << endl;
            cout << "0. Salir del programa" << endl;
            cout << "===============================================" << endl;
            cout << "Seleccione una opción: ";
            cin >> opcion;
            
            switch(opcion) {
                case 1:
                    menuArchivo();
                    break;
                case 2:
                    if (csv.estaArchivoLeido()) {
                        menuVariablesCategoricas();
                    } else {
                        cout << "\n¡ATENCIÓN! Debe cargar un archivo CSV primero." << endl;
                        cout << "Vaya a 'Gestión de archivos CSV' para cargar un archivo." << endl;
                    }
                    break;
                case 3:
                    if (csv.estaArchivoLeido()) {
                        menuVariablesNumericas();
                    } else {
                        cout << "\n¡ATENCIÓN! Debe cargar un archivo CSV primero." << endl;
                        cout << "Vaya a 'Gestión de archivos CSV' para cargar un archivo." << endl;
                    }
                    break;
                case 4:
                    if (csv.estaArchivoLeido()) {
                        menuVariablesBivariadas();
                    } else {
                        cout << "\n¡ATENCIÓN! Debe cargar un archivo CSV primero." << endl;
                        cout << "Vaya a 'Gestión de archivos CSV' para cargar un archivo." << endl;
                    }
                    break;
                case 0:
                    cout << "    ¡GRACIAS POR USAR LA HERRAMIENTA!" << endl;
                    cout << "Programa terminado exitosamente." << endl;
                    break;
                default:
                    cout << "Opción inválida. Por favor seleccione una opción válida." << endl;
            }
        } while(opcion != 0);
    }
};

int main() {
    cout << "   BIENVENIDO!! " << endl;
    cout << "Este programa permite analizar datos de archivos CSV" << endl;
    cout << "con técnicas estadísticas univariadas y bivariadas." << endl;
    cout << "\nCaracterísticas disponibles:" << endl;
    cout << "• Análisis de variables categóricas y numéricas" << endl;
    cout << "• Tablas de contingencia y pruebas Chi-cuadrado" << endl;
    cout << "• Cálculo de estadísticos descriptivos" << endl;
    cout << "• Interfaz interactiva fácil de usar" << endl;
    
    AnalizadorEstadistico analizador;
    analizador.menuPrincipal();
    
    return 0;
}

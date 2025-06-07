#include <iostream>
#include <fstream> // archivo
#include <sstream> // archivo
#include <string>
#include <cmath> // raiz 
using namespace std;

template<typename E>
class Lista {
public:
    Lista() {}
    virtual ~Lista() {}
    virtual void insertar(const E& item) = 0;
    virtual E obtener(int pos) const = 0;
    virtual int longitud() const = 0;
    virtual void limpiar() = 0;
    virtual bool buscar(const E& item) const = 0;
};

template<typename E>
class Nodo {
private:
    E data;
    Nodo* siguiente;

public:
    Nodo(const E& valor, Nodo* sig = nullptr) : data(valor), siguiente(sig) {}
    
    // Getters
    E getData() const { return data; }
    Nodo* getSiguiente() const { return siguiente; }
    
    // Setters
    void setData(const E& valor) { data = valor; }
    void setSiguiente(Nodo* sig) { siguiente = sig; }
};

// Lista Enlazada
template<typename E>
class LLista : public Lista<E> {
private:
    Nodo<E>* cabeza;
    Nodo<E>* cola;
    int tam;

public:
    LLista() : cabeza(nullptr), cola(nullptr), tam(0) {}
    
    ~LLista() {
        limpiar();
    }

    void insertar(const E& item) override {
        Nodo<E>* nuevo = new Nodo<E>(item);
        if (cabeza == nullptr) {
            cabeza = cola = nuevo;
        } else {
            cola->setSiguiente(nuevo);
            cola = nuevo;
        }
        tam++;
    }

    E obtener(int pos) const override {
        if (pos < 0 || pos >= tam) {
            cout<<"Posiicion invalida"<<endl;
        }
        Nodo<E>* actual = cabeza;
        for (int i = 0; i < pos; i++) {
            actual = actual->getSiguiente();
        }
        return actual->getData();
    }

    int longitud() const override {
        return tam;
    }

    void limpiar() override {
        while (cabeza != nullptr) {
            Nodo<E>* temp = cabeza;
            cabeza = cabeza->getSiguiente();
            delete temp;
        }
        cola = nullptr;
        tam = 0;
    }

    bool buscar(const E& item) const override {
        Nodo<E>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->getData() == item) return true;
            actual = actual->getSiguiente();
        }
        return false;
    }

    void mostrar() const {
        Nodo<E>* actual = cabeza;
        while (actual != nullptr) {
            cout << actual->getData() << " ";
            actual = actual->getSiguiente();
        }
        cout << endl;
    }
};

// Cola basada en Arreglo
template<typename E>
class ACola {
private:
    int tamMax;
    int frente;
    int final;
    int tam;
    E* arreglo;

public:
    // lista de inicializacion
    ACola(int _tamMax) : tamMax(_tamMax), frente(0), final(0), tam(0) {
        arreglo = new E[tamMax];
    }

    ~ACola() {
        delete[] arreglo;
    }

    void enqueue(const E& item) {
        if (tam == tamMax) {
            cout << "Cola llena" << endl;
            return;
        }
        arreglo[final] = item;
        final = (final + 1) % tamMax;
        tam++;
    }

    E dequeue() {
        if (tam == 0) {
            cout << "Cola vacia" << endl;
        }
        E item = arreglo[frente];
        frente = (frente + 1) % tamMax;
        tam--;
        return item;
    }

    bool estaVacia() const {
        return tam == 0;
    }

    int longitud() const {
        return tam;
    }
};

// Pila basada en Arreglo (para ordenamiento)
template<typename E>
class APila {
private:
    int tamMax;
    int top;
    int tam;
    E* arreglo;

public:
    // lista de inicializacion
    APila(int _tamMax) : tamMax(_tamMax), top(0), tam(0) {
        arreglo = new E[tamMax];
    }

    ~APila() {
        delete[] arreglo;
    }

    void push(const E& item) {
        if (tam == tamMax) {
            cout << "Pila llena" << endl;
            return;
        }
        arreglo[top] = item;
        top = (top + 1) % tamMax;
        tam++;
    }

    E pop() {
        if (tam == 0) {
            cout << "Pila vacia" << endl;
        }
        top = (top - 1 + tamMax) % tamMax;
        E valor = arreglo[top];
        tam--;
        return valor;
    }

    E obtener(int pos) const {
        if (pos < 0 || pos >= tam) {
            cout << "Pila llena" << endl;
        }
        return arreglo[pos];
    }

    int longitud() const {
        return tam;
    }

    void ordenarMergeSort() {
        mergeSort(arreglo, tam);
        top = tam;
    }

private:
    void merge(E* a, E* a1, int n1, E* a2, int n2, E* resultado) {
        int i = 0, j = 0, k = 0;
        while (i < n1 && j < n2) {
            if (a1[i] <= a2[j]) {
                resultado[k++] = a1[i++];
            } else {
                resultado[k++] = a2[j++];
            }
        }
        while (i < n1) resultado[k++] = a1[i++];
        while (j < n2) resultado[k++] = a2[j++];
    }

    void mergeSort(E* a, int n) {
        if (n <= 1) return;
        int n1 = n / 2;
        int n2 = n - n1;
        E* a1 = new E[n1];
        E* a2 = new E[n2];
        
        for (int i = 0; i < n1; ++i) a1[i] = a[i];
        for (int i = 0; i < n2; ++i) a2[i] = a[n1 + i];
        
        mergeSort(a1, n1);
        mergeSort(a2, n2);
        merge(a, a1, n1, a2, n2, a);
        
        delete[] a1;
        delete[] a2;
    }
};

// Clase para frecuencias categóricas (cambiado de struct a class)
class FrecuenciaCategoria {
private:
    string categoria;
    int frecuenciaAbsoluta;
    double frecuenciaRelativa;

public:
    // Constructores
    FrecuenciaCategoria() : categoria(""), frecuenciaAbsoluta(0), frecuenciaRelativa(0.0) {}
    FrecuenciaCategoria(string cat) : categoria(cat), frecuenciaAbsoluta(1), frecuenciaRelativa(0.0) {}
    
    // Getters
    string getCategoria() const { return categoria; }
    int getFrecuenciaAbsoluta() const { return frecuenciaAbsoluta; }
    double getFrecuenciaRelativa() const { return frecuenciaRelativa; }
    
    // Setters
    void setCategoria(const string& cat) { categoria = cat; }
    void setFrecuenciaAbsoluta(int freq) { frecuenciaAbsoluta = freq; }
    void setFrecuenciaRelativa(double freq) { frecuenciaRelativa = freq; }
    
    // Incrementar frecuencia
    void incrementarFrecuencia() { frecuenciaAbsoluta++; }
    
    // Operador de comparación con string
    bool operator==(const string& cat) const {
        return categoria == cat;
    }
    
    // Operador de comparación con otra FrecuenciaCategoria
    bool operator==(const FrecuenciaCategoria& other) const {
        return categoria == other.categoria;
    }
};

class AnalizadorDataset {
private:
    LLista<string>* nombreColumnas;
    LLista<LLista<string>*>* datosFilas;
    int numFilas;
    int numColumnas;

    // Función auxiliar para parsear CSV correctamente (aqui habia un problema xd los numeros flotantes en los csv tienen comas y los separaba en 2 numeros)
    // para parsear correctamente usamos un metodo aux
    LLista<string>* parsearLineaCSV(const string& linea) {
        LLista<string>* campos = new LLista<string>();
        string campo = "";
        bool dentroComillas = false;
        
        for (size_t i = 0; i < linea.length(); i++) {
            char c = linea[i];
            
            if (c == '"') {
                dentroComillas = !dentroComillas;
            } else if (c == ',' && !dentroComillas) {
                // Fin del campo
                campos->insertar(campo);
                campo = "";
            } else {
                campo += c;
            }
        }
        
        // Agregar el último campo
        if (!campo.empty() || linea.back() == ',') {
            campos->insertar(campo);
        }
        
        return campos;
    }

    // Función para limpiar y convertir valores numéricos
    string limpiarValorNumerico(const string& valor) {
        string valorLimpio = valor;
        
        // Remover comillas al inicio y final si existen
        if (valorLimpio.length() >= 2 && valorLimpio.front() == '"' && valorLimpio.back() == '"') {
            valorLimpio = valorLimpio.substr(1, valorLimpio.length() - 2);
        }
        
        // Reemplazar comas por puntos para decimales
        for (char& c : valorLimpio) {
            if (c == ',') c = '.';
        }
        
        return valorLimpio;
    }

    // Función para verificar si un valor es numérico
    bool esNumerico(const string& valor) {
        string valorLimpio = limpiarValorNumerico(valor);
        if (valorLimpio.empty()) return false;
        
        try { // esto es un prueba error, pertenece a iostream, no te preocupes
            // quiza no lo has visto, es como un if, else pero para verificar errores
            stod(valorLimpio);
            return true;
        } catch (const exception& e) {
            return false;
        }
    }

    // segunda forma (seguro mas viable para el profe :D)
    /*
    bool esNumerico(const string& valor) {
    string v = limpiarValorNumerico(valor);
    if (v.empty()) return false;
    bool punto = false;
    for (int i = 0; i < (int)v.size(); i++) {
        if (isdigit(v[i])) continue;
        if (v[i] == '-' && i == 0) continue;
        if (v[i] == '.' && !punto) { punto = true; continue; }
        return false;
    }
    return true;
    }
    */

public:
    AnalizadorDataset() {
        nombreColumnas = new LLista<string>();
        datosFilas = new LLista<LLista<string>*>();
        numFilas = 0;
        numColumnas = 0;
    }

    ~AnalizadorDataset() {
        // Limpiar datos
        for (int i = 0; i < datosFilas->longitud(); i++) {
            delete datosFilas->obtener(i);
        }
        delete datosFilas;
        delete nombreColumnas;
    }

    bool cargarCSV(const string& rutaArchivo) {
        ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo " << rutaArchivo << endl;
            return false;
        }

        string linea;
        bool primeraLinea = true;

        while (getline(archivo, linea)) { // getline lee una línea completa del archivo y la almacena en la variable linea. Retorna el stream del archivo.
            LLista<string>* fila = parsearLineaCSV(linea);

            if (primeraLinea) {
                // Guardar nombres de columnas
                for (int i = 0; i < fila->longitud(); i++) {
                    nombreColumnas->insertar(fila->obtener(i));
                }
                numColumnas = fila->longitud();
                delete fila; // No guardamos la fila de encabezados
                primeraLinea = false;
            } else {
                datosFilas->insertar(fila);
                numFilas++;
            }
        }

        archivo.close();
        cout << "Dataset cargado exitosamente!" << endl;
        cout << "Filas de datos: " << numFilas << endl;
        cout << "Columnas: " << numColumnas << endl;
        return true;
    }

    void mostrarVistaPrevia() {
        cout << "\n=============== VISTA PREVIA DEL DATASET ===============" << endl;
        cout << "Número de registros: " << numFilas << endl;
        cout << "Número de variables: " << numColumnas << endl;
        cout << "Total de celdas: " << (numFilas * numColumnas) << endl;
        
        cout << "\nVariables disponibles:" << endl;
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        cout << "\nPrimeras 5 filas:" << endl;
        // Mostrar encabezados
        for (int j = 0; j < nombreColumnas->longitud(); j++) {
            cout << nombreColumnas->obtener(j);
            if (j < nombreColumnas->longitud() - 1) cout << "\t";
        }
        cout << endl;
        
        // Mostrar datos
        int filasAMostrar = min(5, numFilas);
        for (int i = 0; i < filasAMostrar; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            for (int j = 0; j < fila->longitud(); j++) {
                string valor = fila->obtener(j);
                // Mostrar valor limpio para números
                if (esNumerico(valor)) {
                    cout << limpiarValorNumerico(valor);
                } else {
                    cout << valor;
                }
                if (j < fila->longitud() - 1) cout << "\t";
            }
            cout << endl;
        }
        cout << "======================================================" << endl;
    }

    void calcularFrecuenciaAbsolutaRelativa() {
        cout << "\n --- Frecuencia absoluta y relativa --" << endl;
        cout << "Seleccione la variable categorica:" << endl;
        
        // Mostrar variables disponibles
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcion;
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        
        if (opcion < 1 || opcion > numColumnas) {
            cout << "Opcion invalida." << endl;
            return;
        }
        
        int columnaSeleccionada = opcion - 1;
        string nombreVariable = nombreColumnas->obtener(columnaSeleccionada);
        
        // Usar LLista para contar frecuencias
        LLista<FrecuenciaCategoria>* frecuencias = new LLista<FrecuenciaCategoria>();
        
        // Procesar cada fila
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string valor = fila->obtener(columnaSeleccionada);
            
            // Buscar si la categoría ya existe
            bool encontrado = false;
            for (int j = 0; j < frecuencias->longitud(); j++) {
                FrecuenciaCategoria categoria = frecuencias->obtener(j);
                if (categoria == valor) {
                    encontrado = true;
                    break;
                }
            }
            
            // Si no existe, crear nueva categoría
            if (!encontrado) {
                frecuencias->insertar(FrecuenciaCategoria(valor));
            }
        }
        
        // Recalcular frecuencias correctamente
        LLista<FrecuenciaCategoria>* frecuenciasFinales = new LLista<FrecuenciaCategoria>();
        
        for (int i = 0; i < frecuencias->longitud(); i++) {
            FrecuenciaCategoria categoria = frecuencias->obtener(i);
            int contador = 0;
            
            // Contar todas las ocurrencias de esta categoría
            for (int j = 0; j < numFilas; j++) {
                LLista<string>* fila = datosFilas->obtener(j);
                string valor = fila->obtener(columnaSeleccionada);
                if (valor == categoria.getCategoria()) {
                    contador++;
                }
            }
            
            categoria.setFrecuenciaAbsoluta(contador);
            categoria.setFrecuenciaRelativa((double)contador / numFilas);
            frecuenciasFinales->insertar(categoria);
        }
        
        // Mostrar resultados
        cout << "\nFrecuencias para la variable: " << nombreVariable << endl;
        cout << "Categoriia\t\tFrec. Absoluta\tFrec. Relativa" << endl;
        cout << "--------------------------------------------------------" << endl;
        
        for (int i = 0; i < frecuenciasFinales->longitud(); i++) {
            FrecuenciaCategoria categoria = frecuenciasFinales->obtener(i);
            cout << categoria.getCategoria() << "\t\t\t" 
                 << categoria.getFrecuenciaAbsoluta() << "\t\t" 
                 << categoria.getFrecuenciaRelativa() << endl;
        }
        
        delete frecuencias;
        delete frecuenciasFinales;
    }

    void calcularPorcentajeValoresFaltantes() {
        cout << "\n--- Valores faltantes ---" << endl;
        
        // AColita para procesar cada columna
        for (int col = 0; col < numColumnas; col++) {
            string nombreVariable = nombreColumnas->obtener(col);
            
            // Crear cola para almacenar todos los valores de la columna
            ACola<string>* colaValores = new ACola<string>(numFilas);
            
            // Llenar la cola con todos los valores de la columna
            for (int fila = 0; fila < numFilas; fila++) {
                LLista<string>* filaActual = datosFilas->obtener(fila);
                string valor = filaActual->obtener(col);
                colaValores->enqueue(valor);
            }
            
            // Procesar la cola para contar valores faltantes
            int valoresFaltantes = 0;
            int totalValores = colaValores->longitud();
            
            // Procesar todos los elementos de la cola
            while (!colaValores->estaVacia()) {
                string valor = colaValores->dequeue();
                
                // Verificar si el valor está faltante
                if (valor.empty() || valor == "NA" || valor == "NULL" || valor == "\"\"" || valor == "\"") {
                    valoresFaltantes++;
                }
            }
            
            double porcentajeFaltantes = (double)valoresFaltantes / totalValores * 100.0;
            
            cout << nombreVariable << ": " 
                 << valoresFaltantes << "/" << totalValores 
                 << " (" << porcentajeFaltantes << "%)" << endl;
            
            delete colaValores;
        }
    }

    void calcularRangoIntercuartilico() {
        cout << "\n--- Rango intercuartilico (IQR) ---" << endl;
        cout << "Seleccione la variable numerica:" << endl;
        
        // Mostrar variables disponibles
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcion;
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        
        if (opcion < 1 || opcion > numColumnas) {
            cout << "Opcion invalida." << endl;
            return;
        }
        
        int columnaSeleccionada = opcion - 1;
        string nombreVariable = nombreColumnas->obtener(columnaSeleccionada);
        
        // Usar APila para almacenar valores numéricos y ordenarlos
        APila<double>* valores = new APila<double>(numFilas);
        
        // Convertir valores a números y almacenar en pila
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string valorStr = fila->obtener(columnaSeleccionada);
            
            if (esNumerico(valorStr)) {
                try {
                    string valorLimpio = limpiarValorNumerico(valorStr);
                    double valor = stod(valorLimpio);
                    valores->push(valor);
                } catch (const exception& e) {
                    cout << "Advertencia: Error al convertir valor: " << valorStr << endl;
                }
            }
        }
        
        if (valores->longitud() == 0) {
            cout << "No se encontraron valores numericos validos." << endl;
            delete valores;
            return;
        }
        
        // Ordenar valores usando mergeSort
        valores->ordenarMergeSort();
        
        // Calcular posiciones de cuartiles
        int n = valores->longitud();
        int posQ1 = n / 4;
        int posQ3 = (3 * n) / 4;
        
        double Q1 = valores->obtener(posQ1);
        double Q3 = valores->obtener(posQ3);
        double IQR = Q3 - Q1;
        
        cout << "\nResultados para la variable: " << nombreVariable << endl;
        cout << "Numero de valores validos: " << n << endl;
        cout << "Q1 (Primer cuartil): " << Q1 << endl;
        cout << "Q3 (Tercer cuartil): " << Q3 << endl;
        cout << "IQR (Rango Intercuartilico): " << IQR << endl;
        
        delete valores;
    }

    void calcularCoeficienteVariacion() {
        cout << "\n--- Coef. de variacion ---" << endl;
        cout << "Seleccione la variable numerica:" << endl;
        
        // Mostrar variables disponibles
        for (int i = 0; i < nombreColumnas->longitud(); i++) {
            cout << i + 1 << ". " << nombreColumnas->obtener(i) << endl;
        }
        
        int opcion;
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        
        if (opcion < 1 || opcion > numColumnas) {
            cout << "Opcion invalida." << endl;
            return;
        }
        
        int columnaSeleccionada = opcion - 1;
        string nombreVariable = nombreColumnas->obtener(columnaSeleccionada);
        
        // Usar arreglo para múltiples pasadas
        double* valores = new double[numFilas];
        int countValores = 0;
        
        // Primera pasada: recopilar valores válidos
        for (int i = 0; i < numFilas; i++) {
            LLista<string>* fila = datosFilas->obtener(i);
            string valorStr = fila->obtener(columnaSeleccionada);
            
            if (esNumerico(valorStr)) {
                try {
                    string valorLimpio = limpiarValorNumerico(valorStr);
                    double valor = stod(valorLimpio);
                    valores[countValores++] = valor;
                } catch (const exception& e) {
                    // Ignorar valores no convertibles
                }
            }
        }
        
        if (countValores == 0) {
            cout << "No se encontraron valores numericos validos." << endl;
            delete[] valores;
            return;
        }
        
        // Segunda pasada: calcular media
        double suma = 0.0;
        for (int i = 0; i < countValores; i++) {
            suma += valores[i];
        }
        double media = suma / countValores;
        
        // Tercera pasada: calcular desviación estándar
        double sumaCuadrados = 0.0;
        for (int i = 0; i < countValores; i++) {
            double diferencia = valores[i] - media;
            sumaCuadrados += diferencia * diferencia;
        }
        double varianza = sumaCuadrados / countValores;
        double desviacionEstandar = sqrt(varianza); // aqui el uso de la libreria cmath para calcular la raiz
        
        // Calcular coeficiente de variación
        double coeficienteVariacion = (media != 0) ? desviacionEstandar / abs(media) : 0;
        
        cout << "\nResultados para la variable: " << nombreVariable << endl;
        cout << "Numero de valores validos: " << countValores << endl;
        cout << "Media: " << media << endl;
        cout << "Desviacion Estandar: " << desviacionEstandar << endl;
        cout << "Coeficiente de Variacion: " << coeficienteVariacion << endl;
        cout << "CV como porcentaje: " << (coeficienteVariacion * 100) << "%" << endl;
        
        delete[] valores;
    }
};

/// funcion principal
void mostrarMenu() {
    cout << "\n--- Tecnicas Univariadas ---" << endl; // version 5 xd
    cout << "1. Vista previa del dataset" << endl;
    cout << "2. Frecuencia absoluta y relativa (Variables Categoricas)" << endl;
    cout << "3. Porcentaje de valores faltantes" << endl;
    cout << "4. Rango intercuartilico - IQR (Variables Numericas)" << endl;
    cout << "5. Coeficiente de variacion (Variables Numericas)" << endl;
    cout << "6. Salir" << endl;
    cout << "===============================================================" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    cout << "--- AED aplicado a Datasets ---" << endl;
    
    AnalizadorDataset analizador;
    string rutaArchivo;
    
    cout << "Ingrese la ruta del archivo CSV: "; // /Users/usuario/Documents/C++/dataset01.csv
    getline(cin, rutaArchivo);
    
    if (!analizador.cargarCSV(rutaArchivo)) {
        return 1;
    }
    
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch (opcion) {
            case 1:
                analizador.mostrarVistaPrevia();
                break;
            case 2:
                analizador.calcularFrecuenciaAbsolutaRelativa();
                break;
            case 3:
                analizador.calcularPorcentajeValoresFaltantes();
                break;
            case 4:
                analizador.calcularRangoIntercuartilico();
                break;
            case 5:
                analizador.calcularCoeficienteVariacion();
                break;
            case 6:
                cout << "¡Gracias por usar el prgrama!" << endl;
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
        }
        
        if (opcion != 6) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while (opcion != 6);
    
    return 0;
}

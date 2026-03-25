# Manual de Usuario: STRUCTS Library (v1.0)

**STRUCTS** es una biblioteca de estructuras de datos de alto rendimiento escrita en C11. Está diseñada bajo una arquitectura de **punteros opacos**, lo que garantiza un encapsulamiento estricto y una interoperabilidad perfecta con lenguajes como **C++, C#, Python y Rust**.

---

## 1. Arquitectura y Filosofía
* **Genérica:** Todas las estructuras almacenan punteros `void*`, permitiendo guardar cualquier tipo de dato (structs, tipos primitivos, objetos).
* **Portable:** Código estándar C11 sin dependencias externas, compatible con `gcc`, `clang` y `msvc`.
* **Persistente:** Sistema integrado para volcar el estado de las estructuras a disco en formatos legibles (CSV/TXT).
* **Interoperable:** Preparada para ser compilada como `shared library` (.dll/.so).

---

## 2. Estructuras de Datos Incluidas

| Estructura | Archivo Header | Casos de Uso |
| :--- | :--- | :--- |
| **LinkedList** | `list.h` | Listas dinámicas, colecciones generales. |
| **Stack / Queue** | `stack.h` / `queue.h` | Algoritmos de deshacer (undo), gestión de turnos, buffers. |
| **HashTable** | `hashtable.h` | Diccionarios, índices de búsqueda rápida $O(1)$. |
| **Tree (AVL/BST)** | `tree.h` | Datos ordenados, búsquedas jerárquicas. |
| **Heap** | `heap.h` | Colas de prioridad, algoritmos de selección. |
| **Graph** | `graph.h` | Redes, topologías, cálculo de rutas (BFS/Shortest Path). |

---

## 3. Guía de Inicio Rápido (API de C)

### Creación y Destrucción
Cada componente sigue el patrón de diseño `create/destroy`. Es vital pasar una función de limpieza si la estructura debe liberar los datos internos.

```c
#include "structs/list.h"

// Crear lista
LinkedList *my_list = list_create();

// Agregar datos
int *val = malloc(sizeof(int));
*val = 100;
list_add_back(my_list, val);

// Destruir (liberando memoria interna)
list_destroy(my_list, free);
```

### Uso de la Tabla Hash
Implementa el algoritmo de hashing **DJB2** por defecto para minimizar colisiones.

```c
#include "structs/hashtable.h"

HashTable *ht = ht_create(hash_djb2);
ht_insert(ht, "user_key", my_data_ptr);

void *data = ht_get(ht, "user_key", NULL);
```

---

## 4. Persistencia Universal
**STRUCTS** permite guardar y cargar estructuras completas mediante el módulo `persistence.h`. 

**Para guardar:**
```c
// Guarda cualquier lista en formato CSV
list_save_txt(my_list, "data.csv", my_formatter_func);
```

**Para cargar:**
```c
// Carga automática detectando el tipo de estructura
LoadedStructure ls = load_from_file("data.csv", TYPE_LIST, my_parser_func);
LinkedList *recovered_list = (LinkedList*)ls.data;
```

---

## 5. Distribución y Binarios
Para utilizar **STRUCTS** en otros lenguajes, se recomienda compilar como librería dinámica:

### En Windows (MSYS2/MinGW):
```bash
gcc -shared -o bin/structs.dll build/**/*.o
```

### En Linux:
```bash
gcc -shared -fPIC -o bin/libstructs.so build/**/*.o
```

---

## 6. Integración con otros lenguajes

### Python (ctypes)
```python
from ctypes import CDLL, c_void_p
lib = CDLL("./structs.dll")
lib.list_create.restype = c_void_p
my_list = lib.list_create()
```

### C# (P/Invoke)
```csharp
[DllImport("structs.dll", CallingConvention = CallingConvention.Cdecl)]
public static extern IntPtr list_create();
```

---

## 7. Guía de Integración: ¿Qué incluir en tu proyecto?

🔹 Para desarrollar/probar:    make     (genera test_library.exe).
🔹 Para distribuir en Windows: make dll (genera structs.dll y libstructs.a).
🔹 Para distribuir en Linux:   make so  (genera libstructs.so).

Para utilizar **STRUCTS** en una aplicación externa, no necesitas todo el código fuente. Solo debes incluir los archivos distribuidos en el "SDK" de la librería.

### 1. Estructura de archivos necesaria

Dependiendo de tu lenguaje, necesitarás una combinación de estos archivos:

| Carpeta | Archivo | Propósito | Obligatorio para: |
| :--- | :--- | :--- | :--- |
| `include/` | `*.h` | Cabeceras con los prototipos de funciones. | C, C++ |
| `bin/` | `structs.dll` | Binario ejecutable para Windows. | Windows (Todos los lenguajes) |
| `bin/` | `libstructs.so` | Binario ejecutable para Linux. | Linux (Todos los lenguajes) |
| `lib/` | `libstructs.a` | Librería estática de enlace (generada por GCC). | C, C++ (Linker) |

---

### 2. Configuración por lenguaje

#### **Para proyectos en C / C++**
Debes configurar tu entorno para que encuentre las definiciones y el código:
1. **Include Path:** Agrega la carpeta `include/` a los directorios de búsqueda de tu compilador (`-Iinclude`).
2. **Library Path:** Agrega la carpeta `bin/` o `lib/` a los directorios de búsqueda del enlazador (`-Lbin`).
3. **Linker Flag:** Indica que quieres usar la librería con `-lstructs`.

**Ejemplo de comando de compilación:**
```bash
gcc mi_programa.c -I./include -L./bin -lstructs -o mi_programa.exe
```

#### **Para proyectos en Python**
Solo necesitas el binario correspondiente a tu sistema operativo.
1. Coloca `structs.dll` (Windows) o `libstructs.so` (Linux) en la raíz de tu script o en una subcarpeta accesible.
2. Utiliza `ctypes.CDLL("./ruta/al/archivo")` para cargarla.

#### **Para proyectos en C# (Windows Forms / .NET)**
1. Copia `structs.dll` a la carpeta de salida de tu proyecto (donde se genera el `.exe`, usualmente `bin/Debug` o `bin/Release`).
2. Usa el atributo `[DllImport("structs.dll")]` en tus clases.

---

### 3. Distribución con tu App (Runtime)
Cuando entregues tu software al usuario final, **DEBES** incluir el archivo `.dll` o `.so` en la misma carpeta que tu ejecutable. Sin este archivo, el programa lanzará un error de "Librería no encontrada".

> **⚠️ Nota de Seguridad:** Nunca incluyas los archivos `.c` (src) en tu distribución final, a menos que desees entregar el código fuente completo bajo los términos de la Licencia MIT.

---

## 8. Solución de problemas
Si encuentras algún error durante la integración de **STRUCTS**, consulta esta guía rápida:

| Estado | Error Común | Causa Probable | Solución ✅ |
| :---: | :--- | :--- | :--- |
| ❌ | `DLL not found` | El archivo `structs.dll` no está en la ruta del ejecutable. | Copia la DLL a la misma carpeta que tu `.exe`. |
| ⚠️ | `OverflowError: int too long` | (Python) Intento de usar punteros de 64 bits como enteros de 32. | Define `restype` y `argtypes` como `c_void_p`. |
| 🐧 | `cannot open shared object file` | Linux no sabe dónde está el archivo `.so`. | Ejecuta: `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./bin`. |
| 🛠️ | `undefined reference to...` | (C/C++) No se vinculó la librería de importación. | Asegúrate de usar `-L./lib -lstructs` al compilar. |
| 🧱 | `fPIC error` | Los objetos de Linux se crearon sin código independiente de posición. | Ejecuta `make clean` y luego `make so`. |

### 💡 Consejos para el Usuario

* **Arquitectura:** Asegúrate de que tanto tu programa (Python/C#/C++) como la librería **STRUCTS** sean de la misma arquitectura (ambos x64 o ambos x86). Mezclarlos causará un error de "Formato de imagen no válido".
* **Depuración:** Si la librería carga pero se cierra inesperadamente, verifica que estés llamando a los destructores (`list_destroy`, `ht_destroy`) para liberar la memoria.
* **UTF-8:** Para evitar caracteres extraños en la consola (como la 'é'), recuerda usar `SetConsoleOutputCP(CP_UTF8)` en Windows antes de imprimir resultados de la librería.

---

## 9. Licencia
**STRUCTS** se distribuye bajo la **Licencia MIT**. Eres libre de usarla, modificarla y distribuirla, incluso en proyectos comerciales, siempre que mantengas el aviso de copyright.

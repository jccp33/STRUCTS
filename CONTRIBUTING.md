# Guía de Contribución: STRUCTS Library

¡Gracias por considerar contribuir a **STRUCTS**! Queremos que esta librería sea la base más sólida y rápida para el manejo de datos en C y otros lenguajes.

Como este es un proyecto de bajo nivel, mantenemos estándares estrictos para asegurar la estabilidad y el rendimiento.

---

## 🛠️ ¿Cómo puedes ayudar?

1.  **Reportar Errores:** Si encuentras un *Segmentation Fault* o una fuga de memoria (*Memory Leak*), abre un **Issue** detallando los pasos para reproducirlo.
2.  **Sugerir Mejoras:** ¿Crees que el Hash Table podría ser más rápido? ¿Quieres agregar un Árbol Rojo-Negro? ¡Sugerencias son bienvenidas!
3.  **Wrappers:** Ayúdanos a crear mejores archivos de conexión para **Java, Rust o Go**.
4.  **Documentación:** Corregir errores tipográficos o mejorar los ejemplos en el manual.

---

## 📜 Reglas de Estilo de Código

Para mantener la consistencia en **STRUCTS**, pedimos seguir estas reglas:

* **Lenguaje:** C11 estándar. No uses extensiones específicas de un compilador (como las de GCC o MSVC) a menos que sea estrictamente necesario para la portabilidad.
* **Nombres:** * Funciones: `snake_case` con prefijo de la estructura (ej: `list_add_back`).
    * Variables: `snake_case` descriptivo.
* **Encapsulamiento:** Nunca expongas la definición de los `struct` en los archivos `.h`. Usa **Punteros Opacos**.
* **Memoria:** Toda función `_create` debe tener su correspondiente `_destroy`. Usa `valgrind` para verificar que no hay fugas antes de enviar un Pull Request.

---

## 🚀 Proceso de Pull Request (PR)

1.  Haz un **Fork** del repositorio.
2.  Crea una rama para tu mejora: `git checkout -b feature/nueva-estructura`.
3.  Asegúrate de que el código **compile sin warnings** usando `make`.
4.  Si agregas una estructura, añade una prueba en la carpeta `tests/`.
5.  Envía el PR con una descripción clara de los cambios.

---

## 🧪 Pruebas y Calidad

Antes de enviar código, verifica lo siguiente:
```bash
# 1. Limpiar y compilar todo
make clean && make

# 2. Generar la DLL/SO
make dll

# 3. Verificar que no haya warnings
# (El compilador debe terminar en silencio)
```



---

## ⚖️ Licencia
Al contribuir, aceptas que tu código sea distribuido bajo la misma **Licencia MIT** que el resto del proyecto.

---

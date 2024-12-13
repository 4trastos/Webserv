# Webserv
This is when you finally understand why a URL starts with HTTP
# Webserv Project

# Webserv Proyecto

## Introducción 🇪🇸

Este proyecto consiste en escribir tu propio servidor HTTP utilizando **C++ 98**. El objetivo es construir un servidor web funcional que pueda procesar peticiones HTTP, manejar conexiones de clientes y servir contenido estático y dinámico.

---

### Paso 1: Entender HTTP y su relación con el servidor

**HTTP (Hypertext Transfer Protocol)** es un protocolo de comunicación utilizado en la web para transferir documentos y datos entre servidores y clientes (generalmente navegadores web). En este proyecto, tu objetivo es implementar un **servidor HTTP** que pueda:

- Procesar y entregar páginas web (HTML, imágenes, etc.).
- Manejar peticiones de clientes utilizando los métodos HTTP: **GET**, **POST** y **DELETE**.
- Enviar respuestas HTTP correctas (códigos de estado, encabezados, etc.).

#### Función del servidor HTTP:
- El servidor almacena y entrega recursos web a los clientes.
- El cliente (como un navegador) envía una petición, y el servidor responde con el contenido (o un error si no puede entregarlo).

---

### Paso 2: Requisitos técnicos del proyecto

#### 2.1. Requisitos generales:

- **Lenguaje**: El servidor debe implementarse en **C++ 98**.
- **Ejecución**: El programa se ejecutará con un archivo de configuración como argumento.
- **No puedes ejecutar otro servidor** (como NGINX o Apache).
- **No debe bloquearse**: Utiliza técnicas de **I/O no bloqueante** (con `poll()`, `select()`, `epoll()`, etc.) para manejar múltiples conexiones al mismo tiempo sin bloquear.
- **Errores de servidor**: El servidor debe tener páginas de error por defecto si no se proporcionan páginas de error personalizadas.

#### 2.2. Compilación:

- Debes proporcionar un **Makefile** con los comandos: `all`, `clean`, `fclean`, `re`.
- Compila el código con las siguientes banderas:
  ```bash
  -Wall -Wextra -Werror -std=c++98
  ```

#### 2.3. Funciones externas permitidas:

- Se pueden usar varias funciones del sistema como `execve`, `dup`, `socket`, `listen`, `send`, `recv`, `poll`, entre otras, para la manipulación de sockets y archivos.

---

### Paso 3: Configuración del servidor

Tu servidor deberá manejar un archivo de configuración donde se definirán varios parámetros importantes:

#### 3.1. Parámetros que debe incluir el archivo de configuración:

- **Puerto y host**: El servidor debe poder escuchar en múltiples puertos y hosts.
- **Nombres de servidor (`server_names`)**: Asignar nombres de servidores.
- **Páginas de error personalizadas**: Configurar las páginas de error que el servidor devolverá en caso de fallos (404, 500, etc.).
- **Límites de tamaño del cuerpo del cliente**: Definir el tamaño máximo permitido para los cuerpos de las peticiones.
- **Métodos aceptados**: Para cada ruta, puedes definir los métodos HTTP permitidos.
- **Rutas**: Define las rutas del servidor y dónde se deben buscar los archivos que se sirvan.
- **CGI (Common Gateway Interface)**: Ejecutar programas CGI basados en la extensión del archivo (por ejemplo, `.php` o `.py`).
  
---

### Paso 4: Implementar las funciones HTTP necesarias

Tu servidor debe implementar los siguientes métodos HTTP:

1. **GET**: Para obtener recursos del servidor (como una página HTML o una imagen).
2. **POST**: Para enviar datos al servidor, generalmente utilizado en formularios.
3. **DELETE**: Para eliminar un recurso en el servidor.

#### 4.1. **Manejo de las respuestas HTTP**:

- Cada petición debe recibir una respuesta HTTP con un código de estado adecuado:
  - **200 OK**: Cuando la petición ha sido procesada correctamente.
  - **404 Not Found**: Cuando el recurso solicitado no existe.
  - **500 Internal Server Error**: Para errores internos del servidor.

#### 4.2. **CGI (Common Gateway Interface)**:

- Tu servidor debe poder ejecutar **CGIs** para manejar peticiones dinámicas. Esto implica que, cuando una petición llega con un archivo que requiera un programa externo (como PHP), el servidor debe ejecutar el CGI correspondiente y devolver el resultado.
  
---

### Paso 5: Manejo de múltiples conexiones

El servidor debe ser capaz de manejar **múltiples clientes simultáneamente**. Para lograrlo, debes utilizar mecanismos de **I/O no bloqueante**. Esto se consigue con `poll()`, `select()`, `epoll()`, o su equivalente.

- **`poll()`** (o su equivalente) debe ser usado para monitorizar múltiples descriptores de archivo al mismo tiempo, tanto para lectura como para escritura.
  
---

### Paso 6: Implementar un sistema de pruebas

Debes asegurarte de que tu servidor es capaz de manejar múltiples escenarios:

- **Pruebas con un navegador web**: Abre tu servidor en un navegador y verifica que sirva contenido estático y ejecute CGI.
- **Pruebas de estrés**: Simula múltiples conexiones para verificar que el servidor sigue respondiendo correctamente bajo carga.
- **Comparación con NGINX**: Verifica que las respuestas de tu servidor sean similares a las de NGINX en términos de códigos de estado y comportamientos de las cabeceras.
  
---

### Paso 7: Bonus (Opcional)

Si implementas todas las partes obligatorias correctamente, puedes añadir algunas características extra como:

- **Manejo de cookies y sesiones**.
- **Soporte para múltiples CGI**.
  
---

### **Recursos útiles**

- **Documentación de HTTP**: Familiarízate con los detalles de la especificación HTTP para manejar correctamente las peticiones y respuestas.
- **RFC de HTTP/1.1**: La especificación oficial de HTTP 1.1 es esencial para entender cómo implementar las cabeceras y el manejo de las conexiones.

---

### **Conclusión**

Este proyecto tiene como objetivo que comprendas a fondo cómo funciona un servidor HTTP y cómo se gestionan las conexiones y peticiones en un entorno web. Es un proyecto desafiante que no solo te ayudará a mejorar tus habilidades en **C++**, sino también a comprender cómo funcionan los protocolos de comunicación en la web.

Si tienes alguna duda específica sobre alguna parte del proyecto, no dudes en preguntar. ¡Buena suerte!



## Introduction 🇬🇧

This project involves writing your own HTTP server using **C++ 98**. The goal is to build a functional web server that can process HTTP requests, handle client connections, and serve static and dynamic content.

---

### Step 1: Understanding HTTP and the Server

**HTTP (Hypertext Transfer Protocol)** is a communication protocol used on the web to transfer documents and data between servers and clients (typically web browsers). In this project, your objective is to implement an HTTP server that can:

- Process and deliver web pages (HTML, images, etc.).
- Handle client requests using the HTTP methods: **GET**, **POST**, and **DELETE**.
- Respond with correct HTTP status codes and headers.

#### Role of the HTTP Server:
- The server stores and delivers web resources to clients.
- Clients (such as browsers) send requests, and the server responds with the appropriate content (or an error if it cannot serve the request).

---

### Step 2: Project Technical Requirements

#### 2.1. General Requirements:

- **Language**: The server must be implemented in **C++ 98**.
- **Execution**: The program will run with a configuration file as an argument.
- **No external servers**: You cannot execute another server like NGINX or Apache.
- **Non-blocking**: Use non-blocking I/O techniques to handle multiple connections without blocking the server.
- **Error Pages**: The server must provide default error pages if custom ones are not provided.

#### 2.2. Compilation:

- Provide a **Makefile** with commands: `all`, `clean`, `fclean`, `re`.
- Compile the code using the following flags:
  ```bash
  -Wall -Wextra -Werror -std=c++98
  ```

#### 2.3. Allowed System Functions:

- Functions like `execve`, `dup`, `socket`, `listen`, `send`, `recv`, `poll`, among others, can be used for socket and file handling.

---

### Step 3: Server Configuration

Your server must accept a configuration file, which defines important parameters such as:

#### 3.1. Parameters to include in the configuration file:

- **Port and host**: The server must be able to listen on multiple ports and hosts.
- **Server names**: Define server names for different virtual hosts.
- **Error Pages**: Customize error pages or use defaults.
- **Client body size limits**: Set limits for client request body sizes.
- **Accepted Methods**: Define accepted HTTP methods for each route.
- **Routes**: Define routes for static content and CGI execution.

---

### Step 4: Implement HTTP Methods

Your server must implement the following HTTP methods:

1. **GET**: Retrieve resources from the server.
2. **POST**: Send data to the server (such as form submissions).
3. **DELETE**: Remove a resource from the server.

#### 4.1. Handle HTTP Responses Correctly

- Return appropriate status codes for each request:
  - **200 OK**: Success.
  - **404 Not Found**: Resource not found.
  - **500 Internal Server Error**: Internal error on the server.

#### 4.2. Implement CGI (Common Gateway Interface)

- Support execution of CGI scripts for dynamic content (e.g., PHP, Python).
- Handle chunked requests and responses appropriately.

---

### Step 5: Handle Multiple Connections

The server must handle **multiple clients simultaneously**. To do this, you must use **non-blocking I/O** mechanisms like `poll()`, `select()`, or `epoll()` to manage several connections efficiently.

---

### Step 6: Testing the Server

- **Browser Testing**: Ensure the server works by testing it in a browser.
- **Stress Testing**: Simulate multiple connections to verify server stability.
- **Comparison with NGINX**: Use NGINX as a reference for behavior and performance.

---

### Step 7: Bonus Features (Optional)

After completing the mandatory parts, consider adding extra features such as:

- **Cookie and session management**.
- **Support for multiple CGI scripts**.

---

### Conclusion

This project aims to deepen your understanding of HTTP servers and how connections and requests are managed in web environments. It's a challenging but rewarding project that will enhance your **C++** and web protocol skills.

Good luck!


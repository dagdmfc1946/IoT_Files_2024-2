// Función que gestiona la petición de la WebApp
function doGet() {
    return HtmlService.createHtmlOutputFromFile('index'); // Carga el archivo index.html
  }
  
  // Función que se ejecuta en el servidor y llama a la función calculadora
  function ejecutarOperacion(operacion, a, b) {
    a = parseFloat(a); // Convertir los valores a número
    b = parseFloat(b);
  
    // Llama a la función calculadora original
    return calculadora(operacion, a, b);
  }
  
  // Función que reinicia las casillas de la WebApp
  function limpiarCasillas() {
    return true; // Esto es solo un marcador, no se requiere lógica adicional
  }
  
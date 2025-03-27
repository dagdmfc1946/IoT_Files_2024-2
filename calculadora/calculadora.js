/**
 * Realiza una de las diferentes 4 operaciones (suma, resta, multiplicación y división) 
 * y según la operación seleccionada, se retorna el respectivo valor de dicha operación.
 *
 * @function: calculadora(operacion, a, b)
 * @param {string} operacion - La operación a realizar. Puede ser "suma", "resta", "multiplicacion" o "division".
 * @param {number} a - Primer número.
 * @param {number} b - Segundo número.
 * @returns {number} El resultado de la operación seleccionada entre 'a' y 'b'.
 * @example
 * // calculadora("suma", 4, 2);
 * // Devuelve: 6
 * 
 * // calculadora("resta", 4, 2);
 * // Devuelve: 2
 * 
 * // calculadora("multiplicacion", 4, 2);
 * // Devuelve: 8
 * 
 * // calculadora("division", 4, 2);
 * // Devuelve: 2
 */
function calculadora(operacion, a, b) {
    switch (operacion) {
      case 'suma':
        return a + b;
      case 'resta':
        return a - b;
      case 'multiplicacion':
        return a * b;
      case 'division':
        if (b !== 0) {
          return a / b;
        } else {
          throw new Error("No se puede dividir entre 0");
        }
      default:
        throw new Error("Operación no válida. Utiliza 'suma', 'resta', 'multiplicacion' o 'division'.");
    }
  }
  
  function pruebas() {
    var a = calculadora("suma",4,2);
    console.log("El resultado de la operaciòn es: " + a);
  }
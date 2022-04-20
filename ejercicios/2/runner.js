/**
 * El runner se corre desde el root del proyecto, de esta forma:
 * node ./ejercicios/1/runner.js <pathInstancias> <sortArg>
 * Donde:
 *   - pathInstancias, string que indica el directorio con las instancias que se quieren correr
 *   - sortArg, string que indica el ordenamiento a utilizar (puede ser asc o desc)
 */

const { execFileSync } = require('child_process');
const fs = require('fs');

const filePath = './build/Release/ejercicio2.exe';
const basePathInstancias = process.argv.length > 2 ? process.argv[2] : './instancias/test/';
const timeoutPorInstancia = 600000;
let printTimings = '';

fs.readdirSync(basePathInstancias).forEach(instancia => {
  try {
    const stdout = execFileSync(
      filePath,
      [`${basePathInstancias}${instancia}`, process.argv.length > 3 ? process.argv[3] : 'desc', 'debug'],
      { encoding: 'utf-8', timeout: timeoutPorInstancia },
    );
    if (stdout) {
      console.log(stdout);
      const executionTime = stdout.split('Tiempo de ejecucion: ');
      const timing = executionTime[1].split(' ms')[0].trim();
      const influencia = executionTime[0].split("Influencia: ")[1].trim();
      const newLine = `${instancia.padEnd(25, ' ')}${influencia.padEnd(15, ' ')}${timing.padEnd(15, ' ')}\n`;
      printTimings = `${printTimings}${newLine}`;
    };
  } catch (error) {
    console.warn(`Timeout de ${timeoutPorInstancia / 1000} segundos excedido mientras se corría la instancia ${instancia}.\n`);
    const newLine = `${instancia.padEnd(25, ' ')}${'undefined'.padEnd(15, ' ')}${`> ${timeoutPorInstancia}`.padEnd(15, ' ')}\n`;
    printTimings = `${printTimings}${newLine}`;
    // Hacemos un return para seguir con la próxima iteración
    return;
  }
});

console.log(printTimings);
fs.writeFileSync('timings.txt', printTimings, { flag: 'w' }, err => {
  console.log("Hubo un error al escribir timings.txt, ", err);
});

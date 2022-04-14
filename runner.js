const { execFileSync, execSync } = require('child_process');
const fs = require('fs');

const filePath = './build/Release/algo3-tp1.exe';

const basePathInstancias = process.argv.length > 2 ? process.argv[2] : './instancias/test/';
let printTimings = '';

fs.readdirSync(basePathInstancias).forEach(instancia => {
  const stdout = execFileSync(filePath, [`${basePathInstancias}${instancia}`, 'desc', 'debug'], { encoding: 'utf-8' });
  if (stdout) {
    console.log(stdout);
    const executionTime = stdout.split('Tiempo de ejecucion: ');
    const timing = executionTime[1].split(' ms')[0].trim();
    const influencia = executionTime[0].split("Influencia: ")[1].trim();
    const newLine = `${instancia}: ${influencia}, ${timing}ms\n`;
    printTimings = `${printTimings}${newLine}`;
  };
});

console.log(printTimings);
fs.writeFileSync('timings.txt', printTimings, { flag: 'w' }, err => {
  console.log("Hubo un error al escribir timings.txt, ", err);
});

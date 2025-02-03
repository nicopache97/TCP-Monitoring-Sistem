import net from 'net';
import chalk from 'chalk';
import os from 'os';
import readline from 'readline';

const PORT = 12345;
let lastConnectedSocket = null;
let bufferedValue = null;
let isWriting = false;

function getAllLocalIPs() {
    const interfaces = os.networkInterfaces();
    const ips = [];
    
    for (const devName in interfaces) {
        const iface = interfaces[devName];
        for (const details of iface) {
            if (details.family === 'IPv4' && !details.internal) {
                ips.push(details.address);
            }
        }
    }
    return ips;
}

const server = net.createServer((socket) => {
    if (!isWriting) {
        console.log(chalk.green(`[CONEXIÓN] Nuevo dispositivo conectado desde: ${socket.remoteAddress}`));
    }
    lastConnectedSocket = socket;

    socket.on('data', (data) => {
        const message = data.toString().trim();
        if (!isWriting) {
            console.log("Valor recibido: ", message);
        }
        
        if (bufferedValue !== null) {
            socket.write(`${bufferedValue}\n`);
            if (!isWriting) {
                console.log(chalk.blue(`Valor buffereado enviado: ${bufferedValue}`));
            }
            bufferedValue = null;
        }
    });

    socket.on('close', () => {
        if (!isWriting) {
            console.log(chalk.yellow(`[DESCONEXIÓN] Dispositivo desconectado de: ${socket.remoteAddress}`));
        }
        if (lastConnectedSocket === socket) {
            lastConnectedSocket = null;
        }
    });

    socket.on('error', (err) => {
        if (!isWriting) {
            console.log(chalk.red(`[ERROR] ${err.message}`));
        }
    });
});

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

rl.on('line', (input) => {
    isWriting = false;
    if (input.toLowerCase() === 'q') {
        rl.close();
        process.exit(0);
    }

    const newValue = parseInt(input);
    if (isNaN(newValue)) {
        console.log(chalk.red('Por favor, ingrese un número válido.'));
    } else if (newValue < 0 || newValue > 4095) {
        console.log(chalk.red('El valor debe estar entre 0 y 4095.'));
    } else if (lastConnectedSocket) {
        lastConnectedSocket.write(`${newValue}\n`);
        console.log(chalk.blue(`Nuevo valor de luz enviado: ${newValue}`));
    } else {
        bufferedValue = newValue;
        console.log(chalk.yellow(`Valor ${newValue} guardado en buffer. Se enviará cuando un dispositivo se conecte.`));
    }

    rl.prompt();
});

rl.on('SIGINT', () => {
    rl.question('¿Estás seguro de que quieres salir? (s/N) ', (answer) => {
        if (answer.match(/^s$/i)) {
            rl.close();
        } else {
            rl.prompt();
        }
    });
});

server.listen(PORT, '0.0.0.0', () => {
    console.log(chalk.green(`Servidor TCP iniciado:`));
    
    const localIPs = getAllLocalIPs();
    localIPs.forEach(ip => {
        console.log(chalk.green(`IP: ${ip}`));
    });
    
    console.log(chalk.green(`Puerto: ${PORT}`));
    
    rl.setPrompt('Ingrese un nuevo valor de luz (o "q" para salir): ');
    rl.prompt();
});

rl.on('SIGTSTP', function () {
    // This will override SIGTSTP and prevent the program from going to the background
    console.log("SIGTSTP desactivado");
});

process.stdin.on('keypress', (str, key) => {
    if (key.name !== 'return' && key.name !== 'enter') {
        isWriting = true;
    }
});
import net from 'net';
import chalk from 'chalk';
import os from 'os';



const PORT = 12345;

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
    console.log(chalk.green(`[CONEXIÓN] Nuevo dispositivo conectado desde: ${socket.remoteAddress}`));

    socket.on('data', (data) => {
        const message = data.toString().trim();
        
        console.log("Valor recibido: ", message)
    });

    socket.on('close', () => {
        console.log(chalk.yellow(`[DESCONEXIÓN] Dispositivo desconectado de: ${socket.remoteAddress}`));
    });

    socket.on('error', (err) => {
        console.log(chalk.red(`[ERROR] ${err.message}`));
    });
});

// Escuchar en todas las interfaces
server.listen(PORT, '0.0.0.0', () => {
    console.log(chalk.green(`Servidor TCP iniciado:`));
    
    // Mostrar todas las IPs locales
    const localIPs = getAllLocalIPs();
    localIPs.forEach(ip => {
        console.log(chalk.green(`IP: ${ip}`));
    });
    
    console.log(chalk.green(`Puerto: ${PORT}`));
});
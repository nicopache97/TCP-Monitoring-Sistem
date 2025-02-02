const net = require('net');

// Crear el cliente TCP
const client = new net.Socket();

const HOST = '192.168.0.215';
const PORT = 12345;

client.connect(PORT, HOST, () => {
  console.log(`Conectado al servidor en ${HOST}:${PORT}`);
  
  // Enviar un mensaje al servidor
  client.write('Hola, servidor!');
});

// Manejar la desconexión del servidor
client.on('close', () => {
  console.log('Conexión cerrada');
});

// Manejar errores
client.on('error', (err) => {
  console.error('Error en la conexión:', err.message);
});

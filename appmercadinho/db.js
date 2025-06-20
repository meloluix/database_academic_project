const sqlite3 = require('sqlite3').verbose();

// Conecta ao banco de dados (cria se não existir)
const db = new sqlite3.Database('./comercio.db', (err) => {
  if (err) {
    console.error('Erro ao conectar ao banco de dados:', err.message);
  } else {
    console.log('Conectado ao banco de dados SQLite');
    criarTabelas();
  }
});

// Cria as tabelas necessárias
function criarTabelas() {
  db.run(`CREATE TABLE IF NOT EXISTS produtos (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nome TEXT NOT NULL,
    preco REAL NOT NULL,
    quantidade INTEGER NOT NULL
  )`);
  
  db.run(`CREATE TABLE IF NOT EXISTS vendas (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    produto_id INTEGER NOT NULL,
    quantidade INTEGER NOT NULL,
    data TEXT NOT NULL,
    FOREIGN KEY(produto_id) REFERENCES produtos(id)
  )`);
}

module.exports = db;
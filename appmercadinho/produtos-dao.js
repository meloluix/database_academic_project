const db = require('./db');

class ProdutoDAO {
  static adicionar(nome, preco, quantidade) {
    return new Promise((resolve, reject) => {
      const sql = 'INSERT INTO produtos (nome, preco, quantidade) VALUES (?, ?, ?)';
      db.run(sql, [nome, preco, quantidade], function(err) {
        if (err) reject(err);
        else resolve(this.lastID);
      });
    });
  }

  static listar() {
    return new Promise((resolve, reject) => {
      db.all('SELECT * FROM produtos', [], (err, rows) => {
        if (err) reject(err);
        else resolve(rows);
      });
    });
  }

  static atualizarEstoque(id, novaQuantidade) {
    return new Promise((resolve, reject) => {
      const sql = 'UPDATE produtos SET quantidade = ? WHERE id = ?';
      db.run(sql, [novaQuantidade, id], function(err) {
        if (err) reject(err);
        else resolve(this.changes);
      });
    });
  }
}

module.exports = ProdutoDAO;
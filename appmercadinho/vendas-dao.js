const db = require('./db');

class VendaDAO {
  static registrar(produtoId, quantidade) {
    return new Promise((resolve, reject) => {
      const data = new Date().toISOString();
      const sql = 'INSERT INTO vendas (produto_id, quantidade, data) VALUES (?, ?, ?)';
      db.run(sql, [produtoId, quantidade, data], function(err) {
        if (err) reject(err);
        else resolve(this.lastID);
      });
    });
  }
}

module.exports = VendaDAO;
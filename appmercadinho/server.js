const express = require('express');
const app = express();
const ProdutoDAO = require('./produtos-dao');
const VendaDAO = require('./vendas-dao');

// Configurações
app.set('view engine', 'ejs');
app.use(express.urlencoded({ extended: true }));
app.use(express.static('public'));

// Rotas
app.get('/', async (req, res) => {
  try {
    const produtos = await ProdutoDAO.listar();
    res.render('index', { produtos });
  } catch (err) {
    res.status(500).send('Erro ao carregar produtos');
  }
});

app.post('/produtos', async (req, res) => {
  const { nome, preco, quantidade } = req.body;
  try {
    await ProdutoDAO.adicionar(nome, parseFloat(preco), parseInt(quantidade));
    res.redirect('/');
  } catch (err) {
    res.status(500).send('Erro ao adicionar produto');
  }
});

app.post('/vendas', async (req, res) => {
  const { produtoId, quantidade } = req.body;
  const qtd = parseInt(quantidade);
  
  try {
    const produtos = await ProdutoDAO.listar();
    const produto = produtos.find(p => p.id === parseInt(produtoId));
    
    if (!produto) {
      return res.status(404).send('Produto não encontrado');
    }
    
    if (produto.quantidade < qtd) {
      return res.status(400).send('Estoque insuficiente');
    }
    
    // Atualiza estoque e registra venda
    await ProdutoDAO.atualizarEstoque(produtoId, produto.quantidade - qtd);
    await VendaDAO.registrar(produtoId, qtd);
    
    res.redirect('/');
  } catch (err) {
    res.status(500).send('Erro ao registrar venda');
  }
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Servidor rodando em http://localhost:${PORT}`);
});
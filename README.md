# Projeto2-Ciferri
Trabalho Pratico de Arquivos

# Integrantes
 - Bruno Flavio- 9791330
 - Edson Toma - 9791305
 - Luca Porto - 9778943
 - Murilo Baldi - 9790990

# Objetivo
Desenvolver um programa capaz de, a partir de um arquivo base, gerar arquivos com organizacoes especificas e seus respectivos arquivos de indice. Tambem foi solicitado que um usuario pudesse inserir ou remover dados dos arquivos gerados, sendo que os arquivos com organizacoes especificas nao sao alterados diretamente pelo usuario. Essas operacoes sao executadas pelo programa com base nos arquivos de indice, a fragmentacao interna pode ser tratadas com tres tecnicas diferentes (first-fit, best-fit, worst-fit).

# O programa
Deve-se desenvolver um programa que possa disponibilizar possibilidade de escolha de qual funcionalidade sera utilizada, alem da impressao de mensagens de erro quando se achar necessacio. Suas funcionalidades sao:
  - Possibilidade de geracao de arquivos de saida com organizacao pre-definida, um para cada tecnica de reutilizacao de espaco diferente (first-fit, best-fit, worst-fit);
  - Possibilidade de geracao de arquivos de indice primario com base no campo ticket dos arquivos acima citados;
  - Possibilidade de remocao nos arquivos com base no ticket, informando se houve ou nao sucesso na operacao;
  - Possibilidade de insercao nos arquivos informando se hoube ou nao sucesso na operacao;
  - Possibilidade de visualizacao de estatisticas sobre os arquivos de indice, mostrando a quantidade de entradas de cada um;
    - Ainda nas estatisticas, o programa deve apresentar a possibilidade de impressao dos registros dos tres indices simultaneamente e um por vez.
  - Possibilidade de visualizacao de estatisticas sobre a lista de registros removidos, indicando a quantidade de registros removidos cada arquivo possui.
    - Nesta ultima opcao deve-se, ainda, permitir ao usuario escolher se deseja visualizar esses registros, um a um, ou nao.

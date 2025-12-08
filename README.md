Instruções para execução adequada do jogo:
1 - Configurar o makefile para o sistema operacional adequado. Nele, há opções para Mingw, Linux e MacOS comentadas, então basta descomentar a variável responsável pelo sistema operacional que o usuário necessite.
2 - Executar o arquivo makefile para compilar o jogo e gerar o executável Janela_jogo.exe. Após isso, basta executar o próprio binário que o jogo irá iniciar normalmente. (basta usar comando make no diretório do game)
3 - A versão enviada deste jogo foi compilada com as bibliotecas do Windows, portanto é necessária uma nova compilação do código.

CONTROLES:
Movimentação de Tanque: Acelerar e girar, sem ré.
UpArrow --> Acelerar a nave.
LeftArrow & RightArrow --> Girar o leme na direção desejada.

Disparo: Todos os objetos são destruídos com apenas um tiro e todos os inimigos concedem 100 pontos.
Space --> Atirar com a nave na direção em que ela aponta.

Obs: Originalmente, o jogo foi construído por cada membro em uma máquina diferente. Fizemos testes no Windows 10 e no Fedora Hat Linux. Para o Windows, utilizamos MSYS Mingw64 e, por isso, usamos makefile e ./Janela_jogo.exe para executar o binário do jogo. Evidentemente, será diferente dependendo do sistema operacional, mas o procedimento é basicamente o mesmo.
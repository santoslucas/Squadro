# Jogabilidade
Após	 estabelecida	 a	 configuração	 inicial	 do	 tabuleiro,	 começa-se	 pelo	
Jogador	Vermelho	que	poderá	escolher	qualquer	uma	de	suas	peças	para	efetuar	
uma	jogada.	A	peça	deverá	movimentar,	no	sentido	indicado	pelo	seu	 “nariz”,	a	
quantidade	de	casas	indicada	pela	 força	de	sua	casa	de	 origem.	Por	exemplo,	 o	
Jogador	 Vermelho	 poderia	 escolher	 sua	 terceira	 peça	 que	 moveria	 duas	 casas.	
Depois	o Jogador	Azul	que	poderia	escolher	sua	segunda	peça	para	movimentar.	
Nesse	caso,	a	peça	azul	“pularia”	a	peça	vermelha	e	terminaria	na	primeira	casa	
vazia	adjacente	a	ela	(independente	de	sua	força),	e	a	peça	adversária	retornaria	
a	seu	último	ponto	de	origem.	Essas	duas	jogadas	podem	ser	vistas	a	seguir.
<p> <img height="300em" src="https://github.com/santoslucas/Squadro/blob/main/images/1.png/">
<p> Agora	o	Jogador	Vermelho poderia	escolher	sua	segunda	peça	para	forçar	
a	peça	adversária	a	retornar	ao	seu	ponto	de	origem.	Note	que	nesse	caso	a	peça	
escolhida	tem	força	três,	mas	ainda	sim	ela	foi	obrigada	a	parar	na	primeira	casa	
vazia	 imediatamente	 posterior	 a	 peça	 adversária.	 Essa	 jogada	 pode	 ser	 vista	 a	
seguir	 à	 esquerda.	 Depois	 o	 Jogador	 Azul	 poderia	 escolher	 outra	 peça	 para	
movimentar,	 como	 a	 quarta	 peça	 cuja	 jogada	 não	 será	 mostrada.	 O	 Jogador	
Vermelho	poderia	então	escolher	novamente	sua	segunda	peça	para	movimentar	
três casas.	Nesse	caso,	essa	peça	chegaria	ao	ponto	final	da	primeira	parte	de	sua	
travesseia	e	 teria	seu	sentido	invertido,	conforme	visto	a	seguir	à	direta.	Agora	
essa	peça	deverá	mover	apenas	uma	casa	por	vez	no	seu	caminho	de	volta.
<p> <img height="300em" src="https://github.com/santoslucas/Squadro/blob/main/images/2.png/">
<p> Considere	 a	 última	 jogada	 do	 Jogador	 Vermelho	 que	 levou	 à	 situação	 a	
seguir	(antes).	O Jogador	Azul	poderia	escolher	a	sua	quarta	peça	para	 forçar	a
peça	 adversária	 a	 retornar	 ao	 seu	 último	 ponto	 de	 origem,	 ou	 seja,	 ao	 começo	
para	realizar	somente	a	segunda	parte	de	sua	travessia	(depois).
<p> <img height="300em" src="https://github.com/santoslucas/Squadro/blob/main/images/3.png/">
<p> Considere	 uma	 outra	 situação	 onde	 agora é	 a	 vez	 do	Jogador	 Vermelho	
escolher	 uma	 peça (à	 esquerda).	 Caso	 ele	 escolha	 a	 sua	 terceira	 peça,	 ela fará	
com	que	as	duas	peças	adversárias	que	estão	em	seu	caminho	voltem	para	sua	
última	 origem e	 a	 peça	 escolhida termine	 sua	 jogada	 na	 próxima	 casa livre (à	
direita).	Como	nesse	caso	a	peça	vermelha	chegou	ao	final	da	primeira	parte	da	
travessia,	seu	sentido	é	invertido.
<p> <img height="300em" src="https://github.com/santoslucas/Squadro/blob/main/images/4.png/">
<p> O jogador	 que	 fizer	 a	 travessia	 de	 ida	 e	 volta	 de quatro	 de	 suas	 cinco	
peças deverá	ser declarado	como	vencedor.	Para	isso	deve-se	exibir	um	pop-up	
mostrando	de	qual	jogador	foi vitória conforme	pode	ser	visto	a	seguir.
<p> <img height="300em" src="https://github.com/santoslucas/Squadro/blob/main/images/5.png/">

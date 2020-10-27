
void inicializarGrafico(int index,float x,float y,int larg, int alt, int id){
	graficos[index].emUso = true;
	
	graficos[index].local.coordx = x;
	graficos[index].local.coordy = y;
	graficos[index].local.largura = larg;
	graficos[index].local.altura = alt;

	graficos[index].id = id;
	
}

void desativarGrafico(int index){
	graficos[index].emUso = false;	
	
	graficos[index].local.coordx = 0;
	graficos[index].local.coordy = 0;
	graficos[index].local.largura = 0;
	graficos[index].local.altura = 0;

	graficos[index].id = 0;
}


void resetarTodosGraficos(){
	for(int i=0;i<(MaxEntitiesPerScreen-1);i++)
		desativarGrafico(i);
}

void inicializarVidas(){ // Utiliza os index de 10 a 18
	for(int i=0;i<8;i++){
		inicializarGrafico(i,-600+40*(i), 320, 32, 32, 36);
	}
}

void criarMenuPrincipal(){ // Cria os icones do menu principal
	for(int i=0;i<3;i++){
		inicializarGrafico(i,0, 0-(100*i), 300, 100, 12+i);
	}
	inicializarGrafico(3 ,0, 200, 500, 250, 11);
	inicializarHighscore();
}

void criarIconeFase(){ // Cria os ícones do menu de seleção de fase 
	for(int i=0;i<15;i++){
		inicializarGrafico(i,-384+(192*(i%5)), 192-(192*(i/5)), 128, 128, i+18);
	}
}

void inicializarPontos(){ // Cria os ícones de pontos na tela
	inicializarGrafico(9,450, 320, 96,32, 35);
	for(int i=10;i<16;i++){
		inicializarGrafico(i,600-(18*(i-10)),320, 16,32, 0);
	}
}

void criarGameOverScreen(){ // Cria os ícones da tela de game over
	inicializarGrafico(0, 0, 0, 300, 100, 13);
	inicializarGrafico(1 ,0, -100, 300, 100, 15);
	inicializarGrafico(2 ,0, -200, 300, 100, 14);
	
	inicializarGrafico(3, 0, 200, 500, 250, 10);
	inicializarHighscore();
	
	inicializarGrafico(11, 450, 286, 96,32, 35);
	for(int i=12;i<18;i++){
		inicializarGrafico(i,600-(18*(i-12)), 286, 16,32, ((int) (jogador.pontos/pow(10, (i-12))))%10);
	}
}

void criarMenuPausa(){ // Cria os icones do menu de pausa
	inicializarGrafico(0 ,0, 150, 384, 128, 16);
	inicializarGrafico(1 ,0, 0, 384, 128, 15);
	inicializarGrafico(2 ,0, -150, 384, 128, 14);
}

void inicializarHighscore(){ // Cria os ícones de HIGHSCORE no jogo
	inicializarGrafico(4, 418, 320, 160,32, 34);
	for(int i=5;i<11;i++){
		inicializarGrafico(i,600-(18*(i-5)),320, 16,32,((int) (maiorPontuacao/pow(10, (i-5))))%10);
	}
}

// Troca o botão selecionado no menu, em acordo da maneira em que os botões na tela foram precionados
void selecionarBotao(int firstIndex,int numHorizButton, int numVertButton){
	if(leftPressed){
		if((menuIndex-firstIndex)%numHorizButton!=0){
			menuIndex--;
			faseAtual = menuIndex;
		}
	}
	if(rightPressed){
		if((menuIndex-firstIndex)%numHorizButton!=numHorizButton-1){
			menuIndex++;
			faseAtual = menuIndex;
			}
	}
	if(upPressed){
		if((menuIndex-firstIndex)/numHorizButton!=0){
			menuIndex-=numHorizButton;
			faseAtual = menuIndex;
		}
	}
	if(downPressed){
		if((menuIndex-firstIndex)/numHorizButton!=numVertButton-1){
			menuIndex+=numHorizButton;
			faseAtual = menuIndex;
		}
	}
}

// Recebe as dimensões da imagem, em altura e largura em pixeis
TexturaTamanho getDimensoesTextura(GLuint id){
	TexturaTamanho size;
	if(id==idSprites){
		size.larguraEmPixels = 672;
		size.alturaEmPixels = 128;
	}
	if(id==idMenu){
		size.larguraEmPixels = 1408;
		size.alturaEmPixels = 640;
	}
	if(id==idBackGround1){
		size.larguraEmPixels = 1280;
		size.alturaEmPixels = 720;
	}
	return size;
}

// identifica qual é a textura da imagem pelo seu id
TexturaInfo getTextura(int id){
	TexturaInfo imagem;
	// se id igual a -1, não há textura
	
	
	if((id>=0)||(id<=9)){ // Números (de 0 a 9)
		imagem.xInicial = 1024+32*(id);
		imagem.yInicial = 0;
		imagem.largura = 32;
		imagem.altura = 64;
		
		imagem.textura = idMenu;
	}
	if(id==10){ // GameOver Screen
		imagem.xInicial = 0;
		imagem.yInicial = 0;
		imagem.largura = 512;
		imagem.altura = 256;
		
		imagem.textura = idMenu;
	}
	if(id==11){ // Título Do Jogo
		imagem.xInicial = 512;
		imagem.yInicial = 0;
		imagem.largura = 512;
		imagem.altura = 256;
		
		imagem.textura = idMenu;
	}
	if((id>=12)&&(id<=17)){ // Placas do menu
		imagem.xInicial = 0+384*((id-12)/3);
		imagem.yInicial = 512-128*((id-12)%3);
		imagem.largura = 384;
		imagem.altura = 128;
		
		imagem.textura = idMenu;
	}
	if((id>=18)&&(id<=33)){ // Caixas de Seleção de fase (da fase 1 até endless)
		imagem.xInicial = 768+128*((id-18)%5);
		imagem.yInicial = 512-128*((id-18)/5);
		imagem.largura = 128;
		imagem.altura = 128;

		imagem.textura = idMenu;
	}
	if(id==34){ // Palavra Highscore:
		imagem.xInicial = 1024;
		imagem.yInicial = 64;
		imagem.largura = 320;
		imagem.altura = 64;

		imagem.textura = idMenu;
	}
	if(id==35){ // Palavra Score:
		imagem.xInicial = 1152;
		imagem.yInicial = 64;
		imagem.largura = 192;
		imagem.altura = 64;

		imagem.textura = idMenu;
	}
	if(id==36){ // Coração/Vida do Jogador
		imagem.xInicial = 1344;
		imagem.yInicial = 0;
		imagem.largura = 64;
		imagem.altura = 64;

		imagem.textura = idMenu;
	}
	if((id>=37)&&(id<=39)){// Inimigo
		imagem.xInicial = 0+128*(id-37);
		imagem.yInicial = 0;
		imagem.largura = 128;
		imagem.altura = 128;

		imagem.textura = idSprites;
	}
	if((id>=40)&&(id<=42)){// Jogador
		imagem.xInicial = 384+64*(id-40);
		imagem.yInicial = 0;
		imagem.largura = 64;
		imagem.altura = 128;

		imagem.textura = idSprites;
	}
	if((id>=43)&&(id<=45)){ // Powerup, tiro inimigo, seringa
		imagem.xInicial = 576+32*(id-43);
		imagem.yInicial = 96;
		imagem.largura = 32;
		imagem.altura = 32;

		imagem.textura = idSprites;
	}
	if((id>=47)){ // Powerup, tiro inimigo, seringa
		imagem.xInicial = 0;
		imagem.yInicial = 0;
		imagem.largura = 1280;
		imagem.altura = 720;

		imagem.textura = idBackGround1;
	}
	return imagem;
}

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h> 
#include <math.h> 
#include <time.h>
#include <stdbool.h>
#include <string.h>


#include "variaveis_globais.h"
#include "tiros.h"
#include "inimigos.h"
#include "jogador.h"
#include "configuracões.h"
#include "graficos.h"

double contador = 0, initial_time, final_time, frame_count = 0;

GLuint carregaTextura(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0)
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    

    return idTextura;
}

// progride um relogio, seje crescente ou decrescente
int ticTimer(int valor, int limite, bool crescente){
	if(crescente){
		valor++;
		if(valor>limite)
			valor=0;
	}
	else {
		valor--;

		if(valor<0)
			valor=limite; 
	}
	return valor;
}

void runGame(){ // Corre a parte a parte funcional do jogo ( as tropas e o jogador)
	
	identificarMovimentoDoJogador();
	if(jogador.timerDano != 0)
		jogador.timerDano = ticTimer(jogador.timerDano,100,false);
	if(jogador.timerPowerUp != 0)
		jogador.timerPowerUp = ticTimer(jogador.timerPowerUp,500,false);
	if(jogador.timerPowerUp == 0){
		jogador.invulneravel = false;
		menu.armaEquipada.variedade = 0;
		menu.armaEquipada.frequenciaTiro = 128;
	}
	if(nextFrotaTimer != 0)
		nextFrotaTimer = ticTimer(nextFrotaTimer,50, true);
	if(nextFrotaTimer == 50)
		armadas(faseAtual-'A'+1,40,5);
	
	if(jogador.vida==0)
		deathTimer++;
	randomEventTimer = ticTimer(randomEventTimer,10,true);
		
	
	// desloca as frotas
	for(int i=0;i<MaxFrotasPorTela;i++){
		if(frotas[i].ativado){
			bool frotaDerrotada = seFrotaFoiDerrotada(i);
			if(!(frotaDerrotada)){
				for(int j=frotas[i].firstTropa;j<(frotas[i].firstTropa+frotas[i].sizeofFrota);j++){
					if(tropas[j].ativado){
						detectarColisaoEntreTropasEJogador(j);
						// responsavel pelo decresimo do timer especifico da tropa
						if(tropas[j].timer1>0){
							tropas[j].timer1=ticTimer(tropas[j].timer1,10,false);
							shakeEnemy(j);
						}
						// responsável pela morte do inimigo
						if((tropas[j].timer1==0)&&(tropas[j].vida==0)){
							frotas[i].tropasVivas--;
							jogador.pontos+=200;
							if(jogador.pontos>maiorPontuacao)
								maiorPontuacao = jogador.pontos;
							for(int i=10;i<16;i++)
								graficos[i].id = ((int) (jogador.pontos/pow(10, (i-10))))%10;
							randomDeathEvent(j);
							estadoInicialTropa(j);
						}

						if(tropas[j].timer2>0){ // responsável pela animação do inimigo
							if(tropas[j].timer2==1)
								tropas[j].id=38;
							if(tropas[j].timer2==5){
								tropas[j].id=39;
								inimigoAtirar(j);
							}
							tropas[j].timer2=ticTimer(tropas[j].timer2,10,true);
							if(tropas[j].timer2==0)
								tropas[j].id=37;
						}			
					}
				}
				if(randomEventTimer==0)
					randomEvent(i);
				if((globalTimer%(frotas[i].tempoDeslocamento))==0)
					deslocarTropas(i);

				// responsavel pela colisão entre tiro e tropas
				for(int j=0; j<MaxTirosPorTela; j++){
					detectarColisaoEntreTirosEInimigos(i,j);
				}
			}
		}
	}

	if(jogador.timerBala!= 0){
		jogador.timerBala = ticTimer(jogador.timerBala,menu.armaEquipada.frequenciaTiro,true);
		if(jogador.timerBala == 16)
			jogador.id = 40;
		else if(jogador.timerBala == menu.armaEquipada.frequenciaTiro-8)
			jogador.id = 41;
	}
	
	// detecta tiros
	if(upPressed){
		if(jogador.timerBala==0){	// inicializa o timer do tiro
			usarArma(menu.armaEquipada.variedade);
			jogador.timerBala++;
			jogador.id = 42;
		}
	}
		
	for(int i=0; i<MaxTirosPorTela;i++){
		detectarColisaoEntreTirosEJogador(i); // responsável pela colisão entre tiros e o jogador
		if((abs(tiros[i].local.coordy)>370)||(abs(tiros[i].local.coordx)>650))
			estadoInicialTiro(i);	// logo depois que o tiro sai da tela, ele será colocado em estado inicial novamente
		if(tiros[i].ativado){
			tiros[i].local.coordy += tiros[i].velocidadeTiro*cos(tiros[i].angulo);
			tiros[i].local.coordx -= tiros[i].velocidadeTiro*sin(tiros[i].angulo);
		}
		// desloca o tiro
	}
	if(deathTimer!=0) // Responsável pela mecânica de terminar o jogo
		deathTimer= ticTimer(deathTimer, 50, true);
	if(deathTimer==50){
		if(jogador.pontos>maiorPontuacao)
			maiorPontuacao=jogador.pontos;
		resetarTodosGraficos();
		criarGameOverScreen();
		menuIndex=0;
		gamemode=2; // Game over
	}
}

void runMenuPrincipal(){ // Corre a logística do menu principal
	if(upPressed||downPressed){
		if(menuTimer==0){
			menuTimer++;
			selecionarBotao(0, 1, 3);
		}		
	}
}

void runSelecaoFase(){ // Corre a logística do menu de seleção de fases
	if(upPressed||downPressed||leftPressed||rightPressed){
		if(menuTimer==0){
			menuTimer++;
			selecionarBotao(0, 5, 3);
		}		
	}
}

void runGameOver(){ // Corre a logística da tela de gameover
	if(upPressed||downPressed){
		if(menuTimer==0){
			menuTimer++;
			selecionarBotao(0, 1, 3);
		}		
	}
}

void runMenuPausa(){ // Corre a logística do menu de quando o jogo é pausado
	if(upPressed||downPressed){
		if(menuTimer==0){
			menuTimer++;
			selecionarBotao(0, 1, 3);
		}		
	}
}

// responsável pelas atualizacoes ocorridas no jogo
void updateGame() {
	if(gamemode==0)
		runMenuPrincipal();
	if(gamemode==1)
		runGame();
	if(gamemode==2){
		deathTimer = 0;
		runGameOver();
	}
	if(gamemode==4)
		runMenuPausa();
	if(gamemode==5)
		runSelecaoFase();
}

// responsável pelo desenho de qualquer objeto na tela
void desenharObjeto(float x, float y, float z, float larg, float alt, int id){
	// Habilita o uso de texturas
	TexturaInfo imagem = getTextura(id);
	TexturaTamanho size = getDimensoesTextura(imagem.textura);
	
	double x1 = (imagem.xInicial)/size.larguraEmPixels;
	double x2 = (imagem.xInicial + imagem.largura)/size.larguraEmPixels;
	double x3 = (imagem.xInicial + imagem.largura)/size.larguraEmPixels;
	double x4 = (imagem.xInicial)/size.larguraEmPixels;

	double y1 = (imagem.yInicial)/size.alturaEmPixels;
	double y2 = (imagem.yInicial)/size.alturaEmPixels;
	double y3 = (imagem.yInicial + imagem.altura)/size.alturaEmPixels;
	double y4 = (imagem.yInicial + imagem.altura)/size.alturaEmPixels;
	
	
	glBindTexture(GL_TEXTURE_2D, imagem.textura);
	glPushMatrix();
	glTranslatef(x, y, z);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(x1, y1);
        glVertex3f(-larg/2, -alt/2, 0);
	glTexCoord2f(x2, y2);
        glVertex3f( larg/2, -alt/2, 0);
	glTexCoord2f(x3, y3);
        glVertex3f( larg/2,  alt/2, 0);
	glTexCoord2f(x4, y4);
        glVertex3f(-larg/2,  alt/2, 0);
	glEnd();
	glPopMatrix();
}


// responsável pelo desenho do menu principal somente
void DesenharMenus(){
	for(int i=0;i<(MaxEntitiesPerScreen-1);i++){
		if(graficos[i].emUso){
			glColor3f(0.7f, 0.7f, 0.7f);
			if(menuIndex==i)
				glColor3f(0.0f, 1.0f, 0.0f);
			else if(i>3)
				glColor3f(1.0f, 1.0f, 1.0f);
			desenharObjeto(graficos[i].local.coordx, graficos[i].local.coordy, 0.75, graficos[i].local.largura, graficos[i].local.altura, graficos[i].id);
		}	
	}
}

// responsável pelo desenho da tela de game over
void DesenharGameOver(){
	for(int i=0;i<(MaxEntitiesPerScreen-1);i++){
		if(graficos[i].emUso){
			if(i>=3)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if(i!=menuIndex)
				glColor3f(0.7f, 0.7f, 0.7f);
			else
				glColor3f(0.0f, 1.0f, 0.0f);
			desenharObjeto(graficos[i].local.coordx, graficos[i].local.coordy, 0.75, graficos[i].local.largura, graficos[i].local.altura, graficos[i].id);
		}	
	}
}

// responsável pelo desenho do menu do jogo
void DesenharSelecaoDeFase(){
	for(int i=0;i<(MaxEntitiesPerScreen-1);i++){
		if(graficos[i].emUso){
			if(fasesDesbloqueadas-'A'<i)
				glColor3f(0.3f, 0.3f, 0.3f);
			else if(menuIndex==i)
				glColor3f(0.0f, 1.0f, 0.0f);
			else
				glColor3f(0.7f, 0.7f, 0.7f);
		desenharObjeto(graficos[i].local.coordx, graficos[i].local.coordy, 0.75, graficos[i].local.largura, graficos[i].local.altura, graficos[i].id);
		}	
	}
}

// responsável pelo desenho do menu do jogo
void DesenharMenuPausa(){
	for(int i=0;i<(MaxEntitiesPerScreen-1);i++){
		if(graficos[i].emUso){
			glColor3f(0.7f, 0.7f, 0.7f);
			if(menuIndex==i)
				glColor3f(0.0f, 1.0f, 0.0f);
			desenharObjeto(graficos[i].local.coordx, graficos[i].local.coordy, 0.75, graficos[i].local.largura, graficos[i].local.altura, graficos[i].id);
		}	
	}
}

// responsável pelo desenho do jogo
void DesenharGame(){    
	// desenha jogador
	if(jogador.ativado){
		if(jogador.invulneravel)
			glColor3f(0.0f, 1.0f, 1.0f);
		else if(jogador.timerDano == 0)
			glColor3f(1.0f, 1.0f, 1.0f);
		else if (jogador.vida==0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else
			glColor3f(1.0f, 1.0f, 1.0f);
		if(jogador.timerDano%10<5){	
			desenharObjeto(jogador.local.coordx, jogador.local.coordy-20, 0, 64, 128, jogador.id);
		}
	}
	
	glColor3f(0.0f, 0.0f, 1.0f);
	
	for(int i=0;i<MaxFrotasPorTela;i++){
		if(frotas[i].ativado){
			if(menuInterno.mostrarFrota) // desenha o limites da frota
				desenharObjeto(frotas[i].borda.coordx, frotas[i].borda.coordy, 0, frotas[i].borda.largura, frotas[i].borda.altura, 67);
			for(int j=frotas[i].firstTropa;j<(frotas[i].firstTropa+frotas[i].sizeofFrota);j++){
				if(tropas[j].timer1==0)
					glColor3f(0.0f, 1.0f, 0.0f);
				else if (tropas[j].vida==0)
					glColor3f(1.0f, 0.0f, 0.0f);
				else
					glColor3f(1.0f, 1.0f, 0.0f);
		
				if(tropas[j].ativado)
					desenharObjeto(tropas[j].posReal.coordx, tropas[j].posReal.coordy, 0.1, tropas[j].larguraTextura, tropas[j].alturaTextura, tropas[j].id);
			}
		}			
	}
	
	// desenha os tiros na tela
	for(int k=0;k<MaxTirosPorTela;k++){
		if(tiros[k].ativado){
			glColor3f(1.0f, 1.0f, 1.0f);
			if(tiros[k].variedade==3)
				glColor3f(0.0f, 1.0f, 1.0f);
			else if(tiros[k].variedade==4)
				glColor3f(1.0f, 0.5f, 0.0f);
			else if(tiros[k].variedade==5)
				glColor3f(0.0f, 1.0f, 0.3f);
			else if(tiros[k].variedade==6)
				glColor3f(1.0f, 0.0f, 0.0f);
			desenharObjeto(tiros[k].local.coordx, tiros[k].local.coordy, -0.1, tiros[k].local.largura, tiros[k].local.altura, tiros[k].id);
		}
	}
	
	// desenha os corações na tela
	glColor3f(1.0f, 1.0f, 1.0f);
	for(int i=0;i<(jogador.vida);i++){
		desenharObjeto(graficos[i].local.coordx,graficos[i].local.coordy, 1, graficos[i].local.largura, graficos[i].local.altura, graficos[i].id);
	}
	
	for(int i=9;i<(MaxEntitiesPerScreen-1);i++){
		desenharObjeto(graficos[i].local.coordx,graficos[i].local.coordy, 1, graficos[i].local.largura, graficos[i].local.altura, graficos[i].id);
	}
}

void renderGame(){ // Responsável por desenhar todo o jogo, dependendo de seu gamemode
	
	glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0f, 1.0f, 1.0f);
		//if(gamemode!=1)
			desenharObjeto(graficos[MaxEntitiesPerScreen-1].local.coordx,graficos[MaxEntitiesPerScreen-1].local.coordy, -1, graficos[MaxEntitiesPerScreen-1].local.largura, graficos[MaxEntitiesPerScreen-1].local.altura, graficos[MaxEntitiesPerScreen-1].id);
	
	if((gamemode == 0))
		DesenharMenus();
	else if(gamemode == 1)
		DesenharGame();
	else if(gamemode == 2)
		DesenharGameOver();
	else if(gamemode == 4)
		DesenharMenuPausa();
	else if(gamemode == 5)
		DesenharSelecaoDeFase();
	glutSwapBuffers();
	
	frame_count++;
	final_time = time(NULL);
	if (final_time - initial_time > 0){
		contador = frame_count/(final_time-initial_time);
		printf("FPS: %f\n", contador);
		frame_count = 0;
		initial_time = final_time;
		
	}
}

// Arquiva valores importantes no arquivo "data.txt"
void saveGame(){
	data = fopen ("data.txt", "w");
	fprintf(data, "level:(%c),points:(%d)", fasesDesbloqueadas, maiorPontuacao);
	fclose;
}

void exitGame(){ // Sai do jogo, salvando informações no bloco de notas
	saveGame();
	exit(0);
}


// Recebe informacao do teclado determinando quando as teclas estao pressionadas 
void teclaPressionada(unsigned char key, int x, int y)
{
	if(key==13){ // Enter
		if((gamemode==0)&&(menuIndex==0)){ // inicializa o jogo da fase 1
			gamemode=1;
			resetarTodosGraficos();
			inicializarJogador();
			inicializarTropas();
			inicializarFrotas();
			inicializarVidas();
			inicializarPontos();
			faseAtual='A';
			armadas(faseAtual+1-'A',40,5);
			glutPostRedisplay();
		}
		else if(((gamemode==0)&&(menuIndex==1))||((gamemode==2)&&(menuIndex==0))){ // abre o menu para selecionar fases determinadas
			resetarTodosGraficos();
			criarIconeFase();
			menuIndex=0;
			gamemode=5;
			jogador.pontos = 0;
		}
		else if(((gamemode==2)&&(menuIndex==1))||((gamemode==4)&&(menuIndex==1))){ // retorna ao menu principal
			resetarTodosGraficos();
			criarMenuPrincipal();
			menuIndex=0;
			gamemode=0;
			jogador.pontos = 0;
		}
		else if((gamemode==5)&&(menuTimer==0)){ // Entra na fase selecionada no menu de seleção
			if((fasesDesbloqueadas-'A')>=menuIndex){
				gamemode=1;
				faseAtual='A'+menuIndex;
				resetarTodosGraficos();
				inicializarJogador();
				inicializarVidas();
				inicializarTropas();
				inicializarFrotas();
				inicializarPontos();
				armadas(faseAtual+1-'A',40,5);
				glutPostRedisplay();
			}
		}
		else if((gamemode==4)&&(menuIndex==0)){ // Voltar ao jogo
			resetarTodosGraficos();
			inicializarVidas();
			inicializarPontos();
			gamemode=1;
		}
		else if(((gamemode==4)&&(menuIndex==2))||((gamemode==2)&&(menuIndex==2)||(gamemode==0)&&(menuIndex==2))){ // Sair do Jogo
			exitGame();
		}
		menuTimer=1;;
		
	}
	if(key==27){ // ESC
		if((gamemode==1)&&(menuTimer==0)){ // Pausa o jogo
			resetarTodosGraficos();
			criarMenuPausa();
			menuIndex=0;
			gamemode=4;
			saveGame();
		}
		else if((gamemode==4)&&(menuTimer==0)){ // Retorna ao jogo
			resetarTodosGraficos();
			inicializarVidas();
			inicializarPontos();
			gamemode=1;
		}
		else if((gamemode==5)&&(menuTimer==0)){
			resetarTodosGraficos();
			criarMenuPrincipal();
			menuIndex=1;
			gamemode=0;
		}
		menuTimer=1;
}
  
// Recebe informacao do teclado não conhecido por ASCII, determinando quando as teclas estao pressionadas 
void setaPressionada(unsigned key, int x, int y){
	if(key == GLUT_KEY_LEFT) {
        	leftPressed = true;
	}
   	if(key == GLUT_KEY_RIGHT) {    
       		rightPressed = true;
	}
	if(key == GLUT_KEY_UP) {
		upPressed = true;
	}
	if(key == GLUT_KEY_DOWN) {
        	downPressed = true;
	}
	if(key == GLUT_KEY_F1){
		mostrarBorda(true);
	}

}

// Recebe informacao do teclado, determinando quando as teclas sao soltas 
void setaSolta(unsigned key, int x, int y){
	if(key == GLUT_KEY_LEFT) {
        	leftPressed = false;
   	}
   	if(key == GLUT_KEY_RIGHT) {       
       		rightPressed = false;
	}
	if(key == GLUT_KEY_UP) {
		upPressed = false;
	}
	if(key == GLUT_KEY_DOWN) {
        	downPressed = false;
	}
	if(key == GLUT_KEY_F1){
		mostrarBorda(false);
	}
}

void gameLoop(){
	globalTimer = ticTimer(globalTimer,30,true);
	if(menuTimer!=0)
		menuTimer = ticTimer(menuTimer,16,true);
	updateGame();
	glutPostRedisplay();
	glutTimerFunc(1000/60, gameLoop, 0);
	
}

void inicializar() 
{
	// define a cor da borracha
	glClearColor(0.0, 0.0, 0.0, 0.0); 
  
	// Define expessura do ponto para 1j
	glPointSize(1.0); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 

	// Define o tamanho da tela em X, Y e Z
	glOrtho(-640.0, 640.0, -360.0, 360.0, 1, -1);

	glEnable(GL_BLEND);
    	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    	idSprites = carregaTextura("enemySpriteSheet.png");
	idBackGround1 = carregaTextura("background1.png");
	idMenu = carregaTextura("menuSpriteSheet.png");

	inicializarGrafico(MaxEntitiesPerScreen-1 , 0, 0, 1280, 720, 47);
} 


int main (int argc, char** argv) 
{	initial_time = time(NULL);
	srand(time(NULL));
	glutInit(&argc, argv); 
      	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
	glutInitWindowSize(1280, 720); 
	glutInitWindowPosition(1, 1); 
	glutCreateWindow("Corona Invaders"); 	
	
	data = fopen ("data.txt", "a+");
	fread(info, 50, 1, data);
	fclose;
	
	char valor[] = "";
	if(strcmp(info,"")==0){
		fasesDesbloqueadas = 'A';
		maiorPontuacao = 0;
	}
	else{
		fasesDesbloqueadas=info[7];
		int i=18;
		char c;
		while(i<30){
			if(info[i]==')')
				break;
			c = info[i];
			i++;
			strncat(valor, &c, 1);
		}
		maiorPontuacao = atoi(valor);
	}
	criarMenuPrincipal();

	configuracoesIniciais(1);
	gamemode = 0;
	
	// Teclado
	glutKeyboardFunc(teclaPressionada);
	glutSpecialFunc(setaPressionada);
	glutSpecialUpFunc(setaSolta);
	
	//ativa texturas
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	
	inicializar();
	
	glutTimerFunc(1000/60, gameLoop, 0);
	glutDisplayFunc(renderGame);
	//glutIdleFunc(gameLoop);
	glutMainLoop(); 
}

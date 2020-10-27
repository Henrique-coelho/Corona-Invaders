
#define MaxFrotasPorTela 8
#define MaxTropasPorTela 60
#define MaxTirosPorTela 16
#define MaxEntitiesPerScreen 20

#define LarguraTropa 40
#define AlturaTropa 80

typedef struct{
	float coordx; // x de um objeto
	float coordy; // y de um objeto
	int altura; // altura de um objeto
	int largura; // largura de um objeto

}Posicao;

typedef struct{
	bool emUso;
	int timer;
	char graficos;
	Posicao local;

	int id; // Textura
}Graficos;

typedef struct{
	bool ativado;
	bool invulneravel;
	int vida;
	int pontos;

	int timerBala; // Periodo de tiro do inimigo
	int timerDano; // Animação de dano do jogador
	int timerPowerUp; // Timer do power up do jogador
	
	int posLimiteMin; // Mínima coordenada cuja o jogador pode alcançar
	int posLimiteMax; // Máxima coordenada cuja o jogador pode alcançar
	Posicao local;

	int id; // Textura
}Jogador;

typedef struct{
	bool ativado;
	bool alistado;
	int variedade;
	int vida;
	int frota;
	
	Posicao posVirtual; // Este se refere a posição original da tropa, isto é a sua formação na frota
	Posicao posReal; // Este se refere a posição verdadeira da tropa, onde ela é desenhada na tela
	
	double larguraTextura;
	double alturaTextura;

	int timer1; // Responsável pela animação de dano/morte do inimigo
	int timer2; // Responsável pela animação de tiro
	int sleepTimer;
	
	int frequenciaDeTiro; // (deve estar entre 0 e 256) Responsável pela propabilidade de que a tropa atire
	int frequenciaDeAvanco; // (deve estar entre 0 e 256) Responsável pela probabilidade de que a tropa avançe (se o valor for menor que frequenciaDeTiro, ela nunca avançanrá)
	
	int estado; // referente as ações de avanço da tropa. Estados podem ser: EmFrota, Avançando, Retorna á Direita, Retorno á Esquerda, Retorno por trás
	
	int deslocamentoIndividual; // Esta variável se refere a ações específicas de quando a tropa se isola da formação da frota
	float angulo; // envolve com o deslocamento individual de avanço. Não refere ao ângulo da tropa, é apenas uma variável que auxilia no calculo do ângulo

	int id; // Textura
}Tropa;

typedef struct{
	bool ativado;
	int firstTropa;
	int sizeofFrota;
	int tropasVivas;
	double deslocamentoHorizontal;
	int deslocamentoVertical;
	int tempoDeslocamento;

	int posLimiteMin; // Mínima coordenada cuja a frota pode alcançar
	int posLimiteMax; // Máxima coordenada cuja a frota pode alcançar
	
	Posicao borda;
}Frota;

typedef struct{
	bool ativado;
	int variedade;
	bool ehInimigo;
	bool ehEspecial;
	
	float angulo;
	float velocidadeTiro;

	Posicao local;
	int id; // Textura
}Tiro;

typedef struct{
	int variedade;
	float frequenciaTiro;
}Arma;

typedef struct{
	Arma armaEquipada;
	bool enemyShakeUponDeath;
}ConfiguracoesPublicas; // Estas configurações serão dispóniveis para interação com o usuário. Infelizmente não tivemos tempos de costumizar ainda mais o menu, sem tempo :P
	
typedef struct{
	bool mostrarFrota;
}ConfiguracoesPrivadas; // Estas configurações são resguardadas para mecânicas internas do jogo não controladas pelo usuário

typedef struct{
	double xInicial;
	double yInicial;

	double largura;
	double altura;

	GLuint textura;
}TexturaInfo;

typedef struct{
	double larguraEmPixels;
	double alturaEmPixels;
}TexturaTamanho;

Jogador jogador;
ConfiguracoesPublicas menu;
ConfiguracoesPrivadas menuInterno;
Frota frotas[MaxFrotasPorTela];
Tropa tropas[MaxTropasPorTela];
Tiro tiros[MaxTirosPorTela];
Graficos graficos[MaxEntitiesPerScreen];

bool leftPressed = false;
bool rightPressed = false;
bool upPressed = false;
bool downPressed = false;

int gamemode = 0;
int globalTimer=0;
int deathTimer = 0;
int nextFrotaTimer = 0;
int randomEventTimer = 0;
int menuTimer = 0;
int menuIndex = 0;

char faseAtual = 'A';
char fasesDesbloqueadas = 'A';
int maiorPontuacao = 0;

FILE* data;
char info[50];

GLuint idSprites;
GLuint idBackGround1;
GLuint idMenu;

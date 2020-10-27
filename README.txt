Corona Invaders

Participantes:
 - Rodrigo Lopes Dias Ferreira
 - Henrique Coelho Mendes

Instruçoes:
 - Acesse via terminal a pasta CoronaInvaders, (cd ....../TP1_RodrigoLopes_HenriqueCoelho/CoronaInvaders)
 - execute o programa com, gcc corona_invaders.c -lm -lglut -lGL -lGLU -lSOIL

Instruções de Jogo:
- Setas para andar e se locomover no menu
- Enter para acessar algo do menu
- ESC par pausar o jogo ou retornar a um menu anterior

Implementações:
 - 15 fases, cujo a ultima é um endless;
 - Diversos menus: game over, menu principal, seleção de fases, menu de pausa;
 - Animações de Tiro do inimigo e jogador;
 - Inimigos realizam rasantes;
 - Inimigo diverso de varios comportamentos: uns atiram no jogador, outros avançam no nele, outros fazem ambos, alguns retorna a frota pela esquerda, outros por trás, outros pela direita
 - Chamadas internas que facilitam a criação de frotas de inimigos, com chamada de funções como (armadas) e (criarFrotas)
 - Power ups do jogador: Tiro triplo, velocidade dobrada, invulnerabilidade
 - Sistema de vidas, pode ter até 8 vidas, inimigo podem "dropar" mais vidas
 - Sistema de pontuação, demonstrado no canto superior direito da tela
 - Sistem de pontuação máxima, o "highscore" é salvo toda vez que se joga o jogo
 - Sistema de armazenamento de dados após o jogo ser fechado, criando um bloco de notas ("data.txt") que armazena a maior pontuação e níveis derrotados
 - Sritesheets personalizada feitas por nó mesmos (com exceção do background)

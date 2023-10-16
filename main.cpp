#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>

#define GRAVITY 9.8

// vari�veis globais
GLfloat eixo = 50.0;
GLfloat timer = 0;
GLint angulo = 1;
GLint velInit = 1;
GLfloat posicaoX = 0;
GLfloat positionX = 5.5;
GLfloat positionY = -4.7;
GLfloat valorAlvoX = 20;
bool disparoAtivo = false;
char texto[30];

void desenharGramado()
{
    glBegin(GL_POLYGON);
    glVertex2f(-eixo, -9);
    glVertex2f(-eixo, -eixo);
    glVertex2f( eixo, -eixo);
    glVertex2f( eixo, -9);
    glEnd();
}

void desenharCorpoTanque()
{
    glBegin(GL_POLYGON);
    glVertex2f(-3.0, 2);
    glVertex2f(-4.0, 0);
    glVertex2f( 4.0, 0);
    glVertex2f( 3.0, 2);
    glEnd();
}

void desenharEsteiraTanque()
{
    glBegin(GL_QUADS);
    glVertex2f(-5.0, 1);
    glVertex2f(-5.0, 0);
    glVertex2f( 5.0, 0);
    glVertex2f( 5.0, 1);
    glEnd();
}

void desenharTorreTanque()
{
    glBegin(GL_QUADS);
    glVertex2f(-1.0, 2);
    glVertex2f(-1.0, 0);
    glVertex2f( 1.0, 0);
    glVertex2f( 1.0, 2);
    glEnd();
}

void desenharCanoTanque()
{
    glBegin(GL_QUADS);
    glVertex2f( 0.0, 0.5);
    glVertex2f( 0.0, 0.0);
    glVertex2f( 3.0, 0.0);
    glVertex2f( 3.0, 0.5);
    glEnd();
}

void desenharAlvo()
{
    glBegin(GL_QUADS);
    glVertex2f( 0.0, 1.0);
    glVertex2f( 0.0, 0.0);
    glVertex2f( 6.0, 0.0);
    glVertex2f( 6.0, 1.0);
    glEnd();
}

void desenharCirculo(GLfloat tamanho, GLfloat vertice)
{
    float anguloCirculo, incremento;
    incremento = (2 * M_PI) / vertice;
    glBegin(GL_POLYGON);
    for(anguloCirculo=0; anguloCirculo<2*M_PI; anguloCirculo+=incremento)
    {
        glVertex2f(tamanho*cos(anguloCirculo), tamanho*sin(anguloCirculo));
    }
    glEnd();
}

void desenharTiro(){
    glPointSize(6);
    glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0, 0.0);
    glEnd();
}

void exibirValores()
{
    //posição onde o texto será colocado
    glRasterPos2f(0, 22);
    glColor3f(0.0f,0.0f,1.0f);

    // Converta o valor da variável para uma string
    sprintf(texto, "Angulo: %d Velocidade: %d", angulo, velInit);

    for (int i = 0; texto[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, texto[i]);
    }
}

void animar(int t)
{
    if(disparoAtivo == true){

        //Converter para radianos
        double angleRadians = angulo *  (M_PI / 180.0);

        //tempo de voo
        double timeFlight = (2 * velInit * sin(angleRadians)) / GRAVITY;

        double y = sin(angleRadians) * 3;
        double x = 3 - (cos(angleRadians) * 3);

        //calcular a posicao da bola
        positionX = (velInit * cos(angleRadians) * timer) + (posicaoX + 5.5 - x);
        positionY = (velInit * sin(angleRadians) * timer - ((GRAVITY * timer * timer) / 2 )) + (-4.7 + y);

        glutPostRedisplay();
        timer = timer + 0.1;
        glutTimerFunc(160, animar, t);
    }
}


// fun��o de redimensionamento da janela de visualiza��o
static void resize(int width, int height)
{

    // Evita a divisao por zero
    if(height == 0) height = 1;

    // Especifica as dimens�es da Viewport
    glViewport(0, 0, width, height);

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (width <= height)
    {
        gluOrtho2D (-25.0f, 25.0f, -25.0f*height/width, 25.0f*height/width);
        eixo = 25.0f;
    }
    else
    {
        gluOrtho2D (-25.0f*width/height, 25.0f*width/height, -25.0f, 25.0f);
        eixo = 25.0f*width/height;
    }

}

void verificaPonto(){
    if(posicaoX > valorAlvoX && posicaoX < valorAlvoX + 6){
        int valorMinimo = 0;
        int valorMaximo = 30;
        // Gera um número aleatório dentro da faixa
        int numeroAleatorio = (rand() % (valorMaximo - valorMinimo + 1)) + valorMinimo;
        valorAlvoX = numeroAleatorio;
    }
}

// fun��o de redesenho da janela de visualiza��o
static void display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 1.0f, 0.3f);
    desenharGramado();

    glPushMatrix();
        glTranslatef(-15, 20, 0);
        glColor3f(1.0f, 1.0f, 0.0f);
        desenharCirculo(4.0, 20.0); //sol
    glPopMatrix();

    glPushMatrix();//faz o tanque andar

        glTranslatef(posicaoX, 0.0f, 0.0f);

        //Desenhando o tanque
        glPushMatrix();
            glTranslatef(2.5, -8, 0);
            glColor3f(0.0f, 0.0f, 0.0f);
            desenharCorpoTanque();
        glPopMatrix();

        //Desenhando a esteira do tanque
        glPushMatrix();
            glTranslatef(2.5, -9, 0);
            glColor3f(1.0f, 0.0f, 0.0f);
            desenharEsteiraTanque();
        glPopMatrix();

        //Desenhar torre de tiro
        glPushMatrix();
            glTranslatef(2.5, -6, 0);
            glColor3f(0.0f, 0.0f, 0.0f);
            desenharTorreTanque();
        glPopMatrix();

        //desenhar o cano de tiro
        glPushMatrix();
            glTranslatef(2.5, -5, 0);
            glRotatef(angulo, 0.0f, 0.0f, 1.0f);
            glColor3f(0.0f, 0.0f, 0.8f);
            desenharCanoTanque();

            if(!disparoAtivo)
                {
                glPushMatrix();
                    glTranslatef(3, 0.3, 0);
                    desenharTiro();
                glPopMatrix();
                }

        glPopMatrix();

    glPopMatrix();

    glPushMatrix();
        glTranslatef(valorAlvoX, -10, 0);
        glColor3f(0.5f, 0.0f, 0.0f);
        desenharAlvo();
    glPopMatrix();

    exibirValores();



    if(disparoAtivo){
        //municao
        glPushMatrix();
            glTranslatef(positionX, positionY, 0);
            desenharTiro();
        glPopMatrix();

        if(positionX > 35){
            timer = 0;
            disparoAtivo = false;
        }else if(positionY < -9){
            timer = 0;
            disparoAtivo = false;
            //verificaPonto();

            printf("Valor do alvo: %f", valorAlvoX);
            if(positionX > valorAlvoX && positionX < valorAlvoX + 6){
                int valorMinimo = 0;
                int valorMaximo = 30;
                // Gera um número aleatório dentro da faixa
                int numeroAleatorio = (rand() % (valorMaximo - valorMinimo + 1)) + valorMinimo;
                valorAlvoX = numeroAleatorio;
                printf("- Valor do alvo dps: %f\n", valorAlvoX);
            }

        }
    }

    glutSwapBuffers();
}

// fun��o para tratar os eventos do teclado
static void key(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);

    if (key == 32 ) { // Tecla de espaço para disparar
        disparoAtivo = true;
        animar(0);
    }
}

void specialKey(int key, int x, int y)
{

    switch (key)
    {
    case GLUT_KEY_LEFT:
        posicaoX -= 0.5;  // Move o tanque para a esquerda
        break;
    case GLUT_KEY_RIGHT:
        posicaoX += 0.5;  // Move o tanque para a direita
        break;

    // Rotaciona braço1
    case GLUT_KEY_DOWN:
        if(angulo > 0)
        {
            angulo = angulo - 1;
        }

        break;
    case GLUT_KEY_UP:
        if(angulo < 80)
        {
            angulo = angulo + 1;
        }
        break;
    case GLUT_KEY_END:
        velInit = velInit +1;// Aumenta a velocidade
        break;
    case GLUT_KEY_HOME:
        velInit = velInit - 1;// Diminui a velocidade
        break;
    }

    display();
}



// fun��o respons�vel por fazer as inicializa��es
void setup(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    strcpy(texto, "(0,0)");
}

// Programa Principal
int main(int argc, char *argv[])
{
    // utilizada para inicializar a biblioteca GLUT
    glutInit(&argc, argv);

    // define do modo de opera��o da GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // especifica o tamanho inicial em pixels da janela GLUT
    glutInitWindowSize(450, 300);

    // especifica a posi��o inicial da janela GLUT
    glutInitWindowPosition(100, 100);

    // cria a janela passando como t�tulo como argumento
    glutCreateWindow("Trabalho");

    // registra a fun��o callback de redesenho da janela de visualiza��o
    glutDisplayFunc(display);

    // registra a fun��o callback para tratar os eventos do teclado
    glutKeyboardFunc(key);

    // Registra a função callback para tratamento das teclas Especiais
    glutSpecialFunc(specialKey);

    // registra a fun��o callback de redimensionamento da janela de visualiza��o
    glutReshapeFunc(resize);

    //Funcao para animar
    glutTimerFunc(160, animar, 0);

    // chama a fun��o respons�vel por fazer as inicializa��es
    setup();

    // inicia o processamento e aguarda intera��es do usu�rio
    glutMainLoop();

    return 0;
}


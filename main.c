#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>
#include <time.h>

/**
 *SpaceCombatX  v.1.0
 *
 *Hra - hrac hraje za lod a strili meteory
 *
 *M.Nemec 3.2017
 */

char vypisCo(int X, int Y); //dle souradnic vypere spravny prvek [strana nahore, strana na boku, meteor, lod, strela, vzduch]
void display(); //zobrazi aktualni hru [hraci pole a skore]
char read(int delay); //cte [delay] ms a vrati psoledni zachzceny znak, v pripade Space(32) ukonci delay, v pripade CtrZ(26) nebo Esc(27) ukonci program #conio.h windows.h stdlib.h stbio.h
void TheGame();
void StartGame(); //uvitani do hry, sposti se hned po startu
void EndGame(); //vypise finalni scoreMax a ceka na space nebo esc nebo ctrZ
void akce(char vstup); //rozhoduje, co se state  s prijatym znakem z klavesnice
void pohyb_Lod(char vstup); //na zaklade vstupu odecte ci pricte 1 k LodX pri jineem vstupu nic
void stvoreni_meteoru(); //MetY = 1; MetX = 1-7; a snizi times o jedna az do times == 2
void pohyb_Meteor();
void stvoreni_strely();
void pohyb_Strela();
void scorer_maker(); //1) meni score 2) vola prvky

char lastCh, Lod = 'x', Meteor = 'o', Strela = 'l';
int i, X, Y, score, scoreMax, MetCount, MetX, MetY, LodX, LodY, ShotX, ShotY, times;

int main(){
    TheGame();
    return 0;
}

void cont(){
    printf("\nCONTROLA\n");
    Sleep(500);
}

void TheGame() {
    srand(time(NULL));
    StartGame();
    stvoreni_meteoru();
    while(1){
        for (i = 0; i < times; i++){
            lastCh = toupper(read(40));
            akce(lastCh);
            scorer_maker();
            pohyb_Strela();
            display();
        }
        pohyb_Meteor();
        scorer_maker();
    }
    EndGame();
}

void scorer_maker(){
    if (LodY == MetY && LodX == MetX)
        EndGame();
    if (ShotY == MetY && ShotX == MetX){
        MetCount++;
        ShotY = 0;
        i = 0;
        score += 10;
        stvoreni_meteoru();
    }
    if(12 == MetY){
        stvoreni_meteoru();
        if (score > 0)
            score -= 5;
    }
    if (scoreMax < score)
        scoreMax = score;
}

//pohyby

void pohyb_Meteor(){
    MetY++;
}

void pohyb_Strela(){
    if (ShotY > 0)
        ShotY--;
}

void pohyb_Lod(char vstup){
    if (vstup == 'A' || vstup == 75){
        if (LodX > 1)
            LodX--;
    }
    if (vstup == 'D' || vstup == 77){
        if (LodX < 7)
            LodX++;
    }
}

 //stvoreni

void stvoreni_strely(){
    ShotY = 11;
    ShotX = LodX;
}

void stvoreni_meteoru(){
    MetY = 1;
    MetX = rand() % 7 + 1;
    if (times > 2){
        if (times >= 8)
            times = times/2;
        else
            times -= 1;
    }
}

void akce(char vstup){
    if (vstup == 'A' || vstup ==  75 || vstup ==  'D' || vstup ==  77)
        pohyb_Lod(vstup);
    if (vstup == 'W' || vstup ==  32 || vstup ==  72)
        stvoreni_strely();
}

void EndGame(){
    system("cls");
    printf("\n\
           \n\
           \n\tGAME OVER\
           \n\
           \n\tYour final score: %d\
           \n\t%d meteor(s) destroyed.\
           \n\
           \n\tPress Space to Play Again or Esc to exit.", scoreMax, MetCount);
    lastCh = '0';
    Sleep(500);
    while( lastCh != 32 && lastCh != 26 && lastCh != 27)
        lastCh = getch();
    if (lastCh == 32)
        TheGame();
    exit(0);
}

void StartGame(){
    system("cls");
    char uvitaci_zprava[] = "\n\
    \n\tHello World!\
    \n\
    \n\t And Welcome\
    \n\t In My Game\
    ", SCX[] = "SpaceCombatX";
    printf("%s", uvitaci_zprava);
    read(1010);
    system("cls");
    printf("\n\n\n\n\n\t");
    times = 110;
    for( i = 0; i < sizeof(SCX)-1; i++){
        printf("%c",SCX[i]);
        char ch = read(times);
        if (ch == 32)
            times = 0;
    }
    read(2010);
    system("cls");
    char rozkazy[] =    "\n\tYou're an astronaut"
                        "\n\tgoverning space ship."
                        "\n\tYour mission is"
                        "\n\tto get on Jupiter"
                        "\n\tbut on the way"
                        "\n\tthere are many meteors"
                        "\n\tand you have to dodge."
                        "\n\tGood luck!";
    times = 23;
    for( i = 0; i < sizeof(rozkazy)-1; i++){
        printf("%c",rozkazy[i]);
        char ch = read(times);
        if (ch == 32)
            times = 0;
    }
    read(2010);
    system("cls");
    printf("\n\tControls:\
            \n\t\
            \n\tMove: Arrows (left & right)\
            \n\tShoot: (space)\
            \n\tEnd Game: Hit Meteor or (control + Z) or (esc)\
            \n\
            \n\tPress Space to Start the Game!");
    i = 0;
    while( i == 0 )
        i = (32 == read(1)) ? 1 : 0;
    score = 0; scoreMax = 0; MetCount = 0; MetX = -1; MetY = -1; LodX = 4; LodY = 11; ShotX; ShotY = 0; times = 128;
}

char vypisCo(int X, int Y){
    if (Y == 0)
        return '_';
    else if (X == 0 || X == 8)
        return '|';
    else if (Y == LodY && X == LodX)
        return Lod;
    else if (Y == MetY && X == MetX)
        return Meteor;
    else if (Y == ShotY && X == ShotX)
        return Strela;
    return ' ';
}

void display(){
    system("cls");
    printf("score: %3d", score);
    for ( Y = 0; Y < 12; Y++){
        printf("\n\t");
        for ( X = 0; X < 9; X++)
            printf("  %c  ", vypisCo(X, Y));
    }
}

char read(int delay){
    int i = 0;
    char ch = '0';
    while ( i != delay ){
    if ( _kbhit() ) {
        ch = _getch();
        if (ch == 26 || ch == 27)
            EndGame();
        if (ch == 32)
            break;
    }
    Sleep(1);
    i++;
    }
    return ch;
}

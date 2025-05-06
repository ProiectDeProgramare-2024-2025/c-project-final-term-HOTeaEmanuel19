#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <direct.h>
#include <unistd.h>
#define RED    "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define DARK_BLUE "\033[34m"
#define RESET   "\033[0m"

typedef struct
{
    char nume[100],descriere[100];
    int an,ore,minute;
} Movie;
typedef struct
{
    Movie *filme;
    int nr;
} MovieList;
const char *optiuni[] =
{
    "Lista de filme",
    "Filme vizionate",
    "Filme de urmarit",
    "Iesire"
};
const char *optiuni1[]=
{
    "Adauga filmul in lista filmelor vizionate",
    "Adauga filmul in lista filmelor de urmarit",
    "Inapoi la meniu"
};
const char *optiuni2[]=
{
    "Sterge film din lista",
    "Iesire din meniu"
};
const char *iesire[]= {"Iesire in meniu"};
void delete_movie(MovieList *lista,int poz)
{
    for(int i=poz; i<lista->nr-1; i++)
    {
        strcpy(lista->filme[i].nume,lista->filme[i+1].nume);
        strcpy(lista->filme[i].descriere,lista->filme[i+1].descriere);
        lista->filme[i].an=lista->filme[i+1].an;
        lista->filme[i].ore=lista->filme[i+1].ore;
        lista->filme[i].minute=lista->filme[i+1].minute;
    }
    lista->nr--;
}
int check(Movie l,MovieList Filme)
{
    for(int i=0; i<Filme.nr; i++)
    {
        if(strcmp(l.nume,Filme.filme[i].nume)==0) return 1;
    }
    return 0;
}
void get_movies(MovieList* lista,const char *fisier)
{
    int x=0;
    FILE *filme=fopen(fisier,"r");
    fscanf(filme,"%d\n",&x);
    char n[100],des[100];
    int an,ore,minute;
    for(int i=0; i<x; i++)
    {
        fgets(n,sizeof(n),filme);
        n[strcspn(n, "\n")] = 0;
        fgets(des,sizeof(des),filme);
        des[strcspn(des, "\n")] = 0;
        fscanf(filme,"%d%d%d\n",&an,&ore,&minute);
        Movie *temp=realloc(lista->filme,(lista->nr+1)*sizeof(Movie));
        if(temp==NULL) return;
        lista->filme=temp;
        strncpy(lista->filme[lista->nr].nume, n, 99);
        strncpy(lista->filme[lista->nr].descriere, des, 99);
        lista->filme[lista->nr].an = an;
        lista->filme[lista->nr].minute=minute;
        lista->filme[lista->nr].ore=ore;
        lista->nr++;
    }
    fclose(filme);
}
void add_movie(MovieList* lista,Movie film)
{
    Movie *temp=realloc(lista->filme,(lista->nr+1)*sizeof(Movie));
    lista->filme=temp;
    strncpy(lista->filme[lista->nr].nume, film.nume, 99);
    strncpy(lista->filme[lista->nr].descriere, film.descriere, 99);
    lista->filme[lista->nr].an = film.an;
    lista->filme[lista->nr].minute=film.minute;
    lista->filme[lista->nr].ore=film.ore;
    lista->nr++;

}
void safe(MovieList *lista,const char *fisier)
{
    FILE *filme=fopen(fisier,"w");
    fprintf(filme,"%d\n",lista->nr);
    for(int i=0; i<lista->nr; i++)
    {
        fprintf(filme,"%s\n",lista->filme[i].nume);
        fprintf(filme,"%s\n",lista->filme[i].descriere);
        fprintf(filme,"%d %d %d\n",lista->filme[i].an,lista->filme[i].ore,lista->filme[i].minute);
        fprintf(filme,"\n");
    }
    fclose(filme);


}
int find_movie(MovieList lista,const char * nume)
{
    for(int i=0; i<lista.nr; i++)
    {
        if(strcmp(lista.filme[i].nume,nume)==0)
        {
            return i;
        }
    }
    return -1;
}
int indicatorPosition(int position, int posMax)
{
    int c = getch();
    if (c == 224)
    {
        c = getch();
        switch (c)
        {
        case 72:
            position--;
            break; // sus
        case 80:
            position++;
            break; // jos
        }
        if (position < 0) position = 0;
        else if (position > posMax) position = posMax;
    }
    else if (c == 13)     // ENTER
    {
        return posMax + 1; // valoare specială
    }
    return position;
}
int afisare_meniu_principal()
{
    int pos = 0;
    int optSelected = -1; //salvam optiunea aleasa
    while (1)
    {
        system("cls");
        // Afișarea opțiunilor cu cursorul
        printf("~ Sistem filme ~\n");
        for (int i = 0; i <= 3; i++)
        {
            if (i == pos)
                printf(">> ");
            else
                printf("   ");
            printf("%s\n", optiuni[i]);
        }

        int result = indicatorPosition(pos, 3);
        if (result == 4)
        {
            optSelected = pos;
            break;
        }
        pos = result;
    }

    // Afișează opțiunea aleasă
    system("cls");
    if(optSelected==3) exit(0);
    if (optSelected != -1)
    {
        return optSelected+1;
    }
}
int afisare_meniuri(int MAX_OPT,const char *optiuni[],char* text,MovieList Filme)
{
    int pos = 0;
    int optSelected = -1; //salvam optiunea aleasa
    while (1)
    {
        system("cls");
        // Afișarea opțiunilor cu cursorul
        printf(BLUE"%s\n" RESET,text);
        if(Filme.nr==0) printf("Lista este goala\n");
        for(int i=0; i<Filme.nr; i++)
            printf( "%d." CYAN "%s " RESET "Genuri: " YELLOW "%s" RESET" An lansare:" RED "%d" RESET "Durata:" RED "%dh%dm\n" RESET,i+1,Filme.filme[i].nume,Filme.filme[i].descriere,Filme.filme[i].an,Filme.filme[i].ore,Filme.filme[i].minute);


        printf("\n");
        for (int i = 0; i <= MAX_OPT; i++)
        {
            if (i == pos)
                printf(">> ");
            else
                printf("   ");
            printf("%s\n", optiuni[i]);
        }

        int result = indicatorPosition(pos, MAX_OPT);
        if (result == MAX_OPT + 1)
        {
            optSelected = pos;
            break;
        }
        pos = result;
    }

    // Afișează opțiunea aleasă
    //system("cls");
    if (optSelected != -1)
    {
        return optSelected+1;
    }
}
void adaugare_film(MovieList Filme,MovieList *A,MovieList *B,int who)
{
    char name[50]="";
    int ok=0,poz;
   // int ch;
 //   while ((ch = getchar()) != '\n' && ch != EOF);
    char sir[50];
    if(who==1) strcpy(sir,"Watched.txt");
    else strcpy(sir,"ToWatch.txt");
    while(!ok)
    {

        printf("Introdu numele filmului sau apasa 1 pentru iesire:\n");
        fgets(name,sizeof(name),stdin);
        name[strcspn(name, "\n")] = 0;
        poz=find_movie(Filme,name);
        if(strcmp(name,"1")==0) break;
        if(poz>=0 && !check(Filme.filme[poz],*A) && !check(Filme.filme[poz],*B)) ok=1;
        else
        if(poz>=0 && check(Filme.filme[poz],*A)) printf("Filmul ales este deja in acesta lista\n");
        else if(poz>=0 && check(Filme.filme[poz],*B)) printf("Filmul ales este deja in lista %s\n",who==1? "filmelor de urmarit":"filmelor vizionate");
        else printf("Filmul ales nu se afla in lista filmelor\n");

    }
    if(poz>=0)
    {
        printf(GREEN "Filmul a fost adaugat cu succes in lista\n" RESET);
        sleep(3);
        add_movie(A,Filme.filme[poz]);
        safe(A,sir);
    }
}
void stergere_film(MovieList *A,int who)
{
    int poz=-1;
    char name[50],sir[50];
   // int ch;
    if(who==1) strcpy(sir,"Watched.txt");
    else strcpy(sir,"ToWatch.txt");
    //while ((ch = getchar()) != '\n' && ch != EOF);
    while(poz<0)
    {
        printf("Introdu numele filmului sau apasa 1 pentru iesire:\n");
        fgets(name,sizeof(name),stdin);
        name[strcspn(name, "\n")] = 0;
        if(strcmp(name,"1")==0) break;
        poz=find_movie(*A,name);
        if(poz<0) printf("Filmul nu a fost gasit in lista\n");

    }
    if(poz>=0)
    {
        printf(RED "Filmul a fost sters din lista\n" RESET);
        sleep(3);
        delete_movie(A,poz);
        safe(A,sir);
    }
}
int main()
{
    MovieList Filme= {NULL,0},Watched= {NULL,0},toWatch= {NULL,0};
    get_movies(&Filme,"Filme.txt");
    get_movies(&Watched,"Watched.txt");
    get_movies(&toWatch,"ToWatch.txt");
    while(1)
    {
        int choice=afisare_meniu_principal();
        if(choice==1)
        {
            while(1)
            {
                system("cls");
                int c=afisare_meniuri(2,optiuni1,"Lista de filme",Filme);
                if(c==3)
                {
                    system("cls");
                    break;
                }
                if(c==1)
                {
                    adaugare_film(Filme,&Watched,&toWatch,1);
                }
                else
                {
                    adaugare_film(Filme,&toWatch,&Watched,2);
                }
            }
        }
        else if(choice==2)
        {

            while(1)
            {
                system("cls");
                if(Watched.nr){
                    choice=afisare_meniuri(1,optiuni2,"Lista filme vizionate",Watched);
                    if(choice==2)break;
                }
                else {afisare_meniuri(0,iesire,"Lista filme vizionate",Watched);
                break;
                }
                stergere_film(&Watched,1);
            }
        }
    else if(choice==3)
    {
        while(1)
            {
                system("cls");
                if(toWatch.nr){
                    choice=afisare_meniuri(1,optiuni2,"Lista filme de vizionat",toWatch);
                    if(choice==2) break;
                }
                else{afisare_meniuri(0,iesire,"Lista filme de vizionat",toWatch);
                break;
                }
                stergere_film(&toWatch,2);
            }
            }

}
safe(&Filme,"Filme.txt");
safe(&Watched,"Watched.txt");
safe(&toWatch,"ToWatch.txt");
return 0;
}

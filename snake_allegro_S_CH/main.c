/**
* @Author Seweryn Cholewa
* @Description Projekt gry Snake
*/


/**
* biblioteka allegro.h potrzebna do części graficznej projektu, biblioteki
* biblioteki stdio.h i stdlib.h zawierają podstawowę funkcje przydatne w programnie
* biblioteka time.h potrzeban jest do losowania współrzędznych jedzenia,
* biblioteka stdbool.h zawiera rzeczy potrzebne do wykorzystnia zmiennych typu bool (są w liscie)
*/

#include <allegro.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

/**
*  funkcja init zwiazana jest z biblioteką allegro i jest wykonywana na początku działania programu, w jej skład wchodzi
*  allegro_init() inicjalizująca tryb allegro
*  set_color_depth(16) - ustawia głębie kolorów (na wartość podaną jako parametr)
*  set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0) - uruchamia tryb graficzny, pierwszy parametr określa tzw. magiczny sterownik allegro (czyli wirtualne sterowniki), kolejne parametry to szerokość i długość rozdzielczośc oraz to szerokość i długość rozdzielczości naszego wirtualnego ekranu
*  install_timer() - funkcja instaluje timer
*  install_keybord() - funkcja odpowiadajaca za obsluge klawiatury
*  install_mouse() - funkcja odpowiadajaca za obsluge myszky
*/

inline void init()
{
	allegro_init();
	set_color_depth(16);
	set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0);

	install_timer();
	install_keyboard();
	install_mouse();
}

/**
* funkcja deinit() wykonywana jest na koniec programu, w jej sklad wchodzi
* clear_keybuf() - funkcja czyści bufor klawiatury
* allegro_exit() -  funkcja zamykajaca tryb allegro
*/

inline void deinit()
{
	clear_keybuf();
	allegro_exit();
}

/**
* funkcja  void plansza() - nie przyjmuje żadnych parametrów a jej zadaniem jest stworzenie planszy, czyli narysowanie dwóch lini poziomych i pionowych tworzących prostokąt (który jest planszą) oraz umieszcza nad nim napis "Gra Snake
* textout_centre_ex() - pojawia się kilkukrotnie w programie, dlatego opiszę ją raz, pozwala nam ona wyświetlić napis na ekranie (przy czym przyjmuje ona parametr x, jako środek napisu),
* a jej parametry to: bitmapa na której operujemy (w tym przypadku ekran), czcionka napisu (tutaj standardowa), wyświetlany napis, jego położenie (współrzędne x i y), kolor pierwszoplanowy i tła (model RGB)
* funkcje vline() i hline() odpowiednio linie pionową i poziomom, przyjmują parametry takie jak: bitmapa (czyli nasz ekran), współrzędne x i y, długość linii i kolor
*/

void plansza(){
    textout_centre_ex(screen,font,"Gra Snake", SCREEN_W/2,100,makecol(255,255,102),-1);
    vline(screen,150,200,400,makecol(0,255,255));
    vline(screen,650,200,400,makecol(0,255,255));
    hline(screen,150,200,650,makecol(0,255,255));
    hline(screen,150,400,650,makecol(0,255,255));
}

/**
* struktura element_listy jest podstawą listy jednokierunkowej, która jest potrzebna do pokazywania listy rekordów
* zawiera wskaźnik na pierwszą litere zmiennej typu string zawierajacej nick gracza, zmienną typu int przechowującą wynik tego gracza, oraz wskażnik na kolejnye element listy
*/

struct element_listy{
    char *nick;
    int punkty;
    struct element_listy *next;
};

/**
* funkcja pomocnicza bool umiesc_przed() - jest to funkcja służąca wstawianiu elementu (struktury) do listy przed określonym elementem (czyli w sytuacji dodania elementu na początku oraz w środku listy), funkcja ta zwraca wartość true jeśli wspomniana czynność się uda lub false jeśli się nie powiedzie,
* jako parametry przyjmuje ona jako parametr element przed którym ma wstawić kolejny element oraz wartość zmiennych tej struktury, czyli nick i punkty
* w funkcji tej tworzymy nowy element listy, na który pamięć jest przydzielona za pomocą funkcji malloc(która jako argument przyjmuje rozmiar w bajtach)
*/

bool umiesc_przed(struct element_listy **wezel, char *nick, int punkty)
{
    struct element_listy *nowy_wezel = (struct element_listy *)
        malloc(sizeof(struct element_listy));
    if(NULL != nowy_wezel)
    {
        nowy_wezel->punkty=punkty;
        nowy_wezel->nick=strdup(nick);
        nowy_wezel->next=*wezel;
        *wezel = nowy_wezel;
        return true;
    }
    return false;
}

/**
* funkcja  pomocnicza bool umiesc_z_tyłu() - jest to funkcja służąca wstawianiu elementu (struktury) do pustej listy lub na jej koncu, funkcja ta zwraca wartość true jeśli wspomniana czynność się uda lub false jeśli się nie powiedzie,
* jako parametry przyjmuje ona jako parametr ostatni element, za który wstawi nowy oraz wartość zmiennych tej struktury, czyli nick i punkty
*/

bool umiesc_z_tylu(struct element_listy **wezel, char *nick, int punkty)
{
    *wezel = (struct element_listy *)malloc(sizeof(struct element_listy));
    if(NULL != *wezel)
    {
        (*wezel)->punkty = punkty;
        (*wezel)->nick = strdup(nick);
        (*wezel)->next = NULL;
        return true;
    }
    return false;
}

/**
* funkcja umiesc_wezel() - jest to funkcja umożliwiająca wstawianie nowego elmentu do listy i korzysta z wymienionych wcześniej dwóch funkcji, funkcja ta przyjmuje jako parametry: wskaźnik na początek listy, wartość zmiennych tej struktury, czyli nick i punkty,
* w razie, gdy wskazwyany przez przekazany wskaźnik element ma mniejszą wartość zmiennej punkty to w takiej sytuacji zwraca on funkcje umiesc_przed(),
* w sytuacji, gdy tak nie jest zwraca ona samą siebie z kolejnym elementem listy jako parametrem i tak w kółko, aż znajdzie elment od, którego ma większą wartość punkty lub gdy dojdzie do końca i tam wstawi nowy element
*/

bool umiesc_wezel(struct element_listy **wezel, char *nick, int punkty)
{
    if(NULL!= *wezel)
    {
        if((*wezel)->punkty<=punkty){
            return umiesc_przed(wezel, nick, punkty);
        }
        else{
            return umiesc_wezel(&(*wezel)->next, nick, punkty);
        }
    }
    else{
        return umiesc_z_tylu(wezel, nick, punkty);
    }
}

/**
* funkcja void usun_liste() - słuzy do usunięcia listy i przyjmuje wskaźnik na początek listy,
* funkcja ta rekurencyjne wywołuje samą siebie względem kolejnego elementu listy i zwalnia pamięć na niego przeznaczoną, aż elementy się skończą
*/

void usun_liste(struct element_listy **wezel)
{
    if(NULL != *wezel)
    {
        usun_liste(&(*wezel)->next);
        free(*wezel);
    }
}

/**
* struct czesc_ciala to podstawowa struktura dwustronnej listy (czyli ciała węża),
*zawiera dwie zmienne typu int x i y określające położenie elemntu oraz dwa wskaźniki na kolejny i poprzedni element listy
*/

struct czesc_ciala
{
    int x,y;
    struct czesc_ciala *poprzedni, *kolejny;
};

/**
* struct czesc_ciala *stworz_glowe() - funkcja tworząca głowę listy (czyli pierwszy element), nie przyjmuje żadnych parametrów
* funkcja ta zwraca glowe, czyli pierwszy element listy
* ponieważ współrzednę głowy są stałe, wąż na początku pojawia się w tym samym miejscu
*/

struct czesc_ciala *stworz_glowe()
{
    struct czesc_ciala *glowa = (struct czesc_ciala *)
        malloc(sizeof(struct czesc_ciala));
    if(NULL!=glowa)
    {
        glowa->poprzedni=glowa->kolejny=NULL;
        glowa->x=400;
        glowa->y=300;
    }
    return glowa;
};

/**
* struct czesc_ciala *wprowadz_poczatek() - funkcja pomocnicza służąca do dodania elmentu na początku listy (czyli stworzenia nowej głowy)
* jako parametry przyjmuje ona wskaźnik na obecną głowę, wskaźnik na nowy element ciała węża, który ma się stać głową oraz zmienną wskazującą kierunek
* ostatni z parametrów wynika z faktu, iż w mojej impelmentacji imitacje ruchu otrzymujemy poprzez tworzenie nowej głowy w określonym kiernuku i usuniecie ostatniego elementu listy
* funkcja zwraca nowy element kolejki
*/

struct czesc_ciala *wprowadz_poczatek(struct czesc_ciala *glowa,
                                struct czesc_ciala *nowa_czesc, char kierunek)
{
    if(kierunek=='D'){
        nowa_czesc->x = glowa->x;
        nowa_czesc->y = glowa->y+5;
        nowa_czesc->kolejny=glowa;
        glowa->poprzedni=nowa_czesc;
        return nowa_czesc;
    }
    else if(kierunek=='L'){
        nowa_czesc->x = glowa->x-5;
        nowa_czesc->y = glowa->y;
        nowa_czesc->kolejny=glowa;
        glowa->poprzedni=nowa_czesc;
        return nowa_czesc;
    }
    else if(kierunek=='P'){
        nowa_czesc->x = glowa->x+5;
        nowa_czesc->y = glowa->y;
        nowa_czesc->kolejny=glowa;
        glowa->poprzedni=nowa_czesc;
        return nowa_czesc;
    }
    else if(kierunek=='G'){
        nowa_czesc->x = glowa->x;
        nowa_czesc->y = glowa->y-5;
        nowa_czesc->kolejny=glowa;
        glowa->poprzedni=nowa_czesc;
        return nowa_czesc;
    }
    else{
        return NULL;
    }
};

/**
* funkcja struct czesc_ciala *wprowadz_czesc()- służy do właściwego wprowadzenia nowego elmentu na poczatku listy,
* jako parametry przyjmuje wskażnik na pierwszy element listy oraz zmienną określającą kierunek ruchu
* w razie powodzenia zwraca funkcje wprowadz_poczatek(), która odpowiednio zmienia współrzędne nowej głowy, w przypadku niepowodzenia alokacji pamięci na nwoy element funkcja zwraca przekazaną jej głowę lub gdy głowa nie istnieje (czyli nie istnieje lista) zwraca NULL
*/

struct czesc_ciala *wprowadz_czesc(struct czesc_ciala *glowa, char kierunek)
{
    if(NULL==glowa)
    {
        return NULL;
    }

    struct czesc_ciala *nowa_czesc= (struct czesc_ciala *)
        malloc(sizeof(struct czesc_ciala));
    if(NULL!=nowa_czesc){
        nowa_czesc->poprzedni=nowa_czesc->kolejny=NULL;
        return wprowadz_poczatek(glowa,nowa_czesc, kierunek);
    }
    else{
        return glowa;
    }
};

/**
* funkcja struct czesc_ciala *znajdz_czesc() to funkcja wyszukujaca ostatni element listy
* jako parametr przyjmuje wskaźnik na głowe, jeśli uda się jej znaleźć głowę to ją zwraca
* funkcja ta jest potrzebna do usuniecia ostatniego elementu (drugi element mechanizmu imitacji ruchu węża)
*/

struct czesc_ciala *znajdz_czesc(struct czesc_ciala *glowa)
{
    while((NULL!=glowa)&&(glowa->kolejny!=NULL)){
        glowa = glowa->kolejny;
    }
    return glowa;
};

/**
* funkcja void usun_koniec() to funkcja usuwająca ostatni element listy,
* jako parametr przyjmuje wskaźnik na ostatni element listy
* używa funkcji free() do zwolnienia pamięci zarezerwowanej dla tego elementu
*/

void usun_koniec(struct czesc_ciala *koniec)
{
    koniec->poprzedni->kolejny=NULL;
    free(koniec);
}

/**
* struct czesc_ciala *usun_czesc() - funkcja służąca do usunięcia elementu listu, korzysta z funkcji wyszukującej i usuwającej ostatni element listy
* jako parametr przyjmuje wskaźnik na pierwszy element listy,
* w razie powodzenia zwraca ona glowe lub jeśli lista nie istnieje (glowa jest równa NULL) zwraca NULL
*/

struct czesc_ciala *usun_czesc(struct czesc_ciala *glowa)
{
    if(NULL==glowa){
        return NULL;
    }
    struct czesc_ciala  *czesc = znajdz_czesc(glowa);
    usun_koniec(czesc);
    return glowa;
};

/**
* void wyświetl_weza() to funkcja wyswietlajaca weza na ekranie
* jako parametr przyjmuje wskaznik na pierwszy element listy
* funkcja kolejno iteruje przez elementy listy i za kazdy element generuje wypełnione zielone kwadraty na ekranie, które tworzą postać węża
* korzysta ona z funkcji rectfill() - generującej wspomniane kwadraty, jako parametry przyjmuje ona bitmape na której je wyświetla, współrzędne ich wierzchołków oraz kolor
*/

void wyswietl_weza(struct czesc_ciala *glowa) // funkcja wyswietlacjąca weza na konsoli
{
    for(; NULL!=glowa;glowa=glowa->kolejny){
        rectfill(screen,glowa->x, glowa->y, glowa->x + 5, glowa->y + 5, makecol(0,255,0));
    }
}

/**
* void usun_weza() - funkcja usuwajaca liste (weza)
* jako parametr przymuje podwójny wskaźnik na głowe
* funkcja w pętli zwalnia pamięć przeznaczoną na kolejne elementy listy za pomocą funkcji free(), aż lista nie będzie pusta
*/

void usun_weza(struct czesc_ciala **glowa) // funkcja usuwajćca węża (liste)
{
    while(NULL!=*glowa){
        struct czesc_ciala *kolejny = (*glowa)->kolejny;
        free(*glowa);
        *glowa=kolejny;
    }
}

/**
* int czy_sciana() - jest to funkcja sprawdzająca czy położenie głowy nie wykroczyło poza położenie ściany, inaczej mówiac sprawdzająca czy wąż nie ugryzł ściany
* jako parametr przyjmuje ona wskaźnika na głowę węża
* w przypadku jeśli, któraś z współrzędnych została przekroczona zwraca ona 0, w innym przypadku zwraca 1
*/

int czy_sciana(struct czesc_ciala *glowa){ // funkcja sprawdzajaca czy wąż dotknął ściany
    if(glowa->y<=200 || glowa->y>=400|| glowa->x<=150 || glowa->x>=650){
        return 0;
    }
    else{
        return 1;
    }
}

/**
* int czy_ugryzl() to funkcja sprawdzajaca czy współrzędne głowy pokrywają się z jakimkolwiek innym elementem listy, czyli sprawdza czy wąż się ugryzł
* jako parametr przyjmuje ona wskaźnik na głowę węża
* funkcja w pętli porównuje współrzędne głowy i poszczególnych elementów (używając continue omijamy głowę przy sprawdzaniu, bo generowałoby to błęy)
* jeśli wspomniane współrzędne się pokrywają funkcja zwraca 0, w innym przypadku zwraca 1
*/

int czy_ugryzl(struct czesc_ciala *glowa){
    int pierwszy_obieg = 0;
    int glowa_x, glowa_y;
    glowa_x = glowa->x;
    glowa_y = glowa->y;
    for(; NULL!=glowa;glowa=glowa->kolejny){
            if(pierwszy_obieg==0){
                pierwszy_obieg=1;
                continue;
            }
            if(glowa->x==glowa_x && glowa->y==glowa_y){
                    return 0;
            }
    }
    return 1;
}

/**
* void wspolrzedne_jedzenia() to funkcja generujaca losowe wspolrzedne jedzenia
* jako parametry przyjmuje wskaźnik na głowę, wskaźnik na zmienną będącą współrzędną x i y jedzenia oraz wskaźnik na zmienną typu int czy_jedzenie
* funkcja w pierwszej kolejności ustawia flage nie_waz na wartość 1 i w pętli sprawdzającej wspomnianą wartosc generuje losowe współrzędne wykrzystując funkcję rand() w określonym przedziale (współrzędne ścian)
* następnie funkcja ta "dopełnia" wygenerowane współrzędne do liczby podzielnej przez 5, co wynika z faktu iż wąż porusza się co pięć jednostek
* kolejnym krokiem jest sprawdzenie czy współrzędne jedzenia i głowy się nie pokrywają, gdy jest prawdą funkcja wraca do etykiety przed pętlą i zaczyna ją od nowa, aż wreszcie współrzędne nie będą się pokrywały (wtedy zmienna nie_waz zmieni swoją wartość co przerwie pętle)
* ostatnim etapem jest zmiana wartości flagi czy_jedzenie, która określa czy jedzenie już istnieje na planszy i nie trzeba generować nowych współrzędnych
*/

void wspolrzedne_jedzenia(struct czesc_ciala *glowa, int  *jedz_x, int *jedz_y, int *czy_jedzenie){
    int nie_waz=1;
    glowa:
    while(nie_waz==1){
        *jedz_x = (rand()%(640-160+1)+160);
        *jedz_y = (rand()%(390-210+1)+210);
        if(*jedz_x%5==1 ){
            *jedz_x=*jedz_x+4;
        }
        if(*jedz_x%5==2 ){
            *jedz_x=*jedz_x+3;
        }
        if(*jedz_x%5==3 ){
            *jedz_x=*jedz_x+2;
        }
        if(*jedz_x%5==4 ){
            *jedz_x=*jedz_x+1;
        }
        if(*jedz_y%5==1 ){
            *jedz_y=*jedz_y+4;
        }
        if(*jedz_y%5==2 ){
            *jedz_y=*jedz_y+3;
        }
        if(*jedz_y%5==3 ){
            *jedz_y=*jedz_y+2;
        }
        if(*jedz_y%5==4 ){
            *jedz_y=*jedz_y+1;
        }
        for(; NULL!=glowa;glowa=glowa->kolejny){
            if(glowa->x==*jedz_x && glowa->y==*jedz_y){
                    goto glowa;
            }
        }
        nie_waz=0;
    }
    *czy_jedzenie=1;
}

/**
* void wypisz_jedzenie() to funkcja wyświetlająca na ekranie jedzenie w postaci czerwonego kwadratu
* jako parametry przyjmuje ona współrzędne x i y jedzenia
* wykorzystuje funkcję rectfill(), którą opisałem przy wysietlaniu węża
*/

void wypisz_jedzenie(int jedz_x, int jedz_y){ //funkcja wyswietlajaca jedzenie na konsoli
   rectfill(screen,jedz_x, jedz_y, jedz_x + 5, jedz_y + 5, makecol(255,0,0));
}

/**
* int czy_waz_zjadl() to funkcja sprawdzająca czy współrzędne głowy węża i jedzenia się nie pokrywają, czyli czy nie zjadł on jedzenia
* jako parametry przyjmuje ona wskaźnik na głowę, współrzędne x i y jedzenia oraz wskaznik na flagę czy_jedzenie
* w sytuacji, gdy wspomniane współrzędne się pokrywają zmienia ona wartość flagi czy_jedzenie na 0 (co daje znać programowi, że trzeba wygenerować nowe współrzędnę jedzenia)
* oraz zwraca ona 1, w przeciwnym razie zwraca 0
*/

int czy_waz_zjadl(struct czesc_ciala *glowa, int jedz_x, int jedz_y, int *czy_jedzenie){ // funkcja sprawdzajaca czy waz zjadl jedzenie
    if(glowa->x==jedz_x && glowa->y==jedz_y ){
        *czy_jedzenie=0;
        return 1;
    }
    return 0;
}

/**
* void menu() to funkcja tworząca menu gry, nie przyjmuje ona parametrów
* w pierwszej kolejności wyświetla ona napis witający gracza (funkcja textout_centre_ex() opisana wcześniej) i za pomocą funkcji readkey() czeka na naciśnięcie dowolnego klawisza przez gracza
* po naciśnięciu czyści bufor klawiatury (clear_keybuf()) oraz czyści ekran (clear(screen)) i chwilowo zatrzymuje wykonanie programu za pomocą funkcji rest(200), wynika to z faktu iż gdyby nie ten krok bufor nie zdążył by się wyczyścić i dalsza część funkcji by źle zadziałała
* następnie wyświetlone zostaje menu tłumaczące zasady i umożliwiające rozpoczęcie gry, w tej sytuacji gracz może nacisnąć przycisk esc co skończy grę, nacisnąć strzałkę do góry co rozpocznię nową grę
* ostatnią opcją jest naciśnięcie r, co spowoduje stworzenie listy rekordów, otworzenie pliku wyniki.txt, odczytanie z niego danych za pomocą funkcji fgets() oraz umieszczenie wczytanych danych w strukturach z których składa się lista
* następnię w pętli dojdzie do iteracji przez listę i wyświetlenie na ekranie pierwszych dziesięciu wyników (ponieważ lista dodaje elementy w sposób malejący, to pierwsze dziesięć jest największych)
* ponownie, gdy gracz nacisnie jakikolwiek klawisz ekran zostanie wyczyszczony i wróci do menu przy okazji niszcząc listę rekordów
*/

void menu(void) // funkcja generujaca menu
{
    int klawisz;
    textout_centre_ex(screen,font,"Witam w grze snake. (nacisnij jakikolwiek klawisz, zeby kontynuowac)",SCREEN_W/2,SCREEN_H/2,makecol(255,255,102),-1);
    readkey();
    clear_keybuf();
    clear(screen);
    rest(200);
    while(klawisz!=15131){
        textout_ex(screen,font,"-> Twoim celeme jest kierowanie wezem w celu zjedzenia jak najwiekszej liczby jedzenia",0,200,makecol(255,255,102),-1);
        textout_ex(screen,font,"bez ugryzienia swojego ciala lub sciany",0,210,makecol(255,255,102),-1);
        textout_ex(screen,font,"-> Poruszasz sie za pomoca klawiszy strzalek",0,230,makecol(255,255,102),-1);
        textout_ex(screen,font,"-> Jesli bedziesz chcial poruszyc sie w strone ciala (np. idac w gore nacisniesz strzalke w dol)",0,250,makecol(255,255,102),-1);
        textout_ex(screen,font,"to zostanie odebrane jako ugryzienie siebie",0,260,makecol(255,255,102),-1);
        textout_ex(screen,font,"-> Nacisnij r, zeby zobaczyc liste rekordow",0,280,makecol(255,255,102),-1);
        textout_ex(screen,font,"-> W trakcie gry nacisnij na esc, zeby zakonczyc",0,300,makecol(255,255,102),-1);
        textout_ex(screen,font,"-> Wcisnij strzalke do gory, zeby rozpoczac nowa gra",0,320,makecol(255,255,102),-1);
        textout_ex(screen,font,"-> Wcisnij klawisz esc, zeby zakonczyc gre",0,340,makecol(255,255,102),-1);
        klawisz=readkey();
        if(klawisz==21504){
            clear(screen);
            rest(200);
            return;
        }
        if(klawisz==4722){
            clear(screen);
            clear_keybuf();
            rest(50);
            struct element_listy *front = NULL;
            char c[10];
            char wyn[10];
            int len, pkt;
            FILE *fp;
            fp = fopen("wyniki.txt","r");
            if(fp == NULL){
                printf("nie mozna otworzyc pliku");
                exit(0);
            }
            for(;;){
                if(fgets(c,10,fp)==NULL){
                    break;
                }
                if(fgets(wyn,10,fp)==NULL){
                    break;
                }
                len=strlen(c);
                if(c[len-1]=='\n'){
                    c[len-1]='\0';
                }
                len=strlen(wyn);
                if(wyn[len-1]=='\n'){
                    wyn[len-1]='\0';
                }
                pkt = atoi(wyn);
                umiesc_wezel(&front, c, pkt);
            }
            fclose(fp);
            int licznik = 0;
            int wysokosc = 170;
            for (; NULL!=front;front=front->next){
                textprintf_centre_ex(screen,font,SCREEN_W/2-20,wysokosc,makecol(255,255,102),-1,"%s      %d",front->nick, front->punkty);
                wysokosc=wysokosc+20;
                licznik++;
                if(licznik>=10){
                    break;
                }
            }
            readkey();
            clear(screen);
            usun_liste(&front);
        }
    }
    clear_keybuf();
    rest(50);
    clear(screen);
    textout_ex(screen,font,"Gra zakonczona",350,300,makecol(255,255,102),-1);
    readkey();
    exit(0);
}

/**
* funkcja main() wykrozystuje wszystkie powyższe funkcje,
* na początek używa funkcji init() inicjując tryb allegro i tworzy zmienne lokalne potrzebne do działania programu
* po stworzeniu zmiennych tworzy ona głowę węża (pierwszy element listy) i 4 kolejne elementy ciała (wynika to z faktu, iż przy 5 elementach można się już ugryźć)
* następnię wyświetlane jest meny za pomocą funkcji menu(), po czym wyświetlana jest plansza i podstawowę ciało węża
* po wspomnianych krokach zaczyna się główna pętla gry, która opiera się na fladzę koniec_gry, której aktualna wartość wynosi 1
* pierwszymi rzeczami jakie są sprawdzane jest to czy wąż ugryzł ścianę lub siebie, w obu przypadkach jeśli do tego dojdzie flaga koniec_gry zmieni swoją wartość na 0 i aktualnyobieg pętli zostanie przerwany oraz przejdziemy do końca gry
* następnie sprawdzane jest czy został naciśnięty jakiś klawisz (a jeśli będzie to klawisz esc, gra się skończy)
* kolejnym krokiem jest sprawdzenie, czy jedzenie jest na wyświetlone na ekranie, jeśli nie to odpowiednia funkcja generuje jej współrzędne, a następnie za pomocą innej funkcji to jedzenie jest na tym ekranie wyświetlane
* dalej sprawdzane jest czy ostatnio naciśnięty klawisz to któraś ze strzałek, a jeśli tak jest to tworzona jest nowa głowa w danym kierunku
* potem sprawdzane jest czy jedzenie zostało zjedzone, jeśli tak jest to zwiększa się licznik punktów, a jeśli tak nie jest zostaje usnięty ostatni element ciała (wspomniałem o tym przy ruchu)
* ostatnia częśc wspomnianej pętli to wyświetlenie planszy, węża oraz napisu z wynikiem gry na ekranie oraz wyczyszczenie ekranu
* całość powtarzana jest do wspomnianej sytuacji ugryzienia ciała węża lub ściany, przez co pętla zostaje przerwana
* w takiej sytuacji wąż (lista dwukierunkowa) zostaje usunięta oraz wyświetla się ekran końca gry, gdzie możemy poznać wynik rozgrywki, podać swój nick, który razem z wynikiem zostanie zapisany do pliku wyniki.txt
* ostatnim krokiem jest zdecydowanie, czy chcemy zagrać ponownie, co możemy zrobić jeśli naciśniemy klawisz y, co przeniesie nas z powrotem do początku gry
* w przypadku jeśli gracz zdecyduje się zakończyć grę to ostatnim krokiem jest funkcja deinit() wyłączająca tryb allegro i usuwająca wprowadzone przez niego zmiany
* ostatecznie funkcja main() zwraca 0
*/

int main()
{
	init();
	char od_nowa='y';
	char nick[10];
	char znak;
	while(od_nowa=='y'){
        srand(time(0));
        int i;
        int ilosc_punktow = 0;
        int koniec_gry= 1;
        int czy_jedzenie = 0;
        int jedz_x=0;
        int jedz_y=0;
        int klawisz = 21504;

        struct czesc_ciala *glowa = stworz_glowe();
        for(i=0;i<4;i++){
            glowa=wprowadz_czesc(glowa,'G');
        }
        menu();
        plansza();
        wyswietl_weza(glowa);
        while(koniec_gry==1){
            do{
                if(czy_sciana(glowa)==0){
                    koniec_gry=0;
                    break;
                }
                if(czy_ugryzl(glowa)==0){
                    koniec_gry=0;
                    break;
                }
                if(keypressed()){
                    klawisz=readkey();
                    if(klawisz==15131){
                        koniec_gry=0;
                        break;
                    }
                }
                if(czy_jedzenie==0){
                    wspolrzedne_jedzenia(glowa, &jedz_x, &jedz_y, &czy_jedzenie);
                }
                wypisz_jedzenie(jedz_x,jedz_y);
                if(klawisz==21504){
                    glowa=wprowadz_czesc(glowa,'G');
                }
                else if(klawisz==21760){
                    glowa=wprowadz_czesc(glowa,'D');
                }
                if(klawisz==20992){
                    glowa=wprowadz_czesc(glowa,'L');
                }
                else if(klawisz==21248){
                    glowa=wprowadz_czesc(glowa,'P');
                }
                if(czy_waz_zjadl(glowa,jedz_x,jedz_y,&czy_jedzenie)==1){
                    ++ilosc_punktow;
                }
                else{
                    glowa=usun_czesc(glowa);
                }
                plansza();
                wyswietl_weza(glowa);
                textprintf_centre_ex(screen,font,SCREEN_W/2,450,makecol(255,255,102),-1,"Ilosc zdobytych punktow: %d",ilosc_punktow);
                rest(50);
                clear(screen);
            }while(klawisz!=15131);
        }
        i=0;
        usun_weza(&glowa);
        clear(screen);
        textout_centre_ex(screen,font,"Koniec gry",SCREEN_W/2,SCREEN_H/2,makecol(255,255,102),-1);
        textprintf_centre_ex(screen,font,SCREEN_W/2,SCREEN_H/2+20,makecol(255,255,102),-1,"Ilosc zdobytych punktow: %d",ilosc_punktow);
        textout_centre_ex(screen,font,"Podaj nick(na koniec nacisnij enter): ",SCREEN_W/2,SCREEN_H/2+40,makecol(255,255,102),-1);
        while(!key[KEY_ENTER]){
            klawisz=readkey();
            znak=klawisz;
            nick[i]=znak;
            i++;
            clear_keybuf();
            rest(50);
        }
        FILE *fp;
        fp = fopen("wyniki.txt","a");
        if(fp == NULL){
            printf("nie mozna otworzyc pliku");
            exit(0);
        }
        char *nick2 = strdup(nick);
        fprintf(fp,"%s",nick2);
        fprintf(fp,"%d\n",ilosc_punktow);
        fclose(fp);
        textout_centre_ex(screen,font,"Czy chcesz zagrac od nowa?(nacisnij y na tak, albo inny klawisz na nie)",SCREEN_W/2,SCREEN_H/2+80,makecol(255,255,102),-1);
        klawisz=readkey();
        if(klawisz==6521){
            od_nowa='y';
            clear(screen);
            clear_keybuf();
            rest(50);
        }
        else{
            od_nowa='n';
        }
	}
	deinit();
	return 0;
}
END_OF_MAIN()

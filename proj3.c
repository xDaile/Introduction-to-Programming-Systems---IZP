

//TODO pred malloc pridaj nejaku premennu a ak pride NULL tak error a end

/**
 * Kostra programu pro 3. projekt IZP 2017/18
 *
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#define DNDEBUG
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#define MAX_DIST 1415 //odmocnina(1000^2+1000^)

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */

#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO
    c->size=0;
    if(cap!=0)//ak je pozadovana kapacita rozna od nuly
        {
        if(((c->obj=malloc(( sizeof(struct obj_t) * cap)))!=NULL) && cap>0)//alokuje priestor pre pole objektov
            {
            c->capacity=cap;
            c->obj->x=0;
            return;
            }
        }
    c->capacity=0; c->obj=NULL;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{assert(c);

    // TODO


  free(c->obj);//zrusenie alokacie pola objektov
c->capacity=0;//pole objektov uz neexistuje a teda kapacita je nula
c->size=0;
c->obj=NULL;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)//ak je kapacita dostacujúca
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;//velkost pre malloc

    void *arr = realloc(c->obj, size);


    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
    while((c->size)==(c->capacity))//bude zvacsovat kapacitu o hodnotu 10 dovtedy kym to nebude dostacujuce
       resize_cluster(c,(int)(c->capacity+CLUSTER_CHUNK));

/**pridanie prvkov za posledny objekt... c->size je index objektu, kde sa nic nenachadza*/
    c->obj[c->size].id=obj.id;
    c->obj[c->size].x=obj.x;
    c->obj[c->size].y=obj.y;

    c->size++;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
    {
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    if((c1->capacity)<=(c1->size + c2->size))//ak je kapacita nedostacujuca
        resize_cluster(c1,(int)(c1->capacity + c2->size) );
    int  iter=0;
    while(iter<c2->size)//pridanie objektov druheho clustra do prveho na koniec
        {
        append_cluster(c1,c2->obj[iter]);
        iter++;
        }
    sort_cluster(c1);//zoradenie clustra
    }

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
	clear_cluster(&carr[idx]);//vycistenie clustra na pocicii idx
//posunutie zhluku doprava od indexu idx
	if(narr>0)for (int i = idx; i <narr-1; i++)
        {
		carr[i] = carr[i + 1];
        }
    return narr-1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
    {
    assert(o1 != NULL);
    assert(o2 != NULL);
    // TODO
    if(o1==NULL && o2== NULL)return -1;
    if(o1==NULL) return -2;
    if(o2==NULL) return -3;
    float x_diff=o2->x-o1->x;//rozdiel suradnic x prveho a druheho objektu
    float y_diff=o2->y-o1->y;
    x_diff*=x_diff;//PYThag. veta
    y_diff*=y_diff;
    x_diff=sqrt(x_diff+y_diff);
    return x_diff;
    }

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
    float distance=0;
    int another_step=0;
    if((c1)==NULL || (c2)== NULL)return -1;
    int step=0;
    int index=0;//pocet spocitanych vzdialenosti
    float result=0;//vysledny sucet vzdialenosti
    while(c1->size!=step)//prechadzanie clustrov
        {
        while(c2->size!=another_step)//prechadzanie clustrov.... system ako buble sort
            {
            distance=obj_distance(&( (*c1).obj[step]),&( (*c2).obj[another_step]));
            another_step++;
            result=result+distance;
            index++;
            }
        step++;
        another_step=0;

        }
        return result/(float)index;


}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    float result=MAX_DIST;
    float dist=0;
    int iter=0;
    int inner_iter=0;
    while(iter<narr)//pokial bolo iteracii menej ako je pocet zhlukov postupne sa meni prvy porovnavany cluster tymto cyklom
    { inner_iter=iter+1;//nasledujuci cluster... aby sa neporovnaval sam so sebou
        while(inner_iter<narr)//jeden cluster so vsetkymi ostatnymi
            {
            dist=cluster_distance(&(carr)[iter],&(carr[inner_iter]));
            if(result>dist)//ak je vysledok ktory mam viac ako aktualny ktory je v dist...
                {
                result=dist;
                *c1=iter;
                *c2=inner_iter;
                }
            inner_iter++;
            }
        iter++;
        inner_iter=iter+1;
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
 // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)//cyklus pre vypisanie vsetkych objektov clustru
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    //co ak je index dva krat zadany ten isty, co ak ma byt vystup viac ako vstup
    assert(arr != NULL);

    // TODO
    int num_lines=0;
    FILE *file = fopen(filename, "r");
    if(! file){
        fprintf(stderr,"Can not open file\n");
       // fclose(file);
        return 0;}

/******inicializacia pola zhlukov*******/
    if(fscanf(file, "count= %d\n", &num_lines)!=1)//ak sa nenacita alebo je to v subore v zlom formate
        {
        fprintf(stderr,"Bad format of first line\n");
        fclose(file);
        return 0;
        }
    else if(num_lines<1)//ak je v subore cislo riadkov mensie ako 1
        {
       fprintf(stderr,"Number of clusters in file is wrong format\n");
       fclose(file);
        return 0;
        }


    *arr = malloc ( sizeof(struct cluster_t) * num_lines);//alokacia pola struktur, tolko kolko je v subore riadkov
    int iteration=0;
    char t='\n';
      int control=0;
    while(iteration<num_lines)//num_lines je udaj z prveho riadku zo suboru, postupne prechadzam riadky v tomto cykle
        {
        //pre test ci uz za poslednym cislom nic nie je
        if(t==-1){(*arr)[iteration].size++;iteration++;break;}
        if(control==0)init_cluster(&(*arr)[iteration], 1);//vytvorenie clustru
//        int i=0;//premenna pre fscanf, ak by nenaplnil niektoru premennu,
control=0;
        long int one=0;//pre index
        int position=0;

/***just hardTRY*/
        while((t=fgetc(file))!=EOF)
            {control=-1;
            if(t=='\n')
                {if(position==2)(*arr)[iteration].obj->y=one;
                position=0;
                (*arr)[iteration].size++;
                iteration++;
                break;}
            if(position>2)
                {
                while(t!='\n')
                    {
                    if((t!=32 && t!=9 && t!=13 && t!=1)) {
                    fprintf(stderr,"Objects in text file is not correct 1\n");
                    fclose(file);
                    return ((iteration+1)*(-1));}
                     t=fgetc(file);
                    }
                position=0;
                (*arr)[iteration].size++;
                iteration++;
                break;
                }
            if((t>57 || t<48 ) && (t!=' ' && t!='	' && t!=13 && t!=1)) {
                fprintf(stderr,"Objects in text file is not correct %d \n",(int)t);
                fclose(file);
                return ((iteration+1)*(-1));}
            if(t!=' ' && t!=9 && t!=13)
                one=one*10+(atoi(&t));
            if(one>INT_MAX)
                {fprintf(stderr,"Object ID is too big\n");
                fclose(file);
                return ((iteration+1)*(-1));}
                //Zlepeny prvy a druhy riadok
            if(position==1 || position==2)
                {
                if(one<0 || one>1000)
                    {fprintf(stderr,"Object coordinates is too big\n");
                    fclose(file);
                    return ((iteration+1)*(-1));}
                }
            if(t==' ')
                {if(position==0)
                    {
                    (*arr)[iteration].obj->id=one;
                    if(iteration>0)
                        {
                        int num_objects=iteration-1;
                        while(num_objects+1)
                            {
                            if((*arr)[iteration].obj->id==(*arr)[num_objects].obj->id)
                                {
                                fprintf(stderr,"Value of index in file is wrong format\n");
                                fclose(file);
                                return ((iteration+1)*(-1));
                                }
                            num_objects--;
                            }
                         }
                    }
                if(position==1)(*arr)[iteration].obj->x=one;
                if(position==2)(*arr)[iteration].obj->y=one;
                position++;
                one=0;
                }
            if(num_lines==iteration+1 && position==2)
                {(*arr)[iteration].obj->y=one;}

            }
        control=control+1;
        if(control==1)
            {
            if((t=fgetc(file))==EOF && num_lines==iteration+1)
                {position=0;
                (*arr)[iteration].size++;
                iteration++;break;
            }
        }
    }
    if(iteration<num_lines){
        fprintf(stderr,"Nesplnitelny argument, v subore je malo objektov\n");
        fclose(file);
        return num_lines*(-1);}

    arr=&(arr[0]);

/************koniec inicializacie********/

//num_lines je pocet clusterov na zaciatku
//current je aktualny pocet clusterov
//end je pozadovany pocet clusterov

//ak je chyba return NULL;
fclose(file);
    return num_lines;}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)//vytlacenie vsetkych clustrov
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{


    struct cluster_t *clusters;
//end je konecny pocet potrebnych zhlukov
//num_lines je aktualny pocet zhlukov
    // TODO
    int end=1;
    if(argc>1 && argc<4)
    {if(argv[2]){//ak je tam argument 2
        int index=0;
        while(argv[2][index]){//Test na argument cislo dva
            if(argv[2][index]> 58 || argv[2][index]< 47)// test na argument dva
                {fprintf(stderr,"Second argument must be positive integer\n");
                return 1;}
            index++;}
        if(index<1)
            {fprintf(stderr,"Second argument wrong \n");return 1;}
        end=atoi(argv[2]);}
        if(end<1)//ak druhy argument je mensi ako 1
                {fprintf(stderr,"Second argument must be bigger than 0\n");return 1;}
        }
    else {fprintf(stderr,"Wrong arguments\n");return 1;}
    int num_lines=load_clusters(argv[1],&clusters);//nacitanie suboru, kazdy riadok do samostatneho clustru
    if(num_lines==0){return 1;}
    if(num_lines<1)
       { num_lines=num_lines*(-1);
       while(num_lines)
            {
            clear_cluster(&clusters[num_lines-1]);
            num_lines--;
            }

            free(clusters);//dealokacia pola clustrov

        return 1;
        }//ak sa stala v load clusters chyba,
    if(end>num_lines){print_clusters(clusters,num_lines);return 0;}//ak je pocet pozadovanych clustrov viac ako pocet clustrov ktore mam v subore
    while(num_lines!=end && num_lines>=end)//cyklus kym nemam pozadovany pocet clustrov
        {
        int first=-1;//hodnota -1 kvoli tomu aby som ju neskor mohol testovat ci sa vo funkcii find_neighbours udialo s nimi nieco
        int second=-1;
        find_neighbours(clusters,num_lines,&first,&second);
        merge_clusters(&clusters[first],&clusters[second]);
        remove_cluster(clusters,num_lines,second);
        num_lines--;
        }
    print_clusters(clusters,num_lines);//vytalci alokovane clustre, uz po analyze
    while(num_lines)//dealokacia, zvysnych clustrov
        {
        remove_cluster(clusters,num_lines,num_lines-1);
        num_lines--;
        }
//        clusters[0].obj->x=2;
//clear_cluster(&clusters[4]);TENTO NECHE DAT PREC>>> PRI DEBUGU OD NULY HORE SOM HO VIDEL PLNY... PRECO????

    free(clusters);//dealokacia pola clustrov
    return 0;
}

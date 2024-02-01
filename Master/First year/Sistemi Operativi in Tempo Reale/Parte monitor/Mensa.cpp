/*
L’azienda di ristorazione che gestisce il servizio mensa del campus ha deciso di adottare una politica di gestione volta ad ottimizzare l’utilizzo dei tavoli ed evitare interferenze tra diverse tipologie dei clienti.
La mensa è dotata di k tavoli con capienza convenzionale 4 persone ciascuno.
Il numero totale di posti disponibili è quindi N=4k. 
Sono previste due tipologie di clienti: Studenti (S) e Professori (P).
Clienti di tipo diverso non devono tuttavia condividere lo stesso tavolo, ovvero se un tavolo è parzialmente occupato da clienti di un tipo in esso non possono essere fatti accomodare clienti dell’altro tipo.
E’ inoltre presente un cameriere di sala che provvede ad accogliere e fare accomodare ciascun cliente (uno per volta) ad un tavolo appropriato, se il posto è disponibile.
Una volta sistemato al tavolo il cliente, il cameriere va ad approvvigionarsi del cibo ordinato per poi fornire il pasto al cliente.
Al termine di questa azione si rende disponibile ad accogliere un altro cliente se possibile, ovvero si pone in attesa.
In assenza di posti disponibili di tipo appropriato i clienti devono restare in attesa.
I clienti, dopo aver ricevuto il cibo consumano liberamente il pasto senza ulteriori interazioni con il cameriere ne con gli altri clienti dello stesso tavolo, e al termine liberano il posto.
La politica proposta prevede che sia data priorità ai Professori e che si ricerchi il più efficiente utilizzo dei tavoli, compatibilmente con le specifiche precedenti.
Si risolva il problema di programmazione concorrente utilizzando il costrutto monitor, obbligatoriamente in semantica Mesa.
Si preveda, oltre alla definizione del tipo, la istanziazione della variabile e la traccia dei processi.
Non è consentito l'impiego delle primitive broadcast e queue sulle variabili condizione.
La soluzione deve essere esente da problemi di attese inutili o attive, starvation e deadlock.
*/

//GIUSEPPE RICCIARDI 355695

//SEMANTICA MESA -> priorità al segnalante

//NB: SOLUZIONE NON CORRETTA AL 100%

//COSTANTI GLOBALI
#define K 2 //numero tavoli
#define MAX_POSTI 4 //numero posti tavolo
#define P 1
#define S 2
//DEFINIZIONE + INIZIALIZZAZIONE MONITOR

type Mensa{
    int occupati[K] = [0,0,0]; //ogni elemento dell'array rappresenta un tavolo, il suo valore è il numero di posti occupati in quel tavolo
    int tipo[K] = [0,0,0]; //0 LIBERO, 1 PROF, 2 STUDENTE
    int num_prof_queue = 0;
    int num_stud_queue = 0;
    int tavolo_assegnato = 0;
    bool cameriere_ready = false;
    bool ok_posto_prof = false;
    bool ok_posto_stud = false;
    bool ok_servito = false;
    condition prof_queue;
    condition stud_queue;
    condition wait_posto;
    condition wait_pranzo;

    //procedura usata dal camierer per trovare un posto (non entry del monitor)
    procedure trova_posto(){
        //cameriere controlla se ci sono posti disponibili per studenti o professori
        for (int i = 0; i < K; i++)
        {
            if(tipo[i] == 0){ //Trovato un tavolo completamente libero
                if(num_prof_queue == 0 && num_stud_queue>0){ //assegna posto a studente se non ci sono prof in coda
                    ok_posto_stud = true
                }
                else if(num_prof_queue>0){ //altrimenti assenga posto al prof -> priorità al prof
                    ok_posto_prof = true; 
                }
                tavolo_assegnato = i; //salva il tavolo da assegnare
            }
            else if(tipo[i] == P){ //trovato tavolo con prof
                if (occupati[i] < MAX_POSTI){ //posto disponibile al tavolo con prof
                    ok_posto_prof = true;
                    tavolo_assegnato = i; //salva il tavolo da assegnare
                }
                 else{
                    ok_posto_prof = false;
                }
            }
            else if(tipo[i] == S){ //tavolo con studente
                if(occupati[i] < MAX_POSTI){ //posto disponibile al tavolo con studente
                    ok_posto_stud = true;
                    tavolo_assegnato = i;
                }
                else{
                    ok_posto_stud = false;
                }
            }
        }
    }
    //PROCEDURE ENTRY CAMERIERE
    procedure entry accogli_cliente()
    {
        tavolo_assegnato = 0;
        cameriere_ready = true;
        trova_posto();
        //se non ci sono posti disponibili o se non ci sono clienti in coda il cameriere attende.
        while((!ok_posto_prof || num_prof_queue ==0) && (!ok_posto_stud || num_stud_queue == 0)){
            wait_cliente.wait();
        }
        cameriere_ready = false; //il cameriere diventa occupato
        //accoglie i clienti, dando priorità ai professori
        if(ok_posto_prof && num_prof_queue>0){
            prof_queue.signal();
        }
        else if(ok_posto_stud && num_stud_queue>0){
            stud_queue.signal()
        }
        ok_servito = false;
    }
    procedure entry servi_cliente()
    {
        ok_servito= true;
        wait_pranzo.signal();
    }
    //PROCEDURE ENTRY CLIENTE
    procedure entry cliente_vuole_tavolo(int tipo)
    {
    if (tipo == P){
        num_prof_queue++
        if(ok_posto_prof && cameriere_ready){ //se c'è un posto per professore -> sveglia cameriere
            wait_posto.signal();
        }
        while(!cameriere_ready){ //"In assenza di posti disponibili di tipo appropriato i clienti devono restare in attesa."
            prof_queue.wait();
        }
        //professore occupa il posto al tavolo assegnato
        num_prof_queue--;
        occupati[tavolo_assegnato]++;
        if(tipo[tavolo_assegnato]==0) //se il tavolo era di tipo libero -> viene settato come tavolo prof per i prossimi clienti
        {
            tipo[tavolo_assegnato] = P;
        }
    }
    else if (tipo == S){
        num_stud_queue++
        if(num_prof_queue== 0 || !ok_posto_prof) //prima precedenza ai prof
        {
        if(ok_posto_stud && cameriere_ready){ //se esiste un posto disponibile per lo studente -> sveglia cameriere
            wait_posto.signal();
        }
        //studente occupa il posto al tavolo assegnato
        num_stud_queue--;
        occupati[tavolo_assegnato]++;
        if(tipo[tavolo_assegnato]==0) //se il tavolo era di tipo libero -> viene settato come tavolo studenti per i prossimi clienti
        {
            tipo[tavolo_assegnato] = S;
        }
        }
        while(num_prof_queue>0 || !cameriere_ready || !ok_posto_stud ){ //"In assenza di posti disponibili di tipo appropriato i clienti devono restare in attesa."
            stud_queue.wait();
        }
    }
    while(!ok_servito){ //fino a quando non viene servito -> cliente resta in attesa
            wait_pranzo.wait();
    }
    return tavolo_assegnato;
    }
    
    procedure entry cliente_lascia_tavolo(int posto)
    {
        //EVENTUALMENTE ASSEGNARE IL PROSSIMO POSTO LIBERO AUTOMATICAMENTE QUI
        occupati[posto]--;
        if(occupati[posto]==0){ //se il tavolo si è svuotato -> cambia tipo in libero
            tipo[posto] = 0;
        }
         wait_posto.signal(); //sveglia il cameriere per avvertire del posto liberato, PER EVITARE STARVATION
    }
}


//CREAZIONE ISTANZA
Mensa podere_lagrande;

//TRACCIA PROCESSI
//cameriere -> servitore
while(1){
    podere_lagrande.accogli_cliente();
    <va a prendere il cibo>
    podere_lagrande.servi_cliente();
}
//professore -> once in a while
int prof = P;
int posto_p;
posto_p = podere_lagrande.cliente_vuole_tavolo(prof);
<mangia indisturbato>
podere_lagrande.cliente_lascia_tavolo(posto_p);

//studente -> once in a while
int studente = S;
posto_s = podere_lagrande.cliente_vuole_tavolo(studente);
<mangia indisturbato>
podere_lagrande.cliente_lascia_tavolo(posto_s);

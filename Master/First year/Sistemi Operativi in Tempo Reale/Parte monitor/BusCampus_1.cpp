/* 
Basta con questo traffico di auto e parcheggio selvaggio al campus! II CdA dell'Atenco ha detto "non se ne può più" e ha stabilito di inibire l'accesso alle auto degli studenti. 
Verrà organizzato un servizio navetta dal parcheggio scambiatore Sud, utilizzando un minibus elettrico con capacità di carico pari a 10 unità (passeggeri equivalenti). 
L'Ateneo dispone di mezzi economici limitati e può permettersi un solo minibus.
Per motivi di sicurezza, di promozione della salute, e di equità nella ripartizione dei costi, ogni studente pagherà un costo per l'uso del minibus in relazione alle proprie dimensioni e avrà a disposizione uno spazio equivalente durante il viaggio.
Sono pertanto previsti abbonamenti per 1 unità di spazio (riservati a studentesse e studenti con taglie XS, S e M), 
per 2 unità di spazio (taglie L e XL.), e per 4 unità di spazio (taglie XXL ed oltre).
Gli studenti sono valutati volumetricamente, in sede di iscrizione all'anno accademico, mediante body scanner 3D.
Il minibus carica al parcheggio scambiatore un numero di studenti tate da ottimizzare il riempimento del bus, con il vincolo che la corsa non può partire se non sono riempite almeno 4 unità di spazio e che non può lasciare a terra studenti in presenza di spazio sufficiente.
Un ulteriore vincolo è che anche in caso di continuativo afflusso nessuna taglia di studenti deve rischiare starvation permanente a causa della politica di caricamento adottata.
Gli studenti dell'Università di Parma sono tutti pigri, e invariabilmente - sia durante l'attesa del minibus che durante il tragitto - riposano.
Il minibus nel suo percorso effettua due fermate: una fermata intermedia in corrispondenza al plesso delle Segreterie Studenti ed una fermata di fine linea all'edicola del Campus. 
II percorso di ritorno è simmetrico. Gli studenti possono scendere alla fermata di loro interesse, che non hanno preventivamente comunicato all'autista.
In altre parole, ad ogni fermata l'autista risveglia tutti gli studenti a bordo: quelli interessati alla fermata scendono, mentre gli altri tornano a sedersi. Il minibus ovviamente non può ripartire fino a quando tutti gli studenti che intendono scendere non sono effettivamente scesi, anche se la durata della discesa è in effetti pari a zero (pura sincronizzazione).
Per motivi di efficienza, la fermata presso le Segreterie Studenti è utilizzabile solo per la discesa. 
Il servizio del minibus deve proseguire correttamente fino alla fermata di fine corsa anche nel caso in cui tutti i passeggeri scendono alle Segreterie Studenti. 
Si risolva il problema di programmazione concorrente utilizzando il costrutto monitor, indicandone in modo esplicito la semantica. 
Si modellino esplicitamente il thread "autista" e i thread "studenti" riportandone la traccia di esecuzione. 
I thread studenti sono caratterizzati da due parametri: la taglia (utilizzata dall'autista per la politica di riempimento) e la fermata destinazione (non comunicata all'autista). 
Si evitino deadlock, starvation, attese inutili e attese attive.
*/

//GIUSEPPE RICCIARDI 355695
//se nessuno studente deve rischiare starvation a causa del suo peso -> uso una coda unica

//SEMANTICA MESA -> priorità resta al segnalante

//COSTANTI 
#define PESO_MAX 10
#define TAGLIA_XS 1
#define TAGLIA_S 1
#define TAGLIA_M 1
#define TAGLIA_L 2
#define TAGLIA_XL 2
#define TAGLIA_XXL 4
#define PESO_MINIMO 4
#define N_FERMATE = 3

//DEFINIZIONE + INIZIALIZZAZIONE MONITOR
type Bus = monitor{ 
    bool bus_ready = false;
    bool studenti_scesi = false;
    bool ritorno = false;
    int fermata_attuale = 0; //PARCHEGGIO_SCAMBIATORE = 0, FERMATA_SEGRETERIA = 1, FERMATA_EDICOLA = 2
    int prossima_fermata = 1;
    int num_studenti_a_bordo = 0; //usato per risvegliare gli studenti a bordo
    int peso_studenti_a_bordo = 0;
    int num_studenti_queue = 0;
    int num_studenti_fermata[3]= 0; //ogni posizione dell'array contiene il numero di studenti che devono scendere all'i-esima fermata 
    condition studenti_queue;
    condition bus_ready;
    condition bus_stop;
    condition bus_ripartenza;

    //ENTRY AUTISTA
    procedure entry carica_studenti(){
        bus_ready = true;
        num_studenti_a_bordo = 0;
        peso_studenti_a_bordo = 0;
        if(num_studenti_queue > 0){
            studenti_queue.signal() //se ci sono passeggeri in coda -> sveglia uno (gli altri passeggeri in coda vengono risvegliati eventualmente dal passeggero stesso)
        }
        while ( peso_studenti_a_bordo < PESO_MINIMO ) { //"la corsa non può partire se non sono riempite almeno 4 unità di spazio"
			bus_ready.wait();
		}
        bus_ready = false; //bus non è disponibile perchè è partito
    }
    procedure entry ferma_bus(){
        fermata_attuale = prossima fermata; //aggiorno la fermata attuale
        if (!ritorno){
             prossima_fermata++;
        }else{
            prossima_fermata--;
        }
        //viaggio simmetrico: 0,1,2 -> 2,1,0
        if(prossima_fermata>2){ //fine viaggio andata -> comincia ritorno
            prossima_fermata = 1;
            ritorno = true;
        }
        else if(prossima_fermata < 0){ //fine viaggio ritorno -> comincia andata
            ritorno = false
            prossima_fermata = 1;
        }
        //autista risveglia tutti gli studenti a bordo
		for ( int i = 0; i < num_studenti_a_bordo; i++ ) {
			bus_stop.signal(); 
		}
        //aspetta che tutti gli studenti della fermata siano scesi
		while(!studenti_scesi){
            bus_ripartenza.wait();
        }
        studenti_scesi = false; //resetto variabile false per la prossima fermata
    }

    //ENTRY STUDENTE
    procedure entry prendi_bus(int peso, int fermata){
        //studente vuole salire e si mette in coda
        num_studenti_queue++
        while (!bus_ready || peso_studenti_a_bordo + peso > PESO_MAX ) { //se bus non disponibile oppure se salendo il bus supererebbe la capacità massima -> attendi in coda
			coda_passeggeri.wait();
		}
        //studente sale a bordo del bus
        num_coda_passeggeri--; 
		num_studenti_a_bordo++;
		peso_studenti_a_bordo += peso;
        num_studenti_fermata[fermata]++;
        if(num_studenti_queue>0 && peso_studenti_a_bordo < PESO_MAX ) //se ci sono ancora posti disponibili -> sveglia prossimo studente in coda
        {
            coda_passeggeri.signal();
        }
        else if(peso_studenti_a_bordo >= PESO_MINIMO){ //altrimenti sveglia autista
            bus_ready.signal();
        }
        //studente dorme fino ad arrivo della fermata
        while(fermata_attuale!=fermata){
            bus_stop.wait();
        }
        //studente scende dal bus
        num_studenti_a_bordo--;
		peso_studenti_a_bordo -= peso;
        num_studenti_fermata[fermata]--;
        if(num_studenti_fermata[fermata] == 0){ //se lo studente è l'ultimo a scendere
            studenti_scesi = true;
            bus_ripartenza.signal();
        }
    }

}

//CREAZIONE ISTANZA
Bus bus_campus;

//TRACCIA DI ESECUZIONE
//Autista -> sempre in esecuzione
while(1){
    bus_campus.carica_studenti();
    <VIAGGIA FINO A PRIMA FERMATA> //da 0->1 oppure 2->1
    bus_campus.ferma_bus();
    <VIAGGIA FINO A SECONDA FERMATA> //da 1->2 oppure 1->0
    bus_campus.ferma_bus(); -> //fine viaggio, ricomincia il ciclo caricando gli studenti (a prescindere se si trova in edicola o al parcheggio scambiatore)
}
//Studente -> once in a while
int taglie[5] = [XS, S, M, L, XL, XXL];
int peso = taglie[randint() % 5];
int fermata = randint(1,2) //studente scende solo in segreteria (=1) o edicola (=2) (in teoria potrebbe scendere a 0, non si capisce bene dalla traccia)
bus_campus.prendi_bus(peso, fermata);

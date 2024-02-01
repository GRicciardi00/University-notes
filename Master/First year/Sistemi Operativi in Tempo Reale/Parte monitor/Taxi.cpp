/*
Un utile ed economico strumento per viaggiare tra Quarzazate e Zagora e ritorno è il taxi collettivo. 
Il taxi è dotato 7 posti per passeggeri, oltre all'autista, e parte da Quarzazate solo con almeno 5 passeggeri a bordo.
In assenza di passeggeri, o con un numero insufficiente di passeggeri, l'autista attende al posto di guida. 
In assenza del taxi o se sono in numero insufficiente, i passeggeri restano in attesa in coda. 
Quando un nuovo cliente rileva che con il suo arrivo si è raggiunto un numero sufficiente di passeggeri, egli provvede a risvegliare l'autista e ad attivare il caricamento degli altri clienti in attesa. 
Una volta che tutti i passeggeri sono a bordo, l'autista parte ed effettua il viaggio fino alla città destinazione di Zagora. 
Una volta arrivati a Zagora l'autista risveglia e a scendere i passeggeri a bordo, che eseguono individualmente la visita o le commissioni che intendono effettuare in tale città.
Se infine arrivando ad Quarzazate l'autista trova già almeno 5 clienti in attesa, egli li carica in ordine FIFO fino alla capienza massima di 7.
Quelli in eccedenza restano in attesa sul posto mantenendo la propria posizione in coda.
Il servizio è quindi gestito in modo tale da massimizzare l'occupazione del taxi, compatibilmente con la sua capienza, evitando tuttavia le attese inutili e garantendo equità nel servizio.
Poiché a Zagora non si può pernottare, l'autista attende che tutti i passeggeri caricati all'andata abbiano terminato la visita e siano rientrati a bordo, per poi riportarli a Ouarzazate. 
Una volta scaricati a Quarzazate i passeggeri che hanno compiuto la visita, egli infine si rende disponibile per caricare e trasportare un nuovo gruppo.
Si ipotizzi per semplicità che sia in servizio sulla linea un solo taxi.
*/

//GIUSEPPE RICCIARDI 355695

//SEMANTICA MESA -> priorità resta al segnalante


//COSTANTI GLOBALI
#define N_POSTI 7
#define MIN_POSTI 5

type Taxi = monitor{
    //DEFINIZIONE + INIZIALIZZAZIONE
    bool taxi_disponibile = false;
    int num_passeggeri_queue = 0;
    int passeggeri_selezionati = 0;
    int posti_occupati = 0;
    bool arrivo_Zagoza = false;
    bool ok_ripartenza = false;
    bool arrivo_Quarzazate = false;
    condition attendi_clienti;
    condition clienti_queue;
    condition wait_boarding;
    condition wait_Zagoza;
    condition wait_ripartenza;
    condition wait_unboarding;
    condition wait_Quarzazate;
    //PROCEDURE ENTRY DEL TAXISTA (SERVITORE)
    procedure entry carica_passeggeri(){
        passeggeri_selezionati = 0;
        taxi_disponibile = true;
        //"In assenza di passeggeri, o con un numero insufficiente di passeggeri, l'autista attende..."
        while(num_passeggeri_queue < MIN_POSTI){ 
            attendi_clienti.wait();
        }
        //è stato raggiunto il numero di passeggeri minimo -> sveglio i passeggeri e faccio salire a bordo
        taxi_disponibile = false;
        passeggeri_selezionati = min(num_passeggeri_queue, MAX_POSTI);
        for (int i = 0; i < passeggeri_selezionati; i++)
        {
            clienti_queue.signal();
        }
        //attendo che tutti i passeggeri siano saliti a bordo
        while(posti_occuapati < passeggeri_selezionati){
            wait_boarding.wait();
        }
        //inizia viaggio verso Zagoza
        arrivo_Zagoza = false;
    }
    procedure entry scarica_e_attendi_passeggeri(){
        arrivo_Zagoza = true;
        //risveglio i clienti per avvertire dell'arrivo
        for (int i = 0; i < posti_occupati; i++) 
        {
            wait_Zagoza.signal();
        }
        //"l'autista attende che tutti i passeggeri caricati all'andata abbiano terminato la visita e siano rientrati a bordo."
        while(!ok_ripartenza){  
            wait_ripartenza.wait();
        }
        //l'autista riparte verso Quarzazate
        arrivo_Quarzazate = false;
        ok_ripartenza = false; 
    }

    //PROCEDURE ENTRY DEL CLIENTE
    procedure entry cliente_sale(){
        num_passeggeri_queue++;
        //"Quando un nuovo cliente rileva che con il suo arrivo si è raggiunto un numero sufficiente di passeggeri, egli provvede a risvegliare l'autista..."
        if(num_passeggeri_queue>= MIN_POSTI){ 
            attendi_clienti.signal(); 
        }
        //"In assenza del taxi o se sono in numero insufficiente, i passeggeri restano in attesa in coda..."
        while(!taxi_disponibile || num_passeggeri_queue < MIN_POSTI){ 
            clienti_queue.wait();
        }
        //Cliente sale a bordo del taxi
        num_passeggeri_queue--;
        posti_occupati++;
        //L'ultimo cliente a salire a bordo risveglia l'autista per partire
        if(posti_occupati == passeggeri_selezionati) 
        {
            wait_boarding.signal();
        }
        //Il cliente attende l'arrivo a destinazione
        while (!arrivo_Zagoza)
        {
            wait_Zagoza.wait();
        }
        //Dopo essere stato risvegliato dal tassista, il cliente scende dal taxi per visitare Zagoza.
        posti_occupati--;
    }
    procedure entry cliente_rientra(){
        posti_occupati++;
        //se il numero di clienti selezionati all'inizio è pari a quelli rientrati, il cliente segnala al tassista di poter rientrare.
        if(posti_occupati == passeggeri_selezionati){ 
            ok_ripartenza = true;
            wait_ripartenza.signal();
        }
        //Il cliente attende il rientro a Quarzazate.
        while(!arrivo_Quarzazate){
            wait_Quarzazate.wait();
        }
        //Una volta risvegliato, il cliente lascia il taxi.
        posti_occupati--;
        //Quando l'ultimo cliente è sceso, risveglia l'autista per poter cominciare una nuova corsa.
        if (posti_occupati == 0){ 
            wait_unboarding.signal(); 
        }
    }
    procedure entry scarica_passeggeri(){
        arrivo_Quarzazate = true;
        //L'autista risveglia i clienti per avvertire dell'arrivo a Quarzazate.
        for (int i = 0; i < posti_occupati; i++) 
        {
            wait_Quarzazate.signal();
        }
        //L'autista aspetta che tutti i clienti siano scesi dal taxi prima di procedere con una nuova corsa.
        while(posti_occupati>0){ 
            wait_unboarding.wait();
        }
    }
}

//CREAZIONE ISTANZA
Taxi taxi;

//TRACCIA DI ESECUZIONE
//thread tassista -> servitore, eseguito sempre
while(1){
taxi.carica_passeggeri();
<viaggia a Zagora>;
taxi.scarica_e_attendi_passeggeri();
<viaggia a Quarzazate>
taxi.scarica_passeggeri();
}

//thread cliente -> once in a while
...
taxi.cliente_sale();
<visita Zagora>
taxi.cliente_rientra();
...
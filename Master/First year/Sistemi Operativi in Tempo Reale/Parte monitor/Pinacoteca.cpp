/*
Una pinacoteca che ospita opere di arte classica prevede precise regole di accesso per i visitatori.
Per preservare lo stato di conservazione delle opere, al più N visitatori possono essere contemporaneamente presenti entro la pinacoteca.
Ciascun visitatore riceve un depliant illustrativo, mentre con un sovrapprezzo sul costo del biglietto può richiedere una presentazione introduttiva delle opere da parte di un esperto di storia dell'arte.
La presentazione da parte dell'esperto viene effettuata in presenza di almeno K1 richieste e per un gruppo di al più K2 persone (ovviamente K1<K2<N; i partecipanti alla presentazione "contano" tra i visitatori).
Al termine della presentazione dell’esperto, gli utenti del gruppo visitano liberamente la pinacoteca con il solo ausilio del depliant, mentre l'esperto può effettuare una nuova presentazione se sussistono le condizioni sopracitate. 
La politica di ammissione degli utenti deve privilegiare l'eventuale presenza di almeno K1 visitatori in attesa della presentazione con esperto non impegnato. 
(A titolo di esempio: N=40, K1=5, K2=10).
*/

//GIUSEPPE RICCIARDI 355695
//SEMANTICA MESA -> priorità resta al segnalante

//privilegio ai visitatori con presentazione -> uso due code separate

//COSTANTI GLOBALI
#define N 40
#define K1 5
#define K2 10

//DEFINIZIONE + INIZIALIZZAZIONE MONITOR
type Pinacoteca{
    //visitatori in coda
    int num_liberi_queue = 0;
    int num_guidati_queue = 0;
    int num_risveglio_visita_guidata = 0;
    int dimensione_gruppo = 0;
    //visitatori entrati
    int num_visitatori_liberi = 0;
    int num_visitatori_guidati = 0;
    bool guida_ready = false;
    bool gruppo_ready = false;
    //bool ok_inizio_spiegazione = false; vedi commento *** procedure entry ingresso guidato
    bool ok_fine_spiegazione = false;
    condition queue_liberi;
    condition queue_guidati;
    condition wait_gruppo;
    condition fine_spiegazione;

    //PROCEDURE ENTRY
    //guida
    procedure entry inizia_visita_guidata(){
        guida_ready = true;
        termina_visita = false;
        num_risveglio_visita_guidata = min(num_guidati_queue,K2);
        //risveglio fino a max K2 clienti guidati
        for (int i = 0; i < num_risveglio_visita_guidata; i++) 
        {
            queue_guidati.signal();
        }
        //la guida aspetta che il gruppo abbia raggiunto dimensione adatta
        while (num_visitatori_liberi<K1 || dimensione_gruppo + num_visitatori_liberi >N)
        {
            wait_gruppo.wait();
        }
        //inizio della visita
        guida_ready = false;
        num_risveglio_visita_guidata = 0;
        // *** guida segnala che la visita ha inizio
        /*
        ok_inizio_spiegazione = true;
        for (int i = 0; i < dimensione_gruppo; i++)
        {
           inizio_spiegazione.signal();
        }
        */

    }
    //cliente guidato
    procedure entry ingresso_guidato(){
        num_guidati_queue++;
        //clienti guidati aspettano in coda se la guida non è disponibile oppure se il gruppo ha raggiunto capacità massima
        while (!guida_ready || dimensione_gruppo == K2) 
        {
            queue_guidati.wait();
        }
        //se il gruppo ha posti disponibili -> cliente esce dalla coda e si aggiunge al gruppo
        num_guidati_queue--;
        dimensione_gruppo++;
        //se il gruppo ha dimensione valida -> sveglia la guida
        if( dimensione_gruppo >= K1 && num_visitatori_liberi + min(dimensione_gruppo, K2)<=N ){
            wait_gruppo.signal();
        }
        // *** cliente aspetta che la visita inizi -> nella traccia non è presente questa attesa, andrebbe fatta visto che la guida potrebbe iniziare la spiegazione mentre l'ultimo del gruppo non è stato ancora aggiunto
        /*
        while (!ok_inizio_spiegazione)
        {
            inizio_spiegazione.wait();
        }
        */
        //cliente guidato entra nel museo
        dimensione_gruppo--;
        num_visitatori_guidati++;
        //cliente guidato attende la fine della spiegazione
        while(!ok_fine_spiegazione){
            fine_spiegazione.wait();
        }
        //una volta finita la spiegazione diventa un cliente libero
        num_visitatori_guidati--;
        num_visitatori_liberi++;
    }
    //cliente libero
    procedure entry ingresso_libero(){ 
        num_liberi_queue++;
        //clienti liberi aspettano solo se il numero totale dei clienti supera N.
        while (num_visitatori_guidati+num_visitatori_liberi>=N) 
        {
            queue_liberi.wait();
        }
        //cliente libero entra nel museo lasciando la coda
        num_liberi_queue--;
        num_visitatori_liberi++;
    }
    //guida
    procedure entry termina_visita_guidata(){
    //la guida risveglia tutti i clienti guidati avvertendoli del termine della visita.
    ok_fine_spiegazione = true;
    for (int i = 0; i < num_visitatori_guidati; i++)
    {
            fine_spiegazione.signal();
    }
        ok_inizio_visita = false;
    //ipoteticamente si potrebbe aggiungere un'altra condizione d'attesa in cui la guida attende che effettivamente il gruppo sia stato completamente rilasciato prima di proseguire.
    }
    //cliente libero
    procedure entry uscita(){
        num_visitatori_liberi--;
        //controllo se è possibile far entrare un altro gruppo
        if(dimensione_gruppo >= K1 && dimensione_gruppo+num_visitatori_liberi <=N && guida_ready ){
            wait_gruppo.signal();
        }
        //altrimenti risveglio i visitatori liberi (sicuramente si ha un posto libero)
        else if (num_liberi_queue>0){
            queue_liberi.signal()
        }
    }
}
//CREAZIONE ISTANZA
Pinacoteca brera;

//TRACCIA PROCESSI
//guida ->servitore
while(1){
    brera.inizia_visita_guidata();
    <svolgimento visita guidata>;
    brera.termina_visita_guidata();
}
//cliente libero -> once in a while
brera.ingresso_libero();
<visita libera>
brera.uscita();

//cliente guida -> once in a while
brera.ingresso_guidato();
<visita guidata e visita libera>
brera.uscita();
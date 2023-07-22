//MATRICOLA 355695, GIUSEPPE RICCIARDI ESAME 14/07/2023, VOTO: 26/30



//CORREZIONI DEL PROF
//Va cambiata la traccia dei thread, il ciclo for non va bene in quanto serializza l'esecuzione. Devono essere usati due thread bus diversi.
//Tutti i booleani e le code (eccetto le code iniziali che devono essere uniche) devono essere messi in array.


//SEMANTICA MESA -> priorità al segnalante


//***DEFINIZIONE COSTANTI GLOBALI***
#define N 2 //numero navette
#define MIN 6 //numero minimo studenti in una navetta
#define MAX 10 //numero massimo studenti in una navetta

//La soluzione proposta non presenta starvation, in quanto la navetta parte solo se ha un numero di studenti m: MIN<m<MAX, e parte solo quando tutti gli studenti sono saliti.
//Inoltre lo studente sale solo quando una delle due navette è disponibile (controllando se la variabile ok_sali è true), a prescindere da quale essa sia.

//***DEFINIZIONE DELLE VARIABILI ED INIZIALIZZAZIONE DEL MONITOR***
Type Limo_service = Monitor{
    int posti_occupati[N];//indicano il numero degli studenti presenti nelle navette
    int selezionati[N];//numero studenti selezionati per salire sulla navetta
    //inizializzazione navette
    for (int i = 0; i < N; i++)
    {
        posti_occupati[i]=0;
        selezionati[i]=0;
    }
    int num_studente_queue = 0; //numero studenti in coda  
    int id_backup = 0; //viene usato dallo studente per ricordarsi l'id della navetta su cui è salito
    bool ok_sali = false; //indica se lo studente può salire sulla navetta
    bool ok_scendi = false; //indica se lo studente può scendere dalla navetta
    bool ok_partenza = false; //indica se la navetta può partire.
    bool arrivato_seconda_fermata = false; //indica se la navetta è arrivata alla seconda fermata.
    bool arrivato_prima_fermata = false; //indica se la navetta è arrivata alla prima fermata.
    condition studente_queue; //variabile di condizione per la coda degli studenti che vogliono salire sulla navetta.
    condition attendi_navetta; //variabile di condizione per la navetta che attende che ci siano abbastanza studenti in coda.
    condition seconda_fermata; //variabile di condizione per segnalare l'arrivo alla seconda fermata.
    condition prima_fermata; //variabile di condizione per segnalare l0arrivo alla prima fermata.
    condition wait_salita; //variabile di condizione per la navetta che attende che tutti gli studenti siano saliti sulla navetta.
    condition wait_discesa; //variabile di condizione per la navetta che attende che tutti gli studenti siano scesi dalla navetta.

    //***PROCEDURE ENTRY STUDENTE***

    //studente in ghiaia vuole salire sul bus-> risveglia le navette in attesa solo quando viene superata la soglia minima.
    int procedure entry studente_vuole_salire(){
        num_studente_queue++;
        //se il numero di studenti raggiunge la quota minima, risveglia la navetta.
        if(num_studente_queue >= MIN){
            attendi_navetta.signal();
        }
        //se la navetta non è disponibile -> attende.
        while(!ok_sali){
            studente_queue.wait()
        }
        //studente lascia la coda e sale sulla navetta.
        num_studente_queue--;
        posti_occupati[id_backup]++;
        //se anche l'ultimo studente è salito -> risveglia la navetta che può partire.
        if(posti_occupati[id_backup] == selezionati[id_backup]){
            ok_partenza = true;
            wait_salita.signal();
        }
        //lo studente attende di essere arrivato alla seconda fermata.
        while(!arrivato_seconda_fermata){
            seconda_fermata.wait();
        }
        //una volta arrivato alla seconda fermata lo studente scende dalla navetta.
        posti_occupati[id_backup]--;
        //Se è l'ultimo a scendere -> avvisa la navetta che tutti gli studenti sono scesi.
        if(posti_occupati[id_backup] == 0){
            ok_scendi = true;
            wait_discesa.signal();
        }
        //procedura termina ed inizio fase attiva.
        return id_backup; //lo studente ritorna l'id della navetta su cui è salito -> utilizzato per il rientro.
    }
    //studente in coda vuole rientrare a casa -> sale sulla navetta su cui era salito precedentemente svuotata.
    procedure entry studente_rientra(int id_navetta){
        //studente risale sulla navetta su cui era salito.
        posti_occupati[id_navetta]++;
        //se è l'ultimo a salire -> avvisa la navetta che tutti gli studenti sono saliti.
        if(posti_occupati[id_navetta] == selezionati[id_navetta]){
            ok_rientro = true;
            wait_rientro.signal();
        }
        //lo studente attende di essere arrivato alla prima fermata.
        while(!arrivato_prima_fermata){
            prima_fermata.wait();
        }
        //una volta arrivato alla prima fermata lo studente scende dalla navetta.
        posti_occupati[id_navetta]--;
        //Se è l'ultimo a scendere -> avvisa la navetta che tutti gli studenti sono scesi.
        if(posti_occupati[id_navetta] == 0){
            ok_scendi = true;
            wait_discesa.signal();
        }
        //lo studente torna a casa per del meritato riposo.
    }
    //***PROCEDURE ENTRY NAVETTA***
    //Fermata Piazza Ghiaia: la navetta deve far salire gli studenti e partire per il campus.
    procedure entry prima_fermata(int id_navetta){
        //la navetta è vuota ed è disponibile per far salire gli studenti.
        ok_sali = false;
        ok_partenza = false;
        id_backup = id_navetta; //viene salvato l'id della navetta su cui sale lo studente -> utilizzato per la discesa
        selezionati[id_navetta] = 0;
        //la navetta risveglia gli studenti in coda precedenti.
        for (int i = 0; i < min(num_studente_queue,MAX); i++)
        {
            studente_queue.signal();
        }
        //la navetta attende che in coda ci sia un numero sufficiente di studenti.
        while(num_studente_queue < MIN){
            attendi_navetta.wait();
        }
        //è stato raggiunto un numero sufficiente nella coda, risveglio fino a MAX studenti per farli salire.
        ok_sali = true;
        selezionati[id_navetta] = min(num_studente_queue,MAX); //in questo punto selezionati sarà sicuramente maggiore>MIN 
        for(int i = 0; i < selezionati; i++){
            studente_queue.signal();
        }
        //la navetta attende che tutti gli studenti selezionati siano saliti a bordo.
        while(!ok_partenza){
            wait_salita.wait();
        }
        //la navetta parte per la seconda fermata.
        arrivato_seconda_fermata = false;
    }
    //Fermata campus: la navetta avvisa che si è arrivati, attende che tutti gli studenti siano scesi, si mette in attesa del rientro di tutti i passeggeri e riparte.
    procedure entry seconda_fermata(int id_navetta){
        //arrivo alla seconda fermata -> avviso gli studenti di scendere.
        arrivo_seconda_fermata = true;
        for(int i = 0; i < posti_occupati[id_navetta]; i++){
            seconda_fermata.signal();
        }
        //attendo che tutti gli studenti siano scesi.
        while(!ok_scendi){
            wait_discesa.wait();
        }
        ok_scendi = false;
        //una volta che tutti gli studenti sono scesi -> la navetta attende che gli studenti rientrano.
        while(!ok_rientro){
            wait_rientro.wait();
        }
        ok_rientro = false;
        //la navetta parte per la prima fermata.
        arrivato_prima_fermata = false;
    }
    //ritorno al Campus-> La navetta attende che tutti gli studenti siano stati scaricati per poter proseguire da capo il giro.
    procedure entry scarica_studenti(int id_navetta){
        //arrivo alla prima fermata -> avviso gli studenti di scendere.
        arrivo_prima_fermata = true;
        for(int i = 0; i < posti_occupati[id_navetta]; i++){
            prima_fermata.signal();
        }
        //attendo che tutti gli studenti siano scesi.
        while(!ok_scendi){
            wait_discesa.wait();
        }
        ok_scendi = false;
        //la navetta è sicuramente vuota -> procede a caricare nuovamente gli studenti per un'altra corsa.
    }
}

//***ISTANZIAZIONE MONITOR***
Limo_service limo_service;

//***TRACCIA D'ESECUZIONE DEI THREAD***
//thread navetta -> servitore, sempre in esecuzione
while (1){
    for (int i = 0; i < N; i++)
    {
    limo_service.prima_fermata(i);
    <viaggio seconda fermata>
    limo_service.seconda_fermata(i);
    <viaggio prima fermata>
    limo_service.scarica_studenti(i);
    }
}
//thread studente -> cliente, once in a while
int id_navetta_studente = 0;
...
id_navetta_studente = limo_service.studente_vuole_salire();
<fase di "studio">
limo_service.studente_rientra(id_navetta_studente);
...

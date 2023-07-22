/*	
	Safari: per il trasporto turisti, c'è un fuoristrada che può trasportare al max 6 persone.
	2 tipi di visitatori del parco safari: quelli che hanno prenotato e quelli che non hanno prenotato.
	Ovviamente chi ha prenotato ha precedenza sugli altri.
	Quando si raggiunge il numero minimo di 4 visitatori, se è disponibile la guida
	allora la visita può iniziare; se non c'è spazio attendono.
	La visita guidata prevede: 
    - trasporto in fuoristrada fino al fiume con illustrazione iniziale del parco safari;
    - fase in cui ogni visitatore visita liberamente il parco per scattare foto; la guida attenderà che tutti abbiano
      finito prima di ripartire.
    Utilizzare il costrutto monitor per risolvere il problema assegnato, e 
    specificare la semantica utilizzata.
*/

//GIUSEPPE RICCIARDI 355695
//SEMANTICA MESA -> priorità resta al segnalante

//COSTANTI GLOBALI
#define MIN 4
#define MAX 6

type Safari = monitor{
    //DEFINIZIONE + INIZIALIZZAZIONE:
    int num_queue_prenotati = 0;
    int num_prenotati = 0;
    int num_queue_liberi = 0;
    int num_liberi = 0;
    int dim_gruppo = 0;
    int sulla_jeep = 0;
    int cliente_scesi = 0;
    bool guida_disponibile = false;
    bool ok_partenza = false;
    bool ok_fine_presentazione = false;
    bool ok_rientro = false;
    bool ok_fine_rientro = false;
    condition queue_prenotati;
    condition queue_liberi;
    condition wait_gruppo;
    condition wait_partenza;
    condition wait_fine_presentazione;
    condition wait_rientro;
    condition wait_fine_rientro;
    //PROCEDURE ENTRY GUIDA
    procedure entry inizia_escursione(){
        //La guida è pronta per il safari
        sulla_jeep = 0;
        clienti_scesi = 0
        guida_disponibile = true;
        //La guida risveglia i clienti prenotati in coda che hanno la precedenza sui liberi
        for (int i = 0; i < min(num_queue_prenotati,MAX); i++)
        {
            queue_prenotati.signal();
        }
        //Se ci sono ancora posti disponibili -> risveglia i clienti liberi in coda
        if(num_prenotati+num_liberi < MAX){
            for (int i = 0; i < min(num_queue_liberi,MAX-num_prenotati-num_liberi); i++)
            {
                queue_liberi.signal();
            }
        }
        //La guida aspetta che il gruppo sia formato
        while(num_prenotati+num_liberi < MIN){
            wait_gruppo.wait();
        }
        //gruppo formato -> salgono tutit sulla jeep
        sulla_jeep = num_prenotati+num_liberi;
        guida_disponibile = false;
        ok_partenza = true;
        for (int i = 0; i < sulla_jeep; i++)
        {
            wait_partenza.signal();
        }
        //guida inizia ad illustrare (per semplicità l'inizio della spiegazione sincronizza anche l'arrivo al fiume)
        ok_termina_illustrazione = false;
    } 
    procedure entry termina_illustrazione()
    {
        //illustrazione terminata -> inizio visita libera
        ok_termina_illustrazione = true;
        for (int i = 0; i < sulla_jeep; i++)
        {
            wait_termina_illustrazione.signal();
        }
        //guida aspetta che tutti abbiano finito la visita libera
        ok_rientro = false; 
    }
    procedure entry termina_escursione(){
        ok_fine_rientro = false;
        //guida aspetta il rientro sulla jeep dei clienti
        while (!ok_rientro)
        {
            wait_rientro.wait();
        }
        //tutti i clienti sono rientrati sulla jeep -> avvisa della fine del safari
        ok_fine_rientro = true;
        for (int i = 0; i < sulla_jeep; i++)
        {
            wait_fine_rientro.signal();
        }
    }

    //PROCEDURE ENTRY CLIENTI
    //cliente prenotato vuole entrare nel safari
    procedure entry cliente_prenotato_want_to_join(){
        num_queue_prenotati++;
        //se la guida non è disponibile o il gruppo è pieno -> resta in coda
        while (!guida_libera || num_prenotati + num_liberi >= MAX)
        {
            queue_prenotati.wait();
        }
        //se la guida è disponibile e il gruppo non è pieno -> entra nel gruppo
        num_queue_prenotati--;
        num_prenotati++;
        if(num_prenotati+num_liberi >= MIN){
            wait_gruppo.signal();
        }
        //aspetta la partenza del gruppo
        while(!ok_partenza){
            wait_partenza.wait();
        }
        //una volta partito il gruppo -> aspetta la fine della presentazione
        num_prenotati--;
        while(!ok_fine_presentazione){
            wait_fine_presentazione.wait();
        }
        //fine presentazione -> il cliente visita liberamente il safari.
        cliente_scesi++;
    }
    procedure entry cliente_libero_want_to_join(){
        //se la guida non è disponibile o il gruppo è pieno oppure ci sono prenotati in coda -> resta in coda
        while (!guida_libera || num_prenotati + num_liberi >= MAX || num_queue_prenotati>0 )
        {
            queue_liberi.wait();
        }
         //se la guida è disponibile e il gruppo non è pieno e non ci sono prenotati in coda -> entra nel gruppo
        num_queue_liberi--;
        num_liberi++;
        if(num_prenotati+num_liberi >= MIN){
            wait_gruppo.signal();
        }
        //aspetta la partenza del gruppo
        while(!ok_partenza){
            wait_partenza.wait();
        }
        //una volta partito il gruppo -> aspetta la fine della presentazione
        num_liberi--;
        while(!ok_fine_presentazione){
            wait_fine_presentazione.wait();
        }
        //fine presentazione -> il cliente visita liberamente il safari.
        cliente_scesi++;
    }
    procedure entry cliente_rientra(){
        //cliente risale sulla jeep
        cliente_scesi--;
        //ultimo cliente è rientrato -> risveglia alla guida per ritornare al punto di partenza
        if(cliente_scesi == 0){
            ok_rientro = true;
            wait_rientro.signal();
        } 
        // il cliente si mette in attesa dell'arrivo al punto di partenza
        while (!ok_fine_rientro)
        {
            wait_fine_rientro.wait();
        }
    }
}
//CREAZIONE ISTANZA
Safari fasano;
//TRACCIA DI ESECUZIONE
//thread guida -> servitore, sempre in esecuzione
while (1)
{
    fasano.inizia_escursione();
    <arrivo al fiume ed illustrazione>
    fasano.termina_illustrazione();
    <attendi rientro dalla visita libera del safari>
    fasano.termina_escursione();
}

//thread cliente prenotato -> once in a while
...
fasano.cliente_prenotato_want_to_join();
<visita del parco>
fasano.cliente_rientra();
...

//thread cliente non prenotato -> once in a while
...
fasano.cliente_libero_want_to_join();
<visita del parco>
...
fasano.cliente_rientra();
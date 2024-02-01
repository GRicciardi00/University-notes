

//SEMANTICA MESA -> while per le wait e if per i signal.
#define K = 10; //numero posti massimo locale

monitor Pizzeria {
    bool posti[K] = false;
    int num_A, num_P = 0;
    int num_liberi = K;
    condition queue_A, queue_P, attendi_clienti, attendi_ordinazione,attendi_serivizio;
    bool ok_ordine, ordinazione, servito = false;

    procedure entry accetta_cliente(){
        while(num_A == 0 && num_liberi==0){
            attendi_clienti.wait(); //se non ci sono clienti Asporto o posti disponibili fai attendere i clienti
        }
        ok_ordina = true; //attiva flag per rendere disponibile l'ordinazione
        if(num_A>0){ //fai entrare i clienti
            queue_A.signal();
        }
        else if  (num_P>0 && num_liberi>0){
            queue_P.signal();
        }
        ordinazione = false;
        while(!ordinazione){
            attendi_ordinazione.wait(); //aspetta che un cliente abbia ordinato
        }
    }
    procedure entry servi_cliente(){
        servito = true;
        attendi_servizio.signal();
    }
    procedure entry pizza_ordinazione(){ //entry per clienti asporto
        if(num_A == 0){ //se il numero di clienti A è uguale a zero -> è il turno del cliente attuale
            attendi_clienti.signal(); 
            num_A++;
            while(!ok_ordina){
                queueA.wait() //wait cliente A fino a quando il pizzaiolo non da l'ok per ordinare
            }
            num_A--;
            ok_ordina = false;
            ordinazione = true;
            attendi_ordinazione.signal();
            while(!servito){
                attendi_servizio.wait(); //aspetta che il pizzaiolo dia la pizza
            }
            servito = false;
        }
    }
    procedure entry entra(int posto_occupato){ //il posto da occupare viene passato all'entry
        if (num_P == 0){
            attendi_clienti.signal();
        }
        num_P++;
        while(!ok_ordina){ //basta solo questo perchè nel momento in cui ok ordina = true significa che è presente almeno un posto disponibile
            queue_P.wait();
        }
        num_P--;
        num_liberi--;
        attendi_ordinazione.signal();
        while(!servito) attendi_servizio.signal();
        servito = false;
        for (int i = 0; i < K; i++)
        {
            if(posti[i]== false){
                posto_occupato = i; //setta il posto passato alla funzione con l'indice che indica la posizione del primo posto disponibile 
                posto[i]=true;
            }
        }
    }
    procedure entry esci(int posto_occupato){
        posti[posto_occupato]= false;
        posti_liberi++;
        attendi_clienti.signal() //evita attesa inutile di un cliente P
    }
}
    //creazione istanza
    Pizzeria speedy_pizza;

    //Traccia processo A (once on a while)
    speedy_pizza.ordina_pizza();

    //Traccia processo P (once on a while)
    int tavolino;
    speedy_pizza.entra(tavolino);
    <mangia>
    speedy_pizza.esci(tavolino);


    //Traccia processo pizzaiolo (sempre)
    while (1)
    {
        speedy_pizza.accetta_cliente();
        <prepara pizza>
        speedy_pizza.servi_cliente();
    }
    
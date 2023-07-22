/*
Un negozio di barbiere ospita, oltre al barbiere stesso, una sedia reclinabile su cui viene fatto accomodare il cliente durante il servizio ed N sedie per i clienti in attesa.
In assenza di clienti, il barbiere si sistema sulla sedia reclinabile e si addormenta.  
Ogni cliente che arriva quando il negozio è vuoto deve quindi provvedere a risvegliare il barbiere addormentato. 
I clienti che arrivano con il barbiere già al lavoro si accomodano sulle sedie, se ci sono posti disponibili, oppure lasciano il negozio, se tutte le sedie sono occupate.
Si risolva il problema mediante il costrutto monitor prestando particolare attenzione ad evitare situazioni di "corsa" nell'interazione tra barbiere e clienti.
*/
//SEMANTICA MESA -> while con wait ed if con signal
#define Nchairs = 5
monitor Salone_barbiere = {
	int waiting_c = 0; //0...Nchairs;		
	bool barber_busy = true; //barbiere inizializzato dorme
	bool chair_occupied, finished = false;
	condition barber_ready, customer_ready, end_of_service, exited_customer;
	procedure entry enter_shop(bool served){ //entry per il cliente
		if (waiting_c == Nchairs){
			served = false //tutte le sedie sono occupate -> lascia il negozio
		}
		else{ //c'è almeno un posto libero
			waiting_c++; //aumento posti occupati 
			while(barber_busy){ 
				barber_ready.wait(); //attendo che il barbiere finisca di lavorare
			}
			waiting_c-- //procedo al prossimo cliente -> decremento clienti in coda
			chair_occupied = false;
			customer_ready.signal(); //sveglio cliente pronto ed inizio taglio capelli
			while(not finished){
				end_of_service.wait(); //aspetto fine taglio capelli
			}
			finished = false;
			exited_customer.signal(); //faccio uscire il cliente
			served = true;
		}
	}
	procedure entry get_next_customer(){ //entry per il barbiere
		barber_busy = false;
		barber_ready.signal();
		while(not chair_occupied){
			customer_ready.wait();
		}
		chair_occupied = false;
		barber_busy = true;
	}
	procedure entry finish_customer(){ //entry per il barbiere
		<prende il denaro e rilascia la ricevuta>;
		finished = true;
		end_of_service.signal();
		while(finished){
			exited_customer.wait();
		}
	}
}

//Creazione istanza ADT
Salone_barbiere b_shop();
//Customer i
repeat /*once in a while*/
bool done = false;
while(done){
	b_shop.enter_shop(done);
	if (not done) sleep(30) //se non è finito allora aspetta 30 minuti, tempo per taglio di capelli e richiama enter_shop
}
<do things> /*for about 1 month*/
forever
//Barber
while(1){
	b_shop.get_next_customer();
	<service>;
	b_shop.finish_customer();
}
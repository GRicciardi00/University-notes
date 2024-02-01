monitor BoatPooling {
    //coda per linux hackers
    condition queue_LH;
    int queue_LH_count = 0;
    //coda per microsoft employees
    condition queue_ME;
    int queue_ME_count = 0;
    //attesa della barca per le condizioni di attraversamento
    condition wait_for_passengers;
    //attesa dei passeggeri durante l'attraversamento del fiume
    condition river_crossing;
    //stato della barca
    bool boat_busy = true;
    //flag per attesa/selezione passeggeri
    int LH_selected = 0;
    int ME_selected = 0;

    void procedure entry LH_want_to_board(){
        if (!boat_busy){//se la barca non è piena->fai entrare i passeggeri
            wait_for_passengers.signal()
        }
        queue_LH_count++
        while(LH_selected == 0){
            queue_LH.wait() //il server barca valuta le condizioni ed evenutlamente segnala LH
        }
        queue_LH_count--; //rimuovi dalla coda
        LH_selected --;
        river_crossing.wait(); //una volta entrato nella barca, aspetta l'attraversamento del fiume
    }
    void procedure entry ME_want_to_board(){
        if (!boat_busy){
            wait_for_passengers.signal()
        }
        queue_ME_count++
        while(ME_selected == 0){
            queue_ME.wait() //il server barca valuta le condizioni ed evenutlamente segnala LH
        }
        queue_ME_count--; //rimuovi dalla coda
        ME_selected --;
        river_crossing.wait(); //una volta entrano nella barca, aspetta l'attraversamento del fiume
    }
    void procedure entry board_passenger(){
        boat_busy = false;
        while((queue_LH_count + queue_ME_count < 4) || 
              (queue_LH_count == 1 && queue_ME_count == 3) ||
              (queue_LH_count == 3 && queue_ME_count == 1)){
            wait_for_passengers.wait(); //se in coda ci sono combinazioni non accettate di equipaggio -> aspetta altri passeggeri
        }
        boat_busy = true;
        if(queue_LH_count>= 2 && queue_ME_count >= 2){ //se ci sono due linux e due microsoft -> seleziona due di ogni tipo ed invia 2 segnali di ogni tipo alle code
            LH_selected = 2;
            ME_selected = 2;
            for(int i = 0; i<2; i++) queue_LH.signal();
            for(int i = 0; i<2; i++) queue_ME.signal();
        }
        else if(queue_LH_count >= 4){ //stesso ragionamento sopra ma con 4 linux
            LH_selected = 4;
            for (int i = 0; i<4; i++) queue_LH.signal();
        }
        else if(queue_ME_count >= 4){ //stesso ragionamento sopra ma con 4 microsoft
            ME_selected = 4;
            for (int i = 0; i<4; i++) queue_ME.signal();
        }
    } //fine procedura entry
    void procedure entry return_boat()
    {
        for (int i = 0; i<4; i++) river_crossing.signal(); //fiume attraversato
    }
}
//Creazione istanza ADT
BoatPooling river_monitor();

//Thread di tipo linux Hacker
<hack()>        //cliente, richiesta occasionale di servizio
river_monitor.LH_want_to_board();
<more_hack()>

//Thread di tipo Microsoft Employee
<fix_bugs()>
river_monitor.ME_want_to_board();
<more_fixes()>

//Thread di tipo Boat
while(1){       //servitore, fa sempre questo!
    river_monitor.board_passenger();
    <cross_river()>;
    river_monitor.return_boat();
}
#include "pintos_thread.h"
#include <stdio.h>

struct station {
	struct condition *cond_train_arrived;
	struct condition *cond_all_passengers_seated;
	struct lock *lck;
	int station_waiting_passengers;
	int train_empty_seats;
	int train_standing_passengers;
};

void print_station(struct station *station) 
{
	printf("[Station|Waiting Passengers: %d, Train Standing Passengers: %d, Train Empty Seats: %d]\n",
			station->station_waiting_passengers,station->train_standing_passengers,station->train_empty_seats);
}

void station_init(struct station *station)
{
	station->cond_train_arrived = malloc(sizeof(struct condition));
	station->cond_all_passengers_seated= malloc(sizeof(struct condition));
	station->lck = malloc(sizeof(struct lock));
	cond_init(station->cond_train_arrived);
	cond_init(station->cond_all_passengers_seated);
	lock_init(station->lck);
	station->station_waiting_passengers = 0;
	station->train_empty_seats = 0;
	station->train_standing_passengers = 0;
	printf("init ->"); print_station(station);
}

void station_load_train(struct station *station, int count)
{
	lock_acquire(station->lck);
	station->train_empty_seats = count;
	printf("train arrives (count: %d)->", count); print_station(station);

	while ((station->station_waiting_passengers > 0) && (station->train_empty_seats > 0)) {
		cond_broadcast(station->cond_train_arrived,station->lck);
		cond_wait(station->cond_all_passengers_seated,station->lck);
	}
	station->train_empty_seats = 0;
	lock_release(station->lck);
}

 void station_wait_for_train(struct station *station)
{
	lock_acquire(station->lck);
	station->station_waiting_passengers++;
	printf("passenger arrived ->"); print_station(station);
	while (station->train_standing_passengers == station->train_empty_seats) //wait for train with empty seats space
		cond_wait(station->cond_train_arrived,station->lck);
	station->train_standing_passengers++;
	station->station_waiting_passengers--;
	printf("passenger boarding ->"); print_station(station);
	lock_release(station->lck);

}

void station_on_board(struct station *station)
{
	lock_acquire(station->lck);
	station->train_standing_passengers--;
	station->train_empty_seats--;
	printf("passenger on board"); print_station(station);
	if ((station->train_empty_seats == 0) || (station->train_standing_passengers == 0))
		cond_signal(station->cond_all_passengers_seated,station->lck);
	
	lock_release(station->lck);
}


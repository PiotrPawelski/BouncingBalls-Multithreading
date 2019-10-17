#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <signal.h>

int maxCol = 0;
int maxRow = 0;
bool quit = false;
bool block = false;
pthread_t ball [10];

int down = 0;

static pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t	mutex	= PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t	cond	= PTHREAD_COND_INITIALIZER;
static pthread_cond_t	cond2	= PTHREAD_COND_INITIALIZER;



void *quiter(void *arg)
{	
	int ch = 0;	

	while(true){
		ch = getch();
		if(ch == 32) break;		
	}
	
	quit = true;
	
	pthread_cond_broadcast(&cond);	
	
	for(int i = 0; i < 10; i++)
		pthread_join(ball[i], NULL);
	
	endwin();  
	printf("Program wykonał i zamknął się poprawnie!\n");  
	exit(0);
}

void generateMap(){
    
	for(int i = 0; i < maxCol; i++){
		mvprintw( 0, i, "-" ); 
		mvprintw( maxRow - 1, i, "-" );
	}

	for(int i = 0; i < maxRow; i++){
		mvprintw( i, 0, "|" ); 
		mvprintw( i, maxCol - 1, "|" );
	}

}


void *generateBall(void *arg){
	int direction = rand() % 7;
	int speed = rand() % 21 + 20;

	int column = maxCol/2;
	int row = 1;
	int sleepTime = 1000000 / speed;
	
	bool crossed = false;
	bool blocking = false;
	
	pthread_mutex_lock( &cs_mutex );
    
	mvprintw( row, column, "O" );
	refresh();	

	pthread_mutex_unlock( &cs_mutex );
	
	while(speed > 5){
		
		if(quit) break;

		switch(direction){
			case 0:
				while(column + 3 < maxCol - 1 && row + 1 < maxRow - 1){
					if(quit) break;
					usleep(sleepTime);					
					if(down < 2 || crossed || row < maxRow - 10){
						if(!block || blocking){
							pthread_mutex_lock( &cs_mutex );
							mvprintw( row, column, " " );
							column += 3;
							row++;
							if (row > maxRow - 10 && !crossed) {
								down++;
								crossed = true;
							}
							mvprintw( row, column, "O" );
							refresh();
							pthread_mutex_unlock( &cs_mutex );
						}
						else{
							pthread_mutex_lock(&mutex);
							pthread_cond_wait(&cond2, &mutex);
							pthread_mutex_unlock(&mutex);
						}
					}
					else{
						pthread_mutex_lock(&cs_mutex);
						pthread_cond_wait(&cond, &cs_mutex);
						pthread_mutex_unlock(&cs_mutex);
					}
					
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column + 3 >= maxCol - 1 && row + 1>= maxRow - 1){
					direction = 7;
					crossed = false;
					block = true;
					blocking = true;
				}
				else if(column + 3 >= maxCol - 1){
					direction = 6;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row + 1 >= maxRow - 1){
					direction = 13;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}
			
				break;
			case 1:
				while(column + 2 < maxCol - 1 && row + 1 < maxRow - 1){
					if(quit) break;
					usleep(sleepTime);
					if(down < 2 || crossed || row < maxRow - 10){
						if(!block || blocking){
							pthread_mutex_lock( &cs_mutex );					
							mvprintw( row, column, " " );
							column += 2;
							row++;
							if (row > maxRow - 10 && !crossed) {
								down++;
								crossed = true;
							}
							mvprintw( row, column, "O" );
							refresh();
							pthread_mutex_unlock( &cs_mutex );
						}
						else{
							pthread_mutex_lock(&mutex);
							pthread_cond_wait(&cond2, &mutex);
							pthread_mutex_unlock(&mutex);
						}
					}
					else{
						pthread_mutex_lock(&cs_mutex);
						pthread_cond_wait(&cond, &cs_mutex);
						pthread_mutex_unlock(&cs_mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column + 2 >= maxCol - 1 && row + 1 >= maxRow - 1){
					direction = 8;
					crossed = false;
					block = true;
					blocking = true;
				}

				else if(column + 2 >= maxCol - 1){
					direction = 5;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row + 1 >= maxRow - 1){
					direction = 12;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				break;
			case 2:
				while(column + 1 < maxCol - 1 && row + 1 < maxRow - 1){
					if(quit) break;
					usleep(sleepTime);	
					if(down < 2 || crossed || row < maxRow - 10){
						if(!block || blocking){
							pthread_mutex_lock( &cs_mutex );				
							mvprintw( row, column, " " );
							column++;
							row++;
							if (row > maxRow - 10 && !crossed) {
								down++;
								crossed = true;
							}
							mvprintw( row, column, "O" );
							refresh();
							pthread_mutex_unlock( &cs_mutex );
						}
						else{
							pthread_mutex_lock(&mutex);
							pthread_cond_wait(&cond2, &mutex);
							pthread_mutex_unlock(&mutex);
						}
					}
					else{
						pthread_mutex_lock(&cs_mutex);
						pthread_cond_wait(&cond, &cs_mutex);
						pthread_mutex_unlock(&cs_mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column + 1 >= maxCol - 1 && row + 1 >= maxRow - 1){
					direction = 9;
					crossed = false;
					block = true;
					blocking = true;
				}

				else if(column + 1 >= maxCol - 1){
					direction = 4;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row + 1 >= maxRow - 1){
					direction = 11;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				break;
			case 3:
				while(row + 1 < maxRow - 1){
					if(quit) break;
					usleep(sleepTime);	
					if(down < 2 || crossed || row < maxRow - 10){	
						if(!block || blocking){				
							pthread_mutex_lock( &cs_mutex );				
							mvprintw( row, column, " " );
							row++;
							if (row > maxRow - 10 && !crossed) {
								down++;
								crossed = true;
							}
							mvprintw( row, column, "O" );
							refresh();
							pthread_mutex_unlock( &cs_mutex );
						}
						else{
							pthread_mutex_lock(&mutex);
							pthread_cond_wait(&cond2, &mutex);
							pthread_mutex_unlock(&mutex);
						}
					}
					else{
						pthread_mutex_lock(&cs_mutex);
						pthread_cond_wait(&cond, &cs_mutex);
						pthread_mutex_unlock(&cs_mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				direction = 10;
				
				crossed = false;

				if(blocking){
					blocking = false;
					block = false;
					pthread_cond_broadcast(&cond2);
				}

				break;
			case 4:
				while(column > 1 && row + 1 < maxRow - 1){
					if(quit) break;
					usleep(sleepTime);
					if(down < 2 || crossed || row < maxRow - 10){	
						if(!block || blocking){
							pthread_mutex_lock( &cs_mutex );				
							mvprintw( row, column, " " );
							column--;
							row++;
							if (row > maxRow - 10 && !crossed) {
								down++;
								crossed = true;
							}
							mvprintw( row, column, "O" );
							refresh();
							pthread_mutex_unlock( &cs_mutex );
						}
						else{
							pthread_mutex_lock(&mutex);
							pthread_cond_wait(&cond2, &mutex);
							pthread_mutex_unlock(&mutex);
						}
					}
					else{
						pthread_mutex_lock(&cs_mutex);
						pthread_cond_wait(&cond, &cs_mutex);
						pthread_mutex_unlock(&cs_mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column <= 1 && row + 1 >= maxRow - 1){
					direction = 11;
					crossed = false;
					block = true;
					blocking = true;
				}

				else if(column <= 1){
					direction = 2;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row + 1 >= maxRow - 1){
					direction = 9;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				break;
			case 5:
				while(column > 2 && row + 1 < maxRow - 1){
					if(quit) break;
					usleep(sleepTime);
					if(down < 2 || crossed || row < maxRow - 10){		
						if(!block || blocking){
							pthread_mutex_lock( &cs_mutex );				
							mvprintw( row, column, " " );
							column -= 2;
							row++;
							if (row > maxRow - 10 && !crossed) {
								down++;
								crossed = true;
							}
							mvprintw( row, column, "O" );
							refresh();
							pthread_mutex_unlock( &cs_mutex );
						}
						else{
							pthread_mutex_lock(&mutex);
							pthread_cond_wait(&cond2, &mutex);
							pthread_mutex_unlock(&mutex);
						}
					}
					else{
						pthread_mutex_lock(&cs_mutex);
						pthread_cond_wait(&cond, &cs_mutex);
						pthread_mutex_unlock(&cs_mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column <= 2 && row + 1 >= maxRow - 1){
					direction = 12;
					crossed = false;
					block = true;
					blocking = true;
				}

				else if(column <= 2){
					direction = 1;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row + 1 >= maxRow - 1){
					direction = 8;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				break;
			case 6:
				while(column > 3 && row + 1 < maxRow - 1){
					if(quit) break;
					usleep(sleepTime);	
					if(down < 2 || crossed || row < maxRow - 10){	
						if(!block || blocking){
							pthread_mutex_lock( &cs_mutex );				
							mvprintw( row, column, " " );
							column -= 3;
							row++;
							if (row > maxRow - 10 && !crossed) {
								down++;
								crossed = true;
							}
							mvprintw( row, column, "O" );
							refresh();
							pthread_mutex_unlock( &cs_mutex );
						}
						else{
							pthread_mutex_lock(&mutex);
							pthread_cond_wait(&cond2, &mutex);
							pthread_mutex_unlock(&mutex);
						}
					}
					else{
						pthread_mutex_lock(&cs_mutex);
						pthread_cond_wait(&cond, &cs_mutex);
						pthread_mutex_unlock(&cs_mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column <= 3 && row + 1 >= maxRow - 1){
					direction = 13;
					crossed = false;
					block = true;
					blocking = true;
				}

				else if(column <= 3){
					direction = 0;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row + 1 >= maxRow - 1){
					direction = 7;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				break;
			case 7:
				while(column > 3 && row > 1){
					if(quit) break;
					usleep(sleepTime);
					if(!block || blocking){
						pthread_mutex_lock( &cs_mutex );					
						mvprintw( row, column, " " );
						column -= 3;
						row--;
						if (row < maxRow - 10 && !crossed) {
							down--;
							crossed = true;
							pthread_cond_signal(&cond);
						}
						mvprintw( row, column, "O" );
						refresh();
						pthread_mutex_unlock( &cs_mutex );
					}
					else{
						pthread_mutex_lock(&mutex);
						pthread_cond_wait(&cond2, &mutex);
						pthread_mutex_unlock(&mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column <= 3 && row <= 1){
					direction = 0;
					crossed = false;
					block = true;
					blocking = true;
				}

				else if(column <= 3){
					direction = 13;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row <= 1){
					direction = 6;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				break;
			case 8:
				while(column > 2 && row > 1){
					if(quit) break;
					usleep(sleepTime);	
					if(!block || blocking){
						pthread_mutex_lock( &cs_mutex );				
						mvprintw( row, column, " " );
						column -= 2;
						row--;
						if (row < maxRow - 10 && !crossed) {
							down--;
							crossed = true;
							pthread_cond_signal(&cond);
						}
						mvprintw( row, column, "O" );
						refresh();
						pthread_mutex_unlock( &cs_mutex );
					}
					else{
						pthread_mutex_lock(&mutex);
						pthread_cond_wait(&cond2, &mutex);
						pthread_mutex_unlock(&mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column <= 2 && row <= 1){
					direction = 1;
					crossed = false;
					block = true;
					blocking = true;
				}

				else if(column <= 2){
					direction = 12;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row <= 1){
					direction = 5;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				break;
			case 9:
				while(column > 1 && row > 1){
					if(quit) break;
					usleep(sleepTime);
					if(!block || blocking){
						pthread_mutex_lock( &cs_mutex );					
						mvprintw( row, column, " " );
						column--;
						row--;
						if (row < maxRow - 10 && !crossed) {
							down--;
							crossed = true;
							pthread_cond_signal(&cond);
						}
						mvprintw( row, column, "O" );
						refresh();
						pthread_mutex_unlock( &cs_mutex );
					}
					else{
						pthread_mutex_lock(&mutex);
						pthread_cond_wait(&cond2, &mutex);
						pthread_mutex_unlock(&mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column <= 1 && row <= 1){
					direction = 2;
					crossed = false;
					block = true;
					blocking = true;
				}

				else if(column <= 1){
					direction = 11;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row <= 1){
					direction = 4;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				break;
			case 10:
				while(row > 1){
					if(quit) break;
					usleep(sleepTime);	
					if(!block || blocking){
						pthread_mutex_lock( &cs_mutex );				
						mvprintw( row, column, " " );
						row--;
						if (row < maxRow - 10 && !crossed) {
							down--;
							crossed = true;
							pthread_cond_signal(&cond);
						}
						mvprintw( row, column, "O" );
						refresh();
						pthread_mutex_unlock( &cs_mutex );
					}
					else{
						pthread_mutex_lock(&mutex);
						pthread_cond_wait(&cond2, &mutex);
						pthread_mutex_unlock(&mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				direction = 3;

				crossed = false;

				if(blocking){
					blocking = false;
					block = false;
					pthread_cond_broadcast(&cond2);
				}

				break;
			case 11:
				while(column + 1 < maxCol - 1 && row > 1){
					if(quit) break;
					usleep(sleepTime);
					if(!block || blocking){
						pthread_mutex_lock( &cs_mutex );					
						mvprintw( row, column, " " );
						column += 1;
						row--;
						if (row < maxRow - 10 && !crossed) {
							down--;
							crossed = true;
							pthread_cond_signal(&cond);
						}
						mvprintw( row, column, "O" );
						refresh();
						pthread_mutex_unlock( &cs_mutex );
					}
					else{
						pthread_mutex_lock(&mutex);
						pthread_cond_wait(&cond2, &mutex);
						pthread_mutex_unlock(&mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column + 1 >= maxCol - 1 && row <= 1){
					direction = 4;
					crossed = false;
					block = true;
					blocking = true;
				}

				else if(column + 1 >= maxCol - 1){
					direction = 9;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row <= 1){
					direction = 2;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				break;
			case 12:
				while(column + 2 < maxCol - 1 && row > 1){
					if(quit) break;
					usleep(sleepTime);
					if(!block || blocking){
						pthread_mutex_lock( &cs_mutex );					
						mvprintw( row, column, " " );
						column += 2;
						row--;
						if (row < maxRow - 10 && !crossed) {
							down--;
							crossed = true;
							pthread_cond_signal(&cond);
						}
						mvprintw( row, column, "O" );
						refresh();
						pthread_mutex_unlock( &cs_mutex );
					}
					else{
						pthread_mutex_lock(&mutex);
						pthread_cond_wait(&cond2, &mutex);
						pthread_mutex_unlock(&mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column + 2 >= maxCol - 1 && row <= 1){
					direction = 5;
					crossed = false;
					block = true;
					blocking = true;
				}

				else if(column + 2 >= maxCol - 1){
					direction = 8;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row <= 1){
					direction = 1;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				break;	
			case 13:
				while(column + 3 < maxCol - 1 && row > 1){
					if(quit) break;
					usleep(sleepTime);	
					if(!block || blocking){
						pthread_mutex_lock( &cs_mutex );				
						mvprintw( row, column, " " );
						column += 3;
						row--;
						if (row < maxRow - 10 && !crossed) {
							down--;
							crossed = true;
							pthread_cond_signal(&cond);
						}
						mvprintw( row, column, "O" );
						refresh();
						pthread_mutex_unlock( &cs_mutex );
					}
					else{
						pthread_mutex_lock(&mutex);
						pthread_cond_wait(&cond2, &mutex);
						pthread_mutex_unlock(&mutex);
					}
				}
				
				speed /= 1.2;
				sleepTime = 1000000 / speed;
				
				if(column + 3 >= maxCol - 1 && row <= 1){
					direction = 6;
					crossed = false;
					block = true;
					blocking = true;
				}

				else if(column + 3 >= maxCol - 1){
					direction = 7;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				else if(row <= 1){
					direction = 0;
					crossed = false;
					if(blocking){
						blocking = false;
						block = false;
						pthread_cond_broadcast(&cond2);
					}
				}

				break;		
			default:
				break;
		}
		
	}
	
	usleep(sleepTime);
	pthread_mutex_lock( &cs_mutex );
	if (row >= maxRow - 10) {
		down--;
		pthread_cond_signal(&cond);
	}
	mvprintw( row, column, " " );
	refresh();
	pthread_mutex_unlock( &cs_mutex );
	pthread_exit(NULL);
}


void main()
{
	
	srand(time(NULL));

    	initscr();
	
	curs_set(0);
	
    	getmaxyx( stdscr, maxRow, maxCol ); 
   
    	generateMap();
	refresh();
	
	pthread_t quitter;
    	
	pthread_create(&quitter, NULL, quiter, NULL);

	while(true){
				
		for(int i = 0; i < 10; i++){
			if(!quit){
				pthread_create(&ball[i], NULL, generateBall, NULL);
	
				sleep(2);
			}
		}
	}
}

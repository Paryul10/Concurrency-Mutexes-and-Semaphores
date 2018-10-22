#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include<semaphore.h>

int rem_pl,rem_ref,players,refrees;
int persons;
int pc,rc;
int n;
// int player_arr[200005];
// int refree_arr[100005];
int match;

pthread_mutex_t player_mux;
pthread_mutex_t refree_mux; 
pthread_mutex_t organiser;
pthread_mutex_t persons_mux;

pthread_t threads[100];
pthread_t orgid;


void *RefreeEntry()
{
    pthread_mutex_lock(&refree_mux);
    rc++;
    // refree_arr[rc]=1;
    rem_ref--;
    printf("Refree %d enters the academey. Remaining refrees = %d\n",rc,rem_ref);
    pthread_mutex_unlock(&refree_mux);

}

void *PlayerEntry()
{
    pthread_mutex_lock(&player_mux);
    pc++;
    // player_arr[pc]=1;
    rem_pl--;
    printf("PLayer %d enters the academey. Remaining players = %d\n",pc,rem_pl);
    pthread_mutex_unlock(&player_mux);

    // 2 players meet organiser and then rest of the work happens
}

void *Organiser()
{
    while(1)
    {
        pthread_mutex_lock(&organiser);
        
        int req_players=match*2;

        pthread_mutex_lock(&player_mux);
        pthread_mutex_lock(&refree_mux);
        if(pc>=req_players && rc>=match)
        {
            printf("Player %d enters the court and starts warmup\n",(match*2)-1);
            sleep(1);

            printf("Refree %d Adjusts the eqiupments\n",(match));
            sleep(.5);

            printf("Player %d enters the court and starts warmup\n",(match*2));
            sleep(1);
            
            printf("Refree %d starts the game and the organiser is now free\n",match);
            match++;
        }
        pthread_mutex_unlock(&player_mux);
        pthread_mutex_unlock(&refree_mux);

        pthread_mutex_unlock(&organiser);
        if(match==(n+1))
        {
            break;
        }
    }
}


int main()
{
    scanf("%d",&n);
    players=2*n;
    refrees=n;   
    int i,j;
    rem_pl=2*n;
    rem_ref=n;
    // 0-> inital , 1->marks entry    , 2-> marks is playing ,3->marks is done playing
    match=1;

    pthread_create(&orgid,NULL,Organiser,NULL);

    while(rem_pl>0 || rem_ref>0)
    {
        int delay = rand()%3;
        sleep(delay);

        //spawn person
        int spawned;
        if(rem_pl+rem_ref>0)
        {
            spawned = rand()%(rem_pl+rem_ref) + 1 ;
        }
        if(spawned<=rem_pl && pc<2*n)
        {

            pthread_mutex_lock(&persons_mux);
            int thid = pthread_create(&threads[pc+rc],NULL,PlayerEntry,NULL);

            if(thid==0)
            {
                // printf("Player spawned\n");
            }
            else
            {
                fprintf(stderr,"Error - pthread_create()  return code: %d\n",thid);                
            }
            pthread_mutex_unlock(&persons_mux);

        }
        else if(spawned>rem_pl && spawned<=(rem_pl+rem_ref) && rc<n)
        {
            pthread_mutex_lock(&persons_mux);
            int thid = pthread_create(&threads[pc+rc],NULL,RefreeEntry,NULL);

            if(thid==0)
            {
                // printf("Refree spawned\n");
            }
            else
            {
                fprintf(stderr,"Error - pthread_create()  return code: %d\n",thid);                
            }
            pthread_mutex_unlock(&persons_mux);
        }
    }

    for(i=0;i<rc+pc;i++)
    {
        pthread_join(threads[i],NULL);
    }
    pthread_join(orgid,NULL);
    return 0;
}
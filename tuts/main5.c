#include <stdio.h>

int main(){
    const int games = 4;
    const int players = 5;

    int scores [4][5];
    int totals [5];
    int highest_players;

    int game_counter, player_counter;

    for (game_counter = 0; game_counter < games; game_counter++){
        printf("Enter the scores for game %d\n", (game_counter + 1));

        for(player_counter = 0; player_counter < players; player_counter++){
            printf("Enter scoring total for players #%d", (player_counter + 1));
            scanf("%d", &scores[game_counter][player_counter]);
        }
    }

    for(player_counter = 0; player_counter < players; player_counter++){
        int player_total = 0;
        for( game_counter = 0; game_counter < games; game_counter++){
            player_total += scores[game_counter][player_counter];
        }
        totals[player_counter] = player_total;
    }

    int highest_total = 0;
    int players_id ;

    for(player_counter = 0; player_counter < players; player_counter++){
        if (totals[player_counter] > highest_total){
            highest_total = totals[player_counter];

            players_id = (player_counter + 1);
            
        }
    }

    float ppg = (float)(highest_total/ 4);

    printf("Player %d has the highest average of %.1f points per game\n", players_id, ppg);


}
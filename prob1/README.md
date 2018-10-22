**Badminton Acadmey:Problem of Concurrency**

**Problem Statement**
Gopichand academy recently started allowing a ​ group of ​ 3 people to play badminton at
their courts. The group should consist of ​ 2 players who play against each other and ​ a
referee to manage the match. Whenever a new player/referee ​ enters the academy​ ,
he/she waits until he/she gets a chance to ​ meet the organizer​ . Whenever the organizer
is free, he waits until a group of 2 players and a referee can be formed among the
unassigned people he met and then the newly formed group ​ enters the court​ . Then the
players ​ warm up for the game and in the meantime referee ​ adjusts the equipment
necessary to manage the game. Once all the three are ​ ready​ , the ​ game is started by
the referee. Organizer waits till the game is started and once it is started he is free and
again starts meeting new players/referees entering the academy.
● Players invoke the following functions in order: ​ enterAcademy, meetOrganizer,
enterCourt, warmUp
● Referees invoke the following functions in order: ​ enterAcademy,
meetOrganizer, enterCourt, adjustEquipment, startGame
Write code that enforces the above with appropriate ​ print statements for each of the
above actions.
Input: an integer ​ n indicating ​ 2*n players and ​ n referees will arrive at the academy in
total. Use sleep(1) for warmUp and sleep(0.5) for adjustEquipment actions. Make the
new person arrive with a ​ random, modulo 3 second delay and probability that he is a
player/referee depends on the remaining number of players/referees that will be coming
and each person is a thread.

**How to Run**

gcc -pthread badacad.c ; ./a.out

Give a number n as input to the prompt.

**Design And Implementation**

All the persons are a thread. Persons are created on a random basis in random time intervals. As soon as a person is created, it invokes the EnterAcadmey function of player/refree(depends on what the person is.)
Now after entering the academy they wait for the organiser(organiser mutex) to meet him. As soon as the player , his counterpart and a refree meet the organiser , they enter the court. Players start warmup. Refree adjusts equipment .
After all this, the match is started by the refree. And the organiser is now available for to organise the next match.



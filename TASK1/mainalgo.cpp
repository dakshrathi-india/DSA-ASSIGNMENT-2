#include <iostream>
// we are including the climits so that we can use the INT_MIN and the INT_MAX
#include <climits>
using namespace std;

//the maximum number of levels present in the skip list are maxLevel
const int maxLevel =  16;

//this function returns the maximum number of levels to which a given element can go
//this is found by performing the random coin toss experiment at each level
int randomLevel(){
    int coin = rand()%2;
    int level = 1;
    // if the coin = 1 -> that head has occured , and we will take the element to the above level
    // else if the coin = 0 -> we will leave that element on that level only
    if (coin == 1)
        level ++;

    else{
        if(level > maxLevel)
            return maxLevel;
        else    
            return level;
    }
}


class Node{
    public:
        int playerID;
        char name[20];
        int score;
        int timestamp;
        int level = randomLevel();

        //creating a forward array of type Node*
};


int main(){
    return 0;
}
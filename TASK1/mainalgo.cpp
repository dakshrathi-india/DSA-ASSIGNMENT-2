#include <iostream>
#include <climits>      // we are including this to use INT_MIN and INT_MAX for sentinel nodes 
#include <ctime>

using namespace std;

//the maximum number of levels present in the skip list are maxLevel
const int maxLevel =  16;


//this function returns the maximum number of levels to which a given element can go
//this is found by performing the random coin toss experiment at each level
//value returned will be <= maxLevel
int randomLevel(){

    //level here is the max number of level to which a element can go
    int level = 0;

    while(level != maxLevel){
        int coin = rand()%2;

        // if the coin = 1 -> that head has occured , and we will take the element to the above level
        // else if the coin = 0 -> we will leave that element on that level only
        if (coin == 1)
            level ++;

        else
            return level;
    }

    return maxLevel;

}


//each player record contains:
//Player ID(unique)
//PLayer Name
//Score 
//Timestamp
class PlayerData{
    public:
        int playerID;
        char name[20];
        int score;
        int timestamp;

        //default constructor [does nothing ,but without it error will be raised in the, as in the Node we are creating PlayerData data which will call the default constructor]
        PlayerData() {}

        //parameterized constructor
        PlayerData(int id, const char* name, int score, int time){
            playerID = id;
            this->score = score;
            timestamp = time;
            
            
            //initializing the name 
            int i=0;
            for(i=0; name[i] != '\0'; i++){
                this->name[i] = name[i]; 
            }
            this->name[i] = '\0';

        }

};

class SkipListNode{
    public:
        PlayerData data;
        // level here denotes the level to which this particular node will go to in the skip list 
        int nodeLevel;

        //creating a next array of type Player* , array[i] points to the element which is just next to our current element 
        //the lenght is taken to be maxLevel+1 because the level start are 0,1,2,3,..,maxLevel
        SkipListNode* next[maxLevel+1];

        SkipListNode(PlayerData &d){
            data = d;
            nodeLevel = randomLevel();

            //initializing the next array
            for(int i=0; i<=maxLevel; i++){
                next[i] = NULL;
            }
        }
};

//this class is used the manange the entire skip list data structure
class SkipList{
    public:
        //here currentlevel denotes the total number of levels in the skipList
        int currentLevel;
        //this pointer points to the left sentinel node 
        SkipListNode* Head;

        //default constructor
        SkipList(){
            currentLevel = 0;
            PlayerData sentinelData(-1, "HEAD", INT_MAX, INT_MIN);
            Head = new SkipListNode(sentinelData);
            Head->nodeLevel = maxLevel;
        }

        //MUTATOR OPERATIONS

        //the rules for ranking the players
        //1) descending order of score 
        //2) ascending order of timestamp (as: "For equal scores, earlier timestamp ranks higher")
        //3) we require a 3rd condition also as there may be some cases which may lead to ambiguity
        // it can be either by taking the id in the ascending order or in the descending order 
        // i am taking the ID to be in the ascending order

        //this function returns whether d1 comes before d2 or not
        //check whether d1 isBefore the d2
        bool isBefore(PlayerData &d1, PlayerData &d2){
            //checking for the first conditon of descending order of score 
            if (d1.score != d2.score)
                return d1.score > d2.score;

            //checking the second condition for the ascending order of the timestamp
            else if(d1.timestamp != d2.timestamp)
                return d1.timestamp < d2.timestamp;
            
            //checking the third condition for the descending order of timestamp
            else
                return d1.playerID < d2.playerID;
        }

        void insertPlayer(int id, const char* name, int score, int time){
            PlayerData Player(id, name, score, time);
            SkipListNode* newNode = new SkipListNode(Player);
            SkipListNode** modifyNode = new SkipListNode*[newNode->nodeLevel+1];

            //temp is used to point at a element just before the inserted element, and then to move down the layer while pointing at the same point
            SkipListNode* temp = Head;   

            //tempNext is used to add it to the arry modifyNode
            SkipListNode* tempNext ;
            int level = newNode->nodeLevel;

            for(int i=currentLevel; i>=0 ;i--){
                tempNext = temp->next[i];
                while(tempNext != NULL && isBefore(tempNext->data, newNode->data)){
                    temp = tempNext;
                    tempNext = tempNext->next[i];
                }

                if(i <= level){
                    modifyNode[i] = tempNext;
                    temp->next[i] = newNode;
                }
            }

            if(level > currentLevel){
                for(int i = currentLevel+1; i <= level; i++){
                    modifyNode[i] = NULL;
                    Head->next[i] = newNode;
                }
                currentLevel = level;
            }
            //updating the values of the next array of the newNode 
            for(int i = 0; i<=level; i++){
                newNode->next[i] = modifyNode[i];
            }

            delete[] modifyNode;
        }

        void deletePlayer(int id, const char* name, int score, int time){

            SkipListNode* target = searchPlayer(id, name, score, time);
            if(target != NULL){
                SkipListNode* temp = Head;
                SkipListNode* tempNext ;
                for(int i=currentLevel; i>=0; i++){
                    tempNext = temp->next[i];
                    while(tempNext != NULL && isBefore(tempNext->data,target->data)){
                        temp = tempNext;
                        tempNext = tempNext->next[i];
                    }
                    temp->next[i] = tempNext;
                }
                delete target;

                //updating the value of currentLevel if required
                for(int i = 0; i<=currentLevel; i++){
                    if(Head->next[i] == NULL){
                        currentLevel = i-1;
                        break;
                    }
                }
            }

            else
                cout<<"the player is not present in the leaderboard";
        
        }

        void updateScore(int id, const char* name, int oldscore, int time, int newscore){
            SkipListNode* target = searchPlayer(id, name, oldscore, time);
            if(target != NULL){
                //as when we change the score the entire structure is changed
                //thus first we will delete the node and then we will insert the node again
                deletePlayer(id, name, oldscore, time);
                insertPlayer(id, name, newscore, time);
            }
            else
                cout<<"the player is not present in the linked list";
        }

        //ACCESSOR OPERATORS

        //searching a node on the basis of ID
        SkipListNode* searchPlayer(int id, const char* name, int score, int time){
            PlayerData Player(id, name, score, time);
            SkipListNode* temp = Head;
            SkipListNode* tempNext ;

            for (int i=currentLevel; i>=0; i--){
                tempNext = temp->next[i];
                while(tempNext != NULL && isBefore(tempNext->data, Player)){
                    temp = tempNext;
                    tempNext = tempNext->next[i];
                }
            }

            //after reaching the 0th level, if the node exist then the temp will be pointing on the node just previous , tempNext to the node just after the node to be searched 
            if(temp->next[0] != NULL && temp->next[0]->data.playerID == id)
                return temp->next[0];
            else
                return NULL;
        }
        
        int getRank(int id){
            SkipListNode* temp = Head;
            int count = 0;
            while(temp != NULL && temp->data.playerID != id){
                count ++;
                temp = temp->next[0];
            }

            if(temp != NULL)
                return count;
            else
                return -1;
        }
    
    void displayDetails(SkipListNode* temp){
        cout<<"Name : "<<temp->data.name<<endl<<"Score : "<<temp->data.score<<endl<<"ID : "<<temp->data.playerID<<endl<<"TimeStamp : "<<temp->data.timestamp<<endl<<endl;
    }
        
    void getTopK(int k){
        SkipListNode* temp = Head->next[0];
        if(k>0){
            for(int i=0; i<k && temp != NULL ;i++){
                    displayDetails(temp);
                    temp = temp->next[0];
            }
        }
        else
            cout<<"enter valid value of k";
    }

    void displayLeaderBoard(){
        int count = 1;
        SkipListNode* temp = Head->next[0];
        while(temp != NULL){
            cout<<"Rank : "<<count++<<endl;
            displayDetails(temp);
            temp = temp->next[0];
        }
    }

    void displaySkipListStructure(){
        cout<<"THE SKIP LIST STRUCTURE IS -> "<<endl<<endl;
        for(int i = currentLevel; i>=0; i--){
            cout<<"LEVEL -> "<<i<<endl<<endl;
            int count = 1;
            SkipListNode* temp = Head->next[i];
            while(temp != NULL){
                cout<<"Rank : "<<count++;
                displayDetails(temp);
                temp = temp->next[i];
            }
        }   
    }

    //destructor
    ~SkipList(){
        SkipListNode* temp = Head->next[0];

        //to delete all the elements of the SkipList
        while(temp  !=  NULL){
            SkipListNode* curr = temp;
            temp = temp->next[0];
            delete curr;
        }

        delete Head;
        Head = NULL;
    }

};

int main(){
    srand(time(0));
    return 0;
}
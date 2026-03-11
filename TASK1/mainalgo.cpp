#include <iostream>
#include <climits> // we are including this to use INT_MIN and INT_MAX for sentinel nodes
#include <ctime>

using namespace std;

// the maximum number of levels present in the skip list are maxLevel
const int maxLevel = 16;

// this function returns the maximum number of levels to which a given element can go
// this is found by performing the random coin toss experiment at each level
// value returned will be <= maxLevel
int randomLevel()
{

    // level here is the max number of level to which a element can go
    int level = 0;

    while (level != maxLevel)
    {
        int coin = rand() % 2;

        // if the coin = 1 -> that head has occured , and we will take the element to the above level
        // else if the coin = 0 -> we will leave that element on that level only
        if (coin == 1)
            level++;

        else
            return level;
    }

    return maxLevel;
}

// each player record contains:
// Player ID(unique)
// PLayer Name
// Score
// Timestamp
class PlayerData
{
public:
    int playerID;
    char name[20];
    int score;
    int timestamp;

    // default constructor [does nothing ,but without it error will be raised in the, as in the Node we are creating PlayerData data which will call the default constructor]
    PlayerData() {}

    // parameterized constructor
    PlayerData(int id, const char *name, int score, int time)
    {
        playerID = id;
        this->score = score;
        timestamp = time;

        // initializing the name
        int i = 0;
        for (i = 0; name[i] != '\0'; i++)
        {
            this->name[i] = name[i];
        }
        this->name[i] = '\0';
    }
};

class SkipListNode
{
public:
    PlayerData data;
    // level here denotes the level to which this particular node will go to in the skip list
    int nodeLevel;

    // creating a next array of type Player* , array[i] points to the element which is just next to our current element
    // the lenght is taken to be maxLevel+1 because the level start are 0,1,2,3,..,maxLevel
    SkipListNode *next[maxLevel + 1];

    SkipListNode(PlayerData &d)
    {
        data = d;
        nodeLevel = randomLevel();

        // initializing the next array
        for (int i = 0; i <= maxLevel; i++)
        {
            next[i] = NULL;
        }
    }
};

// this class is used the manange the entire skip list data structure
class SkipList
{
public:
    // here currentlevel denotes the total number of levels in the skipList
    int currentLevel;
    // this pointer points to the left sentinel node
    SkipListNode *Head;

    // default constructor
    SkipList()
    {
        currentLevel = 0;
        PlayerData sentinelData(-1, "HEAD", INT_MAX, INT_MIN);
        Head = new SkipListNode(sentinelData);
        Head->nodeLevel = maxLevel;
    }

    // MUTATOR OPERATIONS

    // the rules for ranking the players
    // 1) descending order of score
    // 2) ascending order of timestamp (as: "For equal scores, earlier timestamp ranks higher")
    // 3) we require a 3rd condition also as there may be some cases which may lead to ambiguity
    //  it can be either by taking the id in the ascending order or in the descending order
    //  i am taking the ID to be in the ascending order

    // this function returns whether d1 comes before d2 or not
    // check whether d1 isBefore the d2
    bool isBefore(PlayerData &d1, PlayerData &d2)
    {
        // checking for the first conditon of descending order of score
        if (d1.score != d2.score)
            return d1.score > d2.score;

        // checking the second condition for the ascending order of the timestamp
        else if (d1.timestamp != d2.timestamp)
            return d1.timestamp < d2.timestamp;

        // checking the third condition for the ascending order of ID
        else
            return d1.playerID < d2.playerID;
    }

    void insertPlayer(int id, const char *name, int score, int time)
    {
        PlayerData Player(id, name, score, time);
        SkipListNode *newNode = new SkipListNode(Player);
        //here dynamic allocation is done so that the size can be set to level+1 (as the level value will be known during the runtime , thus we can't use the static allocation and we need to use the dynamic allocation)
        SkipListNode **modifyNode = new SkipListNode *[newNode->nodeLevel + 1];

        // temp is used to point at a element just before the inserted element, and then to move down the layer while pointing at the same point
        SkipListNode *temp = Head;

        // tempNext is used to add it to the arry modifyNode
        SkipListNode *tempNext;
        int level = newNode->nodeLevel;

        for (int i = currentLevel; i >= 0; i--)
        {
            tempNext = temp->next[i];
            while (tempNext != NULL && isBefore(tempNext->data, newNode->data))
            {
                temp = tempNext;
                tempNext = tempNext->next[i];
            }

            if (i <= level)
            {
                modifyNode[i] = tempNext;
                temp->next[i] = newNode;
            }
        }

        if (level > currentLevel)
        {
            for (int i = currentLevel + 1; i <= level; i++)
            {
                modifyNode[i] = NULL;
                Head->next[i] = newNode;
            }
            currentLevel = level;
        }
        // updating the values of the next array of the newNode
        for (int i = 0; i <= level; i++)
        {
            newNode->next[i] = modifyNode[i];
        }

        delete[] modifyNode;
       
    }

    void deletePlayer(int id, const char *name, int score, int time)
    {

        SkipListNode *target = searchPlayer(id, name, score, time);
        if (target != NULL)
        {
            SkipListNode *temp = Head;
            SkipListNode *tempNext;
            for (int i = currentLevel; i >= 0; i--)
            {
                tempNext = temp->next[i];
                while (tempNext != NULL && isBefore(tempNext->data, target->data))
                {
                    temp = tempNext;
                    tempNext = tempNext->next[i];
                }
                if (tempNext != nullptr && tempNext == target)
                    temp->next[i] = tempNext->next[i];
            }
            delete target;

            // updating the value of currentLevel if required
            for (int i = 0; i <= currentLevel; i++)
            {
                if (Head->next[i] == NULL)
                {
                    currentLevel = i - 1;
                    break;
                }
            }
        }

        else
            cout << "the player is not present in the leaderboard"<<endl;
    }

    void updateScore(int id, const char *name, int oldscore, int time, int newscore)
    {
        SkipListNode *target = searchPlayer(id, name, oldscore, time);
        if (target != NULL)
        {
            // as when we change the score the entire structure is changed
            // thus first we will delete the node and then we will insert the node again
            deletePlayer(id, name, oldscore, time);
            insertPlayer(id, name, newscore, time);
        }
        else
            cout << "the player is not present in the skip list"<<endl;
    }

    // ACCESSOR OPERATORS

    // searching a node on the basis of ID
    SkipListNode *searchPlayer(int id, const char *name, int score, int time)
    {
        PlayerData Player(id, name, score, time);
        SkipListNode *temp = Head;
        SkipListNode *tempNext;

        for (int i = currentLevel; i >= 0; i--)
        {
            tempNext = temp->next[i];
            while (tempNext != NULL && isBefore(tempNext->data, Player))
            {
                temp = tempNext;
                tempNext = tempNext->next[i];
            }
        }

        //after reaching the 0th level, if the node exist then the temp will be pointing on the node just previous , tempNext to the node to be searched
        //temp->next[0] and tempNext both will be pointing on the same element 
        if (temp->next[0] != NULL && temp->next[0]->data.playerID == id)
            return temp->next[0];
        else
            return NULL;
    }

    int getRank(int id)
    {
        SkipListNode *temp = Head;
        int count = 0;
        while (temp != NULL && temp->data.playerID != id)
        {
            count++;
            temp = temp->next[0];
        }

        if (temp != NULL)
            return count;
        else
            return -1;
    }

    void displayDetails(SkipListNode *temp)
    {
        cout << "Name : " << temp->data.name << endl
             << "Score : " << temp->data.score << endl
             << "ID : " << temp->data.playerID << endl
             << "TimeStamp : " << temp->data.timestamp << endl
             << endl;
    }

    void getTopK(int k)
    {
        SkipListNode *temp = Head->next[0];
        if (k > 0)
        {
            if (temp != NULL){
                for (int i = 0; i < k && temp != NULL; i++)
                {
                    displayDetails(temp);
                    temp = temp->next[0];
                }
            }
            else
                cout<<"the leaderboard is empty";
        }
        else
            cout << "enter valid value of k"<<endl;
    }

    void displayLeaderBoard()
    {
        int count = 1;
        SkipListNode *temp = Head->next[0];
        if (temp != NULL){
            while (temp != NULL)
            {
                cout << "Rank : " << count++ << endl;
                displayDetails(temp);
                temp = temp->next[0];
            }
        }
        else
            cout<<"THE LEADERBOARD IS EMPTY ";
    }

    void displaySkipListStructure()
    {
        if (Head->next[0] == NULL)
            cout<<"THE SKIP LIST IS EMPTY";
        else{
            cout << "THE SKIP LIST STRUCTURE IS -> " << endl<< endl;
            for (int i = currentLevel; i >= 0; i--)
            {
                cout << "LEVEL -> " << i << endl
                    << endl;
                int count = 1;
                SkipListNode *temp = Head->next[i];
                while (temp != NULL)
                {
                    cout << "Rank : " << count++<<endl;
                    displayDetails(temp);
                    temp = temp->next[i];
                }
            }
        }
    }

    // destructor
    ~SkipList()
    {
        SkipListNode *temp = Head->next[0];

        // to delete all the elements of the SkipList
        while (temp != NULL)
        {
            SkipListNode *curr = temp;
            temp = temp->next[0];
            delete curr;
        }

        delete Head;
        Head = NULL;
    }
};

void testcase_report(){
    // ================================
    // TEST CASE 1
    // ================================
    cout << "========== TEST CASE 1 ==========" << endl;

    SkipList tc1;

    tc1.insertPlayer(101, "Aman", 920, 10);
    tc1.insertPlayer(102, "Rohit", 870, 15);
    tc1.insertPlayer(103, "Karan", 900, 12);
    tc1.insertPlayer(104, "Nitin", 880, 18);

    cout << endl << "Leaderboard after insertion:" << endl;
    tc1.displayLeaderBoard();

    cout << endl << "Searching player Karan:" << endl;
    SkipListNode *p1 = tc1.searchPlayer(103, "Karan", 900, 12);
    if (p1 != NULL)
    {
        tc1.displayDetails(p1);
        cout << "Rank of Player 103: " << tc1.getRank(103) << endl;
    }
    else
    {
        cout << "Player not found" << endl;
    }

    cout << endl << "Updating Rohit score from 870 to 940" << endl;
    tc1.updateScore(102, "Rohit", 870, 15, 940);

    cout << endl << "Leaderboard after update:" << endl;
    tc1.displayLeaderBoard();

    cout << endl << "Top 2 Players:" << endl;
    tc1.getTopK(2);


    // ================================
    // TEST CASE 2
    // ================================
    cout << endl << "========== TEST CASE 2 ==========" << endl;

    SkipList tc2;

    tc2.insertPlayer(201, "Arjun", 880, 20);
    tc2.insertPlayer(202, "Dev", 880, 10);
    tc2.insertPlayer(203, "Yash", 860, 25);
    tc2.insertPlayer(204, "Harsh", 910, 14);

    cout << endl << "Leaderboard after insertion:" << endl;
    tc2.displayLeaderBoard();

    cout << endl << "Searching player Dev:" << endl;
    SkipListNode *p2 = tc2.searchPlayer(202, "Dev", 880, 10);
    if (p2 != NULL)
    {
        tc2.displayDetails(p2);
        cout << "Rank of Player 202: " << tc2.getRank(202) << endl;
    }
    else
    {
        cout << "Player not found" << endl;
    }

    cout << endl << "Deleting player Yash" << endl;
    tc2.deletePlayer(203, "Yash", 860, 25);

    cout << endl << "Leaderboard after deletion:" << endl;
    tc2.displayLeaderBoard();

    cout << endl << "Top 3 Players:" << endl;
    tc2.getTopK(3);
}

void testcase_rigrous(){
    // ============================================================
    // TEST CASE 1: BASIC ORDERING + SEARCH + RANK + UPDATE + DELETE
    // ============================================================
    cout << "================ TEST CASE 1 ================" << endl;

    SkipList tc1;
    tc1.insertPlayer(101, "Aarav", 900, 10);
    tc1.insertPlayer(102, "Vivaan", 850, 12);
    tc1.insertPlayer(103, "Aditya", 920, 8);
    tc1.insertPlayer(104, "Krishna", 870, 9);

    cout << endl << "Leaderboard after insertion:" << endl;
    tc1.displayLeaderBoard();

    cout << endl << "Search Aditya:" << endl;
    SkipListNode *p1 = tc1.searchPlayer(103, "Aditya", 920, 8);
    if (p1 != NULL)
    {
        tc1.displayDetails(p1);
        cout << "Rank : " << tc1.getRank(103) << endl;
    }
    else
    {
        cout << "Player not found" << endl;
    }

    cout << endl << "Rank of Aarav: " << tc1.getRank(101) << endl;

    cout << endl << "Top 3 players:" << endl;
    tc1.getTopK(3);

    cout << endl << "Skip List Structure:" << endl;
    tc1.displaySkipListStructure();

    cout << endl << "Updating Vivaan score from 850 to 950" << endl;
    tc1.updateScore(102, "Vivaan", 850, 12, 950);

    cout << endl << "Leaderboard after update:" << endl;
    tc1.displayLeaderBoard();

    cout << endl << "Deleting Krishna" << endl;
    tc1.deletePlayer(104, "Krishna", 870, 9);

    cout << endl << "Leaderboard after deletion:" << endl;
    tc1.displayLeaderBoard();

    cout << endl << "Final Skip List Structure:" << endl;
    tc1.displaySkipListStructure();


    // ============================================================
    // TEST CASE 2: SAME SCORE, DIFFERENT TIMESTAMPS
    // ============================================================
    cout << endl << endl << "================ TEST CASE 2 ================" << endl;

    SkipList tc2;
    tc2.insertPlayer(201, "Rohan", 880, 15);
    tc2.insertPlayer(202, "Arjun", 880, 11);
    tc2.insertPlayer(203, "Kabir", 880, 13);
    tc2.insertPlayer(204, "Ishaan", 910, 20);

    cout << endl << "Leaderboard after insertion:" << endl;
    tc2.displayLeaderBoard();

    cout << endl << "Search Kabir:" << endl;
    SkipListNode *p2 = tc2.searchPlayer(203, "Kabir", 880, 13);
    if (p2 != NULL)
    {
        tc2.displayDetails(p2);
        cout << "Rank : " << tc2.getRank(203) << endl;
    }
    else
    {
        cout << "Player not found" << endl;
    }

    cout << endl << "Rank of Arjun: " << tc2.getRank(202) << endl;

    cout << endl << "Top 2 players:" << endl;
    tc2.getTopK(2);

    cout << endl << "Skip List Structure:" << endl;
    tc2.displaySkipListStructure();

    cout << endl << "Updating Rohan score from 880 to 930" << endl;
    tc2.updateScore(201, "Rohan", 880, 15, 930);

    cout << endl << "Leaderboard after update:" << endl;
    tc2.displayLeaderBoard();

    cout << endl << "Deleting Ishaan" << endl;
    tc2.deletePlayer(204, "Ishaan", 910, 20);

    cout << endl << "Leaderboard after deletion:" << endl;
    tc2.displayLeaderBoard();

    cout << endl << "Final Skip List Structure:" << endl;
    tc2.displaySkipListStructure();


    // ============================================================
    // TEST CASE 3: SAME SCORE AND SAME TIMESTAMP
    // ============================================================
    cout << endl << endl << "================ TEST CASE 3 ================" << endl;

    SkipList tc3;
    tc3.insertPlayer(301, "Dhruv", 870, 25);
    tc3.insertPlayer(302, "Yash", 870, 25);
    tc3.insertPlayer(303, "Kunal", 870, 25);
    tc3.insertPlayer(304, "Samar", 890, 18);

    cout << endl << "Leaderboard after insertion:" << endl;
    tc3.displayLeaderBoard();

    cout << endl << "Search Yash:" << endl;
    SkipListNode *p3 = tc3.searchPlayer(302, "Yash", 870, 25);
    if (p3 != NULL)
    {
        tc3.displayDetails(p3);
        cout << "Rank : " << tc3.getRank(302) << endl;
    }
    else
    {
        cout << "Player not found" << endl;
    }

    cout << endl << "Rank of Kunal: " << tc3.getRank(303) << endl;

    cout << endl << "Top 4 players:" << endl;
    tc3.getTopK(4);

    cout << endl << "Skip List Structure:" << endl;
    tc3.displaySkipListStructure();

    cout << endl << "Updating Dhruv score from 870 to 905" << endl;
    tc3.updateScore(301, "Dhruv", 870, 25, 905);

    cout << endl << "Leaderboard after update:" << endl;
    tc3.displayLeaderBoard();

    cout << endl << "Deleting Yash" << endl;
    tc3.deletePlayer(302, "Yash", 870, 25);

    cout << endl << "Leaderboard after deletion:" << endl;
    tc3.displayLeaderBoard();

    cout << endl << "Final Skip List Structure:" << endl;
    tc3.displaySkipListStructure();


    // ============================================================
    // TEST CASE 4: EDGE CASES
    // ============================================================
    cout << endl << endl << "================ TEST CASE 4 ================" << endl;

    SkipList tc4;
    tc4.insertPlayer(401, "Manav", 760, 30);
    tc4.insertPlayer(402, "Reyansh", 990, 5);
    tc4.insertPlayer(403, "Laksh", 800, 22);

    cout << endl << "Leaderboard after insertion:" << endl;
    tc4.displayLeaderBoard();

    cout << endl << "Search existing player Reyansh:" << endl;
    SkipListNode *p4 = tc4.searchPlayer(402, "Reyansh", 990, 5);
    if (p4 != NULL)
    {
        tc4.displayDetails(p4);
        cout << "Rank : " << tc4.getRank(402) << endl;
    }

    cout << endl << "Search non-existing player:" << endl;
    SkipListNode *p5 = tc4.searchPlayer(999, "Unknown", 500, 50);
    if (p5 == NULL)
        cout << "Player not found" << endl;

    cout << endl << "Top 10 players:" << endl;
    tc4.getTopK(10);

    cout << endl << "Deleting Laksh" << endl;
    tc4.deletePlayer(403, "Laksh", 800, 22);

    cout << endl << "Deleting Manav" << endl;
    tc4.deletePlayer(401, "Manav", 760, 30);

    cout << endl << "Deleting Reyansh" << endl;
    tc4.deletePlayer(402, "Reyansh", 990, 5);

    cout << endl << "Leaderboard after deleting all players:" << endl;
    tc4.displayLeaderBoard();

    cout << endl << "Final Skip List Structure:" << endl;
    tc4.displaySkipListStructure();

}

int main()
{
    srand(11);

    //here i am calling the testcase_report to call the small test cases to be mentioned in the report, but we can call the testcase_rigrous for complex test cases
    testcase_report();
    return 0;
}
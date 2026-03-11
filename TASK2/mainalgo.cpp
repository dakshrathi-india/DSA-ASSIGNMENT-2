#include <iostream>
#include <string>
using namespace std;

class Message{
    public:
        int msgID;
        int senderID;
        string text;
        //timeStamp has the time in seconds
        int timestamp;
        bool isDeleted;

        //default constructor
        Message(){
            msgID = -1;
            senderID = -1;
            text = "";
            timestamp = -1;
            isDeleted = true;
        }

        //constructor
        Message(int msgID, int senderID, string text, int timestamp){
            this->msgID = msgID;
            this->senderID = senderID;
            this->timestamp = timestamp;
            this->text = text;
            this->isDeleted = false;
        }
};

class MessageNode{
    public:
        Message data;
        MessageNode* prev;
        MessageNode* next;

        MessageNode(){
            data = Message();
            prev = NULL;
            next = NULL;
        }
        //constructor
        MessageNode(Message &msg){
            data = msg;
            prev = NULL;
            next = NULL;
        }
};

//for the linked list implementation of the Stack
class StackNode{
    public:
        //we are storing a pointer to the node of the doublly linked list , bcs it will be used for doing the the undoLastDelete 
        MessageNode* msgnode;
        StackNode* next;

        StackNode(MessageNode* temp){
            msgnode = temp;
            next = NULL;
        }
};

//linked list implementation of the stack using the StackNode
//in this both the insert and the pop will be done at the head
class Stack{
    public:
        StackNode* head = NULL;

        //to add the element to the linked list , made for the implementation of the stack
        void push(MessageNode* s){
            StackNode* temp = new StackNode(s);

            temp->next = head;
            head = temp;
        }

        //used to pop a element from the stack
        MessageNode* pop(){
            if(head == NULL){
                cout<<"the stack is empty";
                return NULL;
            }

            else{
                StackNode* temp = head;
                head = head->next;
                MessageNode* poped = temp->msgnode;

                //deallocate the dynamic memory
                delete temp;

                return poped;
            }
        }

        //checking whether the stack is empty 
        bool isEmpty(){
            if (head == NULL){
                return true;
            }
            else    
                return false;
        }
};


class MessengerSystem{
    public:
        //data members
        //for implementing the DLL(doubly linked list)

        MessageNode* msgHead = NULL;
        MessageNode* msgTail = NULL;

        //this stack contains all the messages which have been deleted
        Stack DeletedMessage;   

        //new element will be added from the last
        void sendMessage(int msgID, int senderID, string text, int time){
            Message msg(msgID, senderID, text, time);
            MessageNode* node = new MessageNode(msg);

            //adding this new element to our dll
            if(msgHead == NULL){
                msgHead = msgTail = node;
            }
            else{
                msgTail->next = node;
                node->prev = msgTail;
                msgTail = node;
            }
            //confirmation message 
            cout<<"the meassage has been sent"<<endl;
            
            cout<<"UPDATED CHAT"<<endl<<endl;
            displayChat();
        }

        //this function returns a pointer to the message with the given id if it exist , else it returns false
        //this function traverses in the doubley linked list
        MessageNode* findNode(int id){
            MessageNode* temp = msgHead;
            while(temp != NULL && temp->data.msgID != id){
                temp = temp->next;
            } 
        
            return temp;
        }

        void deleteForEveryone(int id, int currentTime){
            MessageNode* temp = findNode(id);

            //checking whether a message of that exist or not
            if(temp == NULL)
                cout<<"the node doesn't exist"<<endl;

            //checking if the message was already deleted
            else if(temp->data.isDeleted == true)
                cout<<"the message was already deleted"<<endl<<endl;

            //checking whether the constraint is obeyed or not
            else if (currentTime - temp->data.timestamp > 120){
                cout<<"as time time limit of 120 second has been excedeed"<<endl<<"the message can't be deleted for everyone"<<endl;
            }

            else{
                temp->data.isDeleted = true;

                //adding the deleted message to the stack (is it can be asked to undo the lastDeletedMessage)
                DeletedMessage.push(temp);

                //confirmation message 
                cout<<"data has been deleted for everyone"<<endl;
            }
            
            cout<<"UPDATED CHAT"<<endl<<endl;
            displayChat();
        }

        void deleteForMe(int id){
            MessageNode* temp = findNode(id);

            //checking whether a message of that exist or not
            if(temp == NULL){
                cout<<"the node doessn't exist"<<endl;
            }

            //checking if the message was already deleted
            else if(temp->data.isDeleted == true)
                cout<<"the message was already deleted"<<endl<<endl;

            else{
                temp->data.isDeleted =  true;

                //adding the deleted message to the stack (is it can be asked to undo the lastDeletedMessage)
                DeletedMessage.push(temp);

                //confirmation message
                cout<<"the data has been deleted for me"<<endl;
            }

            cout<<"UPDATED CHAT"<<endl<<endl;
            displayChat();
        }

        void displayChat(){
            MessageNode* temp = msgHead;
            int count = 1;
            while (temp != NULL){
                cout<<"MSG NUMBER : "<<count++<<endl;

                //checking if the message has been deleted
                if(temp->data.isDeleted == true)
                    cout<<"This message was deleted"<<endl;

                else    
                    cout<<temp->data.text<<endl;

                cout<<endl;
                temp = temp->next;
            }
        }

        void undoLastDelete(){
            if (DeletedMessage.isEmpty())
                cout<<"no message was deleted"<<endl;

            else{
                MessageNode* temp = DeletedMessage.pop();

                //undo the deletion
                temp->data.isDeleted = false; 

                cout<<"last delete undone"<<endl<<endl;
            }
            
            cout<<"UPDATED CHAT"<<endl<<endl;
            displayChat();
        }

        //destructor
        ~MessengerSystem(){
            //deleting the dynamic memory allocated to the doubly linked list
            MessageNode* temp = msgHead;

            while(temp != NULL){
                MessageNode* curr = temp;
                temp = temp->next;
                delete curr;
            }

            //deleting the memory occupied by the stack in the heap
            while(!DeletedMessage.isEmpty()){
                DeletedMessage.pop();
            }
        }
};

int main()
{
    // ==========================================================
    // TEST CASE 1
    // This test case checks:
    // 1) sending multiple messages
    // 2) delete for everyone within time limit
    // 3) delete for me
    // 4) undo last delete
    // 5) undo multiple times
    // 6) delete already deleted message
    // 7) delete non existing message
    // 8) undo when no deleted message is left
    // ==========================================================

    cout << "================ TEST CASE 1 ================" << endl;

    MessengerSystem chat1;

    cout << "Sending Messages" << endl;

    chat1.sendMessage(1, 101, "Hello", 10);
    chat1.sendMessage(2, 102, "Hi", 20);
    chat1.sendMessage(3, 103, "How are you?", 30);
    chat1.sendMessage(4, 104, "I am fine", 40);
    chat1.sendMessage(5, 105, "Nice to hear that", 50);

    cout << endl << "Delete Message 2 for everyone within time limit" << endl;
    chat1.deleteForEveryone(2, 100);

    cout << endl << "Delete Message 4 for me" << endl;
    chat1.deleteForMe(4);

    cout << endl << "Attempt deleting already deleted Message 2 again" << endl;
    chat1.deleteForMe(2);

    cout << endl << "Attempt deleting non existing Message 10" << endl;
    chat1.deleteForEveryone(10, 80);

    cout << endl << "Undo last delete" << endl;
    chat1.undoLastDelete();

    cout << endl << "Undo again" << endl;
    chat1.undoLastDelete();

    cout << endl << "Undo again when no deleted message is left" << endl;
    chat1.undoLastDelete();



    // ==========================================================
    // TEST CASE 2
    // This test case checks:
    // 1) sending many messages
    // 2) delete for everyone within time limit
    // 3) delete for everyone after time limit
    // 4) delete for me
    // 5) multiple delete operations
    // 6) multiple undo operations
    // 7) undo when stack becomes empty
    // ==========================================================

    cout << endl << "================ TEST CASE 2 ================" << endl;

    MessengerSystem chat2;

    cout << "Sending Messages" << endl;

    chat2.sendMessage(1, 201, "Good morning", 5);
    chat2.sendMessage(2, 202, "Good morning everyone", 15);
    chat2.sendMessage(3, 203, "Are you ready for class?", 25);
    chat2.sendMessage(4, 204, "Yes, almost ready", 35);
    chat2.sendMessage(5, 205, "Let's start", 45);
    chat2.sendMessage(6, 206, "Wait for me", 55);
    chat2.sendMessage(7, 207, "Joining in 2 minutes", 65);
    chat2.sendMessage(8, 208, "Okay", 75);

    cout << endl << "Delete Message 3 for everyone within time limit" << endl;
    chat2.deleteForEveryone(3, 100);

    cout << endl << "Delete Message 6 for me" << endl;
    chat2.deleteForMe(6);

    cout << endl << "Delete Message 8 for everyone within time limit" << endl;
    chat2.deleteForEveryone(8, 120);

    cout << endl << "Attempt delete Message 1 for everyone after time limit" << endl;
    chat2.deleteForEveryone(1, 300);

    cout << endl << "Undo last delete" << endl;
    chat2.undoLastDelete();

    cout << endl << "Undo again" << endl;
    chat2.undoLastDelete();

    cout << endl << "Undo again" << endl;
    chat2.undoLastDelete();

    cout << endl << "Undo again when no deleted message is left" << endl;
    chat2.undoLastDelete();

    return 0;
}
# Task 2 - Report on Messenger System Using Doubly Linked List and Stack

## Introduction

This task presents the implementation of a messenger system in C++. The objective is to manage chat messages in their sending order while supporting message insertion, deletion, chat display, and undo of the most recent deletion.

Each message record stores the following information:

- Message ID
- Sender ID
- Message text
- Timestamp
- Deletion status

The implementation stores messages in a doubly linked list so that the chat remains in the same order in which messages were sent. A stack is used to support undo of the most recent delete operation.

## Core Rules Followed by the Implementation

The program behavior is determined exactly as implemented in the source code:

1. New messages are added at the end of the chat.
2. `deleteForEveryone()` is allowed only if the message exists, has not already been deleted, and the time difference is at most 120 seconds.
3. `deleteForMe()` deletes a message without checking the 120-second limit.
4. Every successful delete operation pushes that message node onto a stack.
5. `undoLastDelete()` restores only the most recently deleted message.


## Data Structures Used

### 1. `Message`

Stores the details of one message:

- `msgID`
- `senderID`
- `text`
- `timestamp`
- `isDeleted`

Purpose in the implementation:

- This is the basic record stored inside every chat node.
- It keeps all information required for insertion, lookup, deletion checks, and display.
- The `timestamp` field is used to enforce the 120-second limit in `deleteForEveryone()`.

Space complexity:

- `O(1)` per message record, since the structure stores a fixed number of fields.

### 2. `MessageNode`

Represents one node of the doubly linked list. Each node contains:

- One `Message` object
- One `prev` pointer
- One `next` pointer

Purpose in the implementation:

- Each node stores one chat message.
- The `next` pointer allows forward traversal for search and display.

Space complexity:

- `O(1)` per node.

### 3. `StackNode`

Represents one node of the stack used for undo. Each node contains:

- A pointer to a `MessageNode`
- A pointer to the next `StackNode`

Purpose in the implementation:

- It stores references to deleted messages.
- This makes it possible to restore the last deleted message without searching for it again.

Space complexity:

- `O(1)` per stack node.

### 4. `Stack`

Implements the undo stack as a singly linked list. It maintains:

- `head`, the top of the stack

Purpose in the implementation:

- `push()` records each successful delete.
- `pop()` returns the most recently deleted message node.
- `isEmpty()` checks whether undo is currently possible.

Overall space complexity:

- If `d` messages have been deleted and not yet undone, the stack uses `O(d)` space.

### 5. `MessengerSystem`

Manages the complete chat system. It maintains:

- `msgHead`, the first message node
- `msgTail`, the last message node
- `DeletedMessage`, the stack of deleted message nodes

Purpose in the implementation:

- It coordinates message sending, deletion, searching, display, undo, and cleanup.
- The doubly linked list stores the chat sequence.
- The stack stores the order of deletions for undo operations.

Overall space complexity:

- For `n` messages and `d` deleted entries currently stored in the stack, total space is `O(n + d)`.

## Functional Operations

### 1. `sendMessage(int msgID, int senderID, string text, int time)`

This function sends a new message by inserting it at the end of the chat.

Working procedure:

1. Create a `Message` object from the supplied values.
2. Create a `MessageNode` containing that message.
3. If the chat is empty, make the new node both head and tail.
4. Otherwise, attach the new node after the current tail.
5. Update the tail pointer.
6. Print a confirmation message and display the updated chat.

Complexity:

- Time complexity: `O(1)`.
- Space complexity: `O(1)` 

### 2. `findNode(int id)`

This function searches for a message node using the message ID.

Working procedure:

1. Start from `msgHead`.
2. Move forward one node at a time.
3. Stop when a node with matching `msgID` is found or the list ends.
4. Return the node pointer if found; otherwise return `NULL`.

Complexity:

- Time complexity: `O(n)`.
- Space complexity: `O(1)`.

### 3. `deleteForEveryone(int id, int currentTime)`

This function deletes a message for everyone, subject to the time constraint.

Conditions checked in the implementation:

- the message must exist
- the message must not already be deleted
- `currentTime - timestamp <= 120`

Working procedure:

1. Call `findNode(id)` to locate the message.
2. If the node does not exist, print an error message.
3. If the message is already deleted, print a message indicating that.
4. If more than 120 seconds have passed, reject the delete request.
5. Otherwise, set `isDeleted = true`.
6. Push the message node onto the stack.
7. Print the updated chat.

Complexity:

- Time complexity: `O(n)` because of the search.
- Space complexity: `O(1)` 

### 4. `deleteForMe(int id)`

This function deletes a message without applying the 120-second condition.

Working procedure:

1. Call `findNode(id)` to locate the message.
2. If the node does not exist, print an error message.
3. If the message is already deleted, print a message indicating that.
4. Otherwise, set `isDeleted = true`.
5. Push the message node onto the stack.
6. Print the updated chat.

Complexity:

- Time complexity: `O(n)` because of the search.
- Space complexity: `O(1)` extra for the operation itself, excluding the new stack node.

### 5. `displayChat()`

This function prints the chat messages in sending order.

Working procedure:

1. Start from `msgHead`.
2. Traverse the list one node at a time.
3. Print the message number for each node.
4. If the message is deleted, print `This message was deleted`.
5. Otherwise, print the actual text of the message.

Complexity:

- Time complexity: `O(n)`.
- Space complexity: `O(1)`.

### 6. `undoLastDelete()`

This function restores the most recently deleted message.

Working procedure:

1. Check whether the stack is empty.
2. If it is empty, print a message indicating that no deleted message is available.
3. Otherwise, pop the top message node from the stack.
4. Set that node's `isDeleted` value back to `false`.
5. Print the updated chat.

Complexity:

- Time complexity: `O(1)`.
- Space complexity: `O(1)`.

### 7. Stack Operations: `push()`, `pop()`, and `isEmpty()`

These helper functions manage the undo stack.

#### `push(MessageNode* s)`

- Inserts a new stack node at the head.
- Time complexity: `O(1)`.
- Space complexity: `O(1)`.

#### `pop()`

- Removes and returns the top stack entry.
- Time complexity: `O(1)`.
- Space complexity: `O(1)`.

#### `isEmpty()`

- Checks whether the stack head is `NULL`.
- Time complexity: `O(1)`.
- Space complexity: `O(1)`.

### 8. Destructor `~MessengerSystem()`

This function releases the dynamic memory used by the system.

Working procedure:

1. Traverse the doubly linked list from head to tail.
2. Delete each message node.
3. Repeatedly pop remaining elements from the stack until it becomes empty.

Complexity:

- Time complexity: `O(n + d)`, where `n` is the number of messages and `d` is the number of stored deleted entries.
- Space complexity: `O(1)` extra.

## Complexity Summary Table

| Function | Time Complexity | Space Complexity | Remarks |
|---|---|---|---|
| `sendMessage()` | `O(1)` | `O(1)` | inserts at tail |
| `findNode()` | `O(n)` | `O(1)` | linear traversal by message ID |
| `deleteForEveryone()` | `O(n)` | `O(1)` | includes search and time-limit check |
| `deleteForMe()` | `O(n)` | `O(1)` | includes search |
| `displayChat()` | `O(n)` | `O(1)` | prints all messages |
| `undoLastDelete()` | `O(1)` | `O(1)` | stack pop and flag reset |
| `push()` | `O(1)` | `O(1)` | inserts at stack head |
| `pop()` | `O(1)` | `O(1)` | removes from stack head |
| `isEmpty()` | `O(1)` | `O(1)` | stack emptiness check |
| `~MessengerSystem()` | `O(n + d)` | `O(1)` | frees list and stack memory |


## Test Cases Included in `main()`

The program includes two test groups. Their purposes are consistent with the comments written in the source code and together they verify both normal behavior and edge conditions.

### Test Case 1: Basic Message Operations and Undo

Purpose:

- sending multiple messages
- `deleteForEveryone()` within the allowed time
- `deleteForMe()`
- undo of the last deletion
- multiple undo operations
- attempting to delete an already deleted message
- attempting to delete a non-existing message
- undo when no deleted message remains

This test case confirms that the basic messenger operations work correctly under normal and invalid conditions.

### Test Case 2: Time Limit, Multiple Deletes, and Multiple Undo Operations

Purpose:

- sending many messages
- `deleteForEveryone()` within the allowed time
- `deleteForEveryone()` after the time limit has passed
- `deleteForMe()`
- multiple delete operations
- multiple undo operations
- undo after the stack becomes empty

This test case verifies the 120-second restriction and checks whether the stack-based undo mechanism behaves correctly after several operations.

## Output Produced by the Program

The program produces output showing:

- confirmation of sent messages
- the chat after every update
- deleted-message placeholders during display
- rejection of invalid delete requests
- restoration of messages after undo

## How to Compile and Run

Using `g++`:

```bash
g++ mainalgo.cpp -o mainalgo
./mainalgo
```

On Windows PowerShell:

```powershell
g++ mainalgo.cpp -o mainalgo
.\mainalgo
```

## Observations

- The doubly linked list preserves the order in which messages are sent.
- The undo feature is implemented using a stack, so only the most recent deletion can be restored first.
- The `deleteForEveryone()` operation strictly follows the 120-second condition present in the implementation.
- Since the code stores only one deletion flag per message, the displayed result of `deleteForEveryone()` and `deleteForMe()` is handled in the same way during chat display.

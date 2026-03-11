# Task 1 - Report on Leaderboard Management Using Skip List

## Introduction

This task presents the implementation of a leaderboard management system using a skip list in C++. The objective is to maintain player records in sorted order so that insertion, search, update, deletion, and leaderboard display operations can be performed efficiently.

Each player record stores the following information:

- Player ID
- Player Name
- Score
- Timestamp

The skip list maintains the leaderboard in ranked order according to the comparison rules defined in the program.

## Ranking Criteria

The ranking of players is determined exactly as implemented in `isBefore()`:

1. Higher score ranks first.
2. If scores are equal, earlier timestamp ranks first.
3. If both score and timestamp are equal, smaller player ID ranks first.

The third rule removes ambiguity in cases where two players have the same score and the same timestamp.

## Data Structures Used

### 1. `PlayerData`

Stores the details of one player:

- `playerID`
- `name`
- `score`
- `timestamp`

Purpose in the implementation:

- This is the basic record stored inside every skip list node.
- It holds all fields needed for ordering and display.
- The ranking comparison uses `score`, `timestamp`, and `playerID`.

Space complexity:

- `O(1)` per player record, since the structure stores a fixed number of fields.

### 2. `SkipListNode`

Represents one node of the skip list. Each node contains:

- One `PlayerData` object
- `nodeLevel`, which indicates the highest level reached by that node
- `next[maxLevel + 1]`, which stores forward pointers for each level

Purpose in the implementation:

- Each node stores one player and the forward links required to move across levels.
- Higher levels allow longer jumps, which reduces the average search path.
- The array size is fixed as `maxLevel + 1`, so each node has space for all possible levels even if it actually uses fewer.

Space complexity:

- `O(maxLevel)` per node.
- Since `maxLevel` is fixed at `16`, this is constant space per node in this implementation.

### 3. `SkipList`

Manages the full leaderboard. It maintains:

- `Head`, a sentinel node
- `currentLevel`, which stores the highest active level currently present in the structure

Purpose in the implementation:

- `Head` is a sentinel node placed before all real players.
- It simplifies insertion, deletion, and traversal because every search begins from the same starting point.
- `currentLevel` avoids scanning unnecessary upper levels when the structure is still small.

Overall space complexity:

- For `n` players, total space is `O(n * maxLevel)`.
- Because `maxLevel` is fixed, this behaves as `O(n)` in this program.

## Random Level Generation Method

The function `randomLevel()` decides how many levels a new node will occupy.

- A coin toss is simulated using `rand() % 2`.
- As long as the result is `1`, the node is promoted to the next level.
- When the result becomes `0`, the process stops.
- The maximum level allowed is `16`.

This follows the probabilistic idea used in skip lists, where not every node appears on every level.

Method summary:

- Start from level `0`.
- Repeatedly simulate a coin toss.
- Promote the node one level higher for every successful toss.
- Stop when the toss fails or when `maxLevel` is reached.

Complexity:

- Time complexity: expected `O(1)`, worst-case `O(maxLevel)`[in our case the maxLevel = 16 , therefore it will be O(1)].
- Space complexity: `O(1)`.

## Comparison Logic for Ordering

The function `isBefore(d1, d2)` determines whether player `d1` should appear before player `d2` in the leaderboard.

Method used:

1. Compare scores. The player with the higher score comes first.
2. If scores are equal, compare timestamps. The player with the smaller timestamp comes first.
3. If both are equal, compare player IDs. The smaller ID comes first.

Complexity:

- Time complexity: `O(1)`.
- Space complexity: `O(1)`.

## Functional Operations

### 1. `insertPlayer(int id, const char *name, int score, int time)`

This function inserts a new player into the leaderboard at the correct sorted position.

- The implementation assumes player IDs are already unique, as stated in the problem constraints.

Working procedure:

1. Create a `PlayerData` object and then a new skip list node.
2. Generate the random level of the new node.
3. Starting from the current highest level, move forward while the next node should stay before the new player according to `isBefore()`.
4. At every relevant level, remember the next pointer that must come after the new node.
5. Reconnect pointers so the new node is inserted in sorted position.
6. If the new node rises above the current highest level, update `currentLevel` and connect the head node at those upper levels.

Complexity:

- Time complexity: expected `O(log n)` (`O(n)` in the worst case, when the skip list degenerates into a linked list and level-wise skipping no longer helps).
- Extra space complexity: `O(maxLevel)` for the temporary `modifyNode` array, which is `O(1)` here because `maxLevel` is fixed.

### 2. `deletePlayer(int id, const char *name, int score, int time)`

This function removes an existing player from the leaderboard.

- The player is first searched.
- If found, pointer links are updated level by level.
- If not found, an error message is shown.

Working procedure:

1. Call `searchPlayer(...)` to locate the target node.
2. If the node is not found, print an error message.
3. Otherwise, start again from the head at the highest active level.
4. Move forward while the next node still comes before the target.
5. Whenever the next pointer at a level points to the target, bypass it by linking to the target's next node at that level.
6. Delete the target node.
7. Reduce `currentLevel` if upper levels become empty.

Complexity:

- Search step: expected `O(log n)` (`O(n)` in the worst case, when the skip list degenerates into a linked list).
- Pointer update step: expected `O(log n)`, worst-case `O(n)`.
- Overall time complexity: expected `O(log n)` (`O(n)` in the worst case, when the skip list degenerates into a linked list).
- Extra space complexity: `O(1)`.

### 3. `updateScore(int id, const char *name, int oldscore, int time, int newscore)`

This function updates the score of an existing player.

- Since score affects ranking, the node is deleted first.
- The updated record is then inserted again.

Working procedure:

1. Search for the player using the old record details.
2. If the player does not exist, print an error message.
3. If found, delete the old node.
4. Insert a new node with the updated score.

Reason reinsertion is required:

- The list is sorted by score, timestamp, and ID.
- Once the score changes, the old position may no longer be correct.
- So the node must be placed again in its proper sorted location.

Complexity:

- It performs one search, one deletion, and one insertion.
- Each of these operations is expected `O(log n)` in a skip list.
- So the overall time complexity is expected `O(log n)` (`O(n)` in the worst case, when the skip list degenerates into a linked list).
- Extra space complexity: `O(1)` besides the temporary insertion array, which is bounded by fixed `maxLevel`.

### 4. `searchPlayer(int id, const char *name, int score, int time)`

This function searches for a player and returns the corresponding node pointer if found.

Working procedure:

1. Build a temporary `PlayerData` object from the given values.
2. Start from the head node at the current highest level.
3. Move forward while the next node should remain before the target record.
4. When no more movement is possible at that level, drop one level lower.
5. After reaching level `0`, check whether the next node has the required player ID.
6. If yes, return that node; otherwise return `NULL`.


Complexity:

- Time complexity: expected `O(log n)` (`O(n)` in the worst case, when the skip list degenerates into a linked list).
- Space complexity: `O(1)`.

### 5. `getRank(int id)`

This function returns the rank of a player by traversing level `0`.

- If the player exists, the returned rank is position-based according to the current leaderboard order.
- If the player does not exist, `-1` is returned.

Working procedure:

1. Start from the head sentinel.
2. Move node by node only through level `0`.
3. Increment a counter until the required `playerID` is found.
4. Return the counter value if found; otherwise return `-1`.

Complexity:

- Time complexity: `O(n)`.
- Space complexity: `O(1)`.


### 6. `getTopK(int k)`

This function displays the top `k` players from the leaderboard.

- If `k <= 0`, an invalid input message is shown.
- If `k` is larger than the number of players, all available players are displayed.

Working procedure:

1. Start from the first real node at level `0`.
2. Traverse forward one player at a time.
3. Print details until either `k` players have been shown or the list ends.

Complexity:

- Time complexity: `O(k)` when `k <= n`, otherwise `O(n)`.
- Space complexity: `O(1)`.

### 7. `displayLeaderBoard()`

This function prints the complete leaderboard in ranked order.

Working procedure:

1. Start from `Head->next[0]`.
2. Traverse level `0` completely.
3. Print the running rank and the player details for each node.

Complexity:

- Time complexity: `O(n)`.
- Space complexity: `O(1)`.

### 8. `displaySkipListStructure()`

This function prints the skip list level by level so that the internal layered arrangement can be observed.

Working procedure:

1. Start from the current highest active level.
2. For each level, traverse all nodes reachable on that level.
3. Print the level number and the player details visible there.
4. Continue downward until level `0`.

Complexity:

- Let `L` be the number of active levels.
- Time complexity: proportional to the total nodes visited across all levels, which can be written as `O(nL)` in general. [in worst case it will be O(n*log(n))]
- Since `maxLevel = 16` is fixed, this behaves as `O(n)` in this implementation.
- Space complexity: `O(1)`.

### 9. Destructor `~SkipList()`

This function deletes all nodes from the structure when the object goes out of scope.

Working procedure:

1. Start from the first real node on level `0`.
2. Move through the list one node at a time.
3. Delete each node.
4. Finally delete the head sentinel.

Complexity:

- Time complexity: `O(n)`.
- Space complexity: `O(1)`.

## Complexity Summary Table

| Function | Time Complexity | Space Complexity | Remarks |
|---|---|---|---|
| `randomLevel()` | expected `O(1)`, worst `O(maxLevel)` | `O(1)` | `maxLevel` is fixed at 16 |
| `isBefore()` | `O(1)` | `O(1)` | constant-time comparison |
| `insertPlayer()` | expected `O(log n)` (`O(n)` if the skip list degenerates into a linked list) | `O(1)` effective | no duplicate-ID check is performed |
| `deletePlayer()` | expected `O(log n)` (`O(n)` if the skip list degenerates into a linked list) | `O(1)` | includes search and relinking |
| `updateScore()` | expected `O(log n)` (`O(n)` if the skip list degenerates into a linked list) | `O(1)` effective | search + delete + insert |
| `searchPlayer()` | expected `O(log n)` (`O(n)` if the skip list degenerates into a linked list) | `O(1)` | uses level-wise traversal |
| `getRank()` | `O(n)` | `O(1)` | scans only level `0` |
| `getTopK()` | `O(k)` or `O(n)` | `O(1)` | prints first `k` players |
| `displayLeaderBoard()` | `O(n)` | `O(1)` | full traversal of level `0` |
| `displaySkipListStructure()` | `O(nL)` general, `O(n)` here | `O(1)` | `L` bounded by fixed max level |
| `~SkipList()` | `O(n)` | `O(1)` | deletes all nodes |

## Test Cases Included in `main()`

The program includes four test groups. Their purposes are consistent with the comments written in the source code and together they verify both normal behavior and edge conditions.

### Test Case 1: Basic Ordering + Search + Rank + Update + Delete

Purpose:

- descending order of score
- search existing player
- `getRank()`
- `getTopK()`
- `updateScore()` causing reposition
- `deletePlayer()` for an existing player

This test case confirms that normal insertion and leaderboard maintenance behave correctly.

### Test Case 2: Same Score, Different Timestamps

Purpose:

- tie-breaking by earlier timestamp
- `getRank()` under tie conditions
- search existing player
- `updateScore()` causing player to move to top
- delete top player

This test case verifies that timestamp is correctly used when scores are equal.

### Test Case 3: Same Score and Same Timestamp

Purpose:

- verify 3rd tie-breaker by player ID
- search existing player
- `getRank()`
- `getTopK()`
- `updateScore()`
- `deletePlayer()`

This test case verifies the additional ordering rule used to avoid ambiguity.

### Test Case 4: Edge Cases and Invalid Operations

Purpose:

- search non-existing player
- `getRank()` for non-existing player
- `getTopK(k > n)`
- `getTopK(0)`
- `updateScore()` for non-existing player
- `deletePlayer()` for non-existing player
- delete remaining players

This test case checks the robustness of the implementation when invalid or boundary conditions are used.

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

- The skip list uses a probabilistic level assignment, so the internal level structure may vary between runs.
- The visible leaderboard order still follows the same ranking rules in every run.
- A sentinel head node is used with extreme values to simplify traversal and insertion logic.

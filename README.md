Welcome to Pavlo Naichuk GitHub page. Here you can find some of the projects I worked on in my spare time. 

<b>Battle City (C++11, Qt, QML, collision detection, path-finding, artificial intelligence)</b>  
An attemp to implement own version of popular game Battle City, developed by Namco in 1985 https://en.wikipedia.org/wiki/Battle_City_(video_game). The game has of a playfield where player tank has to protect the base from enemy tanks which are trying to destroy it.  
<b><li>Qt/QML</li></b>
The game has been implemented in Qt and QML with new features of C++11, such as lambda functions, auto, override, smart pointers, constexpr and enum classes. Qt and QML are utilized to present user interface and visuals in the game. Specifically, "model-view" approach is used to accomplish that to allow us to keep game data seperate from its visual representation. Qt and C++ are used to handle game data and logic while QML is responsible for visual representation of that data.
<b><li>Game Units</li></b>
The game entities are derived from the base object GameUnit. There are two types of game units - stationary (attached to one place on the map) and mobile (that is movable). Walls on the map are examples of stationary units while tank and missile are instances of mobile units.  
<b><li>Level Map</li></b>
Level map is represented as 2D matrix of equally-sized tiles (cells). Each tile contains information about its location and stationary units standing on it. Keeping information about stationary unit associated with the tile allows to speed up collision detection process. 
<b><li>Collision Detection</li></b>
Collsion detection prevents game units from penetrating into other game units while navigating. We test game unit collision versus map borders, stationary units and other mobile units. Since game units are allowed to move only in left, right, up and down directions, collision detection functionality has been optimized specifically for this use-case. Two collision feeler points are attached to the front face of the game unit which are propagated along its heading on the test distance to reveal any obstacles.  
<b><li>Artificial Intelligence</li></b>
Tank object is controlled by Tank AI object. In terms of design patterns, Tank AI could be regarded as a "strategy" or "policy". This allows us to configure the behavior of the enemy tank during its instantiation. For an easy difficulty level of the game, we could use one strategy whereas for the hard level it could be a very aggressive one. For the player tank, tank AI object has dummy (empty) implementation as the player is in charge of making decisions on the next action. Tank AI object for enemy tank consists of goal objects. On each game frame update, the goal is re-evaluated and based on its cost AI object will choose the next action. Enemy tank is supplied with two goals: destroy player base and destroy player. We define goal priority based on how close enemy tank is located to its target. 
<b><li>Navigation</li></b>
In order to find a path to the player base and player tank, tank AI exploits breadth-first search algorithm. Since tank unit has size greater than one map tile, the original breadth-first search algorithm has been modified to take game unit size into account during path-finding stage. Specifically, each map tile is assigned so-called clearance metric which represents the number of tiles in bottom/right direction to the first obstacle. If clearance value of the tile is less than the size of the game unit, that tile should be discarded during path-finding.  
![Alt text](/Images/BattleCity/StartMenu.jpg?raw=true "")
![Alt text](/Images/BattleCity/Gameplay.jpg?raw=true "")

<b>Find Files (C++, Qt, multi-threading)</b>   
FindFiles app demonstrates file search containing a word of interest in a particular directory using multi-threaded approach.

The algorithm starts with accumulating list of files in the directory. This step is accomplished by the main thread. As soon as the list of files in the directory has been populated, we split it into subranges. After, we start a dedicated thread for each subrange of files from the list. The number of the subranges and, consequently, number of threads is defined by the actual count of hardware threads that could be launched on the PC. In Qt this information could be retrieved using QThread::idealThreadCount() function.  

Apart from this, the app demonstrates search progress, exploiting Qt progress bar component. The progress is calculated based on the current number of processed bytes by all the threads relative to the total byte size of all the files.   
The user is allowed to pause the search operation. Qt threading library does not provide support for suspending the thread compared to Win API. To mimic such behaviour, "while (mIsPaused)" polling is used in conjunction with Qt::sleep() to avoid unnecessary CPU work. 
Naturally, the user is allowed to cancel the search in a graceful way as well.   

For the directory with 268 files, the single-threaded search took 21324 milliseconds. In its turn, the multi-threaded search took 5910 milliseconds with launched 8 hardware threads on Intel Core i7 CPU. We observe considerable performance gain in the second scenario. 

It is worth noting that we still encounter performance drop while iterating the file direcory
the collect the files required for the inspection as the first step of the algorithm. As a future improvement this step could
also benefit from multi-threaded approach.

![Alt text](/Images/FindFiles/FoundFiles.jpg?raw=true "")
![Alt text](/Images/FindFiles/Cancel.jpg?raw=true "")

<b>Memory Puzzle (C++, Qt)</b>  
A 4x4 board is full of overturned cards. There is a pair for each card. The player flips over two cards. If they match, then they stay overturned. Otherwise they flip back. The player needs to overturn all the cards in the fewest moves to win.
![Alt text](/Images/MemoryPuzzle/GameStart.jpg?raw=true "")
![Alt text](/Images/MemoryPuzzle/OpenedCards.jpg?raw=true "")
![Alt text](/Images/MemoryPuzzle/LevelCompleted.jpg?raw=true "")
![Alt text](/Images/MemoryPuzzle/Help.jpg?raw=true "")

<b>Tic Tac Toe (C++)</b>  
Console version of a popular Tic Tac Toe game. Tic-tac-toe is a game for two players who take turns to mark free cells in a 3x3 board with X and O. The player who manages to place three of their marks in a horizontal, vertical, or diagonal row wins the game. In case none of the player manages to accomplish that, the game ends in a draw.  

In current implementation, the player is able to select to play versus another player or PC.
![Alt text](/Images/TicTacToe/PlayerVsPlayer.jpg?raw=true "")
![Alt text](/Images/TicTacToe/PlayerVsPC.jpg?raw=true "")

When the game is finished, the player is offered to play the game again.
![Alt text](/Images/TicTacToe/Restart.jpg?raw=true "")

In case user provides invalid keyboard input, he or she is required to repeat the operation.
![Alt text](/Images/TicTacToe/Error.jpg?raw=true "")

<b>File Manager (Java (Swing))</b>  
Wrote application as a pad project, similar in functionality to Total Commander (http://www.ghisler.com).  
The application has a main window with left and right panels, allowing to acomplish the following commands:
- display file name, size, extension and last date of modification
- dynamically specify which file properties display in the table
- copy, move/rename, delete files and create a new folder
- navigate through file system
- open a file through an associated application

<b>Excel Exporter (Java (Swing), C#)</b>   
Wrote application as part of bachelor's thesis to export student grades to Excel file for printing. User interface was written in Java (Swing), which allowed to specify student grades and subjects. The export to Excel format was done by launching extra C# application from Java code by passing student data through the command line arguments.
  

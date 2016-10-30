Welcome to Pavlo Naichuk GitHub page. Here you can find some of the projects I worked on in my spare time. 

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

![Alt text](/Images/TicTacToe/Pause.jpg?raw=true "")

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
  
